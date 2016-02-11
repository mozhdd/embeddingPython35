// x32


#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION	
#ifdef _DEBUG
#define _DEBUG_WAS_DEFINED
#undef _DEBUG
#endif
#include <Python.h>
#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG 1
#endif
#include <iostream>
using namespace std;
#include "C:\Python351-32\Lib\site-packages\numpy\core\include\numpy\arrayobject.h"


class Signal
{
private:
	int len;
	double* net;
	double* signal;

public:
	Signal();
	Signal(int ln, double* nt, double* sig) : len(ln), net(nt), signal(sig) {}

	int getLen() {
		return len;
	}
	double* getNet() {
		return net;
	}
	double* getSig() {
		return signal;
	}


	PyObject *makearray(double* arr, size_t size)
	{
		npy_int dim = size;
		return PyArray_SimpleNewFromData(1, &dim, NPY_DOUBLE, (void *)arr);
	}

	// inserting custom dir to sys path
	void addSysPath(char* ModPath)
	{
		PyObject *sys_path, *path;
		sys_path = PySys_GetObject("path");
		if (sys_path == NULL || !PyList_Check(sys_path))
		{
			cout << "sys path is not defined";
			return;
		}
		//FIXME
		//path = PyUnicode_FromString("D:/Python_proj/");
		path = PyUnicode_FromString(ModPath);
		PyList_Insert(sys_path, 0, path);		//inserting path to sys path
		Py_DECREF(path);
	}
	double* compute(char *modName, char *funcName, char* ModPath)
	{
		PyObject *pModule, *pFunc, *pArgs, *pValue, *pArgTuple;
		double* dArrOut = NULL;
		PyArrayObject* np_ret;

		Py_Initialize();
		addSysPath(ModPath);		// Let's add our directory to a system path
		import_array();		//initializing NumPy Arrays

		if ((pModule = PyImport_ImportModule(modName)) != NULL)
		{
			pFunc = PyObject_GetAttrString(pModule, funcName);		// get the function funcName from module
			if (pFunc && PyCallable_Check(pFunc))
			{
				pArgTuple = PyTuple_New(2);	// creating argument to extend
				npy_int dim = len;
				PyTuple_SetItem(pArgTuple, 0, PyArray_SimpleNewFromData(1, &dim, NPY_DOUBLE, (void *)net));
				PyTuple_SetItem(pArgTuple, 1, PyArray_SimpleNewFromData(1, &dim, NPY_DOUBLE, (void *)signal));

				pValue = PyObject_CallObject(pFunc, pArgTuple);			// calling function
				Py_DECREF(pArgTuple);
				if (pValue != NULL)
				{
					np_ret = reinterpret_cast<PyArrayObject*>(pValue);
					int len{ PyArray_SHAPE(np_ret)[0] };
					dArrOut = reinterpret_cast<double*>(PyArray_DATA(np_ret));

					Py_DECREF(pValue);
				}
			}
			Py_XDECREF(pFunc);										// can take NULL values as a parameters (unlike Py_DECREF)
			Py_DECREF(pModule);
		}
		Py_Finalize();
		return dArrOut;
	}

	double compute2(char *modName, char *funcName)
	{
		PyObject *pModule, *pFunc, *pArgs, *pValue, *pArgTuple;
		double np_ret;
		Py_Initialize();

		if ((pModule = PyImport_ImportModule(modName)) != NULL)
		{
			pFunc = PyObject_GetAttrString(pModule, funcName);		// get the function funcName from module
			if (pFunc && PyCallable_Check(pFunc))
			{
				pArgTuple = PyTuple_New(1);	// creating argument to extend
				PyTuple_SetItem(pArgTuple, 0, PyLong_FromLong(0));

				pValue = PyObject_CallObject(pFunc, pArgTuple);			// calling function
				Py_DECREF(pArgTuple);
				if (pValue != NULL)
				{
					np_ret = PyFloat_AsDouble(pValue);
					Py_DECREF(pValue);
				}
			}
			Py_XDECREF(pFunc);										// can take NULL values as a parameters (unlike Py_DECREF)
			Py_DECREF(pModule);
		}
		Py_Finalize();
		return np_ret;
	}

};


void print(double* arr, int len)
{
	for (int i = 0; i < len; i++)
		cout << arr[i] << " ";
	cout << endl;
}

