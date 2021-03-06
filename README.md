# AthenaCEP
---

AthenaCEP aims at solving the problem of load shedding in CEP. It provides a fundamentally new perspective to shed the exponentially increasing partial matches as well as input event tuples. It has the wisdom, as Athena, the Greek goddess  of wisdom, to decide when, what and how much partial matches or event tuples to drop in order to satisfy\ the strict latency bound and keep the maximum accuracy. 

---
## Prerequisites

* Compilers: The compiler needs to support C++11 or higher. In Makefiles, the default compiler is set as g++.
* Advanced Vector Extensions: We set ccompiler flag -mavx2 for X86 machines. Howevever, if you may remove it from Makefile if it is not supported.
* Libraries: POSIX Threads(Pthreads), STL,  Python, scikit-learn for offiline machine learning.
* AZll running/configuration scripts are written for linux OS.
* We build parsers to parse query workloads from files. We define query workloads in files ending with `.eql`. 

## Code and Data


The main funciton of the CEP engine is in ./AthenaCEP/src_synthetic/src_NS/cep_match/cep_match.cpp

To compile the code in Linux:
1. cd $where you cloned the project$/AthenaCEP/src_synthetic/src_NS/
2. make -f MakefileCM 
3. the executable file is   /AthenaCEP/src_synthetic/src_NS/bin/cep_match

A parser for CEP query has also been implemented, based on Lex and Yacc.

CEP queries for synthetic data sets have been defined in AthenaCEP/queries/Synthetic.eql

CEP queries for real-world data sets, NYC bike sharing data, have been defined in AthenaCEP/queries/Bike.eql

parsing the event schema & queries, and constructing the CEP engine are automatically done. 


=====================================================================================

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

./AthenaCEP/src_synthetic/src_NS//bin/cep_match -c ./Synthetic.eql -q P1 -s -n P1NS -p monitoring_P1NS.csv > runInfor_P1NS.txt
   
   the arguments of the command line is reflected in the main function in cep_match.cpp, which are very straightforward and self explained.

   For the above specific query, 
   
   -c                          ./Synthetic.eql  means the event schema and query configuration file ./Synthetic.eql. 
   
   -q P1                       means to evaluate  query P1.
   
   -n P1NS                     means the prefix of generated results and  static files
   
   -s                          means to appendi the timestamps for events appeared in partial matches and complete matches.
   
   -p monitoring_P1NS.csv      means to dump the throughput information to the file monitoring_P1NS.csv 
        
   more scripts are attached in the repository.
   
   Three files will be generated after evaluation: take the above example 
          monitoring_P1NS.csv   is the monitoring file with the record of the throughput.
          runInfor_P1NS.txt     contains the information of the accuracy, the number of dropped events, the number of droppped partial matches.
          latency_P1NS.csv      contains the record of latency for every complete matches.
          
=====================================================================================

real-world data sets: NYC bike-sharing. 
The link of the data sets: https://www.citibikenyc.com/system-data

A. Transform the time stamp/ data cleaning
   run CleanData.py for 201810-citibike-tripdata.csv data set and get the cleaned data.

B. Set the path of the data source in main function.
   change the streamFile string in main function in ./AthenaCEP/src_testing_bike_sharing/cep_match/

C. Complie the code:
   1.  cd ./AthenaCEP/src_testing_bike_sharing/
   2.  make -f MakefileCM

D. Run the experiments.
   An example is illustrated in ./AthenaCEP/bike_sharing_data/query_p1.sh

=====================================================================================


Please feel free to drop me an e-mail at bo.zhao "at" hu-berlin.de, if you have any questions. 

Or visit https://hu.berlin/bo_zhao

