/***************************************************************************
 *   Copyright (C) 2010                                                    *
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

  \brief Python bindings for the dalColumn class

  \author Lars B&auml;hren
*/

#include "pydal.h"

using DAL::dalColumn;

// ==============================================================================
//
//                                                     Additional Python wrappers
//
// ==============================================================================

//_____________________________________________________________________________
//                                                                  shape_boost

bpl::tuple dalColumn::shape_boost()
{
  std::vector<int> lclvals;
  lclvals = shape();
  bpl::list lcllist;
  
  std::vector<int>::iterator iter;
  for (iter=lclvals.begin(); iter < lclvals.end(); iter++) {
    lcllist.append(*iter);
  }
  
  bpl::tuple lcl_tuple;
  lcl_tuple = bpl::make_tuple(lcllist);
  
  return lcl_tuple;
}

//_____________________________________________________________________________
//                                                                  data_boost1

bpl::numeric::array dalColumn::data_boost1()
{
  return data_boost3( 0, -1 );
}

//_____________________________________________________________________________
//                                                                  data_boost2

bpl::numeric::array dalColumn::data_boost2( int32_t length )
{
  return data_boost3( 0, length );
  
}

//_____________________________________________________________________________
//                                                                  data_boost2

bpl::numeric::array dalColumn::data_boost3 (int64_t offset,
					    int32_t length)
{
  if ( MSCASATYPE == filetype ) {
#ifdef DAL_WITH_CASA
    
    if ("unknown" == casa_datatype) {
      bpl::list lcllist;
      return num_util::makeNum(lcllist);
    }
    
    try {
      if ( isScalar() ) {
	switch ( casa_col_desc.dataType() )
	  {
	  case casa::TpInt:
	    {
	      rosc_int = new casa::ROScalarColumn<casa::Int>( *casa_column );
	      scalar_vals_int = rosc_int->getColumn();
	      data_object = new dalData( filetype, dal_INT, shape(), nrows() );
	      data_object->data = (int *)scalar_vals_int.getStorage(deleteIt);
	      return data_object->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpBool:
	    {
	      rosc_bool = new casa::ROScalarColumn<bool>( *casa_column );
	      scalar_vals_bool = rosc_bool->getColumn();
	      data_object = new dalData( filetype, dal_BOOL, shape(), nrows() );
	      data_object->data = (int *)scalar_vals_bool.getStorage(deleteIt);
	      return data_object->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpDouble:
	    {
	      rosc_dbl = new casa::ROScalarColumn<casa::Double>( *casa_column );
	      scalar_vals_dbl = rosc_dbl->getColumn();
	      data_object = new dalData( filetype, dal_DOUBLE, shape(), nrows() );
	      data_object->data = (double *)scalar_vals_dbl.getStorage(deleteIt);
	      return data_object->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpComplex:
	    {
	      rosc_comp = new casa::ROScalarColumn<casa::Complex>( *casa_column );
	      scalar_vals_comp = rosc_comp->getColumn();
	      data_object = new dalData( filetype, dal_COMPLEX, shape(), nrows() );
	      data_object->data =
		(std::complex<float> *)scalar_vals_comp.getStorage(deleteIt);
	      return data_object->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpString:
	    {
	      rosc_string = new casa::ROScalarColumn<casa::String>( *casa_column );
	      scalar_vals_string = rosc_string->getColumn();
	      data_object = new dalData( filetype, dal_STRING, shape(), nrows() );
	      data_object->data =
		(std::string *)scalar_vals_string.getStorage(deleteIt);
	      return data_object->get_boost3( offset, length );
	    }
	    break;
	    /************************************
	     * ADD MORE TYPES HERES
	     ************************************/
	  default:
	    {
	      std::cerr << "dalColumn::data() Column type not yet supported."
			<< endl;
	      bpl::list tmp_list;
	      tmp_list.append(0);
	      bpl::numeric::array nadata(tmp_list);
	      return nadata;
	    }
	  }
      }
      else if ( isArray() ) {
	switch ( casa_col_desc.dataType() )
	  {
	  case casa::TpInt:
	    {
	      roac_int = new casa::ROArrayColumn<casa::Int>( *casa_column );
	      array_vals_int = roac_int->getColumn();
	      data_object = new dalData( filetype, dal_INT, shape(), nrows() );
	      data_object->data = (int *)array_vals_int.getStorage(deleteIt);
	      return data_object->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpDouble:
	    {
	      roac_dbl = new casa::ROArrayColumn<casa::Double>( *casa_column );
	      array_vals_dbl = roac_dbl->getColumn();
	      data_object = new dalData( filetype, dal_DOUBLE, shape(), nrows() );
	      data_object->data = (double *)array_vals_dbl.getStorage(deleteIt);
	      return data_object->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpComplex:
	    {
	      roac_comp = new casa::ROArrayColumn<casa::Complex>( *casa_column );
	      array_vals_comp = roac_comp->getColumn();
	      data_object = new dalData( filetype, dal_COMPLEX, shape(), nrows() );
	      data_object->data =
		(std::complex<float> *)array_vals_comp.getStorage(deleteIt);
	      return data_object->get_boost3( offset, length );
	    }
	    break;
	  case casa::TpString:
	    {
	      roac_string = new casa::ROArrayColumn<casa::String>( *casa_column );
	      array_vals_string = roac_string->getColumn();
	      data_object = new dalData( filetype, dal_STRING, shape(), nrows() );
	      data_object->data = (std::string *)array_vals_string.getStorage(deleteIt);
	      return data_object->get_boost3( offset, length );
	    }
	    break;
	    /************************************
	     * ADD MORE TYPES HERES
	     ************************************/
	  default:
	    {
	      std::cerr <<
		"dalColumn::data() Column type not yet supported.\n";
	      
	      bpl::list tmp_list;
	      tmp_list.append(0);
	      bpl::numeric::array nadata(tmp_list);
	      return nadata;
	    }
	  }
      }
      else {
	std::cerr << "[dalColumn::data] Column is neither scalar nor array."
		  << " Do not know how to handle."
		  << std::endl;
	
	bpl::list tmp_list;
	tmp_list.append(0);
	bpl::numeric::array nadata(tmp_list);
	return nadata;
      }
    }
    catch (casa::AipsError x) {
      std::cerr << "ERROR: " << x.getMesg() << endl;
      bpl::list tmp_list;
      tmp_list.append(0);
      bpl::numeric::array nadata(tmp_list);
      return nadata;
    }
#endif // DAL_WITH_CASA
  }
  else if ( H5TYPE == filetype ) {
    std::cerr << "ERROR: hdf5 not supported [dalColumn.data - python]\n";
    int start = 0;
    int length = -1;
    data_object = data(start,length);
    return data_object->get_boost3( offset, length );
  }
  else {
    std::cerr <<
      "ERROR: filetype not supported [dalColumn.data - python]\n";
  }
  
  bpl::list tmp_list;
  tmp_list.append(0);
  bpl::numeric::array nadata(tmp_list);
  return nadata;
}

// ==============================================================================
//
//                                                      Wrapper for class methods
//
// ==============================================================================

void export_dalColumn ()
{
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

