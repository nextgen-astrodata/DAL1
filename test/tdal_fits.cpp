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

#include <dal.h>

using namespace DAL;

using std::endl;

// ------------------------------------------------------------------------------

/*!
  \brief Test opening an existing file and extracting some information from it

  This function contains a number of very basic tests for directly interacting
  the CFITSIO library (which in turn is used as underlying engine by the classes
  of the DAL):
  - open an existing FITS file
  - create a new FITS file containing a primary image
  - extract keyword values from the FITS header

  \param filename -- The name of the FITS file to use for testing

  \return nofFailedTests -- The number of failed tests encountered with this
          function
*/
int test_FITS (std::string const &filename)
{
  std::cout << "\n[tdal_fits::test_FITS]\n" << std::endl;
  
  int nofFailedTests = 0;
  int status         = 0;
  
  std::cout << "[1] Opening FITS file for reading ..." << std::endl;
  try {
    fitsfile *fptr;
    fits_open_file(&fptr, filename.c_str(), READWRITE, &status);
    //
    std::cout << "-- Filename = " << filename << std::endl;
    std::cout << "-- Status   = " << status   << std::endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  std::cout << "[2] Creating FITS with primary array image" << std::endl;
  try {
    long nelem (1024);
    long naxis (2);
    long naxes[2] = {nelem,nelem};
    long nelements (nelem*nelem);
    long fpixel (1);
    float pixels[nelem][nelem];

    std::cout << "-- Create new file ..." << std::endl;
    fitsfile *fptr;
    fits_create_file (&fptr, "!testimage1.fits", &status);

    std::cout << "-- Create the primary array image ..." << std::endl;
    fits_create_img (fptr, FLOAT_IMG, naxis, naxes, &status);

    std::cout << "-- Fill the pixel value array ..." << std::endl;
    for (int nx(0); nx<nelem; nx++) {
      for (int ny(0); ny<nelem; ny++) {
	pixels[ny][nx] = 1.0*(nx+ny);
      }
    }

    std::cout << "-- Write the pixel value to the file ..." << std::endl;
    fits_write_img(fptr, TFLOAT, fpixel, nelements, pixels[0], &status);

    std::cout << "-- Close FITS file ..." << std::endl;
    fits_close_file(fptr, &status);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  
  return nofFailedTests;
}

// ------------------------------------------------------------------------------

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
  try {
    std::cout << "-- Creating new dataset..." << endl;
    dalDataset * dataset1;
    std::cout << "-- Make the new dataset of type FITS ..." << endl;
    dataset1 = new dalDataset( fitsfile.c_str(), "FITS" );
    std::cout << "-- Release allocated memory..." << endl;
    delete dataset1;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[2] Open previously create dataset through dalDataset ..." << endl;
  try {
    dalDataset * dataset2;
    dataset2 = new dalDataset;
    if ( 0 != dataset2->open( fitsfile.c_str() ) ) {
      cout << "Problem opening dataset: " << fitsfile << '.' << " Quiting."
	   << endl;
      nofFailedTests++;
    }
  } catch (std::string message) {
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
  \file tdal_fits.cpp

  \ingroup DAL

  \brief A collection of tests to work with FITS type data

  \author Joseph Masters, Lars B&auml;hren
 */

int main (int argc,
	  char *argv[])
{
  int nofFailedTests = 0;

  /* Check parameter provided from the command line */
  if ( argc < 2 ) {
    cout << "[tdal_fits] Too few parameters..." << endl << endl;
    cout << "The first parameter is a fits dataset path and name." << endl;
    cout << endl;
    return DAL::FAIL;
  }

  std::string filename (argv[1]);

  nofFailedTests += test_FITS (filename);

  return nofFailedTests;
}

