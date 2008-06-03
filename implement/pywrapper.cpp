/*------------------------------------------------------------------------*
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
#define PY_ARRAY_UNIQUE_SYMBOL PyArrayHandle

#include <vector>

#include "dal.h"
#include "dalArray.h"
#include "dalTable.h"
#include "dalGroup.h"
#include "dalAttribute.h"
#include "dalDataset.h"
#include "BeamFormed.h"
#include "BeamGroup.h"

using namespace DAL;
using namespace boost::python;

BOOST_PYTHON_MODULE(pydal)
{
    scope().attr("__doc__") =  "Routines for pydal.";

    import_array();
    bpl::numeric::array::set_module_and_type("numpy", "ndarray");

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
	.def("setAttribute_string", &dalTable::setAttribute_string)
	.def("setAttribute_int", &dalTable::setAttribute_int)
	.def("setAttribute_uint", &dalTable::setAttribute_uint)
	.def("setAttribute_double", &dalTable::setAttribute_double)
	.def("findAttribute", &dalTable::findAttribute)
	.def("getNumberOfRows", &dalTable::getNumberOfRows)
#ifdef WITH_CASA
	.def("openTable", &dalTable::ot_nonMStable)
	.def("setFilter", &dalTable::setFilter_boost1)
	.def("setFilter", &dalTable::setFilter_boost2)
	.def("getColumn", &dalTable::getColumn,
	     bpl::return_value_policy<bpl::manage_new_object>())
	.def("getColumn_Float32", &dalTable::getColumn_Float32,
	     bpl::return_value_policy<bpl::manage_new_object>())
	.def("getColumn_complexFloat32", &dalTable::getColumn_complexFloat32,
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
	.def("data", &dalColumn::data_boost2)
#endif
    ;

    bpl::class_<dalData>("dalData")
	.def(bpl::init<>())
	.def("get", &dalData::get_boost)
	.def("get", &dalData::get_boost2)
    ;

    bpl::class_<BeamGroup>("BeamGroup")
	.def(bpl::init<>())
	.def("summary", &BeamGroup::summary_boost)
	.def("ra", &BeamGroup::ra,
	     "Return ra value for the beam")
	.def("dec", &BeamGroup::dec,
	     "Return dec value for the beam")
	.def("n_subbands", &BeamGroup::n_subbands,
	     "Return number of subbands")
	.def("getIntensity", &BeamGroup::getIntensity_boost,
	     "Get a numpy array of intensities for a given subband")
	.def("getSubbandData_X", &BeamGroup::getSubbandData_X_boost,
	     "Get a numpy array of values for a given subband")
	.def("getSubbandData_Y", &BeamGroup::getSubbandData_Y_boost,
	     "Get a numpy array of values for a given subband")
	.def("getSubbandData_XY", &BeamGroup::getSubbandData_XY_boost,
	     "Get a numpy array of values for a given subband")
    ;

    bpl::class_<BeamFormed>("BeamFormed")
	.def(bpl::init<>())
	.def(bpl::init<string>())
	.def("summary", &BeamFormed::summary_boost,
	     "Print a summary of the file contents")
	.def("beams", &BeamFormed::beams_boost,
	     "Return a list of the beams")
	.def("source", &BeamFormed::source_boost,
	     "Return a list of the sources")
	.def("notes", &BeamFormed::notes,
	     "Return the notes from the header")
	.def("filename", &BeamFormed::filename,
	     "Return the filename from the header")
	.def("telescope", &BeamFormed::telescope,
	     "Return telescope the from the header")
	.def("nstations", &BeamFormed::nstations,
	     "Return the nstations from the header")
	.def("datatype", &BeamFormed::datatype,
	     "Return the datatype from the header")
	.def("emband", &BeamFormed::emband,
	     "Return the emband from the header")
	.def("observation_id", &BeamFormed::observation_id,
	     "Return the observation_id from the header")
	.def("proj_id", &BeamFormed::proj_id,
	     "Return the proj_id from the header")
	.def("point_ra", &BeamFormed::point_ra,
	     "Return the point_ra from the header")
	.def("point_dec", &BeamFormed::point_dec,
	     "Return the point_dec from the header")
	.def("observer", &BeamFormed::observer,
	     "Return the observer from the header")
	.def("epoch_mjd", &BeamFormed::epoch_mjd,
	     "Return the epoch_mjd from the header")
	.def("epoch_date", &BeamFormed::epoch_date,
	     "Return the epoch_date from the header")
	.def("epoch_utc", &BeamFormed::epoch_utc,
	     "Return the epoch_utc from the header")
	.def("epoch_lst", &BeamFormed::epoch_lst,
	     "Return the epoch_lst from the header")
	.def("main_beam_diam", &BeamFormed::main_beam_diam,
	     "Return the main_beam_diam from the header")
	.def("center_freq", &BeamFormed::center_freq,
	     "Return the center_freq from the header")
	.def("bandwidth", &BeamFormed::bandwidth,
	     "Return the bandwidth from the header")
	.def("total_integration_time", &BeamFormed::total_integration_time,
	     "Return the total_integration_time from the header")
	.def("breaks", &BeamFormed::breaks,
	     "Return the breaks from the header")
	.def("dispersion_measure", &BeamFormed::dispersion_measure,
	     "Return the dispersion_measure from the header")
	.def("number_of_samples", &BeamFormed::number_of_samples,
	     "Return the number_of_samples from the header")
	.def("sampling_time", &BeamFormed::sampling_time,
	     "Return the sampling_time from the header")
	.def("number_of_beams", &BeamFormed::number_of_beams,
	     "Return the number_of_beams from the header")
	.def("sub_beam_diameter", &BeamFormed::sub_beam_diameter,
	     "Return the sub_beam_diameter from the header")
	.def("weather_temperature", &BeamFormed::weather_temperature,
	     "Return the weather_temperature from the header")
	.def("weather_humidity", &BeamFormed::weather_humidity,
	     "Return the weather_humidity from the header")
	.def("station_temperatures", &BeamFormed::station_temperatures,
	     "Return the station_temperatures from the header")
	.def("getBeam", &BeamFormed::getBeam,
	     bpl::return_value_policy<bpl::manage_new_object>())
    ;
}
