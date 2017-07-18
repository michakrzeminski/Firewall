#ifndef PYTHONADAPTER_H
#define PYTHONADAPTER_H
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/eval.h>
#include <iostream>
#include <string>

namespace py = pybind11;

class PythonAdapter {
public:
    PythonAdapter();
    void init();
    //bool addRule();
    //bool deleteRule();
    //void printAllRules(const std::function<void(std::string)>& f);
    void printAllRules();
    //const py::list getAllRules();
    //PyObject* const m_self;
};
#endif
