/*-------------------------------------------------------------------------*
  | $Id:: dalArray.cc 4393 2010-03-11 20:35:09Z baehren                   $ |
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

#include "dalShortArray.h"

namespace DAL {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  /*!
    Constructor for an extendible short array.  This is usually called from
    the dataset object and not directly by the developer.

    \param obj_id An identifier for the dataset object.
    \param arrayname The name of the array you want to create.
    \param dims The dimensions of the array you want to create.
    \param data A structure containing the data you want to write to the
                array.  The size of the structure should match the dimensions
                of the array.
    \param chnkdims Specifies the chunk size for extendible arrays.
   */
  dalShortArray::dalShortArray( hid_t obj_id,
				string arrayname,
                                vector<int> dims,
				short data[],
                                vector<int> chnkdims )
  {
    hid_t datatype  = 0;
    hid_t dataspace = 0;  // declare a few h5 variables
    name = arrayname;  // set the private name variable to the array name

    // determine the rank from the size of the dimensions vector
    unsigned int rank = dims.size();
    hsize_t mydims[rank];        // declare a dimensions c-array
    hsize_t maxdims[rank];       // declare a maximum dimensions c-array
    hsize_t chunk_dims[ rank ];  // declare chunk dimensions c-array

    // set the c-array dimensions and maxiumum dimensions
    for (unsigned int ii=0; ii<rank; ii++)
      {
        mydims[ii] = dims[ii];  // vector to c-array
        maxdims[ii] = H5S_UNLIMITED;
      }

    // set the c-array chunk dimensions from the chunk dims vector
    for (unsigned int ii=0; ii<chnkdims.size(); ii++)
      {
        chunk_dims[ii] = chnkdims[ii];
      }

    // set the datatype to write
    if ( ( datatype = H5Tcopy(H5T_NATIVE_SHORT) ) < 0 ) {
      std::cerr << "ERROR: Could not set array datatype.\n";
    }
    
    // if there are chunk dimensions, write the data this way
    if ( chnkdims.size()>0 ) {
      if ( ( dataspace = H5Screate_simple(rank,mydims,maxdims) ) < 0 )
	{
	  std::cerr << "ERROR: Could not set array dataspace.\n";
	}
      
      hid_t cparms = H5Pcreate( H5P_DATASET_CREATE );
      if ( cparms < 0 )
	{
	  std::cerr << "ERROR: Could not set array propertylist.\n";
	}
      
      if ( H5Pset_chunk( cparms, rank, chunk_dims ) < 0 )
	{
	  std::cerr << "ERROR: Could not set array chunk size.\n";
	}
      
      if ( ( datasetID_p = H5Dcreate1( obj_id, arrayname.c_str(), datatype,
				     dataspace, cparms) ) < 0 )
	{
	  std::cerr << "ERROR: Could not create array.\n";
	}
      
    }
    // otherwise, write the data this way
    else {
      if ( ( dataspace = H5Screate_simple(rank,mydims,NULL) ) < 0 )
	{
	  std::cerr << "ERROR: Could not set array dataspace.\n";
	}
      
      if ( ( datasetID_p = H5Dcreate1( obj_id, arrayname.c_str(), datatype,
				     dataspace, H5P_DEFAULT ) ) < 0 )
	{
	  std::cerr << "ERROR: Could not create array.\n";
	}
    }
    
    // write the data
    if ( H5Dwrite( datasetID_p, datatype, dataspace, dataspace, H5P_DEFAULT,
                   data) < 0 ) {
      std::cerr << "ERROR: Could not write array.\n";
    }
    
    
    // close local hdf5 objects
    if ( H5Sclose( dataspace ) < 0 ) {
      std::cerr << "ERROR: Could not close array dataspace.\n";
    }
    
    if ( H5Tclose( datatype ) < 0 ) {
      std::cerr << "ERROR: Could not close array datatype.\n";
    }
    
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  /*!
    \param obj_id An identifier for the dataset object.
    \param arrayname The name of the array you want to create.
    \return Pointer to array values.
   */
  short * dalShortArray::readShortArray (hid_t obj_id,
					 string arrayname)
  {
    hid_t filespace = 0;
    hid_t data_rank = 0;
    long size       = 1;  // the size of the array

    // get the dataspace
    if ( ( filespace = H5Dget_space(obj_id) ) < 0 ) {
      std::cerr << "ERROR: Could not get filespace for array '"
		<< arrayname << "'.\n";
      return NULL;
    }
    
    // retrieve the rank of the array
    if ( ( data_rank = H5Sget_simple_extent_ndims( filespace ) ) < 0 ) {
      std::cerr << "ERROR: Could not get rank for array '"
		<< arrayname << "'.\n";
      return NULL;
    }
    
    // Create a dims c-array using the rank
    hsize_t dims[ data_rank ];
    
    // retrieve the dimensions of the array
    if ( H5Sget_simple_extent_dims(filespace, dims, NULL) < 0 )
      {
        std::cerr << "ERROR: Could not get dimensions for array '"
                  << arrayname << "'.\n";
        return NULL;
      }

    // from the dimensions, compute the full size of the array
    for ( int32_t ii = 0; ii < data_rank; ii++ )
      size *= dims[ii];

    short * data = NULL;  // declare a pointer to the data

    try {
      data = new short[size];  // allocate space for the data
    }
    catch ( std::bad_alloc )
      {
        std::cerr << "Can't allocate memory for reading short array '"
                  << arrayname << "'.\n";
        return NULL;
      }
    
    // read the data into the local array
    if ( ( H5LTread_dataset_short( obj_id, arrayname.c_str(), data ) ) < 0 )
      {
        std::cerr << "ERROR: Could not read array '" << arrayname << "'.\n";
        return NULL;
      }
    
    return data;  // return the data pointer
    
  }
  
}  //  END  --  namespace DAL
