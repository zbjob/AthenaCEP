#pragma once
#include <iostream>
#include <string>
#include <random>
#include <fstream>
#include <functional>
#include <thread>
#include <queue>
#include <vector>
#include <utility>

typedef int attr_t;
using namespace std;

class NormalEvent
{
    public:
        NormalEvent(std::string N): name(N){}
        NormalEvent(){}
        ~NormalEvent(){}
       
        attr_t  v1;
        attr_t  v2;
        attr_t  ArrivalQTime;
        attr_t  ID;

        std::string name;
};

class NormalDistGen
{
public:
    NormalDistGen(uint64_t events, string outFile): eventCnt(events),outputFile(outFile), m_StopThread(false) {};
    ~NormalDistGen(){};

    void run(double e11, double d11, double e12, double d12, double e21, double d21, double e22, double d22, double e31, double d31, double e32, double d32);
    void run_uniform(int a1, int b1, int a2, int b2, int c1, int c2);
    bool stop();
    void addDistribution(double e, double d);
    bool isStop() {return m_StopThread;}

private:
    std::thread         m_GenThread;
    volatile bool       m_StopThread;
    
    int                 m_BufferSize; 
    uint64_t            eventCnt;
    std::vector<std::pair<double, double>>       NormalDistributions;

    string outputFile;
};
