/*-------------------------------------------------------------------------*
| $Id:: cs1.h 389 2007-06-13 08:47:09Z baehren                          $ |
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

/**
 * \file dalColumn.cpp
 * \author Joseph Masters
 * \date 12-11-06
 */
 
#ifndef DALCOLUMN_H
#include "dalColumn.h"
#endif

dalColumn::dalColumn()
{}

dalColumn::dalColumn( string colname, string type )
{
	name = colname;
	datatype = type;
}

dalColumn::dalColumn( string complexcolname/*, void * dataformat*/ )
{
	name = complexcolname;
	datatype = "dal_COMPLEX";
}

#ifdef WITH_CASA
dalColumn::dalColumn(casa::Table table, string colname)
{
    type = "MSCASA";
	casa_column = new casa::ROTableColumn( table, colname );
	casa_col_desc = casa_column->columnDesc();
}
#endif

void dalColumn::close()
{
#ifdef WITH_CASA
	delete casa_column;
#endif
}

void dalColumn::getDataType()
{
#ifdef WITH_CASA
	cout << casa_col_desc.dataType() << endl;
#endif
}

int dalColumn::isScalar()
{
#ifdef WITH_CASA
   if ( casa_col_desc.isScalar() )
      return 1;
   else
      return 0;
#endif
}

int dalColumn::isArray()
{
#ifdef WITH_CASA
   if ( casa_col_desc.isArray() )
      return 1;
   else
      return 0;
#endif
}

vector<int> dalColumn::shape()
{
  vector<int> shape_vals;
#ifdef WITH_CASA
  if ( isArray() )
  {
    //cout << casa_column->shapeColumn();
	casa::IPosition ipos = casa_column->shapeColumn();
	casa::Vector<casa::Int> col_shape = ipos.asVector();
	col_shape.tovector( shape_vals );
	return shape_vals;
  }
  else
  {
    cout << "column is not an array" << endl;
    return shape_vals;
  }
#endif
}

unsigned int dalColumn::ndims()
{
#ifdef WITH_CASA
  if ( isArray() )
   return casa_column->ndimColumn();
  else
  {
	return 0;
  }
#endif
}

string dalColumn::getName()
{
  return name;
}

unsigned int dalColumn::nrows()
{
  if ( "MSCASA" == type )
  {
#ifdef WITH_CASA
    num_of_rows = casa_column->nrow();
	return num_of_rows;
#endif
  }
  else
  {
    cout << "dalColumn::nrows() File type " << type << " not yet supported."; 
    return 0;
  }
}

string dalColumn::getType()
{
  return datatype;
}

void dalColumn::data()
{
  if ( "MSCASA" == type )
  {
    if ( isScalar() )
    {
      switch ( casa_col_desc.dataType() )
      {
	     case casa::TpDouble:
		 {
		    double * value = new double[ num_of_rows ];
			for (unsigned int ii=0; ii < num_of_rows; ii++ )
			{	
			  casa_column->getScalar( ii, value[ii] );
			}
		  }
		  break;
		 
		  default:
		   cout << "dalColumn::data() Column type not yet supported.";
	    }
	  }
      else
	  {
	    vector<int> lclshape;
		lclshape = shape();
        int lclndims;
		lclndims = ndims();
		for (unsigned int ii=0; ii < lclshape.size(); ii++ )
		{
		  int cell = lclshape[ ii ];
		  casa::IPosition start (1,cell);
		  casa::Slicer slicer (start);
		  double * values = new double[ num_of_rows ];
//		  casa_column->getColumnSlice( slicer, (double *) values );
		}
	  }	  
    }	
}

int dalColumn::getSize()
{
	if ( dal_INT == getType() ) {
		return sizeof(int);
	}
	else if ( dal_FLOAT == getType() ) {
		return sizeof(float);
	}
	else if ( dal_DOUBLE == getType() ) {
		return sizeof(double);
	}
	return -1;
}

void dalColumn::addMember( string member_name, string member_type )
{
/*    array_tid = H5Tarray_create(H5T_NATIVE_CHAR, ARRAY_RANK,
		    array_dim, NULL);
*/
	if ( member_type == dal_INT )	{
		status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_INT );
	}
	else if ( member_type == dal_UINT )	{
		status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_UINT );
	}
	else if ( member_type == dal_SHORT )	{
		status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_SHORT );
	}
	else if ( member_type == dal_FLOAT )	{
		status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_FLOAT );
	}
	else if ( member_type == dal_DOUBLE )	{
		status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_NATIVE_DOUBLE );
	}
	else if ( member_type == dal_STRING )	{
		status = H5Tinsert(coltype, member_name.c_str(), 0, H5T_C_S1 );
	}
	else {					     
		cout << "ERROR: addMember " << member_name << " "
			<< member_type << " not supported." << endl;
	}
	cout << "addMember " << member_name << " " << member_type << " to " << name << endl;
}
