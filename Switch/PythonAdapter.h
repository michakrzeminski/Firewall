#ifndef PYTHONADAPTER_H
#define PYTHONADAPTER_H
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/eval.h>
#include <iostream>
#include <string>
#include "Rule.h"

namespace py = pybind11;

class PythonAdapter {
public:
    PythonAdapter();
    void init();
    void addRule(Rule rule);
    void deleteRule(Rule rule);
    void printAllRules();
    //const py::list getAllRules();
};
#endif
