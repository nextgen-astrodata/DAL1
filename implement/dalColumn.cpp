/***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
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

string dalColumn::getName()
{
  return name;
}

string dalColumn::getType()
{
  return datatype;
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

	if ( member_type == dal_INT )	{
			H5::CompType mtype1( sizeof(int) );
			mtype1.insertMember( member_name, 0, H5::PredType::NATIVE_INT);
	}
	else if ( member_type == dal_UINT )	{
			H5::CompType mtype1( sizeof(unsigned int) );
			mtype1.insertMember( member_name, 0, H5::PredType::NATIVE_UINT);
	}
	else if ( member_type == dal_SHORT )	{
			H5::CompType mtype1( sizeof(short) );
			mtype1.insertMember( member_name, 0, H5::PredType::NATIVE_SHORT);
	}
	else if ( member_type == dal_FLOAT )	{
			H5::CompType mtype1( sizeof(float) );
			mtype1.insertMember( member_name, 0, H5::PredType::NATIVE_FLOAT);
	}
	else if ( member_type == dal_DOUBLE )	{
			H5::CompType mtype1( sizeof(double) );
			mtype1.insertMember( member_name, 0, H5::PredType::NATIVE_DOUBLE);
	}
	else if ( member_type == dal_STRING )	{
			H5::CompType mtype1( sizeof(H5T_C_S1));
			mtype1.insertMember( member_name, 0, H5T_C_S1);
	}
	else {					     
			cout << "ERROR: addMember " << member_name << " " << member_type << " not supported." << endl;
	}
//	cout << "addMember " << member_name << " " << member_type << " to " << name << endl;
}
