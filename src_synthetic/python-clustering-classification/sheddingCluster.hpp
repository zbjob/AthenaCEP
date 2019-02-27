#pragma once

#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <iostream>

using std::vector;
using std::string;
using std::ifstream;
using std::stringstream;
using std::stoi;
using std::stod;

class SheddingCluster
{
    //the order of lableInfo
public:
    int stateID;
    int clusterID;

    double maxContribution;
    double minContribution;

    double maxConsumption;
    double minConsumption;

    double _50PercentileContribution;
    double _50PercentileConsumption;

    double AVGContribution;
    double AVGConsumption;

    double contribution_consumption_ratio;

    long long clusterSize;
    long long sheddingQuota;

    void print()
    {
       if(contribution_consumption_ratio) 
        std::cout << stateID << " " << clusterID << " " << contribution_consumption_ratio << std::endl; 

       else
        std::cout << stateID << " " << clusterID << "  AVG ratio " << AVGContribution/AVGConsumption << std::endl; 
           
    }


    //reload operator stream in
};


class SheddingClusterPartialOrder
{
public:
    vector<SheddingCluster> m_sheddingClusters;
    void loadLableInfo(string _inputFile){
       ifstream ifs; 
       ifs.open(_inputFile.c_str());
       if(!ifs.is_open())
       {
           std::cout << "can't open file: " << _inputFile << std::endl;
           return;
       }

       string line;
       getline(ifs,line);
       while(getline(ifs,line))
       {
           vector<string> clusterInfo;
           stringstream lineStream(line);
           string cell;

           while(getline(lineStream, cell, ','))
               clusterInfo.push_back(cell);

           SheddingCluster c;
           int i=0;
           c.stateID = stoi(clusterInfo[i++]);
           c.clusterID = stoi(clusterInfo[i++]);
           c.maxContribution = stod(clusterInfo[i++]);
           c.minContribution = stod(clusterInfo[i++]);
           c.maxConsumption = stod(clusterInfo[i++]);
           c.minConsumption = stod(clusterInfo[i++]);
           c._50PercentileContribution = stod(clusterInfo[i++]);
           c._50PercentileConsumption = stod(clusterInfo[i++]);
           c.AVGContribution = stod(clusterInfo[i++]);
           c.AVGConsumption = stod(clusterInfo[i++]);
           c.contribution_consumption_ratio = c._50PercentileContribution/c._50PercentileConsumption;
           c.clusterSize = 0;

           m_sheddingClusters.push_back(c);

       }
      


    }

    void print()
    {
        for(auto &&a : m_sheddingClusters)
            a.print();
    }
    void sortSheddingClusters()
    {
        std::sort(m_sheddingClusters.begin(), m_sheddingClusters.end(),[](const SheddingCluster &lhs, const SheddingCluster &rhs){
                    return lhs.contribution_consumption_ratio < rhs.contribution_consumption_ratio;
                });
    }
    //to sort the m_sheddingClusters based on the contribution_consumption_ratio
    //the sheddingCluster is actually the index of a shedding cluster of a state buffer inthe CEP engine.
};

