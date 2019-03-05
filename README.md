# AthenaCEP

AthenaCEP aims at sovling the problem of load shedding in CEP. It provides a fundamentally new perspective to shed the exponentially increacing partil matches as well as input event tuples. It has the wisdom, as Athena, a Greek goddess, to decide when, what and how much partial match or event tuples to drop to satisfy the strict latency bound and keeps the maximual accuracy. The secret is on her cost model...

=====================================================================================
requirements for compilers: supporting C++11 or C++14.  G++, Clang has been tested.

requirements for libraries: POSIX Threads(Pthreads), Python, scikit-learn

optimizations have been implemented for intel X86 processors including memory alignment, improving cache hit ratio.


The main funciton of the CEP engine is in ./AthenaCEP/src_synthetic/src_NS/cep_match/cep_match.cpp

To compile the code in Linux:
1. cd $where you cloned the project$/AthenaCEP/src_synthetic/src_NS/
2. make -f MakefileCM 
3. the executable file is   /AthenaCEP/src_synthetic/src_NS/bin/cep_match

=======================================================================================

Synthetic data sets:
To run the experiments:
A. Generate synthetic datas
   1. cd  ./AthenaCEP/src_synthetic/NormalEventStreamGen/
   2. sh make.sh
   3. ./UniDistGen.out
   The configuration of attribute values distributions and the number of the data could be found in:
        ./AthenaCEP/src_synthetic/NormalEventStreamGen/UniDistGen.cpp
        
B. Set the path in /AthenaCEP/src_synthetic/src_NS/cep_match/cep_match.cpp. 
   It could be done by setting the value of "string streamFile"

C. Compile the code. 

D. The offile estmation Python code is in  ./AthenaCEP/src_synthetic/python-clustering-classification/ 
   The trained classifiers could be found in .pkl files and could be visualised by .dot and .pdf files.
   To automatically genearated classifiers in C++ code, please find the source-to-source transformation tool  "DecisionTreeToCpp" at https://github.com/papkov/DecisionTreeToCpp    

F. In order to run the CEP engine, you need a simple script like:

./AthenaCEP/src_synthetic/src_NS//bin/cep_match -c ./PMDist_new.eql -q P1 -s -n P1NS -p monitoring_P1NS.csv > runInfor_P1NS.txt
   
   the arguments of the command line is reflected in the main function in cep_match.cpp, which are very straightforward and self explained.
   
   more scripts are attached in the repository.
   
   Three files will be generated after evaluation: take the above example 
          monitoring_P1NS.csv   is the monitoring file with the record of the throughput.
          runInfor_P1NS.txt     contains the information of the accuracy, the number of dropped events, the number of droppped partial matches.
          latency_P1NS.csv      contains the record of latency for every complete matches.
          
=====================================================================================

real-world data sets: NYC bike-sharing. 
Links of the data sets: https://www.citibikenyc.com/system-data

A. Transform the time stamp/ data cleaning
   run CleanData.py for 201810-citibike-tripdata.csv data set and get the cleaned data.

B. Set the path of the data source in main function.
   change the streamFile string in main function in ./AthenaCEP/src_testing_bike_sharing/cep_match/

C. Complie the code:
   1.  cd ./AthenaCEP/src_testing_bike_sharing/
   2.  make -f MakefileCM

D. Run the experiments.
   An example is as  ./AthenaCEP/bike_sharing_data/query_p1.sh

