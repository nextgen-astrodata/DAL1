/*-------------------------------------------------------------------------*
 | $Id:: tdal_fits.cpp 511 2007-08-05 13:14:48Z masters                  $ |
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

#include <dal_config.h>
#include <dalDataset.h>

using namespace DAL;

//_______________________________________________________________________________
//                                                                test_dalDataset

/*!
  \brief Test wrapping access to the FITS file through the DAL::dalDataset class
  
  \param filename -- The name of the FITS file to use for testing
  
  \return nofFailedTests -- The number of failed tests encountered with this
          function
*/
int test_dalDataset (std::string const &filename)
{
  std::cout << "\n[tdal_fits::test_dalDataset ]\n" << endl;

  int nofFailedTests   = 0;
  std::string fitsfile = filename;

  fitsfile += ".fits";

  std::cout << "[1] Creating new dalDataset for type FITS ..." << endl;
  try
    {
      std::cout << "-- Creating new dataset..." << endl;
      dalDataset * dataset1;
      std::cout << "-- Make the new dataset of type FITS ..." << endl;
      dataset1 = new dalDataset( fitsfile.c_str(), "FITS" );
      std::cout << "-- Release allocated memory..." << endl;
      delete dataset1;
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  std::cout << "[2] Open previously create dataset through dalDataset ..." << endl;
  try
    {
      dalDataset * dataset2;
      dataset2 = new dalDataset;
      if ( 0 != dataset2->open( fitsfile.c_str() ) )
        {
          cout << "Problem opening dataset: " << fitsfile << '.' << " Quiting."
               << endl;
          nofFailedTests++;
        }
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

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

  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \file tdal_fits.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of tests to work with FITS type data

  \author Joseph Masters, Lars B&auml;hren
 */

int main (int argc, char *argv[])
{
  int nofFailedTests   = 0;
  bool haveDataset     = false;
  std::string filename = "tdal_FITS.fits";

  /* Check parameter provided from the command line */
  if ( argc>1 ) {
    filename    = std::string(argv[1]);
    haveDataset = true;
  }
  
  return nofFailedTests;
}

