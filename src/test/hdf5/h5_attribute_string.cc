/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <hdf5.h>

/*!
  \file h5_attribute_string.cc

  \ingroup DAL

  \brief Test creating/writing/reading an attribute of type \e string

  \author Lars B&auml;hren

  \date 2011/05/03
*/

const int32_t MAX_COL_NAME_SIZE = 256;

// ==============================================================================
//
//  Test routines
//
// ==============================================================================

//_______________________________________________________________________________
//                                           test_dalCommon_h5setAttribute_string

/*!
  \brief Write attribute of type \e string.

  Original implementation: dalCommon::h5setAttribute_string

  This routine is one of the original methods (\c h5setAttribute_string) written
  by Joe to handle writing string-type attributes. The main drawbacks of this 
  implementation are, that a) the \c data written to the attribute need to passed
  along as a pointer instead of using a reference, and b) the lenght of the 
  string value, as being written to the attribute has a fixed length.

  \param location -- Identifier for the HDF5 object, to which the attribute will
         be attached.
  \param attrname -- Name of the attribute.
  \param data     -- Data written to the attribute.
  \param size     -- Size (number of elements) of the attribute data.
  \return status  -- Status of the operation; returns \e false in case an error
          was encountered, e.g. failure to write the data.
*/
bool test_dalCommon_h5setAttribute_string (hid_t const &location,
					   std::string attrname,
					   std::string const * data,
					   int size )
{
  hid_t att       = 0;
  hid_t dataspace = 0;
  hsize_t dims[1] = { size };

  /* Allocated memory */
  char ** string_attr = (char**)malloc( size * sizeof(char*) );

  for ( int ii = 0; ii < size; ii++ ) {
    string_attr[ii] = (char*)malloc(MAX_COL_NAME_SIZE * sizeof(char));
    strcpy( string_attr[ii], data[ii].c_str() );
  }
  
  hid_t type = H5Tcopy (H5T_C_S1);
  if ( type < 0 ) {
    std::cerr << "ERROR: Could not set attribute '" << attrname
	      << "' type.\n";
    return false;
  }
  
  if ( H5Tset_size(type, H5T_VARIABLE) < 0 ) {
    std::cerr << "ERROR: Could not set attribute '" << attrname
	      << "' size.\n";
    return false;
  }
  
  dataspace = H5Screate_simple(1, dims, 0);
  if ( dataspace < 0 ) {
    std::cerr << "ERROR: Could not set attribute '" << attrname
	      << "' dataspace.\n";
    return false;
  }
  
  att = H5Acreate( location, attrname.c_str(), type, dataspace, 0, 0 );
  if ( att < 0 ) {
    std::cerr << "ERROR: Could not create attribute '" << attrname << "'.\n";
    return false;
  }
  
  if ( H5Awrite( att, type, string_attr ) < 0 ) {
    std::cerr << "ERROR: Could not write attribute '" << attrname << "'.\n";
    return false;
  }
  
  for ( int ii = 0; ii < size; ii++ ) {
    free( string_attr[ii] );
  }
  free( string_attr );
  
  /* release HDF5 handlers */
  if (H5Iis_valid(att))       H5Aclose (att);
  if (H5Iis_valid(dataspace)) H5Sclose (dataspace);
  if (H5Iis_valid(type))      H5Tclose (type);
  
  return true;
}

//_______________________________________________________________________________
//                                                                 writeAttribute

/*!
  \brief Set the value of an attribute attached to a group or dataset
  
  Original implementation: dalCommon::h5set_attribute

  \param datatype    -- HDF5 datatype of the attribute
  \param location_id -- HDF5 identifier of the attribute within the file
  \param name        -- Name of the attribute
  \param value       -- Value of the attribute
  \param size        -- The size of the attribute
  
  \return status -- Status of the operation; returns \e false in case an error
          was encountered and detected.
*/
bool writeAttribute (hid_t const &datatype,
		     hid_t const &location_id,
		     std::string name,
		     std::string * value,
		     int size)
{
  hid_t   attribute_id = 0;
  hid_t   dataspace_id = 0;
  hsize_t dims[1]      = { size };
  hsize_t *maxdims     = 0;

  /* Feedback */
  std::cout << "[dalCommon::h5set_attribute]"     << std::endl;
  std::cout << "-- Datatype       = " << datatype    << std::endl;
  std::cout << "-- Location ID    = " << location_id << std::endl;
  std::cout << "-- Attribute name = " << name        << std::endl;
  
  /* Check if location_id points to a valid HDF5 object */
  
  if (!H5Iis_valid(location_id)) {
    std::cerr << "[dalCommon::h5set_attribute]"
	 << " Unable to set attribute - invalid object identifier!"
	 << std::endl;
    return false;
  }
  
  /* Check if the attribute already exists; if this is the case we update
   * the contents - otherwise we newly create the attribute.
   */
  if (H5Aexists(location_id,name.c_str())) {
    attribute_id = H5Aopen (location_id,
			    name.c_str(),
			    H5P_DEFAULT);
  }
  else {
    // Create the ddataspace attached to the attribute
    dataspace_id  = H5Screate_simple( 1, dims, maxdims );
    if ( dataspace_id < 0 ) {
      std::cerr << "[h5set_attribute] ERROR: Could not set attribute '" << name
		<< "' dataspace.\n";
      return false;
    }
    // Create the attribute itself
    attribute_id = H5Acreate (location_id,
			      name.c_str(),
			      datatype,
			      dataspace_id,
			      0,
			      0);
    if ( attribute_id < 0 ) {
      std::cerr << "[h5set_attribute] ERROR: Could not create attribute '" << name
		<< "'.\n";
      return false;
    }
  }
  
  if ( H5Awrite(attribute_id, datatype, value) < 0 ) {
    std::cerr << "[h5set_attribute] ERROR: Could not write attribute '"
	      << name << "'" << std::endl;
    return false;
  }
  
  if (H5Iis_valid(attribute_id)) { H5Aclose (attribute_id); }
  if (H5Iis_valid(dataspace_id)) { H5Sclose (dataspace_id); }
  
  return true;
}

