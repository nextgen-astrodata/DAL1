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

#ifndef DALCOMMON_H
#include "dalCommon.h"
#endif

namespace DAL {

  // ============================================================================
  //
  //  Conversion routines
  //
  // ============================================================================

  // --------------------------------------------------------------------- julday

  long double julday (time_t seconds,
		      long *intmjd,
		      long double *fracmjd)
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

  // ------------------------------------------------------------------- mjd2unix
  
  /*!
    \param mjd_time -- The time as Modified Julian Date
    
    \return unix -- The time as UNIX seconds
  */
  double mjd2unix (double mjd_time)
  {
    // The Unix base date is MJD 40587.
    // and 1 mjd Day = 24 hours or 1440 minutes or 86400 seconds
    // so (unix seconds) = (mjd seconds) - ( unix base date in seconds )
    return ( mjd_time - (40587 * 86400) );
  }
  
  // ============================================================================
  //
  //  Service routines
  //
  // ============================================================================
  
  // ============================================================================
  //
  //  Access to HDF5 attributes
  //
  // ============================================================================

  // -------------------------------------------------------- h5attribute_summary
  
  /*!
    \param os           -- Output stream to which the summary is written
    \param attribute_id -- HDF5 identifier pointing at the attribute to show
  */
  void h5attribute_summary (std::ostream &os,
                            hid_t const &attribute_id)
  {
    bool status (true);

    /*
     * Datatype of the attribute
     */
    hid_t datatype_id        = H5Aget_type (attribute_id);
    hsize_t datatype_size    = H5Tget_size (datatype_id);
    bool datatype_is_integer = H5Tdetect_class (datatype_id,H5T_INTEGER);
    bool datatype_is_float   = H5Tdetect_class (datatype_id,H5T_FLOAT);
    bool datatype_is_string  = H5Tdetect_class (datatype_id,H5T_STRING);

    os << "\t-- Datatype ID             = " << datatype_id  << endl;
    os << "\t-- Datatype size [Bytes]   = " << datatype_size << endl;
    os << "\t-- Datatype is H5T_INTEGER = " << datatype_is_integer << endl;
    os << "\t-- Datatype is H5T_FLOAT   = " << datatype_is_float << endl;
    os << "\t-- Datatype is H5T_STRING  = " << datatype_is_string << endl;

    H5Tclose (datatype_id);

    /*
     * Dataspace of the attribute
     */
    int rank (0);
    hid_t dataspace_id       = H5Aget_space (attribute_id);
    bool dataspace_is_simple = H5Sis_simple(dataspace_id);
    herr_t h5error;

    try {
      rank = H5Sget_simple_extent_ndims (dataspace_id);
    }
    catch (std::string message) {
      cerr << "[h5attribute_summary] " << message << endl;
      status = false;
    }
    
    os << "\t-- Dataspace ID            = " << dataspace_id << endl;
    os << "\t-- Dataspace is simple?    = " << dataspace_is_simple << endl;
    os << "\t-- Rank of the data array  = " << rank << endl;
    
    if (dataspace_id > 0) {
      h5error = H5Sclose (dataspace_id);
      h5error = H5Eclear1 ();
    }
  }
  
  // ------------------------------------------------------ h5setAttribute_string
  
  /*!
    \brief Add a string attribute.
    
    Add a string attribute to the hdf5 object.

    \param obj_id The hdf5 object identifier.
    \param attrname The name of the attribute you want to add.
    \param data The value of the attribute you want to add.
    \param size The dimension of the attribute.
    \return DAL::FAIL or DAL::SUCCESS
   */
  bool h5setAttribute_string( hid_t const &obj_id,
			      std::string attrname,
                              std::string * data,
			      int size )
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

    att = H5Acreate( obj_id, attrname.c_str(), type, dataspace, NULL, NULL );
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

#ifdef HAVE_CASA

  // ------------------------------------------------------------- h5get_quantity

  /*!
    \param value -- Identifier for the attribute storing the numerical value of
           the quantity.
    \param unit  -- Identifier for the attribute storing the physical unit of
           the quantity
    \param location_id -- Identifier of the structure within the file, to which
           the attribut is attached to.

    \return quantity -- The physical quantity.
  */
  Quantity h5get_quantity (Attributes const &value,
				 Attributes const &unit,
				 hid_t const &location_id)
  {
    if (location_id > 0)
      {
        bool status (true);
        double qValue;
        std::string qUnit;
        // retrieve the value of the quantity
        status *= h5get_attribute(qValue,
                                  attribute_name(value),
                                  location_id);
        // retrieve the unit of the quantity
        status *= h5get_attribute(qUnit,
                                  attribute_name(unit),
                                  location_id);
        // put together the Quantity object
        if (status)
          {
	    Quantity val = Quantity (qValue,
						 casa::Unit(qUnit));
            return val;
          }
        else
          {
            return Quantity();
          }
      }
    else
      {
        cerr << "[h5get_quantity] Unusable ID for HDF5 object!" << endl;
        return Quantity();
      }
  }

