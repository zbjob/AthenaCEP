#include "NormalDistGen.h"
//#include "RingBuffer.h"
#include <chrono>
//#include "GlobalClock.h"
#include <fstream>
#include <string>

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
           //while(m_Buffer.size() > m_BufferSize )
           //{
           //     this_thread::sleep_for(chrono::milliseconds(10));
           //}
           //cout << "stream generator " << eventCnt << endl;
                NormalEvent Event;
                int dice_roll = distribution(generator);  // generates number in the range [1,20]
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

                //m_Buffer.push_back(Event);
                //if (eventCnt < 2)
                //{
                //    //m_StopThread = true;
                //    break;
                //}
            
        } //for
        ofs.close();
        m_StopThread = true;


    });
  
}


/*
 ***** we generate 4 types of events A B C D, each has 25% frequency. 
 */ 
void NormalDistGen::run_uniform(int a1, int a2, 
                                int b1, int b2, 
                                int c1, int c2)
{
    time_point<high_resolution_clock> g_BeginClock = high_resolution_clock::now();

    m_GenThread = thread ([=] () { 
        ofstream ofs;
        ofs.open(outputFile.c_str());

        default_random_engine generator;
        uniform_int_distribution<int> distribution(1,4); //control the frequency of each type of events
        uniform_int_distribution<int> IDgen(1,10);  // the range of ID
        
        uniform_int_distribution<int> GenA(a1,a2);  
        uniform_int_distribution<int> GenB(b1,b2);  
        uniform_int_distribution<int> GenC(c1,c2);  
        uniform_int_distribution<int> GenC2(12,20);  
        uniform_int_distribution<int> GenD(0,1);   // control the noisy of D



        uint64_t timeCnt = 1;
        
        for(uint64_t cnt = 1; cnt <= eventCnt; ++cnt)
        {
           //while(m_Buffer.size() > m_BufferSize )
           //{
           //     this_thread::sleep_for(chrono::milliseconds(10));
           //}
           //cout << "stream generator " << eventCnt << endl;
            NormalEvent Event;

            //                if(cnt == eventCnt/2)
            //                {
            //                    Event.name = "S";
            //                    Event.v1 = 0;
            //                    Event.v2 = 0;
            //                    Event.ArrivalQTime = cnt;
            //                    Event.ID = (uint64_t) IDgen(generator);
            //
            //                }

            int dice_roll = distribution(generator);  // generates number in the range [1,4]

            if(cnt < eventCnt/2)
            {
                if(dice_roll == 1 ) {
                    Event.name = "A";
                    Event.v1 = (attr_t) GenA(generator);
                    Event.v2 = 0; 
                    //Event.ArrivalQTime = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count();
                    Event.ArrivalQTime = cnt; 
                    Event.ID = (uint64_t) IDgen(generator);

                }
                else if(dice_roll == 2){
                    Event.name = "B";
                    Event.v1 = (attr_t) GenB(generator);
                    Event.v2 = 0; 
                    //Event.ArrivalQTime = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count();
                    Event.ArrivalQTime = cnt;
                    Event.ID = (uint64_t) IDgen(generator);


                }
                else if(dice_roll == 3){
                    Event.name = "C";
                    Event.v1 = (attr_t) GenC(generator);
                    Event.v2 = 0;
                    //Event.ArrivalQTime = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count();
                    Event.ArrivalQTime = cnt; 
                    Event.ID = (uint64_t) IDgen(generator);
                }
                else
                {
                    Event.name = "D";
                    Event.v1 = (attr_t) GenD(generator);
                    Event.v2 = 0;
                    Event.ArrivalQTime = cnt; 
                    Event.ID = (uint64_t) IDgen(generator);
                }
            }
            else if(cnt == eventCnt/2)
            {
                Event.name = "S";
                Event.v1 = 0;
                Event.v2 = 0;
                Event.ArrivalQTime = cnt;
                Event.ID = (uint64_t) IDgen(generator);
            }
            else
            {
                if(dice_roll == 1 ) {
                    Event.name = "A";
                    Event.v1 = (attr_t) GenA(generator);
                    Event.v2 = 0; 
                    //Event.ArrivalQTime = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count();
                    Event.ArrivalQTime = cnt; 
                    Event.ID = (uint64_t) IDgen(generator);

                }
                else if(dice_roll == 2){
                    Event.name = "B";
                    Event.v1 = (attr_t) GenB(generator);
                    Event.v2 = 0; 
                    //Event.ArrivalQTime = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count();
                    Event.ArrivalQTime = cnt;
                    Event.ID = (uint64_t) IDgen(generator);


                }
                else if(dice_roll == 3){
                    Event.name = "C";
                    Event.v1 = (attr_t) GenC2(generator);
                    Event.v2 = 0;
                    //Event.ArrivalQTime = (uint64_t)duration_cast<microseconds>(high_resolution_clock::now() - g_BeginClock).count();
                    Event.ArrivalQTime = cnt; 
                    Event.ID = (uint64_t) IDgen(generator);
                }
                else
                {
                    Event.name = "D";
                    Event.v1 = (attr_t) GenD(generator);
                    Event.v2 = 0;
                    Event.ArrivalQTime = cnt; 
                    Event.ID = (uint64_t) IDgen(generator);
                }

            }

            ofs << Event.name << "," << Event.ArrivalQTime << "," << Event.ID << "," << Event.v1 << "," <<  Event.v2 <<  endl;

                //m_Buffer.push_back(Event);
                //if (eventCnt < 2)
                //{
                //    //m_StopThread = true;
                //    break;
                //}
            
        } //for
        ofs.close();
        m_StopThread = true;


    });
  
}


bool NormalDistGen::stop()
{
    //if(m_StopThread) 
        m_GenThread.join();

    return m_StopThread;
}

int main()
{
    int eventCnt = 500000;
    //for(int i=2; i<=11; ++i)
    //{
        
        //string outputFile = "Uniform_StreamLog_c_"+ to_string(i)+ "-"+ to_string(i+1)+"_500K.csv";
        string outputFile = "Uniform_StreamLog_change_500K.csv";
        NormalDistGen StreamGenerator(eventCnt, outputFile);
        //StreamGenerator.run(45,5,//Distibution for A.v1
        //                    50,3,  // ... A.v2
        //                    40,5,  // ... B.v1
        //                    55,3,   // ... B.v2
        //                    50,3,  // ... C.v1
        //                    40,3);  // ... C.v2

        StreamGenerator.run_uniform(1,10,//Distibution for A.v1
                1,10,  // ... B.v1
                2,10);  // ... C.v1

        StreamGenerator.stop();
    //}
    
    return 0;
}
