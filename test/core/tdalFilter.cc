/***************************************************************************
 *   Copyright (C) 2010 by                                                 *
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
  \file tdalFilter.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::dalFilter class

  \author Lars B&auml;hren

  \date 2010/03/11
*/

#include <dalFilter.h>

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test the various constructors for an object of type DAL::dalFilter

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors ()
{
  std::cout << "\n[tdalDataset::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  std::string fileType ("H5TYPE");
  std::string columns ("Data");

  std::cout << "[1] Testing dalFilter() ..." << std::endl;
  try {
    DAL::dalFilter filter;
    //
    filter.summary();
  }
  catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  std::cout << "[2] Testing dalFilter(string) ..." << std::endl;
  try {
    DAL::dalFilter filter (fileType,columns);
    //
    filter.summary();
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
  std::string filename ("tdalFilter.h5");

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

  return nofFailedTests;
}
