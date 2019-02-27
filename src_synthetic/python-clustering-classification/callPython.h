#include <iostream>
//#include "python/Python.h"
#include "Python.h"
#include <ctime>
#include <string>
#include <vector>
#include <deque>

using namespace std;

int fun()
{
    Py_Initialize();
    if (!Py_IsInitialized()){
        return -1;
    }

    //string path = "/vol/home-vol2/pda/zhaobo/CEP_load_shedding/src_PM_Distribution_test/python-clustering-classification";
    string path = "/home/zhaobo/HU/CEP_load_shedding/src_PM_Distribution_test/python-clustering-classification";
    //string path = "/home/bo/CEP_load_shedding/src_PM_Distribution_test/python-clustering-classification";
    string chdir_cmd = string("sys.path.append(\"") + path + "\")";
    const char* cstr_cmd = chdir_cmd.c_str();
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(cstr_cmd);

    PyObject* moduleName = PyString_FromString("DS4PM");
    PyObject* pModule = PyImport_Import(moduleName);
    if (!pModule){
        cout << "[ERROR] Python get module failed." << endl;
        return 0;
    }

    cout << "[INFO] Python get module succeed." << endl;

    //PyObject* pv = PyObject_GetAttrString(pModule, "DS");
    //if (!pv || !PyCallable_Check(pv)) {
    //    cout << "[ERROR] Can't find funftion (DS)" << endl;
    //    return 0;
    //}
   
    int i = 1;
    string DS = string("DS")+to_string(i);

    PyObject* pv1 = PyObject_GetAttrString(pModule,DS.c_str()); 
    if (!pv1 || !PyCallable_Check(pv1)) {
        cout << "[ERROR] Can't find funftion (DS1)" << endl;
        return 0;
    }

    vector<vector<deque<int> > > buffers;
    buffers.resize(4);
    for(int i=0;i<4;++i)
        buffers[i].resize(6);


    cout << "[INFO] Get function (DS) (DS1) succeed." << endl;

    clock_t beg = clock();

    PyObject* args = PyTuple_New(1);

    //PyObject* arg0 = Py_BuildValue("i",0);
    PyObject* arg1 = Py_BuildValue("i",0);
    PyObject* arg2 = Py_BuildValue("i",45);

    
    
    PyObject* arg0 = Py_BuildValue("s","0,0,45");
    PyTuple_SetItem(args, 0, arg0);


    string str1="1,0,45,50";

    PyObject* arg_str_s = PyTuple_New(1);
    //PyObject* arg_str = Py_BuildValue("s","1,0,45,50");
    PyObject* arg_str = Py_BuildValue("s",str1.c_str());
    PyTuple_SetItem(arg_str_s, 0, arg_str);
    

    clock_t beg1 = clock();

    PyTuple_SetItem(args, 0, arg0);
    PyTuple_SetItem(args, 1, arg1);
    PyTuple_SetItem(args, 2, arg2);

    PyObject* pRet = PyObject_CallObject(pv1, args);
    clock_t end = clock();

    clock_t beg2_1 = clock(); 
    PyObject* pRet1 = PyObject_CallObject(pv1, arg_str_s);
    clock_t end2_1 = clock(); 

    clock_t beg3_1 = clock(); 
    PyObject* pRet3 = PyObject_CallObject(pv1, args);
    clock_t end3_1 = clock(); 

    if (pRet){
        int result = PyInt_AsLong(pRet);
        cout << "result for 45: " << result << endl << "py computation time with intial python values" << double(end - beg) / CLOCKS_PER_SEC << endl << "without intial python values" << double(end - beg1)/CLOCKS_PER_SEC << endl;
    }

    if (pRet1){
        int result = PyInt_AsLong(pRet1);
        for(auto&& iter: buffers[result])
        {
                iter.push_back(6);
        }
        for(auto&& deq: buffers[result])
            cout << deq.size() << endl;

        cout << "result for (45,50): " << result << endl << "without intial python values" << double(end2_1 - beg2_1)/CLOCKS_PER_SEC << endl;
    }
    
    cout << "2nd computing time " << double(end2_1 - beg2_1)/CLOCKS_PER_SEC << endl; 
    cout << "3rd computing time " << double(end3_1 - beg3_1)/CLOCKS_PER_SEC << endl; 





    Py_Finalize();

    return 0;

}
