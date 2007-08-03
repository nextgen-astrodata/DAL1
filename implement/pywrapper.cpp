#include <boost/python.hpp>
using namespace boost;

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
    python::class_<dalDataset>("dalDataset")
	.def(python::init<char*>())
	.def(python::init<char*, string>())
        .def("open", &dalDataset::open)
        .def("close", &dalDataset::close)
        .def("getType", &dalDataset::getType)
/*	.def("openTable", &dalDataset::openTable,
		return_value_policy<manage_new_object>())*/
	.def("createTable", &dalDataset::createTable,
		python::return_value_policy<python::manage_new_object>())
	.def("createTableInGroup", &dalDataset::createTableInGroup,
		python::return_value_policy<python::manage_new_object>())
	.def("createGroup", &dalDataset::createGroup,
		python::return_value_policy<python::manage_new_object>())
    ;
    python::class_<dalGroup>("dalGroup")
	.def(python::init<>())
	.def(python::init<char*, void*>())
 	.def(python::init<hid_t,char*>())
        .def("getName", &dalGroup::getName)
        .def("setName", &dalGroup::setName)
        .def("getId", &dalGroup::getId)
        .def("getAttributes", &dalGroup::getAttributes)
        .def("setAttribute_string", &dalGroup::setAttribute_string)
        .def("setAttribute_int", &dalGroup::setAttribute_int)
        .def("setAttribute_uint", &dalGroup::setAttribute_uint)
        .def("setAttribute_double", &dalGroup::setAttribute_double)
    ;
    python::class_<dalTable>("dalTable")
        .def("getAttributes", &dalTable::getAttributes)
	.def("openTable", &dalTable::openTable)
	.def("createTable", &dalTable::createTable)
	.def("addColumn", &dalTable::addColumn)
	.def("addArrayColumn", &dalTable::addArrayColumn)
	.def("addComplexColumn", &dalTable::addComplexColumn)
	.def("removeColumn", &dalTable::removeColumn)
	.def("writeDataByColNum", &dalTable::writeDataByColNum)
	.def("appendRow", &dalTable::appendRow,
		python::return_value_policy<python::return_opaque_pointer>())
	.def("appendRows", &dalTable::appendRows)
	.def("listColumns", &dalTable::listColumns)
	.def("readRows", &dalTable::readRows)
/*	.def("getAttribute", &dalTable::getAttribute,
		python::return_value_policy<python::manage_new_object>())*/
        .def("printColumns", &dalTable::printColumns)
        .def("setAttribute_string", &dalTable::setAttribute_string)
        .def("setAttribute_int", &dalTable::setAttribute_int)
        .def("setAttribute_uint", &dalTable::setAttribute_uint)
        .def("setAttribute_double", &dalTable::setAttribute_double)
	.def("findAttribute", &dalTable::findAttribute)
    ;
    python::class_<dalColumn>("dalColumn")
	.def(python::init<>())
	.def(python::init<string>())
	.def(python::init<string,string>())
	.def("addMember", &dalColumn::addMember)
	.def("getName", &dalColumn::getName)
	.def("getType", &dalColumn::getType)
	.def("getSize", &dalColumn::getSize)
    ;
}
