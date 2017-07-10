#include "PythonAdapter.h"

#include <boost/python/module.hpp>
#include <boost/python/class.hpp>

//BOOST_PYTHON_MODULE(iptables_module);

bool PythonAdapter::addRule()
{
    return false;
}

bool PythonAdapter::deleteRule()
{
    return false;
}

void PythonAdapter::printAllRules(PyObject* m_self)
{
    //call<double>(func);
    call_method<void>(m_self, "printAllRules");
}