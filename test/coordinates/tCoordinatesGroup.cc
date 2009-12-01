/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 2286 2009-02-03 10:50:48Z baehren                  $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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

#include <CoordinatesGroup.h>

// Namespace usage
using DAL::CoordinatesGroup;

/*!
  \file tCoordinatesGroup.cc

  \ingroup DAL

  \brief A collection of test routines for the CoordinatesGroup class
 
  \author Lars B&auml;hren
 
  \date 2009/09/13
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new CoordinatesGroup object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tCoordinatesGroup::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    CoordinatesGroup coord;
    //
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                        test_io

/*!
  \brief Test reading/writing of the cordinates group to a file

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_io ()
{
  int nofFailedTests (0);

#ifdef HAVE_HDF5
#endif

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  
  // Test reading/writing of the cordinates group to a file
  nofFailedTests += test_io ();

  return nofFailedTests;
}
