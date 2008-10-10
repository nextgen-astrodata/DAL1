/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#include "HDF5Common.h"

using std::cerr;
using std::cout;
using std::endl;

#ifdef HAVE_CASA
using casa::MDirection;
using casa::MPosition;
using casa::Quantity;
#endif

namespace DAL {  // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Inspection and Feedback
  //
  // ============================================================================
  
  // ------------------------------------------------------ h5get_dataset_shape

  bool h5get_dataset_shape (std::vector<uint> &shape,
                            hid_t const &dataset_id)
  {
    bool status (true);
    herr_t h5error;
    hid_t dataspace_id   = H5Dget_space (dataset_id);
    int rank             = H5Sget_simple_extent_ndims (dataspace_id);
    hsize_t * dimensions = NULL;

    if (rank > 0)
      {
        dimensions = new hsize_t[rank];
        h5error = H5Sget_simple_extent_dims(dataspace_id,
                                            dimensions,
                                            NULL);
        shape.resize(rank);
        for (int n(0); n<rank; n++)
          {
            shape[n] = dimensions[n];
          }
      }
    else
      {
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

    if ( dimensions )
      {
        delete [] dimensions;
        dimensions = NULL;
      }

    return status;
  }

  // ------------------------------------------------------ h5get_dataset_shape

#ifdef HAVE_CASA
  bool h5get_dataset_shape (casa::IPosition &shape,
                            hid_t const &attribute_id)
  {
    bool status (true);
    herr_t h5error (0);
    hid_t dataspace_id = H5Dget_space (attribute_id);
    int rank           = H5Sget_simple_extent_ndims (dataspace_id);
    hsize_t *dimensions;

    if (rank > 0)
      {
        dimensions = new hsize_t[rank];
        h5error = H5Sget_simple_extent_dims(dataspace_id,
                                            dimensions,
                                            NULL);
        shape.resize(rank);
        for (int n(0); n<rank; n++)
          {
            shape(n) = dimensions[n];
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

  // ------------------------------------------------------ h5get_dataspace_shape

  bool h5get_dataspace_shape (std::vector<uint> &shape,
                              hid_t const &attribute_id)
  {
    bool status (true);
    herr_t h5error;
    hid_t dataspace_id   = H5Aget_space (attribute_id);
    int rank             = H5Sget_simple_extent_ndims (dataspace_id);
    hsize_t * dimensions = NULL;

    if (rank > 0)
      {
        dimensions = new hsize_t[rank];
        h5error = H5Sget_simple_extent_dims(dataspace_id,
                                            dimensions,
                                            NULL);
        shape.resize(rank);
        for (int n(0); n<rank; n++)
          {
            shape[n] = dimensions[n];
          }
      }
    else
      {
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

    if ( dimensions )
      {
        delete [] dimensions;
        dimensions = NULL;
      }

    return status;
  }

  // ------------------------------------------------------ h5get_dataspace_shape

#ifdef HAVE_CASA
  bool h5get_dataspace_shape (casa::IPosition &shape,
                              hid_t const &attribute_id)
  {
    bool status (true);
    herr_t h5error (0);
    hid_t dataspace_id = H5Aget_space (attribute_id);
    int rank           = H5Sget_simple_extent_ndims (dataspace_id);
    hsize_t *dimensions;

    if (rank > 0)
      {
        dimensions = new hsize_t[rank];
        h5error = H5Sget_simple_extent_dims(dataspace_id,
                                            dimensions,
                                            NULL);
        shape.resize(rank);
        for (int n(0); n<rank; n++)
          {
            shape(n) = dimensions[n];
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

  // ----------------------------------------------------------------- h5get_name

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

  bool h5get_name (std::string &name,
                   hid_t const &object_id,
                   hsize_t const &index)
  {
//     H5I_type_t objectType = H5Iget_type (object_id);
    hsize_t nofObjects    = 0;
    herr_t h5error        = 0;

    // Check if the HDF5 object is a group

//     if (objectType != H5I_GROUP) {
//       std::cerr << "[h5get_name] Running index only supported for groups!"
// 		<< std::endl;
//       return false;
//     }

    // Check if the transient index is within the valid range

    h5error = H5Gget_num_objs(object_id,
                              &nofObjects);

    if (index > nofObjects)
      {
        std::cerr << "[h5get_name] Running index outside valid range!"
                  << std::endl;
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
                  << std::endl;
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

  bool h5get_filename (std::string &filename,
                       hid_t const &object_id)
  {

    /*
     * Basic check: does the provided identifier make any sense?
     */
    if (object_id < 1)
      {
        std::cout << "[h5get_filename] Invalid value of object identifier;"
                  << " must be greater zero!"
                  << std::endl;
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

    if (objectType == H5I_BADID)
      {
        std::cerr << "[h5get_filename] Bad object type - aborting now!" << std::endl;
        return false;
      }
    else if (objectType == H5I_FILE)
      {
        status = h5get_name (filename,
                             object_id);
      }
    else
      {
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
  //  Access attribute values
  //
  // ============================================================================

  // ------------------------------------------------------------ h5get_attribute
  
  /*!
    \retval value      -- Value of the attribute
    \param name        -- Name of the attribute
    \param location_id -- Identifier for the HDF5 object - file, group, dataset,
           array - the attribute is attached to.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered
   */
  template <class T>
  bool h5get_attribute (T &value,
                        std::string const &name,
                        hid_t const &locationID)
  {
    bool status (true);
    hid_t attribute_id (0);
    herr_t h5error (0);

    // get the identifier for the attribute
    attribute_id = H5Aopen_name(locationID,
                                name.c_str());

    if (attribute_id > 0)
      {
        /* forward the call to retrieve the actual value of the attribute */
        status = h5get_attribute (value,
                                  attribute_id);
        /* release the identifier used for retrieval of the value */
        h5error = H5Aclose (attribute_id);
        /* clean up the error message buffer */
        h5error = H5Eclear1();
      }
    else
      {
        cerr << "[h5get_attribute] No valid ID for attribute "
             << name
             << std::endl;
        status = false;
      }

    return status;
  }

  // ------------------------------------------------------------ h5get_attribute

  template <class T>
  bool h5get_attribute (std::vector<T> &value,
                        std::string const &name,
                        hid_t const &locationID)
  {
    bool status (true);
    hid_t attribute_id (0);
    herr_t h5error (0);

    // get the identifier for the attribute
    attribute_id = H5Aopen_name(locationID,
                                name.c_str());

    if (attribute_id > 0)
      {
        status = h5get_attribute (value,
                                  attribute_id);
        /* release the identifier used for retrieval of the value */
        h5error = H5Aclose (attribute_id);
        /* clean up the error message buffer */
        h5error = H5Eclear1();
      }
    else
      {
        cerr << "[h5get_attribute] No valid ID for attribute " << name << endl;
        status = false;
      }

    return status;
  }

  // -------------------------------------------------------- h5get_attribute (T)

  template <class T>
  bool h5get_attribute (T &value,
                        hid_t const &attribute_id)
  {
    // Get the datatype of the attribute's value
    hid_t datatype_id (H5Aget_type (attribute_id));

    // read attribute value into buffer
    herr_t h5error = H5Aread (attribute_id,
                              datatype_id,
                              &value);
    // clean up the error message buffer
    h5error = H5Eclear1();

    if (h5error == 0)
      {
        return true;
      }
    else
      {
        return false;
      }
  }

  // --------------------------------------------------- h5get_attribute (string)

  bool h5get_attribute (std::string &value,
                        hid_t const &attribute_id)
  {
    bool status (true);
    herr_t h5error (0);
    hid_t dataspace_id = H5Aget_space (attribute_id);
    hid_t datatype_id  = H5Aget_type (attribute_id);

    if (datatype_id > 0)
      {
        if (H5Tget_class (datatype_id) == H5T_STRING)
          {
            char* data;
            hsize_t datatype_size = H5Tget_size (datatype_id);
            data = new char[datatype_size];
            h5error = H5Aread(attribute_id,
                              datatype_id,
                              data);
            if (h5error == 0)
              {
                value = data;
              }
            else
              {
                value  = "";
                status = false;
              }
          }
      }
    else
      {
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

  // ------------------------------------------------ h5get_attribute (vector<T>)

  template <class T>
  bool h5get_attribute (std::vector<T> &value,
                        hid_t const &attribute_id)
  {
    bool status (true);
    std::vector<uint> shape;

    // get the shape of the dataspace
    status = h5get_dataspace_shape (shape,attribute_id);

    if (shape.size() == 1)
      {
        // adjust size of vector returning the result
        value.resize(shape[0]);
        // additional local variables
        herr_t h5error     = 0;
//       hid_t dataspace_id = H5Aget_space (attribute_id);
        hid_t datatype_id  = H5Aget_type (attribute_id);
        T *buffer;
        // allocate buffer memory
        buffer = new T [shape[0]];
        // read attribute value into buffer
        h5error = H5Aread (attribute_id,
                           datatype_id,
                           buffer);
        // copy retrieved data to returned vector
        if (h5error == 0)
          {
            for (uint n(0); n<shape[0]; n++)
              {
                value[n] = buffer[n];
              }

            delete [] buffer;
            buffer = NULL;

          }
        else
          {
            cerr << "[h5get_attribute] Error reading value of attribute." << endl;
            status = false;
          }
      }
    else
      {
        cerr << "[h5get_attribute] Wrong shape of attribute dataspace!"
             << std::endl;
        status = false;
      }

    return status;
  }

  // ------------------------------------------ h5get_attribute (casa::Vector<T>)

#ifdef HAVE_CASA
  template <class T>
  bool h5get_attribute (casa::Vector<T> &value,
                        std::string const &name,
                        hid_t const &locationID)
  {
    bool status (true);
    hid_t attribute_id (0);

    // get the identifier for the attribute
    attribute_id = H5Aopen_name(locationID,
                                name.c_str());

    if (attribute_id > 0)
      {
        status = h5get_attribute (value,
                                  attribute_id);
        H5Aclose (attribute_id);
      }
    else
      {
        cerr << "[h5get_attribute] No valid ID for attribute " << name << endl;
        status = false;
      }

    return status;
  }

  // ------------------------------------------ h5get_attribute (casa::Vector<T>)

  template <class T>
  bool h5get_attribute (casa::Vector<T> &value,
                        hid_t const &attribute_id)
  {
    bool status (true);
    casa::IPosition shape;

    // get the shape of the dataspace
    status = h5get_dataspace_shape (shape,
                                    attribute_id);

    if (shape.nelements() == 1)
      {
        // adjust size of vector returning the result
        value.resize(shape);
        // additional local variables
        herr_t h5error (0);
//       hid_t dataspace_id = H5Aget_space (attribute_id);
        hid_t datatype_id  = H5Aget_type (attribute_id);
        T *buffer;
        // allocate buffer memory
        buffer = new T [shape(0)];
        // read attribute value into buffer
        h5error = H5Aread (attribute_id,
                           datatype_id,
                           buffer);
        // copy retrieved data to returned vector
        if (h5error == 0)
          {
            for (int n(0); n<shape(0); n++)
              {
                value(n) = buffer[n];
              }
          }
        else
          {
            cerr << "[h5get_attribute] Error reading value of attribute." << endl;
            status = false;
          }
      }
    else
      {
        cerr << "[h5get_attribute] Wrong shape of attribute dataspace!" << endl;
        status = false;
      }

    return status;
  }

#endif // HAVE_CASA

  // ------------------------------------------------------------ h5set_attribute

  bool h5set_attribute (hid_t const &location_id,
                        std::string const &name,
                        uint const &val)
  {
    bool status (true);
    hid_t attribute_id (0);
    hid_t dataspace_id (0);
    herr_t h5error;

    /* [1] Check if the attribute already exists */
    try
      {
        attribute_id = H5Aopen_name(location_id,
                                    name.c_str());
      }
    catch (std::string message)
      {
        cerr << "[h5set_attribute] " << message << std::endl;
        status = false;
      }

    /* [2] If the attribute does not exist yet, we need to create it first */

    if (attribute_id > 0)
      {
        cout << "-- Attribute " << name << " already exists." << endl;
        // Retrieve the ID of the associated dataspace
        dataspace_id = H5Aget_space (attribute_id);
        // Check if the value matches the type of the existing dataspace
      }
    else
      {
        cout << "-- Attribute "
             << name
             << " does not exist yet; creating it now ..."
             << endl;
        // Create dataspace associated with the attribute
        dataspace_id = H5Screate (H5S_SCALAR);
        // Create the attribute itself
        attribute_id = H5Acreate1 (location_id,
                                   name.c_str(),
                                   H5T_NATIVE_UINT,
                                   dataspace_id,
                                   H5P_DEFAULT);
      }

    // Summary
    cout << "-- Location ID     = " << location_id  << std::endl;
    cout << "-- Attribute name  = " << name         << std::endl;
    cout << "-- Attribute value = " << val          << std::endl;
    cout << "-- Attribute ID    = " << attribute_id << std::endl;
    cout << "-- Dataspace ID    = " << dataspace_id << std::endl;

    // Close dataspace and attribute
    if (dataspace_id > 0)
      {
        h5error = H5Dclose (dataspace_id);
      }

    if (attribute_id > 0)
      {
        h5error = H5Aclose (attribute_id);
      }

    return status;
  }

  /*
   * Operator function.
   * This function is used in combination with H5Aiterate.
   * It prints out the value of attribute.
   */
  herr_t
  attr_info(hid_t loc_id, const char * name, void * opdata)
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


  // ============================================================================
  //
  //  Dataspaces and Datatypes
  //
  // ============================================================================


  // ============================================================================
  //
  //  Template instantiation
  //
  // ============================================================================

  template bool h5get_attribute (uint &value,
                                 std::string const &name,
                                 hid_t const &locationID);
  template bool h5get_attribute (int &value,
                                 std::string const &name,
                                 hid_t const &locationID);
  template bool h5get_attribute (double &value,
                                 std::string const &name,
                                 hid_t const &locationID);
  template bool h5get_attribute (std::string &value,
                                 std::string const &name,
                                 hid_t const &locationID);

  template bool h5get_attribute (std::vector<uint> &value,
                                 hid_t const &attribute_id);
  template bool h5get_attribute (std::vector<uint> &value,
                                 std::string const &name,
                                 hid_t const &location_id);

  template bool h5get_attribute (std::vector<int> &value,
                                 hid_t const &attribute_id);
  template bool h5get_attribute (std::vector<int> &value,
                                 std::string const &name,
                                 hid_t const &location_id);

  template bool h5get_attribute (std::vector<double> &value,
                                 hid_t const &attribute_id);
  template bool h5get_attribute (std::vector<double> &value,
                                 std::string const &name,
                                 hid_t const &location_id);

  template bool h5get_attribute (std::vector<std::string> &value,
                                 hid_t const &attribute_id);
  template bool h5get_attribute (std::vector<std::string> &value,
                                 std::string const &name,
                                 hid_t const &location_id);

#ifdef HAVE_CASA
  template bool h5get_attribute (casa::Vector<uint> &value,
                                 hid_t const &attribute_id);
  template bool h5get_attribute (casa::Vector<uint> &value,
                                 std::string const &name,
                                 hid_t const &location_id);

  template bool h5get_attribute (casa::Vector<int> &value,
                                 hid_t const &attribute_id);
  template bool h5get_attribute (casa::Vector<int> &value,
                                 std::string const &name,
                                 hid_t const &location_id);

  template bool h5get_attribute (casa::Vector<double> &value,
                                 hid_t const &attribute_id);
  template bool h5get_attribute (casa::Vector<double> &value,
                                 std::string const &name,
                                 hid_t const &location_id);
  template bool h5get_attribute (casa::Vector<casa::String> &value,
                                 hid_t const &attribute_id);
  template bool h5get_attribute (casa::Vector<casa::String> &value,
                                 std::string const &name,
                                 hid_t const &location_id);
#endif

} // Namespace DAL -- end
