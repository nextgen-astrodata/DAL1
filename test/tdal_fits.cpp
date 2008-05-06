/*-------------------------------------------------------------------------*
 | $Id:: tUseMeasures.cc 511 2007-08-05 13:14:48Z baehren                $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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
  \file tdal_fits.cpp
  
  \ingroup DAL

  \brief Test program for working with FITS data

  \author Joseph Masters
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#ifndef DALGROUP_H
#include <dalGroup.h>
#endif

#ifndef TIMESERIES_H
#include <timeseries.h>
#endif

using namespace DAL;

/*! sample doxygen comment in dal.cpp */
int main(int argc, char *argv[])
{

  // parameter check
  if ( argc < 2 )
  {
     cout << endl << "Too few parameters..." << endl << endl;
     cout << "The first parameter is a fits dataset path and name." << endl;
     cout << endl;
     return DAL::FAIL;
  }

// create a dataset
  dalDataset * dataset1;
//   dataset1->open(argv[1]);
//  if ( NULL == argv[2] )
	  dataset1 = new dalDataset( argv[1], "FITS" );
//   else
// 	  dataset1 = new dalDataset( argv[1], argv[2] );


//    dalDataset * dataset1;
//    dataset1 = new dalDataset( FILENAME );

// close dataset
//    dataset1->close();
   delete dataset1;


//    dalDataset * dataset2;
//    dataset2 = new dalDataset;
//    if ( 0 != dataset2->open( FILENAME ) )
//    {
// 	cout << "Problem opening dataset: " << FILENAME << '.' << " Quiting."
// 	     << endl;
//    	exit(FAIL);
//    }

//    dalGroup * groupA = dataset2->createGroup( "groupA" );
//    dalGroup * groupB = dataset2->createGroup( "groupB" );
//    dalGroup * groupC = dataset2->createGroup( "groupC" );
// 
//    dalTable * table1 = dataset2->createTable( "table1", "groupA" );
//    dalTable * table2 = dataset2->createTable( "table2", "groupA" );
//    dalTable * table3 = dataset2->createTable( "table3", "groupB" );
// 
// 
//    table1->addColumn( "col1", dal_INT );
//    table1->addColumn( "col2", dal_SHORT );
//    table1->addColumn( "col3", dal_FLOAT );
// 
//    table2->addColumn( "col1", dal_UINT );
//    table2->addColumn( "col2", dal_STRING );
//    table2->addColumn( "col3", dal_DOUBLE );
// 
//    table3->addColumn( "col1", dal_INT );
//    table3->addColumn( "col2", dal_INT );
//    table3->addColumn( "col3", dal_INT );
// 
//    dataset2->close();
// 
//    delete table1;
//    delete table2;
//    delete table3;
//    delete groupA;
//    delete groupB;
//    delete groupC;
//    delete dataset2;

   cout << "SUCCESS" << endl;
   return DAL::SUCCESS;
}

