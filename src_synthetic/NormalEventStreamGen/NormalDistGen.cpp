#include "NormalDistGen.h"

#include <chrono>

#include <fstream>

using namespace std;
using namespace std::chrono;

void NormalDistGen::run(double e11, double d11, 
                        double e12, double d12, 
                        double e21, double d21, 
                        double e22, double d22, 
                        double e31, double d31, 
                        double e32, double d32)
{
    time_point<high_resolution_clock> g_BeginClock = high_resolution_clock::now();

    m_GenThread = thread ([=] () { 
        ofstream ofs;
        ofs.open(outputFile.c_str());

        default_random_engine generator;
        uniform_int_distribution<int> distribution(1,20);
        uniform_int_distribution<int> IDgen(1,10);

        normal_distribution<double> N11(e11,d11);
        normal_distribution<double> N12(e12,d12);
        normal_distribution<double> N21(e21,d21);
        normal_distribution<double> N22(e22,d22);
        normal_distribution<double> N31(e31,d31);
        normal_distribution<double> N32(e32,d32);

        uint64_t timeCnt = 1;
        
        for(int cnt = 0; cnt < eventCnt; ++cnt)
        {
           
                NormalEvent Event;
                int dice_roll = distribution(generator);  
                if(dice_roll == 1 || dice_roll > 3) {
                    Event.name = "A";
                    Event.v1 = (attr_t) N11(generator);
                    Event.v2 = (attr_t) N12(generator);
                    Event.ArrivalQTime = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count();
                    Event.ID = (uint64_t) IDgen(generator);

                }
                else if(dice_roll == 2){
                    Event.name = "B";
                    Event.v1 = (attr_t) N21(generator);
                    Event.v2 = (attr_t) N22(generator);
                    Event.ArrivalQTime = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count();
                    Event.ID = (uint64_t) IDgen(generator);

                }
                else if(dice_roll ==3){
                    Event.name = "C";
                    Event.v1 = (attr_t) N31(generator);
                    Event.v2 = (attr_t) N32(generator);
                    Event.ArrivalQTime = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count();
                    Event.ID = (uint64_t) IDgen(generator);

                }

                ofs << Event.name << "," << Event.ArrivalQTime << "," << Event.ID << "," << Event.v1 << "," <<  Event.v2 <<  endl;

        } 
        ofs.close();
        m_StopThread = true;

    });
  
}

bool NormalDistGen::stop()
{
    
        m_GenThread.join();

    return m_StopThread;
}

int main()
{
    int eventCnt = 5000000;
    string outputFile = "StreamLog_500K.csv";
    NormalDistGen StreamGenerator(eventCnt, outputFile);
    
    StreamGenerator.run(45,10,
                        50,10,  
                        40,10,  
                        55,10,   
                        50,10,  
                        33,10);  
    
    StreamGenerator.stop();
    
    return 0;
}
