#include <iostream>

#include "Python.h"
#include <ctime>
#include <string>
#include <vector>
#include <deque>

#include "pyhelper.hpp"
#include "classA.hpp"
#include <chrono>

using namespace std;

int main()
{
    
    CPyInstance hInstance;
    A Ainstance;
    B Binstance;

    string path = "/home/bo/CEP_load_shedding/src_PM_Distribution_test/python-clustering-classification";
    
    string chdir_cmd = string("sys.path.append(\"") + path + "\")";
    const char* cstr_cmd = chdir_cmd.c_str();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(cstr_cmd);

    string str1="0,0,46";
    string _pFile = string("DS4PM");
    string _pFunc = string("DS1");

    Ainstance.set(_pFile, _pFunc);
    Binstance.a.set(_pFile, _pFunc);

    clock_t beg1 = clock();

    auto start = std::chrono::high_resolution_clock::now();
    Ainstance.callPythonDSTree(string("1,0,42,55"));
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    clock_t end1 = clock();
    cout << "single call time: in ns " << microseconds << endl;
    cout << Ainstance.callPythonDSTree(string("1,0,44,45"))<< endl;
    
    clock_t beg = clock();
    for(int i=40; i<55; ++i)
        for(int j=40; j<55; ++j)
    {
        string str = string("1,0,")+to_string(i)+","+to_string(j);
        cout << Ainstance.callPythonDSTree(str)<< ", " << flush;

    }

    cout << endl;
    clock_t end = clock();

    cout << "acutal calling python time for 225 times : " << double(end - beg)/CLOCKS_PER_SEC << endl;

    for(int i=40; i<55; ++i)
        for(int j=40; j<55; ++j)
    {
        string str = string("1,0,")+to_string(i)+","+to_string(j);
        cout << Binstance.a.callPythonDSTree(str)<< ", " << flush;

    }
    cout << endl;
    cout <<"0,0,46 : "<<  Ainstance.callPythonDSTree(string("0,0,46")) << endl;

    return 0;

}
