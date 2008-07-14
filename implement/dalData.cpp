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

namespace DAL {

unsigned long dalData::fortran_index(long idx1, long idx2, long idx3)
{
   vector<long> indices;

   if( idx1>-1 )
     indices.push_back( idx1 );
   if( idx2>-1 )
     indices.push_back( idx2 );
   if( idx3>-1 )
     indices.push_back( idx3 );

   if (indices.size() != (shape.size()) )
   {
      cout << "ERROR: Number of indices do not match shape of column." << endl;
      exit(-1);
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

unsigned long dalData::c_index(long idx1, long idx2, long idx3)
{
   vector<long> indices;

   if( idx1>-1 )
     indices.push_back( idx1 );
   if( idx2>-1 )
     indices.push_back( idx2 );
   if( idx3>-1 )
     indices.push_back( idx3 );

   if (indices.size() != (shape.size()) )
   {
      cout << "ERROR: Number of indices do not match shape of column." << endl;
      exit(-1);
   }

     unsigned long index = 0;
     long bb = 1;

     //   cout << "HDF5 HDF5 HDF5 HDF5 HDF5 HDF5" << endl;
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

dalData::~dalData()
{
  if ( data )
   free(data);
}

string dalData::get_datatype()
{
   return datatype;
}

void dalData::toFloat()
{
   if ( dal_COMPLEX == datatype )
   {
      long length = 1;
      for ( unsigned int hh=0; hh < shape.size(); hh++ )
      {
	     length *= shape[ hh ];
	  }
	  data2 = new float[ length ];
	  float lclval;
      for ( int cell=0; cell < length; cell++ )
      {
	     cout << abs(*(&(((complex<float>*)data)[ cell ]))) << endl;
	     lclval = abs(*(&(((complex<float>*)data)[ cell ])));
		 exit(2);
	  }
	  cout << "length: " << length << endl;
	  exit(45);
   }

}
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

   if ( dal_COMPLEX == datatype )
      return (&(((complex<float>*)data)[ index ]));

   else if ( dal_COMPLEX_CHAR == datatype )
      return (&(((complex<char>*)data)[ index ]));

   else if ( dal_COMPLEX_SHORT == datatype )
      return (&(((complex<short>*)data)[ index ]));

   else if ( dal_DOUBLE == datatype )
      return (&(((double*)data)[ index ]));

   else if ( dal_INT == datatype )
      return (&(((int*)data)[ index ]));

   else if ( dal_SHORT == datatype )
      return (&(((short*)data)[ index ]));

   else if ( dal_FLOAT == datatype )
      return (&(((float*)data)[ index ]));

   else if ( dal_CHAR == datatype )
      return (&(((char*)data)[ index ]));

   else if ( dal_STRING == datatype )
      return (&(((string*)data)[ index ]));

   return NULL;
}

dalData::dalData()
{
  datatype = "";
  filetype = "";
  array_order = "";
  data = NULL;
}

dalData::dalData(string lclfiletype, string lcldatatype,
   vector<int> lclshape, long lclnrows)
{
    filetype = lclfiletype;
    if ( MSCASATYPE == lclfiletype )
       array_order = "fortran";
    else if ( H5TYPE == lclfiletype )
       array_order = "c";
    datatype = lcldatatype;
    shape = lclshape;
    nrows = lclnrows;
    data = NULL;
}

#ifdef PYTHON

bpl::numeric::array dalData::get_boost1()
{
   return get_boost3(0,-1);
}

bpl::numeric::array dalData::get_boost2( int32_t length )
{
   return get_boost3(0,length);
}

bpl::numeric::array dalData::get_boost3( int64_t offset, int32_t length )
{
  bpl::list data_list;
  vector<int> mydims;
      if ( dal_CHAR == datatype )
      {
        unsigned int hh = 0;

	if (length>0) { mydims.push_back(length); hh=1; }
        for (; hh<shape.size(); hh++)
	{ mydims.push_back(shape[hh]); }
	bpl::numeric::array narray = num_util::makeNum(((char*)data)+offset,mydims);
	return narray;
      }
      else if ( dal_INT == datatype )
      {
        unsigned int hh = 0;

	if (length>0) { mydims.push_back(length); hh=1; }
        for (; hh<shape.size(); hh++)
	{ mydims.push_back(shape[hh]); }
	bpl::numeric::array narray = num_util::makeNum(((int*)data)+offset,mydims);
	return narray;
      }
      else if ( dal_FLOAT == datatype )
      {
        unsigned int hh = 0;

	if (length>0) { mydims.push_back(length); hh=1; }
        for (; hh<shape.size(); hh++)
	{ mydims.push_back(shape[hh]); }
	bpl::numeric::array narray = num_util::makeNum(((float*)data)+offset,mydims);
	return narray;
      }
      else if ( dal_DOUBLE == datatype )
      {
        unsigned int hh = 0;

	if (length>0) { mydims.push_back(length); hh=1; }
        for (; hh<shape.size(); hh++)
	{ mydims.push_back(shape[hh]); }
	bpl::numeric::array narray = num_util::makeNum(((double*)data)+offset,mydims);
	vector<int> fshape = num_util::shape(narray);
	return narray;
      }
      else if ( dal_COMPLEX == datatype )
      {
        unsigned int hh = 0;

	if (length>0) { mydims.push_back(length); hh=1; }
        for (; hh<shape.size(); hh++)
	{ mydims.push_back(shape[hh]); }
	bpl::numeric::array narray = num_util::makeNum(((complex<float>*)data)+offset,mydims);
	vector<int> fshape = num_util::shape(narray);
	return narray;
      }
      else if ( dal_COMPLEX_CHAR == datatype )
      {
        unsigned int hh = 0;

	if (length>0) { mydims.push_back(length); hh=1; }
        for (; hh<shape.size(); hh++)
	{ mydims.push_back(shape[hh]); }
	bpl::numeric::array narray = num_util::makeNum(((complex<char>*)data)+offset,mydims);
	vector<int> fshape = num_util::shape(narray);
	return narray;
      }
      else if ( dal_COMPLEX_SHORT == datatype )
      {
        unsigned int hh = 0;

	if (length>0) { mydims.push_back(length); hh=1; }
        for (; hh<shape.size(); hh++)
	{ mydims.push_back(shape[hh]); }
	bpl::numeric::array narray = num_util::makeNum(((complex<short>*)data)+offset,mydims);
	vector<int> fshape = num_util::shape(narray);
	return narray;
      }
      else if ( dal_STRING == datatype )
      {
        unsigned int hh = 0;

	if (length>0) { mydims.push_back(length); hh=1; }
        for (; hh<shape.size(); hh++)
	{ mydims.push_back(shape[hh]); }
	bpl::list data_list;
	if ( 1 == shape.size() ) // 1D case
	{
          for (int ii=0; ii<nrows; ii++)
          {
             data_list.append( (*((string*)get(ii))) );
          }
	}
	else if ( 2 == shape.size() ) // 2D case
        {
                for ( int xx=0; xx<shape[0]; xx++)
                  for ( int yy=0; yy<shape[1]; yy++)
              data_list.append( (*((string*)get(xx,yy))) );
        }
        else if ( 3 == shape.size() ) // 3D case
        {
                for ( int xx=0; xx<shape[0]; xx++)
                  for ( int yy=0; yy<shape[1]; yy++)
                    for ( int zz=0; zz<shape[2]; zz++)
                data_list.append( (*((string*)get(xx,yy,zz))) );
        }
	else
	{
	  cout << "ERROR: string array rank > 3 not supported. dalData::get_boost()\n";
	}

	bpl::numeric::array narray = num_util::makeNum(data_list);
	return narray;
      }
      else 
      {
	cout << "ERROR:  Datatype '" << datatype << "' not yet supported.  (dalData::get_boost)\n";
	for (int ii=0; ii<1; ii++)
		data_list.append(0);
		bpl::numeric::array nadata( data_list );
	return nadata;
      }
}

#endif  // PYTHON


} // DAL namespace

