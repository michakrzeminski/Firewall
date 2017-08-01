#ifndef PYTHONADAPTER_H
#define PYTHONADAPTER_H
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/eval.h>
#include <pybind11/stl.h>
#include <iostream>
#include <string>
#include "Rule.h"

namespace py = pybind11;

class PythonAdapter {
public:
    static PythonAdapter* getInstance();
    void init();
    bool addRule(Rule rule);
    bool deleteRule(Rule rule);
    void printAllRules();
    std::vector<std::string> getRules();
private:
    PythonAdapter();
    static PythonAdapter* instance;
};
#endif