  // ------------------------------------------------------------ h5get_direction

  /*!
    \param value -- Identifier for the attribute storing the numerical value of
           the quantity.
    \param unit  -- Identifier for the attribute storing the physical unit of
           the quantity
    \param frame -- Identifier for the attribute storing the identifier for the
           reference frame within which the physical quantity is defined.
    \param location_id -- Identifier of the structure within the file, to which
           the attribut is attached to.

    \return direction -- The physical quantity.
  */
  MDirection h5get_direction (Attributes const &value,
                              Attributes const &unit,
                              Attributes const &frame,
                              hid_t const &location_id)
  {
    MDirection dir = MDirection();

    if (location_id > 0) {
      bool status (true);
      casa::Vector<double> values;
      casa::Vector<casa::String> units;
      MDirection::Types tp;
      std::string refcode;
      // retrieve the numerical values of the position
      status *= h5get_attribute(values,
				attribute_name(value),
				location_id);
      // retrieve the physical units of the position
      status *= h5get_attribute(units,
				attribute_name(unit),
				location_id);
      // retrieve the frame of the position
      status *= h5get_attribute(refcode,
				attribute_name(frame),
				location_id);
      status *= MDirection::getType (tp,refcode);
      // assemble MDirection object
      if (status) {
	int nofValues = values.nelements();
	int nofUnits  = units.nelements();
	if (nofValues == 2 && nofValues == nofUnits)
	  {
	    // create MDirection object
	    dir = MDirection ( Quantity( values(0), units(0)),
				     Quantity( values(1), units(1)),
				     MDirection::Ref(tp));
	    // return result
	    return dir;
	  }
	else {
	  cerr << "[h5get_direction] Mismatching number of values and units!"
	       << endl;
	  dir = MDirection();
	}
      }
      else
	{
	  cerr << "[h5get_direction] Error retrieving components for MDirection!" << endl;
	  dir = MDirection();
	}
    }
    else {
      cerr << "[h5get_direction] Unusable ID for HDF5 object!" << endl;
      dir = MDirection();
    }
    
    return dir;
  }
  
  // ------------------------------------------------------------- h5get_position
  
  MPosition h5get_position (Attributes const &value,
                            Attributes const &unit,
                            Attributes const &frame,
                            hid_t const &location_id)
  {
    MPosition obs = MPosition();

    if (location_id > 0)
      {
        bool status (true);
        casa::Vector<double> values;
        casa::Vector<casa::String> units;
        MPosition::Types tp;
        std::string refcode;
        // retrieve the numerical values of the position
        status *= h5get_attribute(values,
                                  attribute_name(value),
                                  location_id);
        // retrieve the physical units of the position
        status *= h5get_attribute(units,
                                  attribute_name(unit),
                                  location_id);
        // retrieve the frame of the position
        status *= h5get_attribute(refcode,
                                  attribute_name(frame),
                                  location_id);
        status *= MPosition::getType (tp,refcode);
        // assemble MPosition object
        if (status)
          {
            int nofValues = values.nelements();
            int nofUnits  = units.nelements();
            if (nofValues == 3 && nofValues == nofUnits)
              {
                // create MPosition object
                obs = MPosition ( casa::MVPosition( Quantity( values(0), units(0)),
                                                    Quantity( values(1), units(1)),
                                                    Quantity( values(2), units(2))),
                                  MPosition::Ref(tp));
                // return result
                return obs;
              }
            else
              {
                cerr << "[h5get_position] Mismatching number of values and units!" << endl;
                obs = MPosition();
              }
          }
        else
          {
            cerr << "[h5get_position] Error retrieving components for MPosition!" << endl;
            obs = MPosition();
          }
      }
    else
      {
        cerr << "[h5get_position] Unusable ID for HDF5 object!" << endl;
        obs = MPosition();
      }

    return obs;
  }

#endif

  // ============================================================================
  //
  //  Boost.Python wrappers
  //
  // ============================================================================
  
#ifdef PYTHON
  
  // ------------------------------------------------------------- mjd2unix_boost
  
  /*!
    - The Unix base date is MJD 40587.
    - 1 mjd Day = 24 hours or 1440 minutes or 86400 seconds
    - (unix seconds) = (mjd seconds) - ( unix base date in seconds )

    \param mjd_time The time as Modified Julian Date.
  */
  bpl::numeric::array mjd2unix_boost( bpl::numeric::array mjd_time )
  {
    int array_size = bpl::len( mjd_time );
    double unix_base_time (40587);
    double seconds_per_day (86400);
    double adjustment_factor = unix_base_time*seconds_per_day;
    for ( int idx=0; idx < array_size; idx++ )
      {
        mjd_time[ idx ] = bpl::extract<double>( mjd_time[ idx ] ) - adjustment_factor;
      }

    return mjd_time;
  }
#endif
  
} // namespace DAL
