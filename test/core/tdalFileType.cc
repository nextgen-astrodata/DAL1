/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 3179 2009-10-14 13:20:10Z baehren                  $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#include <dalFileType.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::dalFileType;

/*!
  \file tdalFileType.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the dalFileType class
 
  \author Lars B&auml;hren
 
  \date 2010/04/07
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new dalFileType object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tdalFileType::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing dalFileType() ..." << endl;
  try {
    dalFileType type;
    //
    type.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Testing dalFileType(dalFileType::Type) ..." << endl;
  try {
    dalFileType type_hdf5 (dalFileType::HDF5);
    dalFileType type_fits (dalFileType::FITS);
    dalFileType type_mscasa (dalFileType::MSCASA);
    //
    type_hdf5.summary();
    type_fits.summary();
    type_mscasa.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Testing dalFileType(std::string) ..." << endl;
  try {
    dalFileType type_hdf5 ("HDF5");
    dalFileType type_fits ("FITS");
    dalFileType type_mscasa ("MSCASA");
    //
    type_hdf5.summary();
    type_fits.summary();
    type_mscasa.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_parameters

/*!
  \brief Test methods to access parameters

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_parameters ()
{
  cout << "\n[tdalFileType::test_parameters]\n" << endl;

  int nofFailedTests (0);
  dalFileType type;

  type.summary();

  cout << "[1] Set file type ..." << endl;
  try {
    type.setType (dalFileType::HDF5);
    type.summary();
    
    type.setType (dalFileType::FITS);
    type.summary();
    
    type.setType (dalFileType::MSCASA);
    type.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Set filetype name ..." << endl;
  try {
    type.setType ("HDF5");
    type.summary();
    
    type.setType ("FITS");
    type.summary();
    
    type.setType ("MSCASA");
    type.summary();
    
    type.setType ("MSTABLE");
    type.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();
  // Test methods to access parameters
  nofFailedTests += test_parameters ();

  return nofFailedTests;
}
