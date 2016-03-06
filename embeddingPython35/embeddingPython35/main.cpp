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
#include "IncPath.h"
//#include "C:\Python351-32\Lib\site-packages\numpy\core\include\numpy\arrayobject.h"
#include <fstream>

class Signal
{
private:
	int len, len2;
	double* lattice;
	double* signal;

public:
	Signal();
	Signal(int ln, int ln2, double* lt, double* sig) : len(ln), len2(ln2), lattice(lt), signal(sig) {}

	int getLen() {
		return len;
	}
	double* getNet() {
		return lattice;
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
	void addSysPath()
	{
		PyObject *sys_path, *path;
		sys_path = PySys_GetObject("path");
		if (sys_path == NULL || !PyList_Check(sys_path))
		{
			cout << "sys path is not defined";
			return;
		}
		//path = PyUnicode_FromString("D:/Python_proj/");
		path = PyUnicode_FromString(modPath);
		PyList_Insert(sys_path, 0, path);		//inserting path to sys path
		Py_DECREF(path);
	}

	double* compute()
	{
		PyObject *pModule, *pFunc, *pArgs, *pValue, *pArgTuple;
		double* dArrOut = NULL;
		PyArrayObject* np_ret;

		Py_Initialize();
		addSysPath();		// Let's add our directory to a system path
		import_array();				//initializing NumPy Arrays

		if ((pModule = PyImport_ImportModule(modName)) != NULL)
		{
			pFunc = PyObject_GetAttrString(pModule, funcName);		// get the function funcName from module
			if (pFunc && PyCallable_Check(pFunc))
			{
				pArgTuple = PyTuple_New(2);	// creating argument to extend
				npy_int dim = len;
				PyTuple_SetItem(pArgTuple, 0, PyArray_SimpleNewFromData(1, &dim, NPY_DOUBLE, (void *)lattice));
				PyTuple_SetItem(pArgTuple, 1, PyArray_SimpleNewFromData(1, &dim, NPY_DOUBLE, (void *)signal));

				pValue = PyObject_CallObject(pFunc, pArgTuple);			// calling function
				Py_DECREF(pArgTuple);
				if (pValue != NULL)
				{
					np_ret = reinterpret_cast<PyArrayObject*>(pValue);
					int leng{ PyArray_SHAPE(np_ret)[0] };
					len = leng;
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

	//double* computeMatr(char *modName, char *funcName)
	//{
	//	PyObject *pModule, *pFunc, *pArgs, *pValue, *pArgTuple;
	//	double* dArrOut = NULL;
	//	PyArrayObject* np_ret;

	//	Py_Initialize();
	//	addSysPath();		// Let's add our directory to a system path
	//	import_array();				//initializing NumPy Arrays

	//	if ((pModule = PyImport_ImportModule(modName)) != NULL)
	//	{
	//		pFunc = PyObject_GetAttrString(pModule, funcName);		// get the function funcName from module
	//		if (pFunc && PyCallable_Check(pFunc))
	//		{

	//			pArgTuple = PyTuple_New(1);	// creating argument to extend
	//			npy_int dim = len;
	//			npy_intp dims[2]{len, len2};
	//			//PyTuple_SetItem(pArgTuple, 0, PyArray_SimpleNewFromData(2, dims, NPY_DOUBLE, (void *)signal));
	//			const int ND{ 2 };
	//			PyArrayObject *np_arg = reinterpret_cast<PyArrayObject*>(PyArray_SimpleNewFromData(ND, dims, NPY_LONGDOUBLE,
	//				reinterpret_cast<void*>(signal)));
	//			PyTuple_SetItem(pArgTuple, 0, reinterpret_cast<PyObject*>(np_arg));




	//			PyObject_CallObject(pFunc, pArgTuple);			// calling function
	//			Py_DECREF(pArgTuple);

	//		}
	//		Py_XDECREF(pFunc);										// can take NULL values as a parameters (unlike Py_DECREF)
	//		Py_DECREF(pModule);
	//	}
	//	Py_Finalize();
	//	return dArrOut;
	//}


};


void print(double* arr, int len)
{
	for (int i = 0; i < len; i++)
		cout << arr[i] << "\n";
	cout << endl;
}
double* readArr(string path)
{
	int Len = 709;
	double* arr = new double[Len];
	ifstream file(path);

	if (file.is_open())
	{
		double tmp;
		for (int i = 0; i < Len; ++i)
		{
			file >> tmp;
			arr[i] = tmp;
		}
	}
	return arr;
}

int main(int argc, char *argv[])
{
	int len = 3, len2 = 3;
	//Signal sig(len, new double[len] {3, 4, 5, 4, 3}, new double[len] {6, 5, 4, 5, 6});

	//double* res = sig.compute("myMod", "SummArr");
	//print(res, len);

	
	double **x = new double* [len];
	/*x[0] = new double[4]{13.75, NAN, NAN, 65.4392 };
	x[1] = new double[4]{ 13.8, NAN, NAN, 65.4392 };
	x[2] = new double[4]{ 719.0, NAN, NAN, -15.0};*/

	x[0] = new double[len2]{ 13.75, 1, 65.4392 };
	x[1] = new double[len2]{ 13.8, 2, 65.4392 };
	x[2] = new double[len2]{ 719.0, 2, -15.0 };


	len = 709;
	Signal sig(len, len, readArr("D:\\Python_proj\\Las\\dept.txt"), readArr("D:\\Python_proj\\Las\\GR1.txt"));
	double* res = sig.compute();
	//print(res, sig.getLen());



	//Signal sig(len, len2,  NULL, x);
	//sig.computeMatr("myMod", "myPrint");
	
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