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

/*!
	\file pywrapper.cc
	\ingroup DAL
	\author Joseph Masters
*/

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