int main(int argc, char *argv[])
{
	int len = 5;
	Signal sig(len, new double[len] {3, 4, 5, 4, 3}, new double[len] {6, 5, 4, 5, 6});

	double* res = sig.compute("myMod", "SummArr", "D:/Python_proj/");
	print(res, len);

	/*Signal sig(5, new double[5] {3, 4, 5, 4, 3}, new double[5] {6, 5, 4, 5, 6});
	double res = sig.compute2("math", "exp");
	cout << res;*/

	getchar();
	return 0;
	//test settings
}


//PyObject *makearray(double* arr, size_t size)
//{
//	npy_int dim = size;
//	return PyArray_SimpleNewFromData(1, &dim, NPY_DOUBLE, (void *)arr);
//}
//// inserting custom dir to sys path
//void addSysPath()
//{
//	PyObject *sys_path, *path;
//	sys_path = PySys_GetObject("path");
//	if (sys_path == NULL || !PyList_Check(sys_path))
//	{
//		cout << "sys path is not defined";
//		return;
//	}
//	path = PyUnicode_FromString("D:/Python_proj/");
//	PyList_Insert(sys_path, 0, path);		//inserting path to sys path
//	Py_DECREF(path);
//}
////sending any array to python by looping the array, returning number (works)
//double compute3(double *x, char *modName, char *funcName)
//{
//	PyObject *pModule, *pFunc, *pArgs, *pValue, *pArgTuple;
//	double result = NULL;
//	double dArr[5] = { 1.2, 2.3, 4.3, 0.1, 2 };
//	double* dArrOut;
//	PyArrayObject* np_ret;
//
//	Py_Initialize();
//	addSysPath();		// Let's add our directory to a system path
//						//import_array();		//initializing NumPy Arrays
//
//
//	if ((pModule = PyImport_ImportModule(modName)) != NULL)
//	{
//		pFunc = PyObject_GetAttrString(pModule, funcName);		// get the function funcName from module
//		if (pFunc && PyCallable_Check(pFunc))
//		{
//			pArgTuple = PyTuple_New(1);	// creating argument to extend
//										//PyTuple_SetItem(pArgTuple, 0, reinterpret_cast<PyObject*>(makeDPyArr(dArr, 5)));
//			PyTuple_SetItem(pArgTuple, 0, makearray(dArr, 5));
//
//			pValue = PyObject_CallObject(pFunc, pArgTuple);			// calling function
//			Py_DECREF(pArgTuple);
//			if (pValue != NULL)
//			{
//				np_ret = reinterpret_cast<PyArrayObject*>(pValue);
//				int len{ PyArray_SHAPE(np_ret)[0] };
//				dArrOut = reinterpret_cast<double*>(PyArray_DATA(np_ret));
//
//				cout << "Printing output array" << endl;
//				for (int i{}; i < len; i++)
//					cout << dArrOut[i] << ' ';
//				cout << endl;
//
//				Py_DECREF(pValue);
//			}
//		}
//		Py_XDECREF(pFunc);										// can take NULL values as a parameters (unlike Py_DECREF)
//		Py_DECREF(pModule);
//	}
//	Py_Finalize();
//	return result;
//}
//double* compute4(double *dArr, int len, char *modName, char *funcName)
//{
//	PyObject *pModule, *pFunc, *pArgs, *pValue, *pArgTuple;
//	double* dArrOut = NULL;
//	PyArrayObject* np_ret;
//
//	Py_Initialize();
//	addSysPath();		// Let's add our directory to a system path
//	import_array();		//initializing NumPy Arrays
//
//	if ((pModule = PyImport_ImportModule(modName)) != NULL)
//	{
//		pFunc = PyObject_GetAttrString(pModule, funcName);		// get the function funcName from module
//		if (pFunc && PyCallable_Check(pFunc))
//		{
//			pArgTuple = PyTuple_New(1);	// creating argument to extend
//			PyTuple_SetItem(pArgTuple, 0, makearray(dArr, len));
//
//			pValue = PyObject_CallObject(pFunc, pArgTuple);			// calling function
//			Py_DECREF(pArgTuple);
//			if (pValue != NULL)
//			{
//				np_ret = reinterpret_cast<PyArrayObject*>(pValue);
//				int len{ PyArray_SHAPE(np_ret)[0] };
//				dArrOut = reinterpret_cast<double*>(PyArray_DATA(np_ret));
//
//				Py_DECREF(pValue);
//			}
//		}
//		Py_XDECREF(pFunc);										// can take NULL values as a parameters (unlike Py_DECREF)
//		Py_DECREF(pModule);
//	}
//	Py_Finalize();
//	return dArrOut;
//}