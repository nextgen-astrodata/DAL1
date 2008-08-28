/*------------------------------------------------------------------------*
| $Id:: pywrapper.h 389 2007-06-13 08:47:09Z masters                    $ |
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

#include "dal.h"

using namespace DAL;
using namespace boost::python;

BOOST_PYTHON_MODULE(pydal)
{
  scope().attr("__doc__") =
    "Routines for python bindings to the data access library (DAL)."
  ;

  import_array();
  bpl::numeric::array::set_module_and_type("numpy", "ndarray");

  def( "mjd2unix", &mjd2unix_boost,
       "Convert Modified Julian Date (mjd) to unix time.\n"
       "The Unix base date is MJD 40587 and 1 mjd Day = 24 hours \n"
       "or 1440 minutes or 86400 seconds so: \n"
       "(unix seconds) = (mjd seconds) - ( unix base date in seconds )." );

//---------------------------------------------------------------- dalArray

  bpl::class_<dalArray>("dalArray")
  .def( "setAttribute_char", &dalArray::setAttribute_char,
        "Set a character attribute" )
  .def( "setAttribute_short", &dalArray::setAttribute_short,
        "Set a short integer attribute" )
  .def( "setAttribute_int", &dalArray::setAttribute_int,
        "Set a integer attribute" )
  .def( "setAttribute_uint", &dalArray::setAttribute_uint,
        "Set a unsigned integer attribute" )
  .def( "setAttribute_long", &dalArray::setAttribute_long,
        "Set a long integer attribute" )
  .def( "setAttribute_float", &dalArray::setAttribute_float,
        "Set a floating point attribute" )
  .def( "setAttribute_double", &dalArray::setAttribute_double,
        "Set a double precision floating point attribute" )
  .def( "setAttribute_string", &dalArray::setAttribute_string,
        "Set a string attribute" )
  .def( "getAttributes", &dalArray::getAttributes,
        "Print the attributes of the array." )
  .def( "extend", &dalArray::extend_boost,
        "Extend an array." )
  ;

//---------------------------------------------------------------- dalColumn

  bpl::class_<dalColumn>("dalColumn")
  .def( bpl::init<>())
  .def( bpl::init<string>())
  .def( bpl::init<string,string>())
  .def( "addMember", &dalColumn::addMember,
        "This method is useful for hdf5 files when creating a column \n"
        "with a compound datatype.  For example, use this method if an \n"
        "individual column needs to contain an int, a float, and a short." )
  .def( "getName", &dalColumn::getName,
        "Return the name of the column." )
  .def( "getSize", &dalColumn::getSize,
        "Return the size of the column for any 'normal'\n"
        "type (i.e. not compound)." )
  .def( "data", &dalColumn::data_boost1,
        "Returns the column data into a numpy array.  With no parameters \n"
        "it tries to return the entire column." )
  .def( "data", &dalColumn::data_boost2,
        "Returns the column data into a numpy array.  With one parameter, it \n"
        "returns N elements [data(N)]." )
  .def( "data", &dalColumn::data_boost3,
        "Returns the column data into a numpy array.  With three parameters, \n"
        "it returns N elements beginning at offset O [data(O,N)]." )
#ifdef WITH_CASA
  .def( "getDataType", &dalColumn::getDataType,
        "Return the datatype of the column (casa only)." )
  .def( "ndims", &dalColumn::ndims,
        "Return the number of column dimensions. (casa only)" )
  .def( "shape", &dalColumn::shape_boost,
        "Return the shape of the column (casa only)" )
  .def( "isScalar", &dalColumn::isScalar,
        "Returns true of the column is a scalar type." )
  .def( "isArray", &dalColumn::isArray,
        "Returns true of the column is an array type." )
#endif
  ;

//---------------------------------------------------------------- dalData

  bpl::class_<dalData>("dalData")
  .def( "get", &dalData::get_boost1, "Get the data." )
  .def( "get", &dalData::get_boost2, "Get the data." )
  .def( "get", &dalData::get_boost3, "Get the data." )
  ;

//---------------------------------------------------------------- dalDataset

  bpl::class_<dalDataset>("dalDataset")
  .def( bpl::init<char*, string>() )
  .def( "setAttribute_char", &dalDataset::setAttribute_char,
        "Set a character attribute" )
  .def( "setAttribute_short", &dalDataset::setAttribute_short,
        "Set a short integer attribute" )
  .def( "setAttribute_int", &dalDataset::setAttribute_int,
        "Set a integer attribute" )
  .def( "setAttribute_uint", &dalDataset::setAttribute_uint,
        "Set a unsigned integer attribute" )
  .def( "setAttribute_long", &dalDataset::setAttribute_long,
        "Set a long integer attribute" )
  .def( "setAttribute_float", &dalDataset::setAttribute_float,
        "Set a floating point attribute" )
  .def( "setAttribute_double", &dalDataset::setAttribute_double,
        "Set a double precision floating point attribute" )
  .def( "setAttribute_string", &dalDataset::setAttribute_string,
        "Set a string attribute" )
  .def( "open", &dalDataset::open,
        ( bpl::arg("dataset_name") ),
        "Opens a dataset." )
  .def( "close", &dalDataset::close,
        "Closes a dataset." )
  .def( "getType", &dalDataset::getType,
        "Get the file type of dataset." )
  .def( "createTable", &dalDataset::ct1_boost,
        bpl::return_value_policy<bpl::manage_new_object>(),
        ( bpl::arg("table_name") ),
        "Create a new table in the dataset." )
  .def( "createTable", &dalDataset::ct2_boost,
        bpl::return_value_policy<bpl::manage_new_object>(),
        ( bpl::arg("table_name"), bpl::arg("group_name") ),
        "Create a new table in the dataset." )
  .def( "createGroup", &dalDataset::createGroup,
        bpl::return_value_policy<bpl::manage_new_object>(),
        ( bpl::arg("group_name") ),
        "Create a new group in the dataset." )
  .def( "openTable", &dalDataset::ot1_boost,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Open a table in the dataset.")
  .def( "openTable", &dalDataset::ot2_boost,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Open a table in the dataset.")
  .def( "openGroup", &dalDataset::openGroup,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Open a group in the dataset.")
  .def( "createIntArray", &dalDataset::cia_boost1,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Create an integer array in the dataset." )
  .def( "createIntArray", &dalDataset::cia_boost2,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Create an integer array in the dataset." )
  .def( "createIntArray", &dalDataset::cia_boost_numarray1,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Create an integer array in the dataset." )
  .def( "createIntArray", &dalDataset::cia_boost_numarray2,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Create an integer array in the dataset." )
  .def( "createFloatArray", &dalDataset::cfa_boost,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Create an floating-point array in the dataset." )
  .def( "createFloatArray", &dalDataset::cfa_boost_numarray,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Create an floating-point array in the dataset." )
  .def( "readIntArray", &dalDataset::ria_boost,
        "Read an integer array from the dataset." )
  .def( "readFloatArray", &dalDataset::rfa_boost,
        "Read a floating-point array from the dataset." )
  .def( "createArray", &dalDataset::createArray,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Create an array from a dalData object" )
  .def( "setFilter", &dalDataset::setFilter_boost1,
        ( bpl::arg("columns") ),
        "Set a filter on the dataset (casa only)." )
  .def( "setFilter", &dalDataset::setFilter_boost2,
        ( bpl::arg("columns"), bpl::arg("conditons") ),
        "Set a filter on the dataset (casa only)." )
  .def( "listTables", &dalDataset::listTables_boost,
        "Return a list of the tables in the dataset." )
  ;

//---------------------------------------------------------------- dalGroup

  bpl::class_<dalGroup>("dalGroup")
  .def( bpl::init<>())
  .def( bpl::init<char*, void*>())
  .def( bpl::init<hid_t,char*>())
  .def( "setAttribute_char", &dalGroup::setAttribute_char,
        "Set a character attribute" )
  .def( "setAttribute_short", &dalGroup::setAttribute_short,
        "Set a short integer attribute" )
  .def( "setAttribute_int", &dalGroup::setAttribute_int,
        "Set a integer attribute" )
  .def( "setAttribute_uint", &dalGroup::setAttribute_uint,
        "Set a unsigned integer attribute" )
  .def( "setAttribute_long", &dalGroup::setAttribute_long,
        "Set a long integer attribute" )
  .def( "setAttribute_float", &dalGroup::setAttribute_float,
        "Set a floating point attribute" )
  .def( "setAttribute_double", &dalGroup::setAttribute_double,
        "Set a double precision floating point attribute" )
  .def( "setAttribute_string", &dalGroup::setAttribute_string,
        "Set a string attribute" )
  .def( "getName", &dalGroup::getName,
        "Return the group name." )
  .def( "setName", &dalGroup::setName,
        "Set the name of the group." )
  .def( "getId", &dalGroup::getId,
        "Return the group identifier." )
  .def( "createIntArray", &dalGroup::cia_boost_list,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Create an integer array in the group." )
  .def( "createFloatArray", &dalGroup::cfa_boost_list,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Create a floating point array in the group." )
  .def( "readIntArray", &dalGroup::ria_boost,
        "Read an integer array from the group." )
  ;

//---------------------------------------------------------------- dalTable

  bpl::class_<dalTable>("dalTable")
  .def( bpl::init<char*>())
  .def( "setAttribute_char", &dalTable::setAttribute_char,
        "Set a character attribute" )
  .def( "setAttribute_short", &dalTable::setAttribute_short,
        "Set a short integer attribute" )
  .def( "setAttribute_int", &dalTable::setAttribute_int,
        "Set a integer attribute" )
  .def( "setAttribute_uint", &dalTable::setAttribute_uint,
        "Set a unsigned integer attribute" )
  .def( "setAttribute_long", &dalTable::setAttribute_long,
        "Set a long integer attribute" )
  .def( "setAttribute_float", &dalTable::setAttribute_float,
        "Set a floating point attribute" )
  .def( "setAttribute_double", &dalTable::setAttribute_double,
        "Set a double precision floating point attribute" )
  .def( "setAttribute_string", &dalTable::setAttribute_string,
        "Set a string attribute" )
  .def( "openTable", &dalTable::ot_hdf5,
        "Open an hdf5 table object." )
  .def( "createTable", &dalTable::createTable,
        "Create a table object." )
  .def( "addColumn", &dalTable::addColumn,
        "Add a column to the table." )
  .def( "addComplexColumn", &dalTable::addComplexColumn,
        "Add a complex column to the table." )
  .def( "removeColumn", &dalTable::removeColumn,
        "Remove a column from the table." )
  .def( "writeDataByColNum", &dalTable::writeDataByColNum,
        "Write column data using an index." )
  .def( "appendRow", &dalTable::append_row_boost,
        "Append a row to the table." )
  .def( "appendRows", &dalTable::appendRows,
        "Append multiple rows to the table." )
  .def( "printColumns", &dalTable::printColumns,
        "Print the column tables to the screen." )
  .def( "listColumns", &dalTable::listColumns_boost,
        "Return a list of the table columns." )
  .def( "readRows", &dalTable::readRows,
        bpl::return_value_policy<bpl::return_opaque_pointer>(),
        "Read table rows." )
  .def( "getAttribute", &dalTable::getAttribute_boost,
        "Return the value of a column attribute." )
  .def( "findAttribute", &dalTable::findAttribute,
        "Return true if the attribute exists for the table." )
  .def( "getNumberOfRows", &dalTable::getNumberOfRows,
        "Return the number of rows in the table." )
#ifdef WITH_CASA
  .def( "openTable", &dalTable::ot_nonMStable,
        "Open a casa table." )
  .def( "getName", &dalTable::getName,
        "Get the name of a casa table." )
  .def( "setFilter", &dalTable::setFilter_boost1,
        "Set a filter on a casa table." )
  .def( "setFilter", &dalTable::setFilter_boost2,
        "Set a filter on a casa table." )
  .def( "getColumn", &dalTable::getColumn,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Return a casa column object." )
  .def( "getColumn_Float32", &dalTable::getColumn_Float32,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Return a 32-bit floating point casa column." )
  .def( "getColumn_complexFloat32", &dalTable::getColumn_complexFloat32,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Return a complex 32-bit floating point casa column." )
#endif
  ;

// --------------------------  BEAM FORMED ------------------------------

// ---------------------------------------------------------- BeamGroup

  bpl::class_<BeamGroup>("BeamGroup")
  .def( bpl::init<>())
  .def( "summary", &BeamGroup::summary_boost)
  .def( "ra", &BeamGroup::ra,
        "Return ra value for the beam")
  .def( "dec", &BeamGroup::dec,
        "Return dec value for the beam")
  .def( "n_subbands", &BeamGroup::n_subbands,
        "Return number of subbands")
  .def( "getIntensity", &BeamGroup::getIntensity_boost,
        "Get a numpy array of intensities for a given subband")
  .def( "getSubbandData_X", &BeamGroup::getSubbandData_X_boost,
        "Get a numpy array of values for a given subband")
  .def( "getSubbandData_Y", &BeamGroup::getSubbandData_Y_boost,
        "Get a numpy array of values for a given subband")
  .def( "getSubbandData_XY", &BeamGroup::getSubbandData_XY_boost,
        "Get a numpy array of values for a given subband")
  ;

//--------------------------------------------------------------- BeamFormed

  bpl::class_<BeamFormed>("BeamFormed")
  .def( bpl::init<>())
  .def( bpl::init<string>())
  .def( "summary", &BeamFormed::summary_boost,
        "Print a summary of the file contents" )
  .def( "beams", &BeamFormed::beams_boost,
        "Return a list of the beams" )
  .def( "source", &BeamFormed::source_boost,
        "Return a list of the sources" )
  .def( "notes", &BeamFormed::notes,
        "Return the notes from the header" )
  .def( "filename", &BeamFormed::filename,
        "Return the filename from the header" )
  .def( "telescope", &BeamFormed::telescope,
        "Return telescope the from the header" )
  .def( "nstations", &BeamFormed::nstations,
        "Return the nstations from the header" )
  .def( "datatype", &BeamFormed::datatype,
        "Return the datatype from the header" )
  .def( "emband", &BeamFormed::emband,
        "Return the emband from the header" )
  .def( "observation_id", &BeamFormed::observation_id,
        "Return the observation_id from the header" )
  .def( "proj_id", &BeamFormed::proj_id,
        "Return the proj_id from the header" )
  .def( "point_ra", &BeamFormed::point_ra,
        "Return the point_ra from the header" )
  .def( "point_dec", &BeamFormed::point_dec,
        "Return the point_dec from the header" )
  .def( "observer", &BeamFormed::observer,
        "Return the observer from the header" )
  .def( "epoch_mjd", &BeamFormed::epoch_mjd,
        "Return the epoch_mjd from the header" )
  .def( "epoch_date", &BeamFormed::epoch_date,
        "Return the epoch_date from the header" )
  .def( "epoch_utc", &BeamFormed::epoch_utc,
        "Return the epoch_utc from the header" )
  .def( "epoch_lst", &BeamFormed::epoch_lst,
        "Return the epoch_lst from the header" )
  .def( "main_beam_diam", &BeamFormed::main_beam_diam,
        "Return the main_beam_diam from the header" )
  .def( "center_freq", &BeamFormed::center_freq,
        "Return the center_freq from the header" )
  .def( "bandwidth", &BeamFormed::bandwidth,
        "Return the bandwidth from the header" )
  .def( "total_integration_time", &BeamFormed::total_integration_time,
        "Return the total_integration_time from the header" )
  .def( "breaks", &BeamFormed::breaks,
        "Return the breaks from the header" )
  .def( "dispersion_measure", &BeamFormed::dispersion_measure,
        "Return the dispersion_measure from the header" )
  .def( "number_of_samples", &BeamFormed::number_of_samples,
        "Return the number_of_samples from the header" )
  .def( "sampling_time", &BeamFormed::sampling_time,
        "Return the sampling_time from the header" )
  .def( "number_of_beams", &BeamFormed::number_of_beams,
        "Return the number_of_beams from the header" )
  .def( "sub_beam_diameter", &BeamFormed::sub_beam_diameter,
        "Return the sub_beam_diameter from the header" )
  .def( "weather_temperature", &BeamFormed::weather_temperature,
        "Return the weather_temperature from the header" )
  .def( "weather_humidity", &BeamFormed::weather_humidity,
        "Return the weather_humidity from the header" )
  .def( "station_temperatures", &BeamFormed::station_temperatures,
        "Return the station_temperatures from the header" )
  .def( "getBeam", &BeamFormed::getBeam,
        bpl::return_value_policy<bpl::manage_new_object>(),
        "Return a beam object from the file." )
  ;
}
