#include "PythonAdapter.h"

int add(int i, int j) {
	return i+ j;
}

PYBIND11_PLUGIN(pybind11_math) {
	py::module m("pybind11_math");
	m.def("add", &add);
	return m.ptr();
}

//bool PythonAdapter::addRule()
//{
//    return false;
//}

//bool PythonAdapter::deleteRule()
//{
//    return false;
//}

//void PythonAdapter::printAllRules(PyObject* m_self)
//{
    //call<double>(func);
//    boost::python::call_method<void>(m_self, "printAllRules");
//}
