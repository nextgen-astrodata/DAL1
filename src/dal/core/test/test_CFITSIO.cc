/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

/*!
  \file testCFITSIO.cc

  \ingroup DAL
  \ingroup core

  \brief A number of basic test for working with the CFITSIO library
  
  \author Lars B&auml;hren

  <h3>Synopsis</h3>

  Though ultimately the aim of the DA is to abstract away some of the
  peculiarities of the wrapped underlying low-level library calls, it makes 
  sense to test working with them separately.
*/

#include <iostream>
#include <string>

#include <dal_config.h>

#ifdef DAL_WITH_CFITSIO

//_______________________________________________________________________________
//                                                                test_create_new

int test_create_new (std::string const &filename,
		     long const &nelem=512)
{
  int nofFailedTests  = 0;
  int status          = 0;
  long rank           = 2;
  long nelements      = 1;
  long fpixel         = 1;
  std::string outfile = "!" + filename;
  long naxes[rank];
  float pixels[nelem][nelem];
  fitsfile *fptr;

  for (long n=0; n<rank; ++n) {
    naxes[n]   = nelem;
    nelements *= nelem;
  }

  /*__________________________________________________________________
    Test 1: Create new FITS file
  */
  
  std::cout << "[1] Creating new FITS file ..." << std::endl;
  try {
    fits_create_file (&fptr, outfile.c_str(), &status);
    //
    std::cout << "-- Filename = " << filename << std::endl;
    std::cout << "-- Status   = " << status   << std::endl;
  } catch (std::string& message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 2: Create primary image array
  */
  
  std::cout << "[2] Creating primary image array ..." << std::endl;
  try {
    fits_create_img (fptr, FLOAT_IMG, rank, naxes, &status);
    //
    std::cout << "-- nof axes = " << rank << std::endl;
  } catch (std::string& message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  /*__________________________________________________________________
    Test 3: Write primary image array
  */
  
  std::cout << "[3] Write primary image array ..." << std::endl;
  try {
    
    std::cout << "-- Fill the pixel value array ..." << std::endl;
    for (long nx(0); nx<nelem; nx++) {
      for (long ny(0); ny<nelem; ny++) {
	pixels[ny][nx] = 1.0*(nx+ny);
      }
    }
    
    std::cout << "-- write image array to file ..." << std::endl;
    fits_write_img(fptr, TFLOAT, fpixel, nelements, pixels[0], &status);
    
  } catch (std::string& message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  /*__________________________________________________________________
    Test 4
  */
  
  try {
    std::cout << "-- closing file " << filename << " ..." << std::endl;
    fits_close_file (fptr, &status);
    fptr = NULL;
  } catch (std::string& message) {
    std::cerr << message << std::endl;
    ++nofFailedTests;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                             test_open_existing

int test_open_existing (std::string const &filename)
{
  int nofFailedTests (0);

  std:: cout << "-- filename = " << filename << std::endl;

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc, char *argv[])
{
  int nofFailedTests (0);
  std::string filename ("testCFITSIO.fits");

  nofFailedTests += test_create_new (filename);

  if (argc>1) {
    // get name of FITS file
    filename        = argv[1];
    // run tests on the file
    nofFailedTests += test_open_existing (filename);
  }
  
  return nofFailedTests;
}

#else 

int main ()
{
  std::cerr << "[test_CFITSIO] DAL build without support for CFITSIO - skipping test!"
	    << std::endl;

  return -1;
}

#endif 
