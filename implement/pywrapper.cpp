/*-------------------------------------------------------------------------*
| $Id:: cs1.h 389 2007-06-13 08:47:09Z baehren                          $ |
*-------------------------------------------------------------------------*
***************************************************************************
*   Copyright (C) 2007 by Joseph Masters                                  *
*   jmasters@science.uva.nl                                               *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include <boost/python.hpp>
// #include <num_util.h>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/implicit.hpp>
#include <boost/python/class.hpp>
#include <boost/python/list.hpp>
#include <boost/python/numeric.hpp>
#include <boost/python/tuple.hpp>

//using namespace boost;
namespace bpl = boost::python;
#include <vector>

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
typedef struct opaque_ *opaque;
BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(opaque_)

BOOST_PYTHON_MODULE(pydal)
{
//     bpl::import_array();
//     numeric::array::set_module_and_type("numpy", "ndarray");


    bpl::class_<dalDataset>("dalDataset")
	.def(bpl::init<char*, string>())
        .def("open", &dalDataset::open, bpl::arg("datasetname"),
		"Opens a dataset.")
        .def("close", &dalDataset::close, "Closes a dataset.")
        .def("getType", &dalDataset::getType, "Get the file type of dataset.")
	.def("createTable", &dalDataset::ct1_boost,
		bpl::return_value_policy<bpl::manage_new_object>(),
		"Create a new table in the dataset.")
	.def("createTable", &dalDataset::ct2_boost,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createGroup", &dalDataset::createGroup,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("openTable", &dalDataset::ot1_boost,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("openTable", &dalDataset::ot2_boost,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("openGroup", &dalDataset::openGroup,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createIntArray", &dalDataset::cia_boost1,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createIntArray", &dalDataset::cia_boost2,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createIntArray", &dalDataset::cia_boost_numarray1,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createIntArray", &dalDataset::cia_boost_numarray2,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createFloatArray", &dalDataset::cfa_boost,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createFloatArray", &dalDataset::cfa_boost_numarray,
		bpl::return_value_policy<bpl::manage_new_object>())
// 	.def("createComplexArray", &dalDataset::cca_boost,
// 		bpl::return_value_policy<bpl::manage_new_object>())
	.def("readIntArray", &dalDataset::ria_boost)
	.def("readFloatArray", &dalDataset::rfa_boost)
	.def("createArray", &dalDataset::createArray,
		bpl::return_value_policy<bpl::manage_new_object>(),
		"Create an array from a dalData object")
	.def("setFilter", &dalDataset::setFilter_boost1)
	.def("setFilter", &dalDataset::setFilter_boost2)
    ;

    bpl::class_<dalGroup>("dalGroup")
	.def(bpl::init<>())
	.def(bpl::init<char*, void*>())
 	.def(bpl::init<hid_t,char*>())
        .def("getName", &dalGroup::getName)
        .def("setName", &dalGroup::setName)
        .def("getId", &dalGroup::getId)
        .def("getAttributes", &dalGroup::getAttributes)
        .def("setAttribute_string", &dalGroup::setAttribute_string)
        .def("setAttribute_int", &dalGroup::setAttribute_int)
        .def("setAttribute_uint", &dalGroup::setAttribute_uint)
        .def("setAttribute_double", &dalGroup::setAttribute_double)
	.def("getAttribute", &dalGroup::getAttribute,
		bpl::return_value_policy<bpl::return_opaque_pointer>())
	.def("createIntArray", &dalGroup::cia_boost1,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createIntArray", &dalGroup::cia_boost2,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createIntArray", &dalGroup::cia_boost_numarray1,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createIntArray", &dalGroup::cia_boost_numarray2,
		bpl::return_value_policy<bpl::manage_new_object>())
/*	.def("createIntArray", &dalGroup::cia_boost,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createIntArray", &dalGroup::cia_boost_numarray,
		bpl::return_value_policy<bpl::manage_new_object>())*/
	.def("createFloatArray", &dalGroup::cfa_boost,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("createFloatArray", &dalGroup::cfa_boost_numarray,
		bpl::return_value_policy<bpl::manage_new_object>())
	.def("readIntArray", &dalGroup::ria_boost)
    ;
    bpl::class_<dalTable>("dalTable")
	.def(bpl::init<char*>())
        .def("getAttributes", &dalTable::getAttributes)
	.def("openTable", &dalTable::ot_hdf5)
	.def("openTable", &dalTable::ot_nonMStable)
	.def("createTable", &dalTable::createTable)
	.def("addColumn", &dalTable::addColumn)
	.def("addArrayColumn", &dalTable::addArrayColumn)
	.def("addComplexColumn", &dalTable::addComplexColumn)
	.def("removeColumn", &dalTable::removeColumn)
	.def("writeDataByColNum", &dalTable::writeDataByColNum)
	.def("appendRow", &dalTable::append_row_boost)
	.def("appendRows", &dalTable::appendRows)
	.def("listColumns", &dalTable::listColumns)
	.def("readRows", &dalTable::readRows,
		bpl::return_value_policy<bpl::return_opaque_pointer>())
	.def("getAttribute", &dalTable::getAttribute,
		bpl::return_value_policy<bpl::return_opaque_pointer>())
//         .def("printColumns", &dalTable::printColumns)
        .def("setAttribute_string", &dalTable::setAttribute_string)
        .def("setAttribute_int", &dalTable::setAttribute_int)
        .def("setAttribute_uint", &dalTable::setAttribute_uint)
        .def("setAttribute_double", &dalTable::setAttribute_double)
	.def("findAttribute", &dalTable::findAttribute)
	.def("getNumberOfRows", &dalTable::getNumberOfRows)
#ifdef WITH_CASA
//	.def("openTable", &dalTable::ot_ms1)
// 	.def("openTable", &dalTable::ot_ms2)
	.def("setFilter", &dalTable::setFilter_boost1)
	.def("setFilter", &dalTable::setFilter_boost2)
	.def("getColumn", &dalTable::getColumn,
		bpl::return_value_policy<bpl::manage_new_object>())
#endif
    ;

    bpl::class_<dalArray>("dalArray")
	.def("getAttributes", &dalArray::getAttributes)
	.def("setAttribute_string", &dalArray::setAttribute_string)
	.def("setAttribute_int", &dalArray::sai_boost)
	.def("setAttribute_float", &dalArray::saf_boost)
	.def("extend", &dalArray::extend_boost)
    ;

    bpl::class_<dalIntArray,bpl::bases<dalArray> >("dalIntArray")
    ;

    bpl::class_<dalColumn>("dalColumn")
	.def(bpl::init<>())
	.def(bpl::init<string>())
	.def(bpl::init<string,string>())
	.def("addMember", &dalColumn::addMember)
	.def("getName", &dalColumn::getName)
	.def("getDataType", &dalColumn::getDataType)
	.def("getSize", &dalColumn::getSize)
	.def("ndims", &dalColumn::ndims)
	.def("shape", &dalColumn::shape_boost)
	.def("isScalar", &dalColumn::isScalar)
	.def("isArray", &dalColumn::isArray)
#ifdef WITH_CASA
 	.def("data", &dalColumn::data_boost)
#endif
    ;

    bpl::class_<dalData>("dalData")
	.def(bpl::init<>())
	.def("get", &dalData::get_boost)
    ;
}
