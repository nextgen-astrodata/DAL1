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
#include "Common.h"
#endif

namespace DAL
  {


  // ---------------------------------------------------------- julday

  long double julday(time_t seconds,long *intmjd, long double *fracmjd)
  {
    long double dayfrac, jd, sec;
    int year, yday;
    int hour, min;
    struct tm *ptr = NULL;

    unsigned int nd;

    ptr = gmtime(&seconds);

    hour = ptr->tm_hour;
    min = ptr->tm_min;
    sec = (long double)ptr->tm_sec;

    year = ptr->tm_year;
    yday = ptr->tm_yday + 1;

    dayfrac = ( (sec/60.0L + (long double) min)/60.0L + \
                (long double)hour)/24.0L;
    nd = year * 365;
    nd += (year - 1)/4;
    nd += yday + 2415020;

    *intmjd = nd - 2400001;
    *fracmjd = dayfrac;

    jd = (long double)nd + dayfrac - 0.5L;

    return jd;
  }


  // ---------------------------------------------------------- mjd2unix

  // To convert Modified Julian Date (mjd) to unix time
  double mjd2unix( double mjd_time )
  {
    // The Unix base date is MJD 40587.
    // and 1 mjd Day = 24 hours or 1440 minutes or 86400 seconds
    // so (unix seconds) = (mjd seconds) - ( unix base date in seconds )
    return ( mjd_time - (40587 * 86400) );
  }


  // ------------------------------------------- h5setAttribute_string

  /*!
    \brief Add a string attribute.

    Add a string attribute to the hdf5 object.

    \param obj_id The hdf5 object identifier.
    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \param size The dimension of the attribute.
    \return DAL::FAIL or DAL::SUCCESS
   */
  bool h5setAttribute_string( hid_t const &obj_id, std::string attrname,
                              std::string * data, int size )
  {
    hid_t att       = 0;
    hid_t dataspace = 0;
    hsize_t dims[1] = { size };

    char ** string_attr = (char**)malloc( size * sizeof(char*) );
    for ( int ii = 0; ii < size; ii++ )
      {
        string_attr[ii] = (char*)malloc(MAX_COL_NAME_SIZE * sizeof(char));
        strcpy( string_attr[ii], data[ii].c_str() );
      }

    hid_t type = H5Tcopy (H5T_C_S1);
    if ( type < 0 )
      {
        std::cerr << "ERROR: Could not set attribute '" << attrname
                  << "' type.\n";
        return DAL::FAIL;
      }

    if ( H5Tset_size(type, H5T_VARIABLE) < 0 )
      {
        std::cerr << "ERROR: Could not set attribute '" << attrname
                  << "' size.\n";
        return DAL::FAIL;
      }

    dataspace = H5Screate_simple(1, dims, NULL);
    if ( dataspace < 0 )
      {
        std::cerr << "ERROR: Could not set attribute '" << attrname
                  << "' dataspace.\n";
        return DAL::FAIL;
      }

    att = H5Acreate1( obj_id, attrname.c_str(), type, dataspace, H5P_DEFAULT);
    if ( att < 0 )
      {
        std::cerr << "ERROR: Could not create attribute '" << attrname << "'.\n";
        return DAL::FAIL;
      }

    if ( H5Awrite( att, type, string_attr ) < 0 )
      {
        std::cerr << "ERROR: Could not write attribute '" << attrname << "'.\n";
        return DAL::FAIL;
      }

    if (  H5Aclose( att ) < 0 )
      {
        std::cerr << "ERROR: Could not close attribute '" << attrname << "'.\n";
        return DAL::FAIL;
      }

    for ( int ii = 0; ii < size; ii++ )
      {
        free( string_attr[ii] );
      }
    free( string_attr );

    return DAL::SUCCESS;
  }

// ---------------------------------------------------- h5setAttribute_double
  /*!
    \brief Add a double-precision floating point attribute.

    Add a double-precision floating point attribute to the hdf5 object.

    \param obj_id The hdf5 object identifier.
    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \param size The dimension of the attribute.
    \return DAL::FAIL or DAL::SUCCESS
   */
  bool h5setAttribute_double( hid_t const &obj_id, std::string attrname, double * data,
                              int32_t size )
  {
    hid_t   datatype = H5T_NATIVE_DOUBLE;
    return h5setAttribute( datatype, obj_id, attrname, data, size );
  }

// --------------------------------------------------------- h5setAttribute_int
  /*!
    \brief Add an integer attribute.

    Add an integer attribute to an hdf5 object.

    \param obj_id The hdf5 object identifier.
    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \param size The dimension of the attribute.
    \return DAL::FAIL or DAL::SUCCESS
   */
  bool h5setAttribute_int( hid_t const &obj_id, std::string attrname,
                           int * data, int32_t size )
  {
    hid_t   datatype = H5T_NATIVE_INT;
    return h5setAttribute( datatype, obj_id, attrname, data, size );
  }

// ------------------------------------------------------- h5setAttribute_uint
  /*!
    \brief Add an unsigned integer attribute.

    Add an unsigned integer attribute to an hdf5 object.

    \param obj_id The hdf5 object identifier.
    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \param size The dimension of the attribute.
    \return DAL::FAIL or DAL::SUCCESS
   */
  bool h5setAttribute_uint( hid_t const &obj_id, std::string attrname,
                            uint * data, int32_t size )
  {
    hid_t   datatype = H5T_NATIVE_UINT;
    return h5setAttribute( datatype, obj_id, attrname, data, size );
  }

// ------------------------------------------------------- h5setAttribute_float
  /*!
    \brief Add an unsigned integer attribute.

    Add an unsigned integer attribute to an hdf5 object.

    \param obj_id The hdf5 object identifier.
    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \param size The dimension of the attribute.
    \return DAL::FAIL or DAL::SUCCESS
   */
  bool h5setAttribute_float( hid_t const &obj_id, std::string attrname,
                             float * data, int32_t size )
  {
    hid_t   datatype = H5T_NATIVE_FLOAT;
    return h5setAttribute( datatype, obj_id, attrname, data, size );
  }

#ifdef PYTHON


  // ---------------------------------------------------------- mjd2unix_boost


  // To convert Modified Julian Date (mjd) to unix time
  bpl::numeric::array mjd2unix_boost( bpl::numeric::array mjd_time )
  {
    // The Unix base date is MJD 40587.
    // and 1 mjd Day = 24 hours or 1440 minutes or 86400 seconds
    // so (unix seconds) = (mjd seconds) - ( unix base date in seconds )
    int array_size = bpl::len( mjd_time );
    double adjustment_factor = 3506716800; // 40587 * 86400;
    for ( int idx=0; idx < array_size; idx++ )
      {
        mjd_time[ idx ] = bpl::extract<double>( mjd_time[ idx ] ) - adjustment_factor;
      }

    return mjd_time;
  }
#endif

} // namespace DAL
