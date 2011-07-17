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

#include "dalData.h"

namespace DAL {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                      dalData
  
  dalData::dalData ()
    : dalObjectBase ()
  {
    init();
  }
  
  //_____________________________________________________________________________
  //                                                                      dalData
  
  /*!
    \param filetype -- File type: CASA_MS, HDF5, FITS, etc.
    \param datatype -- Type of the data,  i.e. "dal_COMPLEX", "dal_INT", "dal_FLOAT"
    \param shape    -- Shape of the data array.
    \param nofRows  -- Number of rows inside the array.
  */
  dalData::dalData (dalFileType::Type const &filetype,
		    std::string const &datatype,
		    std::vector<int> const &shape,
		    long const &nofRows)
    : dalObjectBase (filetype)
  {
    init ();
    itsDatatype   = datatype;
    itsShape      = shape;
    itsNofRows    = nofRows;
  }
  
  //_____________________________________________________________________________
  //                                                                      dalData
  
  /*!
    \param filetype -- File type: CASA_MS, HDF5, FITS, etc.
    \param datatype -- Type of the data,  i.e. "dal_COMPLEX", "dal_INT", "dal_FLOAT"
    \param shape    -- Shape of the data array.
    \param nofRows  -- Number of rows inside the array.
  */
  dalData::dalData (dalFileType const &filetype,
		    std::string const &datatype,
		    std::vector<int> const &shape,
		    long const &nofRows)
    : dalObjectBase (filetype)
  {
    init ();

    itsDatatype = datatype;
    itsShape    = shape;
    itsNofRows  = nofRows;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void dalData::summary (std::ostream &os)
  {
    os << "[dalData] Summary of object properties"  << std::endl;
    os << "-- File type      = " << itsFiletype.name() << std::endl;
    os << "-- I/O mode flags = " << itsFlags.names()   << std::endl;
    os << "-- Data type      = " << itsDatatype        << std::endl;
    os << "-- Array order    = " << itsArrayOrder      << std::endl;
  }
  
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
    
    if (indices.size() != (itsShape.size()) )
      {
        cerr << "ERROR: Number of indices do not match shape of column." << endl;
        return(-1);
      }
    
    unsigned long index = 0;
    long bb = 1;
    
    itsShape.insert( itsShape.begin(), 1 );
    for (unsigned int dim=0; dim<itsShape.size()-1; dim++) {
      for (unsigned int ss=dim; ss>0; ss--)
	bb *= itsShape[ ss ];
      
      index += indices[dim]*bb;
      bb=1;
    }
    itsShape.erase( itsShape.begin() );
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

    if (indices.size() != (itsShape.size()) )
      {
        cerr << "ERROR: Number of indices do not match shape of column.\n";
        return(-1);
      }

    unsigned long index = 0;
    long bb = 1;

    //   std::cerr << "HDF5 HDF5 HDF5 HDF5 HDF5 HDF5" << endl;
    // indx = xx*shape3[1]*shape3[2] + yy*shape3[2] + zz;
    for (unsigned int dim = 0; dim < itsShape.size(); dim++)
      {
        for (unsigned int ss = itsShape.size()-1; ss > dim; ss--)
          {
            bb *= itsShape[ ss ];
          }

        index += indices[dim]*bb;
        bb=1;
      }
    return index;
  }

  //_____________________________________________________________________________
  //                                                                          get
  
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
    if ( itsFiletype.isCASA() ) {
      index = fortran_index( idx1, idx2, idx3 );
    }
    else if ( itsFiletype.isHDF5() ) {
      index = c_index( idx1, idx2, idx3 );
    }
    else {
      std::cerr << "ERROR: file type not supported in dalData object.\n";
      return NULL;
    }
    
    if ( dal_COMPLEX == itsDatatype )
      return (&(((std::complex<float>*)data)[ index ]));
    
    else if ( dal_COMPLEX_CHAR == itsDatatype )
      return (&(((std::complex<char>*)data)[ index ]));

    else if ( dal_COMPLEX_SHORT == itsDatatype )
      return (&(((std::complex<short>*)data)[ index ]));

    else if ( dal_DOUBLE == itsDatatype )
      return (&(((double*)data)[ index ]));

    else if ( dal_INT == itsDatatype )
      return (&(((int*)data)[ index ]));

    else if ( dal_SHORT == itsDatatype )
      return (&(((short*)data)[ index ]));

    else if ( dal_FLOAT == itsDatatype )
      return (&(((float*)data)[ index ]));

    else if ( dal_CHAR == itsDatatype )
      return (&(((char*)data)[ index ]));

    else if ( dal_STRING == itsDatatype )
      return (&(((std::string*)data)[ index ]));

    return NULL;
  }

  // ============================================================================
  //
  //  Private methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init
  
  void dalData::init ()
  {
    itsDatatype   = "UNKNOWN";
    itsArrayOrder = "UNKNOWN";
    data          = NULL;

    if ( itsFiletype.isCASA() ) {
      itsArrayOrder = "fortran";
    } else if ( itsFiletype.isHDF5() ) {
      itsArrayOrder = "c";
    }
  }
  
} // DAL namespace

