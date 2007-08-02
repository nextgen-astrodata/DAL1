#include <boost/python.hpp>
using namespace boost::python;

#include "dal.h"
#include "dalArray.h"
#include "dalTable.h"
#include "dalGroup.h"
#include "dalAttribute.h"
#include "dalDataset.h"

// BOOST_PYTHON_FUNCTION_OVERLOADS(
// 	ct_overloads,
// 	dalDataset::createTable,
// 	1, 2
// )

BOOST_PYTHON_MODULE(dal)
{
    class_<dalGroup>("dalGroup")
	.def(init<>())
// 	.def(init<char*, void*>())
 	.def(init<hid_t,char*>())
        .def("getName", &dalGroup::getName)
        .def("setName", &dalGroup::setName)
        .def("getId", &dalGroup::getId)
        .def("getAttributes", &dalGroup::getAttributes)
        .def("setAttribute_string", &dalGroup::setAttribute_string)
        .def("setAttribute_int", &dalGroup::setAttribute_int)
        .def("setAttribute_uint", &dalGroup::setAttribute_uint)
        .def("setAttribute_double", &dalGroup::setAttribute_double)
    ;
    class_<dalDataset>("dalDataset")
	.def(init<char*>())
	.def(init<char*, string>())
        .def("open", &dalDataset::open)
        .def("close", &dalDataset::close)
        .def("getType", &dalDataset::getType)
/*	.def("openTable", &dalDataset::openTable,
		return_value_policy<manage_new_object>())*/
	.def("createTable", &dalDataset::createTable,
		return_value_policy<manage_new_object>())
	.def("createTableInGroup", &dalDataset::createTable,
		return_value_policy<manage_new_object>())
    ;
    class_<dalTable>("dalTable")
         .def("getAttributes", &dalTable::getAttributes)
         .def("printColumns", &dalTable::printColumns)
    ;
}
