/*
 * Program: cpp2python_img.cpp
 * Author: Sushil RAUT
 * Email: robosushil@gmail.com/ sushil@hiroshima-u.ac.jp
 * Description: This code executes ISCamera capture, Uses pyobject to call function in python file, sends and receive live stream from cpp2 python
 * Compile: g++ -o cpp2python_img cpp2python_img.cpp -lpthread -I/usr/include/python3.6 -I/usr/include/python3.6/numpy -lpython3.6m -L/usr/local/lib `pkg-config opencv4 --libs`
 * Run:./cpp2python_img
 */
#include <iostream>
#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "arrayobject.h"
#include "opencv2/opencv.hpp"
#include <fstream>
#include <string>
using namespace std;
using namespace cv;

#define IMG_WIDTH	640
#define IMG_HEIGHT	480

PyObject* m_PyDict,  *m_PyTransReceiver[3];
PyObject* m_PyModule;

int main(int argc, char **argv, wchar_t **wargv){
	//const char* argv[] = { "program name", "arg1", "arg2", NULL };
	//int argc = sizeof(argv) / sizeof(char*) - 1;
	
	
	Py_Initialize();
	//PySys_SetArgv(argc, (wchar_t**)wargv);
    PyObject* sys_path = PySys_GetObject("path");
    PyList_Append(sys_path, PyUnicode_FromString("."));
    import_array1(-1);
    m_PyModule = PyImport_ImportModule("pythonScript");

	cout<<"m_PyModule-"<<m_PyModule<<endl;
	cv::VideoCapture cap;
    if (!cap.open(0)) return 0;
    cv::Mat frame;
    
    while (m_PyModule != NULL && waitKey(1)!=27) {

        cap >> frame;
        cv::imshow("frame", frame);
        m_PyDict = PyModule_GetDict(m_PyModule);
        m_PyTransReceiver[0] = PyDict_GetItemString(m_PyDict, "Received_Image_1");
        m_PyTransReceiver[1] = PyDict_GetItemString(m_PyDict, "Received_Image_2");
        m_PyTransReceiver[2] = PyDict_GetItemString(m_PyDict, "main");
        if (m_PyTransReceiver != NULL && !frame.empty()) {

            int nElem = IMG_WIDTH * IMG_HEIGHT * 3;
            uchar* m = new uchar[nElem];
            std::memcpy(m, frame.data, nElem * sizeof(uchar));
            npy_intp mdim[] = { IMG_HEIGHT, IMG_WIDTH, 3 };// the dimensions of the matrix
            PyObject* mat = PyArray_SimpleNewFromData(3, mdim, NPY_UINT8, (void*)m);  // convert the cv::Mat to numpy.array
            PyObject* args = Py_BuildValue("(O)", mat);
            PyObject* pData = PyObject_CallObject(m_PyTransReceiver[0], args);

            uchar* data = (uchar*)PyByteArray_AsString(pData);
            cv::Mat img(frame.rows, frame.cols, CV_8UC1, data);

            cv::imshow("Receied from python", img);
         

            Py_XDECREF(mat);
            Py_XDECREF(pData);
            Py_XDECREF(args);
            delete[] m;
        }
        
    }

    return 0;
    
	}
