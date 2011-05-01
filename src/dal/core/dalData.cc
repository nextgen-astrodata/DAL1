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

#include <core/dalData.h>

namespace DAL {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      dalData
  
  dalData::dalData ()
  {
    dataType_p  = "UNKNOWN";
    filetype_p  = "UNKNOWN";
    array_order = "UNKNOWN";
    data        = NULL;
  }
  
  //_____________________________________________________________________________
  //                                                                      dalData
  
  /*!
    \param lclfiletype The file type (i.e. "MSCASA", "HDF5", etc.)
    \param lcldatatype The data type this instance of the class will
                       contain.
    \param lclshape The shape of the data object.
    \param lclnrows The number of rows in the data object.
   */
  dalData::dalData(std::string lclfiletype, std::string lcldatatype,
                   std::vector<int> lclshape, long lclnrows)
  {
    filetype_p = lclfiletype;
    if ( MSCASATYPE == lclfiletype )
      array_order = "fortran";
    else if ( H5TYPE == lclfiletype )
      array_order = "c";
    dataType_p = lcldatatype;
    shape = lclshape;
    nrows = lclnrows;
    data = NULL;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                fortran_index
  
  /*!
    This is a helper function that is usually called by the dataset
    object and not by the developer.  Its purpose is to find a single
    index value within a multi-dimensional array, but that index depends
    on the storage method of the underlying file.  CASA arrays are stored
    in FORTAN order.  HDF5 arrays are stored in C order.

    \param idx1 Specifies the first index.
    \param idx2 Specifies the second index.
    \param idx3 Specifies the third index.

    \return A single value index for the multi-dimensional array.
  */
  unsigned long dalData::fortran_index (long idx1,
					long idx2,
					long idx3)
  {
    std::vector<long> indices;
    
    if ( idx1>-1 )
      indices.push_back( idx1 );
    if ( idx2>-1 )
      indices.push_back( idx2 );
    if ( idx3>-1 )
      indices.push_back( idx3 );
    
    if (indices.size() != (shape.size()) )
      {
        cerr << "ERROR: Number of indices do not match shape of column." << endl;
        return(-1);
      }
    
    unsigned long index = 0;
    long bb = 1;
    
    shape.insert( shape.begin(), 1 );
    for (unsigned int dim=0; dim<shape.size()-1; dim++)
      {
        for (unsigned int ss=dim; ss>0; ss--)
          bb *= shape[ ss ];

        index += indices[dim]*bb;
        bb=1;
      }
    shape.erase( shape.begin() );
    return index;
  }
  
  //_____________________________________________________________________________
  //                                                                      c_index
  
  /*!
    This is a helper function that is usually called by the dataset
    object and not by the developer.  Its purpose is to find a single
    index value within a multi-dimensional array, but that index depends
    on the storage method of the underlying file.  CASA arrays are stored
    in FORTAN order.  HDF5 arrays are stored in C order.

    \param idx1 Specifies the first index.
    \param idx2 Specifies the second index.
    \param idx3 Specifies the third index.

    \return A single value index for the multi-dimensional array.
   */
  unsigned long dalData::c_index (long idx1,
				  long idx2,
				  long idx3)
  {
    std::vector<long> indices;

    if ( idx1>-1 )
      indices.push_back( idx1 );
    if ( idx2>-1 )
      indices.push_back( idx2 );
    if ( idx3>-1 )
      indices.push_back( idx3 );

    if (indices.size() != (shape.size()) )
      {
        cerr << "ERROR: Number of indices do not match shape of column.\n";
        return(-1);
      }

    unsigned long index = 0;
    long bb = 1;

    //   std::cerr << "HDF5 HDF5 HDF5 HDF5 HDF5 HDF5" << endl;
    // indx = xx*shape3[1]*shape3[2] + yy*shape3[2] + zz;
    for (unsigned int dim = 0; dim < shape.size(); dim++)
      {
        for (unsigned int ss = shape.size()-1; ss > dim; ss--)
          {
            bb *= shape[ ss ];
          }

        index += indices[dim]*bb;
        bb=1;
      }
    return index;
  }

  // ------------------------------------------------------------ get
  
  /*!
    \param idx1 Optional parameter specifying the first index.
    \param idx2 Optional parameter specifying the second index.
    \param idx3 Optional parameter specifying the third index.

    \return void * Pointer to an arbitrary data structure.
   */
  void * dalData::get (long idx1,
		       long idx2,
		       long idx3)
  {
    unsigned long index = 0;
    
    //
    // Determine the correct index value, depending on the order
    //   of the underlying array (determined by the filetype)
    //
    if ( MSCASATYPE == filetype_p )
      {
        index = fortran_index( idx1, idx2, idx3 );
      }
    else if ( H5TYPE == filetype_p )
      {
        index = c_index( idx1, idx2, idx3 );
      }
    else
      {
        std::cerr << "ERROR: file type not supported in dalData object.\n";
        return NULL;
      }

    if ( dal_COMPLEX == dataType_p )
      return (&(((std::complex<float>*)data)[ index ]));

    else if ( dal_COMPLEX_CHAR == dataType_p )
      return (&(((std::complex<char>*)data)[ index ]));

    else if ( dal_COMPLEX_SHORT == dataType_p )
      return (&(((std::complex<short>*)data)[ index ]));

    else if ( dal_DOUBLE == dataType_p )
      return (&(((double*)data)[ index ]));

    else if ( dal_INT == dataType_p )
      return (&(((int*)data)[ index ]));

    else if ( dal_SHORT == dataType_p )
      return (&(((short*)data)[ index ]));

    else if ( dal_FLOAT == dataType_p )
      return (&(((float*)data)[ index ]));

    else if ( dal_CHAR == dataType_p )
      return (&(((char*)data)[ index ]));

    else if ( dal_STRING == dataType_p )
      return (&(((std::string*)data)[ index ]));

    return NULL;
  }

} // DAL namespace

