/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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
  \file pydal_core_dalColumn.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::dalColumn class

  \author Lars B&auml;hren
*/

#include "pydal.h"
#include <core/dalColumn.h>

using std::string;
using DAL::dalColumn;

// ==============================================================================
//
//                                                     Additional Python wrappers
//
// ==============================================================================

//_____________________________________________________________________________
//                                                                  shape_boost

boost::python::tuple dalColumn::shape_boost()
{
  std::vector<int> lclvals;
  lclvals = shape();
  boost::python::list lcllist;
  
  std::vector<int>::iterator iter;
  for (iter=lclvals.begin(); iter < lclvals.end(); iter++) {
    lcllist.append(*iter);
  }
  
  boost::python::tuple lcl_tuple;
  lcl_tuple = boost::python::make_tuple(lcllist);
  
  return lcl_tuple;
}

//_____________________________________________________________________________
//                                                                  data_boost1

boost::python::numeric::array dalColumn::data_boost1()
{
  return data_boost3( 0, -1 );
}

//_____________________________________________________________________________
//                                                                  data_boost2

boost::python::numeric::array dalColumn::data_boost2( int32_t length )
{
  return data_boost3( 0, length );
  
}

//_____________________________________________________________________________
//                                                                  data_boost2

boost::python::numeric::array dalColumn::data_boost3 (int64_t offset,
						      int32_t length)
{
  if (itsFiletype.type()==DAL::dalFileType::CASA_MS) {
#ifdef DAL_WITH_CASA
    
    if ("unknown" == itsCasaDatatype) {
      boost::python::list lcllist;
      return num_util::makeNum(lcllist);
    }
    
    try {
      if ( isScalar() ) {
	switch ( itsColumnDesc.dataType() )
	  {
	  case casa::TpInt:
	    {
	      rosc_int = new casa::ROScalarColumn<casa::Int>( *itsROTableColumn );
	      casa::Vector<int> data = rosc_int->getColumn();
	      itsColumnData = new dalData( itsFiletype, dal_INT, shape(), nofRows() );
	      itsColumnData->data = (int *)data.getStorage(deleteIt);
	      return itsColumnData->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpBool:
	    {
	      rosc_bool = new casa::ROScalarColumn<bool>( *itsROTableColumn );
	      casa::Vector<bool> data = rosc_bool->getColumn();
	      itsColumnData = new dalData( itsFiletype, dal_BOOL, shape(), nofRows() );
	      itsColumnData->data = (int *)data.getStorage(deleteIt);
	      return itsColumnData->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpDouble:
	    {
	      rosc_dbl = new casa::ROScalarColumn<casa::Double>( *itsROTableColumn );
	      casa::Vector<double> data = rosc_dbl->getColumn();
	      itsColumnData = new dalData( itsFiletype, dal_DOUBLE, shape(), nofRows() );
	      itsColumnData->data = (double *)data.getStorage(deleteIt);
	      return itsColumnData->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpComplex:
	    {
	      rosc_comp = new casa::ROScalarColumn<casa::Complex>( *itsROTableColumn );
	      casa::Vector<casa::Complex> data = rosc_comp->getColumn();
	      itsColumnData = new dalData( itsFiletype, dal_COMPLEX, shape(), nofRows() );
	      itsColumnData->data =
		(std::complex<float> *)data.getStorage(deleteIt);
	      return itsColumnData->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpString:
	    {
	      rosc_string = new casa::ROScalarColumn<casa::String>( *itsROTableColumn );
	      casa::Vector<casa::String> data = rosc_string->getColumn();
	      itsColumnData = new dalData( itsFiletype, dal_STRING, shape(), nofRows() );
	      itsColumnData->data =
		(std::string *)data.getStorage(deleteIt);
	      return itsColumnData->get_boost3( offset, length );
	    }
	    break;
	    /************************************
	     * ADD MORE TYPES HERES
	     ************************************/
	  default:
	    {
	      std::cerr << "dalColumn::data() Column type not yet supported."
			<< endl;
	      boost::python::list tmp_list;
	      tmp_list.append(0);
	      boost::python::numeric::array nadata(tmp_list);
	      return nadata;
	    }
	  }
      }
      else if ( isArray() ) {
	switch ( itsColumnDesc.dataType() )
	  {
	  case casa::TpInt:
	    {
	      roac_int = new casa::ROArrayColumn<casa::Int>( *itsROTableColumn );
	      casa::Array<int> data = roac_int->getColumn();
	      itsColumnData = new dalData( itsFiletype, dal_INT, shape(), nofRows() );
	      itsColumnData->data = (int *)data.getStorage(deleteIt);
	      return itsColumnData->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpDouble:
	    {
	      roac_dbl = new casa::ROArrayColumn<casa::Double>( *itsROTableColumn );
	      casa::Array<double> data = roac_dbl->getColumn();
	      itsColumnData = new dalData( itsFiletype, dal_DOUBLE, shape(), nofRows() );
	      itsColumnData->data = (double *)data.getStorage(deleteIt);
	      return itsColumnData->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpComplex:
	    {
	      roac_comp = new casa::ROArrayColumn<casa::Complex>( *itsROTableColumn );
	      casa::Array<casa::Complex> data = roac_comp->getColumn();
	      itsColumnData = new dalData( itsFiletype, dal_COMPLEX, shape(), nofRows() );
	      itsColumnData->data =
		(std::complex<float> *)data.getStorage(deleteIt);
	      return itsColumnData->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpString:
	    {
	      roac_string = new casa::ROArrayColumn<casa::String>( *itsROTableColumn );
	      casa::Array<casa::String> data = roac_string->getColumn();
	      itsColumnData = new dalData( itsFiletype, dal_STRING, shape(), nofRows() );
	      itsColumnData->data = (std::string *)data.getStorage(deleteIt);
	      return itsColumnData->get_boost3( offset, length );
	    }
	    break;
	    /************************************
	     * ADD MORE TYPES HERES
	     ************************************/
	  default:
	    {
	      std::cerr <<
		"dalColumn::data() Column type not yet supported.\n";
	      
	      boost::python::list tmp_list;
	      tmp_list.append(0);
	      boost::python::numeric::array nadata(tmp_list);
	      return nadata;
	    }
	  }
      }
      else {
	std::cerr << "[dalColumn::data] Column is neither scalar nor array."
		  << " Do not know how to handle."
		  << std::endl;
	
	boost::python::list tmp_list;
	tmp_list.append(0);
	boost::python::numeric::array nadata(tmp_list);
	return nadata;
      }
    }
    catch (casa::AipsError x) {
      std::cerr << "ERROR: " << x.getMesg() << endl;
      boost::python::list tmp_list;
      tmp_list.append(0);
      boost::python::numeric::array nadata(tmp_list);
      return nadata;
    }
#endif // DAL_WITH_CASA
  }
  else if (itsFiletype.type()==DAL::dalFileType::HDF5) {
    std::cerr << "ERROR: hdf5 not supported [dalColumn.data - python]\n";
    int start = 0;
    int length = -1;
    itsColumnData = data(start,length);
    return itsColumnData->get_boost3( offset, length );
  }
  else {
    std::cerr <<
      "ERROR: filetype not supported [dalColumn.data - python]\n";
  }
  
  boost::python::list tmp_list;
  tmp_list.append(0);
  boost::python::numeric::array nadata(tmp_list);
  return nadata;
}

// ==============================================================================
//
//                                                      Wrapper for class methods
//
// ==============================================================================

void export_dalColumn ()
{
  
  //________________________________________________________
  // Specialisation of overloaded methods
  
  void (dalColumn::*summary1)() 
    = &dalColumn::summary;
  void (dalColumn::*summary2)(std::ostream &) 
    = &dalColumn::summary;
  
  //________________________________________________________
  // Bindings for class and its methods
  
  boost::python::class_<dalColumn>("dalColumn")
    /* Construction */
    .def( boost::python::init<>())
    .def( boost::python::init<string const &>())
    .def( boost::python::init<string const &,
			      string const &>())
    .def( boost::python::init<hid_t const &,
			      hid_t const &,
			      DAL::dalFileType const &,
			      string const &,
			      string const &,
			      string const &>())
    /* Public methods */
    .def( "addMember", &dalColumn::addMember,
	  "This method is useful for hdf5 files when creating a column \n"
	  "with a compound datatype.  For example, use this method if an \n"
	  "individual column needs to contain an int, a float, and a short." )
    .def( "setName", &dalColumn::setName,
	  "Set the name of the column." )
    .def( "name", &dalColumn::name,
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
    .def("summary",
	 summary1,
	 "Summary of the object's internal parameters and status.")
    .def("summary",
	 summary2,
	 "Summary of the object's internal parameters and status.")
#ifdef DAL_WITH_CASA
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
  
}

