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

//_______________________________________________________________________________
//                                                                 writeAttribute

/*!
  \brief Write attribute of type \e string.

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
bool writeAttribute (hid_t const &location,
		     std::string attrname,
		     std::string const * data,
		     int size )
{
  hid_t att       = 0;
  hid_t dataspace = 0;
  hsize_t dims[1] = { size };
  
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
//                                                                           main

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

    data   = "Hello World";
    status = writeAttribute (fileID, "String1",&data,1);
    
  }
    
  //________________________________________________________
  // Release HDF5 objects
  
  if (H5Iis_valid(fileID)) {
    H5Fclose(fileID);
  }
  
  return nofFailedTests;
}
