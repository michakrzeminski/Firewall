#include "PythonAdapter.h"

PYBIND11_PLUGIN(libPythonAdapter) {
	py::module m("libPythonAdapter");

	m.def("printAllRules", &PythonAdapter::printAllRules);
	//m.def("addRule", &PythonAdapter::addRule);
	//m.def("deleteRule", &PythonAdapter::deleteRule)

	return m.ptr();
}

void PythonAdapter::addRule(Rule rule)
{
	auto module = py::module::import("IptablesAdapter");
	auto result = module.attr("addRule")(rule.toString());
	auto ret = result.cast<bool>();
	std::cout<<ret<<std::endl;
}

void PythonAdapter::deleteRule(Rule rule)
{
	auto module = py::module::import("IptablesAdapter");
	auto result = module.attr("deleteRule")(rule.toString());
	auto ret = result.cast<bool>();
	std::cout<<ret<<std::endl;
}

PythonAdapter::PythonAdapter() {
	std::cout << "Creating PythonAdapter" <<std::endl;
	init();
	printAllRules();
	//adding rule OUTPUT -j DROP
	Rule testrule;
	testrule.chain = "OUTPUT";
	testrule.target = "DROP";
	addRule(testrule);
	printAllRules();
	//deleting rule OUTPUT -j DROP
	deleteRule(testrule);
	printAllRules();
}

void PythonAdapter::init() {
	std::string plugInInitCode;
	Py_Initialize();
	pybind11_init();

	plugInInitCode = "import sys\nimport os\nsys.path.append(os.getcwd())";
	PyRun_SimpleString(plugInInitCode.c_str());
}

void PythonAdapter::printAllRules()
{
	auto module = py::module::import("IptablesAdapter");
	auto result = module.attr("printAllRules")("test");
	auto ret = result.cast<bool>();
	std::cout<<ret<<std::endl;
}
