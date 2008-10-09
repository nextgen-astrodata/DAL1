/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters                                  *
 *   J.S.Masters@uva.nl                                                    *
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

#ifndef COMMON_H
#define COMMON_H

#ifndef DALBASETYPES_H
#include "dalBaseTypes.h"
#endif

/*!
  \file Common.h

  \ingroup DAL 

  \brief A collection of commonly used routines for the Data Access Library

  \author Joseph Masters
*/

namespace DAL {
  
  //! 
  long double julday (time_t seconds,
		      long *intmjd,
		      long double *fracmjd);
  
  //! Convert Modified Julian Date (mjd) to unix time  
  double mjd2unix( double mjd_time );

  //! Set attribute of type \e string
  bool h5setAttribute_string( hid_t const &obj_id,
			      std::string attrname,
                              std::string * data,
			      int size );
  //! Set attribute of type \e double
  bool h5setAttribute_double( hid_t const &obj_id,
			      std::string attrname,
                              double * data,
			      int32_t size );
  //! Set attribute of type \e int
  bool h5setAttribute_int( hid_t const &obj_id,
			   std::string attrname,
                           int * data,
			   int32_t size );
  //! Set attribute of type \e uint
  bool h5setAttribute_uint( hid_t const &obj_id,
			    std::string attrname,
                            uint * data,
			    int32_t size );
  //! Set attribute of type \e float
  bool h5setAttribute_float( hid_t const &obj_id, std::string attrname,
                             float * data, int32_t size );


  // check if an object exists in a vector
  // ---------------------------------------------------------- it_exists

  // check if an object exists in a vector
  template <class T>
  bool it_exists( std::vector<T> vec, T item )
  {
    typename std::vector<T>::iterator it;
    for ( it=vec.begin() ; it < vec.end(); it++ )
      {
        if ( item == *it )
          return true;
      }
    return false;
  }

  // ---------------------------------------------------------- list_vector_members

  template <class T>
  void list_vector_members( std::vector<T> vec )
  {
    typename std::vector<T>::iterator it;
    for ( it=vec.begin() ; it < vec.end(); it++ )
      {
        std::cerr << *it << std::endl;
      }
  }

  // ------------------------------------------- h5setAttribute

  template <class T>
  bool h5setAttribute( hid_t const &datatype, hid_t obj_id, std::string attrname,
                       T * data, int size )
  {
    hid_t   attr1    = 0;  /* Attribute identifier */
    hid_t   aid1     = 0;  /* Attribute dataspace identifier */
    hsize_t dims[1]  = { size };

    aid1  = H5Screate_simple( 1, dims, NULL );
    if ( aid1 < 0 )
      {
        std::cerr << "ERROR: Could not set attribute '" << attrname
                  << "' dataspace.\n";
        return DAL::FAIL;
      }

    attr1 = H5Acreate( obj_id, attrname.c_str(),
                        datatype, aid1, NULL, NULL );
    if ( attr1 < 0 )
      {
        std::cerr << "ERROR: Could not create attribute '" << attrname
                  << "'.\n";
        return DAL::FAIL;
      }

    if ( H5Awrite(attr1, datatype, data) < 0 )
      {
        std::cerr << "ERROR: Could not write attribute '" << attrname << "'.\n";
        return DAL::FAIL;
      }

    if ( H5Aclose( attr1 ) < 0 )
      {
        std::cerr << "ERROR: Could not close attribute '" << attrname << "'.\n";
        return DAL::FAIL;
      }

    return DAL::SUCCESS;

  }

  // ---------------------------------------------------------- getAttribute

  /*!
    \brief Get the value of an attribute.

    Get the value of an attribute.  This is different from printAttribute
    because the value of the attribute is returned into a structure
    instead of simply printing.

    \param attrname The name of the attribute you want to retrieve.

  */
  template <class T>
  bool h5getAttribute( hid_t const &obj_id, std::string attrname, T &value )
  {
    hsize_t dims;
    H5T_class_t type_class;
    size_t type_size;
    hid_t datatype = 0;
    int rank = 0;

    // Check if attribute exists
    if ( H5Aexists( obj_id, attrname.c_str()) <= 0 )
      {
        std::cerr << "ERROR: Attribute '" << attrname << "' does not exist.\n";
        return DAL::FAIL;
      }

    std::string fullname = "/";

    if ( H5LTget_attribute_ndims( obj_id, fullname.c_str(), attrname.c_str(),
                                  &rank ) < 0 )
      {
        std::cerr << "ERROR: Attribute '" << attrname << "' does not exist.\n";
        return DAL::FAIL;
      }

    if ( H5LTget_attribute_info( obj_id, fullname.c_str(), attrname.c_str(),
                                 &dims, &type_class, &type_size ) < 0 )
      {
        std::cerr << "ERROR: Could not get attribute '" << attrname
                  << "' info.\n";
        return DAL::FAIL;
      }
    else if ( H5T_FLOAT == type_class )
      {
        datatype = H5T_NATIVE_FLOAT;
      }
    else if ( H5T_INTEGER == type_class )
      {
        datatype = H5T_NATIVE_INT;
      }
    else
      {
        return DAL::FAIL;
      }

    if ( H5LTget_attribute( obj_id, fullname.c_str(), attrname.c_str(),
                            datatype, &value) < 0 )
      {
        std::cerr << "ERROR: Could not get attribute '" << attrname << "'.\n";
        return DAL::FAIL;
      }

    return DAL::SUCCESS;
  }

  /*
     std::string type specialization
   */
  inline bool h5getAttribute( hid_t const &obj_id, std::string attrname,
                              std::string &value )
  {
    hsize_t dims;
    H5T_class_t type_class;
    size_t type_size;
    int rank = 0;

    // Check if attribute exists
    if ( H5Aexists( obj_id, attrname.c_str()) <= 0 )
      {
        std::cerr << "ERROR: Attribute '" << attrname << "' does not exist.\n";
        return DAL::FAIL;
      }

    std::string fullname = "/";

    if ( H5LTget_attribute_ndims( obj_id, fullname.c_str(), attrname.c_str(),
                                  &rank ) < 0 )
      {
        std::cerr << "ERROR: Attribute '" << attrname << "' does not exist.\n";
        return DAL::FAIL;
      }

    if ( H5LTget_attribute_info( obj_id, fullname.c_str(), attrname.c_str(),
                                 &dims, &type_class, &type_size ) < 0 )
      {
        std::cerr << "ERROR: Could not get attribute '" << attrname
                  << "' info.\n";
        return DAL::FAIL;
      }

    if ( H5T_STRING == type_class )
      {
        char * data;
        data =  new char[256];
        if ( H5LTget_attribute_string( obj_id, fullname.c_str(),
                                       attrname.c_str(), data ) < 0 )
          {
            std::cerr << "ERROR: Could not get attribute '" << attrname << "'.\n";
            return DAL::FAIL;
          }

        value = stringify( data );

        return DAL::SUCCESS;
      }

    return DAL::FAIL;

  }

#ifdef PYTHON
  //! Convert Modified Julian Date (mjd) to unix time  
  bpl::numeric::array mjd2unix_boost( bpl::numeric::array mjd_time );
#endif

} // namespace DAL

#endif // COMMON_H
