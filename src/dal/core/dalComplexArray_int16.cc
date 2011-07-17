/***************************************************************************
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

#include <core/dalComplexArray_int16.h>

namespace DAL {   // BEGIN -- namespace DAL
  
  /*!
    Constructor for an extendible complex int16 array.  This is
    usually called from the dataset object and not directly by the developer.

    \param obj_id    -- Dataset file handle
    \param arrayname -- The name of the array you want to create.
    \param dims      -- Vector of the array dimensions
    \param data      -- complex<float> vector of data to write
    \param chnkdims  -- resizing (chunking) dimensions. Empty vector if the
           size of the array is fixed.
  */
  dalComplexArray_int16::dalComplexArray_int16 (hid_t obj_id,
						std::string arrayname,
						std::vector<int> dims,
						std::complex<Int16> data[],
						std::vector<int> chnkdims)
  {
    hid_t datatype    = 0;
    hid_t dataspace   = 0;
    hid_t complex_id  = 0;
    unsigned int rank = dims.size();

    // Set the private name variable to the array name
    itsName = arrayname;  

    // determine the rank from the size of the dimensions vector
    hsize_t mydims[rank];  // declare a dimensions c-array
    hsize_t maxdims[rank]; // declare a maximum dimensions c-array
    hsize_t chunk_dims[ rank ];  // declare chunk dimensions c-array

    // set the c-array dimensions and maxiumum dimensions
    for (unsigned int ii=0; ii<rank; ii++) {
      mydims[ii] = dims[ii];  // vector to c-array
      maxdims[ii] = H5S_UNLIMITED;
    }
    
    // set the c-array chunk dimensions from the chunk dims vector
    for (unsigned int ii=0; ii<chnkdims.size(); ii++)
      {
        chunk_dims[ii] = chnkdims[ii];
      }
    
    // create a new hdf5 datatype for complex values
    if ( ( complex_id = H5Tcreate( H5T_COMPOUND, sizeof( DAL::Complex_Int16 )) )
         < 0 )
      {
        std::cerr << "ERROR: Could not create compound datatype.\n";
      }

    if ( H5Tinsert( complex_id, "real", HOFFSET(DAL::Complex_Int16,real),
                    H5T_NATIVE_SHORT) < 0 )
      {
        std::cerr << "ERROR: Could not create compound datatype (r).\n";
      }

    if ( H5Tinsert( complex_id, "imag", HOFFSET(DAL::Complex_Int16,imag),
                    H5T_NATIVE_SHORT) < 0 )
      {
        std::cerr << "ERROR: Could not create compound datatype (i).\n";
      }

    // set the datatype to write
    if ( ( datatype = H5Tcopy( complex_id ) ) < 0 )
      {
        std::cerr << "ERROR: Could not create datatype.\n";
      }

    // if there are chunk dimensions, write the data this way
    if ( chnkdims.size()>0 )
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,maxdims) ) < 0 )
          {
            std::cerr << "ERROR: Could not create dataspace for '"
                      << arrayname << "'.\n";
          }

        hid_t cparms;
        if ( ( cparms = H5Pcreate( H5P_DATASET_CREATE ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create property list for '"
                      << arrayname << "'.\n";
          }

        if ( ( H5Pset_chunk( cparms, rank, chunk_dims ) ) < 0 )
          {
            std::cerr << "ERROR: Could not set chunk size for '"
                      << arrayname << "'.\n";
          }

        if ( ( itsDatasetID = H5Dcreate( obj_id, arrayname.c_str(), datatype,
                                      dataspace, H5P_DEFAULT, cparms, H5P_DEFAULT ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array '" << arrayname << "'.\n";
          }
      }
    // otherwise, write the data this way
    else
      {
        if ( ( dataspace = H5Screate_simple(rank,mydims,NULL ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create dataspace for '"
                      << arrayname << "'.\n";
          }

        if ( ( itsDatasetID = H5Dcreate( obj_id, arrayname.c_str(), datatype,
                                      dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT ) ) < 0 )
          {
            std::cerr << "ERROR: Could not create array '" << arrayname << "'.\n";
          }

      }

    // write the data
    if ( H5Dwrite( itsDatasetID, datatype, dataspace, dataspace, H5P_DEFAULT,
                   data) < 0 )
      {
        std::cerr << "ERROR: Could not write array '" << arrayname << "'\n";
      }
    
    // close local hdf5 objects
    if ( H5Sclose( dataspace ) < 0 ) {
      std::cerr << "ERROR: Could not close dataspace for '"
		<< arrayname << "'.\n";
    }
    
    if ( H5Tclose( datatype ) < 0 ) {
      std::cerr << "ERROR: Could not close datatype for '"
		<< arrayname << "'.\n";
    }
    
  }

} //   END -- namespace DAL
