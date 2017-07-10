#ifndef PYTHONADAPTER_H
#define PYTHONADAPTER_H
#include <boost/python.hpp>
#include <iostream>
#include <string>

using namespace boost::python;

class PythonAdapter {
public:
    PythonAdapter(PyObject* self) : m_self(self) {}

    bool addRule();
    bool deleteRule();
    void printAllRules(PyObject* m_self);
    //const py::list getAllRules();
    PyObject* const m_self;
};
#endif