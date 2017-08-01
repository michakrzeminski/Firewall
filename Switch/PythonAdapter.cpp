#include "PythonAdapter.h"
#include <typeinfo>

PYBIND11_PLUGIN(libPythonAdapter) {
	py::module m("libPythonAdapter");

	m.def("printAllRules", &PythonAdapter::printAllRules);
	//m.def("addRule", &PythonAdapter::addRule);
	//m.def("deleteRule", &PythonAdapter::deleteRule)

	return m.ptr();
}

PythonAdapter* PythonAdapter::instance = nullptr;

PythonAdapter* PythonAdapter::getInstance() {
    if(!instance) {
	instance = new PythonAdapter;
    }
    return instance;
}

bool PythonAdapter::addRule(Rule rule)
{
	auto module = py::module::import("IptablesAdapter");
	auto result = module.attr("addRule")(rule.toString());
	auto ret = result.cast<bool>();
	return ret;
}

bool PythonAdapter::deleteRule(Rule rule)
{
	auto module = py::module::import("IptablesAdapter");
	auto result = module.attr("deleteRule")(rule.toString());
	auto ret = result.cast<bool>();
	return ret;
}

PythonAdapter::PythonAdapter() {
	std::cout << "Creating PythonAdapter" <<std::endl;
	init();
}

void PythonAdapter::init() {
	std::string plugInInitCode;
	Py_Initialize();
	pybind11_init();

	plugInInitCode = "import sys\nimport os\nsys.path.append(os.getcwd())";
	PyRun_SimpleString(plugInInitCode.c_str());
}

void PythonAdapter::printAllRules() {
	auto module = py::module::import("IptablesAdapter");
	auto result = module.attr("printAllRules")("test");
	auto ret = result.cast<bool>();
	std::cout<<ret<<std::endl;
}

std::vector<std::string> PythonAdapter::getRules() {
	auto module = py::module::import("IptablesAdapter");
	auto result = module.attr("getRules")();
	auto ret = result.cast<py::list>();
	std::vector<std::string> temp = ret.cast<std::vector<std::string> >();
	return temp;
}