//_______________________________________________________________________________
//                                                       test_HDF5Attribute_write

/*!
  Original implementation: HDF5Attribute::write

  \param location -- Identifier to the HDF5 object.
  \param name     -- Name of the attribute.
  \param data     -- Array with the data written to the attribute.
  \param size     -- Number of elements in the value array.
*/
bool test_HDF5Attribute_write (hid_t const &location,
		     std::string const &name,
		     std::string const *data,
		     unsigned int const &size)
{
  bool status       = true;
  hid_t   attribute = 0;
  hid_t   dataspace = 0;
  hid_t   datatype  = H5Tcopy (H5T_C_S1);
  hsize_t dims[1]   = { size };
  hsize_t *maxdims  = 0;
  herr_t h5err      = 0;
  
  /*________________________________________________________________
    Basic checks for reference location and attribute name.
  */
  
  if (H5Iis_valid(location)) {
    h5err = H5Aexists (location,
		       name.c_str());
  } else {
    std::cerr << "[HDF5Attribute::write]"
	      << " No valid HDF5 object found at reference location!"
	      << std::endl;
    return false;
  }
  
  /*____________________________________________________________
    Check if attribute 'name' already exits at the given
    'location'; if this is not the case, we need to create the 
    attribute.
  */
  
  if (h5err>0) {
    attribute = H5Aopen (location,
			 name.c_str(),
			 H5P_DEFAULT);
  } else {
    /* Create dataspace for the attribute */
    h5err     = H5Tset_size (datatype, H5T_VARIABLE);
    dataspace = H5Screate_simple (1, dims, maxdims );
    if (H5Iis_valid(dataspace)) {
      /* Create the attribute itself ... */
      attribute = H5Acreate (location,
			     name.c_str(),
			     datatype,
			     dataspace,
			     0,
			     0);
      /* ... and check if creation was successful */
      if (H5Iis_valid(attribute)) {
	status = true;
      } else {
	std::cerr << "[HDF5Attribute::write]"
		  << " H5Acreate() failed to create attribute "
		  << name
		  << std::endl;
	status = false;
      }
    } else {
      std::cerr << "[HDF5Attribute::write]"
		<< " H5Screate_simple() failed to create dataspace!"
		<< std::endl;
      status = false;
    }
  }
  
  /*____________________________________________________________
    H5Awrite() returns a non-negative value if successful;
    otherwise returns a negative value. 
  */
  
  if (status) {
    datatype = H5Aget_type(attribute);
    /* Write the data to the attribute ... */
    h5err = H5Awrite (attribute, datatype, data);
    /* ... and check the return value of the operation */
    if (h5err<0) {
      std::cerr << "[HDF5Attribute::write]"
		<< " H5Awrite() failed to write attribute!"
		<< std::endl;
      status = false;
    }
  }
  
  /*____________________________________________________________
    Release HDF5 object handles
  */
  H5Tclose (datatype);
  H5Sclose (dataspace);
  H5Aclose (attribute);
  
  return status;
}


// ==============================================================================
//
//  Main routine
//
// ==============================================================================

/*!
  \brief Main routine of the test program.

  \return nofFailedTests -- nof. failed tests encountered within this test
          program.
 */
int main (int argc, char *argv[])
{
  int nofFailedTests   = 0;
  bool status          = true;
  hid_t fileID         = 0;
  std::string filename = "h5_attribute_string.h5";
  std::string name;
  std::string data;

  //________________________________________________________
  // Process command line parameters

  if (argc>1) {
    filename = argv[1];
  }

  //________________________________________________________
  // Open/create HDF5 file to work with
  
  fileID = H5Fcreate (filename.c_str(),
		      H5F_ACC_TRUNC,
		      H5P_DEFAULT,
		      H5P_DEFAULT);
  
  //________________________________________________________
  // Run the tests
  
  if (H5Iis_valid(fileID)) {

    name   = "String1";
    data   = "h5setAttribute_string";
    status = test_dalCommon_h5setAttribute_string (fileID, name, &data, 1);
    
    // data   = "h5set_attribute";
    // status = writeAttribute (H5T_STRING, fileID, "String2", &data, 1);

    /* HDF5Attribute::write */
    name = "String3";
    data = "HDF5Attribute write";
    status = test_HDF5Attribute_write (fileID, name, &data, 1);
    
  }
    
  //________________________________________________________
  // Release HDF5 objects
  
  if (H5Iis_valid(fileID)) {
    H5Fclose(fileID);
  }
  
  return nofFailedTests;
}
