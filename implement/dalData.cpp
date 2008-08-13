/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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
#ifdef PYTHON
#define PY_ARRAY_UNIQUE_SYMBOL PyArrayHandle
#define NO_IMPORT_ARRAY
#endif

#ifndef DALDATA_H
#include "dalData.h"
#endif

namespace DAL
  {

// ------------------------------------------------------------ dalData

  dalData::dalData()
  {
    dtype = "UNKNOWN";
    filetype = "UNKNOWN";
    array_order = "UNKNOWN";
    data = NULL;
  }

// ------------------------------------------------------------ dalData

  /*!
    \brief Constructor with a specific file type.
     Constructor with a specific file type.

    \param lclfiletype The file type (i.e. "MSCASA", "HDF5", etc.)
    \param lcldatatype The data type this instance of the class will
                       contain.
    \param lclshape The shape of the data object.
    \param lclnrows The number of rows in the data object.
   */
  dalData::dalData(std::string lclfiletype, std::string lcldatatype,
                   std::vector<int> lclshape, long lclnrows)
  {
    filetype = lclfiletype;
    if ( MSCASATYPE == lclfiletype )
      array_order = "fortran";
    else if ( H5TYPE == lclfiletype )
      array_order = "c";
    dtype = lcldatatype;
    shape = lclshape;
    nrows = lclnrows;
    data = NULL;
  }

// ------------------------------------------------------------ fortran_index

  /*!
    \brief Get the fortran index value of up to a three-dimensional array.

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
  unsigned long dalData::fortran_index(long idx1, long idx2, long idx3)
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


// ------------------------------------------------------------ c_index

  /*!
    \brief Get the C index value of up to a three-dimensional array.

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
  unsigned long dalData::c_index(long idx1, long idx2, long idx3)
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


// ------------------------------------------------------------ ~dalData

  /*!
    \brief Default destructor.

    Defulat data object destructor.
   */
  dalData::~dalData()
  {
    if ( data )
      free(data);
  }

// ------------------------------------------------------------ get

  /*!
    \brief Get the data.

    Retrieve the data out of the object.

    \param idx1 Optional parameter specifying the first index.
    \param idx2 Optional parameter specifying the second index.
    \param idx3 Optional parameter specifying the third index.

    \return void * Pointer to an arbitrary data structure.
   */
  void * dalData::get( long idx1, long idx2, long idx3 )
  {
    unsigned long index = 0;

    //
    // Determine the correct index value, depending on the order
    //   of the underlying array (determined by the filetype)
    //
    if ( MSCASATYPE == filetype )
      {
        index = fortran_index( idx1, idx2, idx3 );
      }
    else if ( H5TYPE == filetype )
      {
        index = c_index( idx1, idx2, idx3 );
      }
    else
      {
        std::cerr << "ERROR: file type not supported in dalData object.\n";
        return NULL;
      }

    if ( dal_COMPLEX == dtype )
      return (&(((complex<float>*)data)[ index ]));

    else if ( dal_COMPLEX_CHAR == dtype )
      return (&(((complex<char>*)data)[ index ]));

    else if ( dal_COMPLEX_SHORT == dtype )
      return (&(((complex<short>*)data)[ index ]));

    else if ( dal_DOUBLE == dtype )
      return (&(((double*)data)[ index ]));

    else if ( dal_INT == dtype )
      return (&(((int*)data)[ index ]));

    else if ( dal_SHORT == dtype )
      return (&(((short*)data)[ index ]));

    else if ( dal_FLOAT == dtype )
      return (&(((float*)data)[ index ]));

    else if ( dal_CHAR == dtype )
      return (&(((char*)data)[ index ]));

    else if ( dal_STRING == dtype )
      return (&(((std::string*)data)[ index ]));

    return NULL;
  }

#ifdef PYTHON

// ------------------------------------------------------------ get_boost1

  bpl::numeric::array dalData::get_boost1()
  {
    return get_boost3(0,-1);
  }

// ------------------------------------------------------------ get_boost2

  bpl::numeric::array dalData::get_boost2( int32_t length )
  {
    return get_boost3(0,length);
  }

// ------------------------------------------------------------ get_boost3

  bpl::numeric::array dalData::get_boost3( int64_t offset, int32_t length )
  {
    bpl::list data_list;
    std::vector<int> mydims;

    unsigned int hh = 0;

    if (length>0)
      {
        mydims.push_back(length);
        hh=1;
      }
    for (; hh<shape.size(); hh++)
      {
        mydims.push_back(shape[hh]);
      }

    if ( dal_CHAR == dtype )
      {
        return num_util::makeNum( ((char*)data) + offset, mydims );
      }
    else if ( dal_BOOL == dtype )
      {
        return num_util::makeNum( ((unsigned char*)data) + offset, mydims );
      }
    else if ( dal_INT == dtype )
      {
        return num_util::makeNum( ((int*)data) + offset, mydims );
      }
    else if ( dal_FLOAT == dtype )
      {
        return num_util::makeNum(((float*)data)+offset,mydims);
      }
    else if ( dal_DOUBLE == dtype )
      {
        return num_util::makeNum(((double*)data)+offset,mydims);
      }
    else if ( dal_COMPLEX == dtype )
      {
        return num_util::makeNum(((complex<float>*)data)+offset,mydims);
      }
    else if ( dal_COMPLEX_CHAR == dtype )
      {
        return num_util::makeNum(((complex<char>*)data)+offset,mydims);
      }
    else if ( dal_COMPLEX_SHORT == dtype )
      {
        return num_util::makeNum(((complex<short>*)data)+offset,mydims);
      }
    else if ( dal_STRING == dtype )
      {
        bpl::list data_list;

        if ( 1 == shape.size() ) // 1D case
          {
            for (int ii=0; ii<nrows; ii++)
              {
                data_list.append( (*((std::string*)get(ii))) );
              }
          }
        else if ( 2 == shape.size() ) // 2D case
          {
            for ( int xx=0; xx<shape[0]; xx++)
              for ( int yy=0; yy<shape[1]; yy++)
                data_list.append( (*((std::string*)get(xx,yy))) );
          }
        else if ( 3 == shape.size() ) // 3D case
          {
            for ( int xx=0; xx<shape[0]; xx++)
              for ( int yy=0; yy<shape[1]; yy++)
                for ( int zz=0; zz<shape[2]; zz++)
                  data_list.append( (*((std::string*)get(xx,yy,zz))) );
          }
        else
          {
            std::cerr << "ERROR: string array rank > 3 not supported. "
                      << "dalData::get_boost()\n";
          }

        bpl::numeric::array narray = num_util::makeNum(data_list);
        return narray;
      }
    else
      {
        std::cerr << "ERROR:  Datatype '" << dtype
                  << "' not yet supported.  (dalData::get_boost)\n";

        for (int ii=0; ii<1; ii++)
          data_list.append(0);

        bpl::numeric::array nadata( data_list );

        return nadata;
      }
  }

#endif  // PYTHON


} // DAL namespace

