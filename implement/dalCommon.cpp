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
    int year (0);
    int yday (0);
    int hour (0);
    int min (0);
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

  // ----------------------------------------------------------------- h5get_name

  /*!
    \retval name     -- Name of the object (if set or defined).
    \param object_id -- Identifier for the objects of which to retrieve the
           name.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_name (std::string &name,
                   hid_t const &object_id)
  {
    herr_t h5error        = 0;
    H5I_type_t objectType = H5Iget_type (object_id);

    /* Check: does the object ID point to a valid object type? */

    if (objectType == H5I_BADID)
      {
        return false;
      }

    /* If the object ID is valid, we try to retrieve its name. */

    ssize_t name_length   = 0;
    size_t buffer_size    = 10;
    char *buffer;

    buffer = new char[buffer_size];

    if (objectType == H5I_FILE)
      {
        // first call to get proper length of the name string
        name_length = H5Fget_name (object_id,
                                   buffer,
                                   buffer_size);
        // clear error stack
        h5error = H5Eclear1 ();
        // retrieve the file name
        if (name_length > 0)
          {
            // release the previously allocated buffer ...
            delete [] buffer;
            // ... and readjust it to the proper values retrieved above
            buffer_size = name_length+1;
            buffer      = new char[buffer_size];
            // second function call to retrieve the object's name
            name_length = H5Fget_name (object_id,
                                       buffer,
                                       buffer_size);
            name = buffer;
          }
        else
          {
            std::cerr << "[h5get_name] Object name of zero characters!" << endl;
            return false;
          }
      }
    else
      {
        name_length = H5Iget_name (object_id,
                                   buffer,
                                   buffer_size);
        h5error = H5Eclear1 ();

        if (name_length > 0)
          {
            // release the previously allocated buffer ...
            delete [] buffer;
            // ... and readjust it to the proper values retrieved above
            buffer_size = name_length+1;
            buffer      = new char[buffer_size];
            // second function call to retrieve the object's name
            name_length = H5Iget_name (object_id,
                                       buffer,
                                       buffer_size);
            name = buffer;
          }
        else
          {
            return false;
          }
      }

    return true;
  }

  // ----------------------------------------------------------------- h5get_name

  /*!
    \retval name     -- Name of the object (if set or defined).
    \param object_id -- Identifier for the objects of which to retrieve the
           name.
    \param index     -- Transient index identifying the object whose name to
           retrieve.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_name (std::string &name,
                   hid_t const &object_id,
                   hsize_t const &index)
  {
//     H5I_type_t objectType = H5Iget_type (object_id);
    hsize_t nofObjects    = 0;
    herr_t h5error        = 0;

    // Check if the transient index is within the valid range

    h5error = H5Gget_num_objs(object_id,
                              &nofObjects);

    if (index > nofObjects)
      {
        std::cerr << "[h5get_name] Running index outside valid range!"
                  << endl;
        return false;
      }

    /*
      Get the name of the object identified by the transient index; first
      function call is to retrieve retrieve the number of characters in the
      object's name.
    */

    ssize_t name_length = 0;
    size_t buffer_size  = 10;
    char *buffer;

    // first function call to get the number of characters in the object's name
    try
      {
        buffer      = new char[buffer_size];
        name_length = H5Gget_objname_by_idx (object_id,
                                             index,
                                             buffer,
                                             buffer_size);
      }
    catch (std::string message)
      {
        std::cerr << "[h5get_name] Error calling H5Gget_objname_by_idx; "
                  << message
                  << endl;
        return false;
      }

    if (name_length > 0)
      {
        // release the previously allocated buffer ...
        delete [] buffer;
        // ... and readjust it to the proper values retrieved above
        buffer_size = name_length+1;
        buffer      = new char[buffer_size];
        // second function call to retrieve the object's name
        name_length = H5Gget_objname_by_idx (object_id,
                                             index,
                                             buffer,
                                             buffer_size);
        name = buffer;
      }
    else
      {
        std::cerr << "[h5get_name] Object name of zero characters!" << endl;
        return false;
      }

    // release the previously allocated buffer ...
    delete [] buffer;

    return true;
  }

  // ------------------------------------------------------------- h5get_filename

  /*!
    \retval filename -- Name of the HDF5 file within which the object is
            contained
    \param object_id -- Identifier for the objects of which to retrieve the
           name.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_filename (std::string &filename,
                       hid_t const &object_id)
  {

    //________________________________________________________________
    // Basic check for the provided object ID
    
    if (object_id < 1) {
      std::cout << "[h5get_filename] Invalid value of object identifier;"
		<< " must be greater zero!"
		<< endl;
      return false;
    }
    
    bool status (true);
    herr_t h5error (0);
    H5I_type_t objectType;

    /*
     * If the provided object ID does not belong to a file already, we first need
     * to obtain extactly this ID based on the object's ID.
     */
    objectType = H5Iget_type(object_id);

    if (objectType == H5I_BADID) {
      std::cerr << "[h5get_filename] Bad object type - aborting now!" << endl;
      return false;
    }
    else if (objectType == H5I_FILE) {
      status = h5get_name (filename,
			   object_id);
    }
    else {
      hid_t file_id = H5Iget_file_id (object_id);
      status = h5get_name (filename,
			   file_id);
      h5error = H5Fclose (file_id);
    }
    
    /* clean up the error message buffer */
    h5error = H5Eclear1();
    
    return status;
  }
  
  // ============================================================================
  //
  //  Get HDF5 attributes
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

  // ------------------------------------------------------------------ attr_info

  /*!
    \param loc_id -- Identifier for the HDF5 object - file, group, dataset,
           array - the attribute is attached to.
    \param name   -- Name of the attribute
    \param opdata -- not used, but necessary as a placeholder
    \return error status
  */
  herr_t attr_info (hid_t loc_id,
		    const char * name,
		    void * opdata)
  {
    /*  Open the attribute using its name.  */
    hid_t attr = H5Aopen_name(loc_id, name);

    /* Get attribute datatype, dataspace, rank, and dimensions.  */
    hid_t atype  = H5Aget_type(attr);
    hid_t aspace = H5Aget_space(attr);

    hsize_t * sdim = new hsize_t[ 64 ];
    herr_t ret = H5Sget_simple_extent_dims(aspace, sdim, NULL);
    delete [] sdim;
    sdim = NULL;

    opdata = opdata;  // avoid compiler warnings of unused parameter

    if (H5T_INTEGER == H5Tget_class(atype))
      {
        int point_out = 0;
        ret  = H5Aread(attr, H5T_NATIVE_INT, &point_out);
        cout << name << " = " << point_out << endl;
      }
    else if (H5T_FLOAT == H5Tget_class(atype))
      {
        size_t npoints = H5Sget_simple_extent_npoints(aspace);
        float * float_array = new float[ npoints ];
        if ( H5Aread( attr, H5T_NATIVE_FLOAT, float_array ) < 0 )
          return 1;

        std::cerr << name << " = ";
        for (int i = 0; i < (int)npoints; i++) printf("%f ", float_array[i]);
        printf("\n");
        delete [] float_array;
        float_array = NULL;
      }
    else if (H5T_STRING == H5Tget_class (atype))
      {
        char * string_attr;
        hid_t type = H5Tget_native_type( atype, H5T_DIR_ASCEND);
        if ( H5Aread( attr, type, &string_attr) < 0 )
          return 1;

        fprintf(stderr, "%s = %s\n", name, string_attr );
        free(string_attr);
      }
    ret = H5Tclose(atype);
    ret = H5Sclose(aspace);
    ret = H5Aclose(attr);

    return 0;
  }
  
  // -------------------------------------------------------- h5get_dataset_shape

  /*!
    \param dataset_id -- Identifier of the dataset within the HDF5 file
    \reval shape -- The shape of the dataset i.e.the length of the array axes in
                    case of multidimensional data

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_dataset_shape (hid_t const &dataset_id,
			    std::vector<uint> &shape,
			    bool const &maxdims)
  {
    bool status (true);
    herr_t h5error;
    hid_t dataspace_id        = H5Dget_space (dataset_id);
    int rank                  = H5Sget_simple_extent_ndims (dataspace_id);
    hsize_t * dataset_dims    = NULL;
    hsize_t * dataset_maxdims = NULL;

    if (rank > 0) {
        shape.resize(rank);
        dataset_dims    = new hsize_t[rank];
        dataset_maxdims = new hsize_t[rank];
	//
        h5error = H5Sget_simple_extent_dims(dataspace_id,
                                            dataset_dims,
                                            dataset_maxdims);
	//
	if (maxdims) {
	  for (int n(0); n<rank; n++) {
	    shape[n] = dataset_maxdims[n];
	  }
	} else {
	  for (int n(0); n<rank; n++) {
	    shape[n] = dataset_dims[n];
	  }
	}
    }
    else {
      shape.resize(1);
      shape[0] = 0;
      status   = false;
    }
    
    // release allocated identifiers
    if (dataspace_id > 0)
      {
        h5error = H5Sclose (dataspace_id);
        h5error = H5Eclear1 ();
      }
    
    if ( dataset_dims )
      {
        delete [] dataset_dims;
        dataset_dims = NULL;
      }

    return status;
  }

  // -------------------------------------------------------- h5get_dataset_shape

#ifdef HAVE_CASA
  /*!
    \param dataset_id -- Identifier of the dataset within the HDF5 file
    \reval shape -- The shape of the dataset i.e.the length of the array axes in
           case of multidimensional data
    \param maxdims -- Return maximum dimensions instead of present dimensions?

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_dataset_shape (hid_t const &attribute_id,
			    casa::IPosition &shape,
			    bool const &maxdims)
  {
    bool status (true);
    herr_t h5error (0);
    hid_t dataset_id = H5Dget_space (attribute_id);
    int rank           = H5Sget_simple_extent_ndims (dataset_id);
    hsize_t *dataset_dims;
    hsize_t *dataset_maxdims;

    if (rank > 0) {
      shape.resize(rank);
      dataset_dims    = new hsize_t[rank];
      dataset_maxdims = new hsize_t[rank];
      // get dimensions of simple dataspace
      h5error = H5Sget_simple_extent_dims(dataset_id,
					  dataset_dims,
					  dataset_maxdims);
      // copy dataset information to return value
      if (maxdims) {
	for (int n(0); n<rank; n++) {
	  shape(n) = dataset_maxdims[n];
	}
      } else {
	for (int n(0); n<rank; n++) {
	  shape(n) = dataset_dims[n];
	}
      }
    }
    else {
      shape.resize(1);
      shape(0) = 0;
      status = false;
    }
    
    // release allocated identifiers
    if (dataset_id > 0)
      {
        h5error = H5Sclose (dataset_id);
        h5error = H5Eclear1 ();
      }

    return status;
  }
#endif

  // ------------------------------------------------------ h5get_dataspace_shape

  /*!
    \param attribute_id -- Identifier of the attribute within the HDF5 file
    \reval shape -- The shape of the dataspace attached to the attribute, i.e.
           the length of the array axes in case of multidimensional data

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_dataspace_shape (hid_t const &attribute_id,
			      std::vector<uint> &shape,
			      bool const &maxdims)
  {
    bool status (true);
    herr_t h5error;
    hid_t dataspace_id   = H5Aget_space (attribute_id);
    int rank             = H5Sget_simple_extent_ndims (dataspace_id);
    hsize_t * dataspace_dims    = NULL;
    hsize_t * dataspace_maxdims = NULL;

    if (rank > 0) {
      // set array sizes
      shape.resize(rank);
      dataspace_dims    = new hsize_t[rank];
      dataspace_maxdims = new hsize_t[rank];
      // get dataspace dimensions
      h5error = H5Sget_simple_extent_dims(dataspace_id,
					  dataspace_dims,
					  dataspace_maxdims);
      // copy dataspace information to return value
      if (maxdims) {
	for (int n(0); n<rank; n++) {
	  shape[n] = dataspace_maxdims[n];
	}
      } else {
	for (int n(0); n<rank; n++) {
	  shape[n] = dataspace_dims[n];
	}
      }
    }
    else {
      shape.resize(1);
      shape[0] = 0;
      status   = false;
    }
    
    // release allocated identifiers
    if (dataspace_id > 0) {
      h5error = H5Sclose (dataspace_id);
      h5error = H5Eclear1 ();
    }
    
    if ( dataspace_dims ) {
      delete [] dataspace_dims;
      delete [] dataspace_maxdims;
      dataspace_dims    = NULL;
      dataspace_maxdims = NULL;
    }
    
    return status;
  }
  
  // ------------------------------------------------------ h5get_dataspace_shape

#ifdef HAVE_CASA
  /*!

    \param attribute_id -- Identifier of the attribute within the HDF5 file
    \reval shape -- The shape of the dataspace attached to the attribute, i.e.
           the length of the array axes in case of multidimensional data

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  bool h5get_dataspace_shape (hid_t const &attribute_id,
			      casa::IPosition &shape,
			      bool const &maxdims)
  {
    bool status (true);
    herr_t h5error (0);
    hid_t dataspace_id = H5Aget_space (attribute_id);
    int rank           = H5Sget_simple_extent_ndims (dataspace_id);
    hsize_t *dataspace_dims;
    hsize_t *dataspace_maxdims;

    if (rank > 0) {
      shape.resize(rank);
      dataspace_dims    = new hsize_t[rank];
      dataspace_maxdims = new hsize_t[rank];
      //
      h5error = H5Sget_simple_extent_dims(dataspace_id,
					  dataspace_dims,
					  dataspace_maxdims);
      //
      if (maxdims) {
	for (int n(0); n<rank; n++) {
	  shape(n) = dataspace_maxdims[n];
	}
      } else {
	for (int n(0); n<rank; n++) {
	  shape(n) = dataspace_dims[n];
	}
      }
    }
    else
      {
        shape.resize(1);
        shape(0) = 0;
        status = false;
      }
    
    // release allocated identifiers
    if (dataspace_id > 0)
      {
        h5error = H5Sclose (dataspace_id);
        h5error = H5Eclear1 ();
      }
    
    return status;
  }
#endif
  
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
  bool h5setAttribute_string (hid_t const &obj_id,
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

  //_____________________________________________________________________________
  // Get the value of an attribute attached to a group or dataset
  
  /*!
    \param attribute_id -- Identifier of the attribute within the HDF5 file
    \retval value       -- Value of the attribute
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  template <>
  bool h5get_attribute (hid_t const &attribute_id,
			std::string &value)
  {
    bool status (true);
    herr_t h5error (0);
    hid_t dataspace_id            = H5Aget_space (attribute_id);
    hid_t datatype_id             = H5Aget_type (attribute_id);
    H5T_class_t datatype_class_id = H5Tget_class (datatype_id);
    hid_t native_datatype_id      = H5Tget_native_type(datatype_id, H5T_DIR_ASCEND);
    
    if (datatype_id > 0) {
      if (datatype_class_id == H5T_STRING) {
	hsize_t datatype_size         = H5Tget_size (datatype_id);
	htri_t is_variable_string     = H5Tis_variable_str(datatype_id);
	
#ifdef DEBUGGING_MESSAGES
	std::cout << "[DAL::h5get_attribute]" << endl;
	std::cout << "-- Attribute ID       = " << attribute_id  << endl;
	std::cout << "-- Dataspace ID       = " << dataspace_id  << endl;
	std::cout << "-- Datatype ID        = " << datatype_id   << endl;
	std::cout << "-- Datatype size      = " << datatype_size << endl;
	std::cout << "-- Is variable length = " << is_variable_string << endl;
#endif
	
	if (is_variable_string) {
	  char *data;
	  h5error = H5Aread(attribute_id,
			    native_datatype_id,
			    &data);
	  if (h5error == 0) {
	    value = data;
	  }
	}
	else {
	  char* data;
	  data = new char[datatype_size];
	  h5error = H5Aread(attribute_id,
			    datatype_id,
			    data);
	  if (h5error == 0) {
	    value = data;
	  }
	}
	
	if (h5error != 0) {
	  value  = "";
	  status = false;
	}
      }
    }
    else {
      value  = "";
      status = false;
    }
    
    // Release identifiers
    H5Tclose (datatype_id);
    H5Sclose (dataspace_id);
    // clean up the error message buffer
    h5error = H5Eclear1();

    return status;
  }

  /*!
    \param attribute_id -- Identifier of the attribute within the HDF5 file
    \retval value       -- Value of the attribute
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
  */
  template <>
    bool h5get_attribute (hid_t const &attribute_id,
			  std::vector<std::string> &value)
    {
      bool status                   = true;
      herr_t h5error                = 0;
      hid_t datatype_id             = H5Aget_type (attribute_id);
      hid_t native_datatype_id      = H5Tget_native_type(datatype_id, H5T_DIR_ASCEND);
      std::vector<uint> shape;
      
      status = h5get_dataspace_shape (attribute_id,shape);
      
      if (shape.size() > 0) {
	char *buffer[shape[0]];
	h5error = H5Aread(attribute_id,
			  native_datatype_id,
			  &buffer);
	//
	if (h5error == 0) {
	  value.resize(shape[0]);
	  for (uint n(0); n<shape[0]; n++) {
	    value[n] = buffer[n];
	  }
	}
      }
      else {
	cerr << "[h5get_attribute] Unsupported shape of attribute dataspace!"
	     << endl;
	status = false;
      }
      
      return status;
    }
  
  // ============================================================================
  //
  //  Set HDF5 attributes
  //
  // ============================================================================

  //_____________________________________________________________________________
  // Set the value of an attribute attached to a group or dataset
  
  /*!
    \param datatype    -- HDF5 datatype of the attribute
    \param location_id -- HDF5 identifier of the attribute within the file
    \param name        -- Name of the attribute
    \param value       -- Value of the attribute
    \param size        -- The size of the attribute
  */
  template <>
    bool h5set_attribute (hid_t const &datatype,
			  hid_t const &location_id,
			  std::string name,
			  std::string * value,
			  int size)
    {
      hid_t   attribute_id = 0;  /* Attribute identifier */
      hid_t   dataspace_id = 0;  /* Attribute dataspace identifier */
      hsize_t dims[1]      = { size };

      if (datatype != H5T_STRING) {
	std::cerr << "[dalCommon::h5set_attribute] Wrong input datatype!"
		  << std::endl;
	return false;
      }
      
      char ** string_attr = (char**)malloc( size * sizeof(char*) );
      for ( int ii = 0; ii < size; ii++ ) {
	string_attr[ii] = (char*)malloc(MAX_COL_NAME_SIZE * sizeof(char));
	strcpy( string_attr[ii], value[ii].c_str() );
      }
      
      hid_t type = H5Tcopy (H5T_C_S1);
      if ( type < 0 ) {
	std::cerr << "ERROR: Could not set attribute '" << name << "' type."
		  << std::endl;
	return false;
      }
      
      if ( H5Tset_size(type, H5T_VARIABLE) < 0 ) {
	std::cerr << "ERROR: Could not set attribute '" << name
		  << "' size." << std::endl;
	return false;
      }
      
      dataspace_id = H5Screate_simple(1, dims, NULL);
      if ( dataspace_id < 0 ) {
	std::cerr << "ERROR: Could not set attribute '" << name
		  << "' dataspace.\n";
	return false;
      }
      
      attribute_id = H5Acreate( location_id,
				name.c_str(),
				type,
				dataspace_id,
				NULL, NULL );
      if ( attribute_id < 0 ) {
	std::cerr << "ERROR: Could not create attribute '" << name << "'.\n";
	return false;
      }
      
      if ( H5Awrite( attribute_id, type, string_attr ) < 0 ) {
	std::cerr << "ERROR: Could not write attribute '" << name << "'.\n";
	return false;
      }
      
      if (  H5Aclose( attribute_id ) < 0 ) {
	std::cerr << "ERROR: Could not close attribute '" << name << "'.\n";
	return false;
      }
      
      for ( int ii = 0; ii < size; ii++ ) {
	free( string_attr[ii] );
      }
      free( string_attr );
      
      return true;
    }
  
  //_____________________________________________________________________________
  // Set the value of an attribute attached to a group or dataset
  
  /*!
    \param location_id -- HDF5 identifier of the attribute within the file
    \param name        -- Name of the attribute
    \param value       -- Value of the attribute
    \param size        -- The size of the attribute
  */
  template <>
  bool h5set_attribute (hid_t const &location_id,
			std::string name,
			int * value,
			int size)
  {
    hid_t datatype = H5T_NATIVE_INT;
    return h5set_attribute (datatype,
			    location_id,
			    name,
			    value,
			    size);
  }
  
  /*!
    \param location_id -- HDF5 identifier of the attribute within the file
    \param name        -- Name of the attribute
    \param value       -- Value of the attribute
    \param size        -- The size of the attribute
  */
  template <>
  bool h5set_attribute (hid_t const &location_id,
			std::string name,
			uint * value,
			int size)
  {
    hid_t datatype = H5T_NATIVE_UINT;
    return h5set_attribute (datatype,
			    location_id,
			    name,
			    value,
			    size);
  }
  
  /*!
    \param location_id -- HDF5 identifier of the attribute within the file
    \param name        -- Name of the attribute
    \param value       -- Value of the attribute
    \param size        -- The size of the attribute
  */
  template <>
  bool h5set_attribute (hid_t const &location_id,
			std::string name,
			short * value,
			int size)
  {
    hid_t datatype = H5T_NATIVE_SHORT;
    return h5set_attribute (datatype,
			    location_id,
			    name,
			    value,
			    size);
  }
  
  template <>
  bool h5set_attribute (hid_t const &location_id,
			std::string name,
			long * value,
			int size)
  {
    hid_t datatype = H5T_NATIVE_LONG;
    return h5set_attribute (datatype,
			    location_id,
			    name,
			    value,
			    size);
  }
  
  template <>
  bool h5set_attribute (hid_t const &location_id,
			std::string name,
			float * value,
			int size)
  {
    hid_t datatype = H5T_NATIVE_FLOAT;
    return h5set_attribute (datatype,
			    location_id,
			    name,
			    value,
			    size);
  }
  
  template <>
  bool h5set_attribute (hid_t const &location_id,
			std::string name,
			double * value,
			int size)
  {
    hid_t datatype = H5T_NATIVE_DOUBLE;
    return h5set_attribute (datatype,
			    location_id,
			    name,
			    value,
			    size);
  }
  
  template <>
  bool h5set_attribute (hid_t const &location_id,
			std::string name,
			std::string * value,
			int size)
  {
    hid_t datatype = H5T_STRING;
    return h5set_attribute (datatype,
			    location_id,
			    name,
			    value,
			    size);
  }
  
  //_____________________________________________________________________________
  // Access to HDF5 attributes through casacore array classes
  
