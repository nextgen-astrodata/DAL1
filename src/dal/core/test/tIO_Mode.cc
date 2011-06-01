/***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#include <core/IO_Mode.h>

// Namespace usage
using DAL::IO_Mode;

/*!
  \file tIO_Mode.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the IO_Mode class
 
  \author Lars B&auml;hren
*/

//_______________________________________________________________________________
//                                                            test_static_methods

/*!
  \brief Test statc methods

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_methods ()
{
  std::cout << "\n[tIO_Mode::test_static_methods]\n" << std::endl;

  int nofFailedTests (0);

  /*________________________________________________________
    Test 1: Retrive of map containing the available flags
   */
  
  std::cout << "[1] Testing IO_Mode::flagsMap() ..." << std::endl;
  try {
    std::map<IO_Mode::Flags,std::string> flags = IO_Mode::flagsMap();
    std::map<IO_Mode::Flags,std::string>::iterator it;

    for (it=flags.begin(); it!=flags.end(); ++it) {
      std::cout << " [ " << it->first << " , " << it->second << " ]" << std::endl;
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 2: Retrive array of flags
   */
  
  std::cout << "[2] Testing IO_Mode::flagsType() ..." << std::endl;
  try {
    std::vector<IO_Mode::Flags> flags = IO_Mode::flagsType();
    unsigned int nofFlags             = flags.size();

    for (unsigned int n=0; n<nofFlags; ++n) {
      std::cout << " [ " <<flags[n] << " ]" << std::endl;
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new IO_Mode object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  std::cout << "\n[tIO_Mode::test_constructors]\n" << std::endl;

  int nofFailedTests (0);

  /*________________________________________________________
    Test 1: Default constructor
  */
  
  std::cout << "[1] Testing IO_Mode() ..." << std::endl;
  try {
    IO_Mode mode;
    //
    mode.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 2: Argumented constructor with single flag
  */
  
  std::cout << "[2] Testing IO_Mode(IO_Mode::Flags) ..." << std::endl;
  try {
    std::vector<IO_Mode::Flags> flags = IO_Mode::flagsType ();

    for (unsigned int n=0; n<flags.size(); ++n) {
      IO_Mode mode (flags[n]);
      mode.summary(); 
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  /*________________________________________________________
    Test 3: Argumented constructor with multiple flags
  */
  
  std::cout << "[2] Testing IO_Mode(int) ..." << std::endl;
  try {
    IO_Mode mode1 (IO_Mode::Open|IO_Mode::ReadOnly);
    mode1.summary(); 
    //
    IO_Mode mode2 (IO_Mode::Open|IO_Mode::ReadWrite);
    mode2.summary(); 
    //
    IO_Mode mode3 (IO_Mode::Create|IO_Mode::WriteOnly);
    mode3.summary(); 
  } catch (std::string message) {
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

  // Test for the static methods
  nofFailedTests += test_static_methods ();

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

  return nofFailedTests;
}
