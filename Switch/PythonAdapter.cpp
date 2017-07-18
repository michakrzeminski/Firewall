#include "PythonAdapter.h"
#include <exception>

//std::function<void(std::string)> printRules;

struct Rule {
	std::string protocol;
};

PYBIND11_PLUGIN(libPythonAdapter) {
	py::module m("libPythonAdapter");

	m.def("printAllRules", &PythonAdapter::printAllRules);

	py::class_<Rule>(m, "Rule")
		.def(py::init<>())
		.def_readwrite("protocol", &Rule::protocol);
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

PythonAdapter::PythonAdapter() {
	std::cout << "Creating PythonAdapter" <<std::endl;
	init();
	try {
		printAllRules();
	}
	catch(std::exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void PythonAdapter::init() {
	std::cout<<"adapter init"<<std::endl;
	std::string plugInInitCode;
	Py_Initialize();
	pybind11_init();

	plugInInitCode = "import sys\nimport os\nsys.path.append(os.getcwd())";

	PyRun_SimpleString(plugInInitCode.c_str());
}

void PythonAdapter::printAllRules()
{
	std::cout<<"Debug: printAllRules"<<std::endl;
	//printRules = f;
	auto module = py::module::import("IptablesAdapter");
	auto result = module.attr("printAllRules")("bum");
	std::string ret = result.cast<std::string>();
	std::cout<<ret<<std::endl;
}