#ifdef HAVE_CASA
  
  // ------------------------------------------------------------ h5get_attribute
  
  template <typename T>
  bool h5get_attribute (hid_t const &attribute_id,
			casa::Vector<T> &value)
  {
    bool status (true);
    casa::IPosition shape;
    
    // get the shape of the dataspace
    status = h5get_dataspace_shape (attribute_id,shape);
    
    if (shape.nelements() == 1) {
      // adjust size of vector returning the result
      value.resize(shape);
      // additional local variables
      herr_t h5error (0);
      hid_t datatype_id  = H5Aget_type (attribute_id);
      T *buffer;
      // allocate buffer memory
      buffer = new T [shape(0)];
      // read attribute value into buffer
      h5error = H5Aread (attribute_id,
			 datatype_id,
			 buffer);
      // copy retrieved data to returned vector
      if (h5error == 0) {
	for (int n(0); n<shape(0); n++) {
	  value(n) = buffer[n];
	}
      }
      else {
	cerr << "[h5get_attribute] Error reading value of attribute." << endl;
	status = false;
      }
    }
    else {
      cerr << "[h5get_attribute] Wrong shape of attribute dataspace!" << endl;
      status = false;
    }
    
    return status;
  }

  // ------------------------------------------------------------ h5get_attribute
  
  template <typename T>
  bool h5get_attribute (hid_t const &location_id,
                        std::string const &name,
			casa::Vector<T> &value)
  {
    bool status (true);
    hid_t attribute_id (0);
    
    // get the identifier for the attribute
    attribute_id = H5Aopen_name(location_id,
                                name.c_str());
    
    if (attribute_id > 0) {
      status = h5get_attribute (attribute_id,
				value);
      H5Aclose (attribute_id);
    }
    else
      {
        cerr << "[h5get_attribute] No valid ID for attribute " << name << endl;
        status = false;
      }
    
    return status;
  }
  
  // ------------------------------------------------------------- h5get_quantity
  
  /*!
    \param location_id -- Identifier of the structure within the file, to which
           the attribut is attached to.
    \param value -- Identifier for the attribute storing the numerical value of
           the quantity.
    \param unit  -- Identifier for the attribute storing the physical unit of
           the quantity

    \return quantity -- The physical quantity.
  */
  Quantity h5get_quantity (hid_t const &location_id,
			   Attributes const &value,
			   Attributes const &unit)
  {
    if (location_id > 0) {
      bool status (true);
      double qValue;
      std::string qUnit;
      // retrieve the value of the quantity
      status *= h5get_attribute(location_id,
				attribute_name(value),
				qValue);
      // retrieve the unit of the quantity
      status *= h5get_attribute(location_id,
				attribute_name(unit),
				qUnit);
      // put together the Quantity object
      if (status) {
	Quantity val = Quantity (qValue,
				 casa::Unit(qUnit));
	return val;
      }
      else {
	return Quantity();
      }
    }
    else {
      cerr << "[h5get_quantity] Unusable ID for HDF5 object!" << endl;
      return Quantity();
    }
  }
  
  // ------------------------------------------------------------ h5get_direction
  
  /*!
    \param location_id -- Identifier of the structure within the file, to which
           the attribut is attached to.
    \param value -- Identifier for the attribute storing the numerical value of
           the quantity.
    \param unit  -- Identifier for the attribute storing the physical unit of
           the quantity
    \param frame -- Identifier for the attribute storing the identifier for the
           reference frame within which the physical quantity is defined.

    \return direction -- The physical quantity.
  */
  MDirection h5get_direction (hid_t const &location_id,
			      Attributes const &value,
                              Attributes const &unit,
                              Attributes const &frame)
  {
    MDirection dir = MDirection();
    
    if (location_id > 0) {
      bool status (true);
      casa::Vector<double> values;
      casa::Vector<casa::String> units;
      MDirection::Types tp;
      std::string refcode;
      // retrieve the numerical values of the position
      status *= h5get_attribute(location_id,
				attribute_name(value),
				values);
      // retrieve the physical units of the position
      status *= h5get_attribute(location_id,
				attribute_name(unit),
				units);
      // retrieve the frame of the position
      status *= h5get_attribute(location_id,
				attribute_name(frame),
				refcode);
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
      else {
	cerr << "[h5get_direction] Error retrieving components for MDirection!"
	     << endl;
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
  
  /*!
    \param location_id -- Identifier of the structure within the file, to which
           the attribut is attached to.
    \param value -- Identifier for the attribute storing the numerical value of
           the quantity.
    \param unit  -- Identifier for the attribute storing the physical unit of
           the quantity
    \param frame -- Identifier for the attribute storing the identifier for the
           reference frame within which the physical quantity is defined.

    \return position -- The position as casa::Measure.
  */
  MPosition h5get_position (hid_t const &location_id,
			    Attributes const &value,
                            Attributes const &unit,
                            Attributes const &frame)
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
        status *= h5get_attribute(location_id,
                                  attribute_name(value),
                                  values);
        // retrieve the physical units of the position
        status *= h5get_attribute(location_id,
                                  attribute_name(unit),
                                  units);
        // retrieve the frame of the position
        status *= h5get_attribute(location_id,
                                  attribute_name(frame),
                                  refcode);
        status *= MPosition::getType (tp,refcode);
        // assemble MPosition object
        if (status)
          {
            int nofValues = values.nelements();
            int nofUnits  = units.nelements();
            if (nofValues == 3 && nofValues == nofUnits) {
	      // create MPosition object
	      obs = MPosition ( MVPosition( Quantity( values(0), units(0)),
					    Quantity( values(1), units(1)),
					    Quantity( values(2), units(2))),
				MPosition::Ref(tp));
	      // return result
	      return obs;
	    }
            else {
	      cerr << "[h5get_position] Mismatching number of values and units!"
		   << endl;
	      obs = MPosition();
	    }
          }
        else {
	  cerr << "[h5get_position] Error retrieving components for MPosition!"
	       << endl;
	  obs = MPosition();
	}
      }
    else {
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
    for ( int idx=0; idx < array_size; idx++ ) {
      mjd_time[ idx ] = bpl::extract<double>( mjd_time[ idx ] ) - adjustment_factor;
    }
    
    return mjd_time;
  }
#endif
  
  // ============================================================================
  //
  //  Template instantiation
  //
  // ============================================================================

#ifdef HAVE_CASA

  template bool h5get_attribute (hid_t const &attribute_id,
				 casa::Vector<int> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 casa::Vector<uint> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 casa::Vector<short> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 casa::Vector<long> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 casa::Vector<float> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 casa::Vector<double> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 casa::Vector<casa::String> &value);
  
  template bool h5get_attribute (hid_t const &attribute_id,
				 std::string const &name,
				 casa::Vector<int> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 std::string const &name,
				 casa::Vector<uint> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 std::string const &name,
				 casa::Vector<short> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 std::string const &name,
				 casa::Vector<long> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 std::string const &name,
				 casa::Vector<float> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 std::string const &name,
				 casa::Vector<double> &value);
  template bool h5get_attribute (hid_t const &attribute_id,
				 std::string const &name,
				 casa::Vector<casa::String> &value);
  
#endif
  
} // namespace DAL
