/***************************************************************************
 *   Copyright (C) 2008 by                                                 *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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
  \file tdalArray.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::dalArray class

  \author Lars B&auml;hren

  \date 2010/02/16
*/

#include <dalArray.h>

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test the various constructors for an object of type DAL::dalDataset

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors ()
{
  std::cout << "\n[tdalDataset::test_constructors]\n" << std::endl;

  int nofFailedTests (0);

  std::cout << "[1] Default constructor..." << std::endl;
  try {
    DAL::dalArray arr;
    //
    arr.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test the various constructors for an object of type DAL::dalDataset

  \param filename -- Name of the input HDF5 data file

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors (std::string const &filename)
{
  std::cout << "\n[tdalDataset::test_constructors]\n" << std::endl;

  int nofFailedTests (0);

  std::cout << "[1] Default constructor..." << std::endl;
  try {
    DAL::dalArray arr;
    //
    arr.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc, char *argv[])
{
  int nofFailedTests (0);
  bool haveDataset (true);
  std::string filename ("tdalArray.h5");

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    filename    = argv[1];
    haveDataset = true;
  }

  //________________________________________________________
  // Run the tests

  nofFailedTests += test_constructors ();

  if (haveDataset) {
    nofFailedTests += test_constructors (filename);
  }

  return nofFailedTests;
}
