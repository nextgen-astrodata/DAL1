/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#include <core/dalObjectBase.h>
#include <core/dalFileType.h>
#include <core/IO_Mode.h>

// Namespace usage
using DAL::dalObjectBase;

/*!
  \file tdalObjectBase.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::dalObjectBase class
 
  \author Lars B&auml;hren
 
  \date 2011-07-11
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new dalObjectBase object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tdalObjectBase::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing dalObjectBase() ..." << std::endl;
  try {
    dalObjectBase newObject;
    //
    newObject.summary(); 
  } catch (std::string& message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing dalObjectBase(string) ..." << std::endl;
  try {
    std::string name ("NewObject");
    //
    dalObjectBase newObject (name);
    //
    newObject.summary(); 
  } catch (std::string& message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] Testing dalObjectBase(string,dalFileType) ..." << std::endl;
  try {
    std::string name ("NewObject");
    DAL::dalFileType filetype (DAL::dalFileType::HDF5);
    //
    dalObjectBase newObject (name, filetype);
    //
    newObject.summary(); 
  } catch (std::string& message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[4] Testing dalObjectBase(string,dalFileType,IO_Mode) ..." << std::endl;
  try {
    std::string name ("NewObject");
    DAL::dalFileType filetype (DAL::dalFileType::HDF5);
    DAL::IO_Mode flags (DAL::IO_Mode::Open);
    //
    dalObjectBase newObject (name, filetype,flags);
    //
    newObject.summary(); 
  } catch (std::string& message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

/*!
  \brief Main routine of the test program

  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

  return nofFailedTests;
}
