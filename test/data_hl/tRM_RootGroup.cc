/***************************************************************************
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

#include <data_hl/RM_RootGroup.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::CommonAttributes;
using DAL::Filename;
using DAL::RM_RootGroup;

/*!
  \file tRM_RootGroup.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the RM_RootGroup class
 
  \author Lars B&auml;hren
 
  \date 2009/10/28
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new RM_RootGroup object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (std::string const &filename)
{
  cout << "\n[tRM_RootGroup::test_constructors]\n" << endl;

  int nofFailedTests (0);

  cout << "[1] Testing RM_RootGroup (string) ..." << endl;
  try {
    RM_RootGroup rm (filename);
    rm.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_attributes

/*!
  \brief Test access to the attributes attached to the Beam group

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_attributes (std::string const &filename)
{
  cout << "\n[tRM_RootGroup::test_attributes]\n" << endl;

  int nofFailedTests (0);
  RM_RootGroup rm (filename);
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);
  std::string filename ("tRM_RootGroup.h5");

  // Test for the constructor(s)
  nofFailedTests += test_constructors (filename);
  // Test access to the attributes
  nofFailedTests += test_attributes (filename);

  return nofFailedTests;
}
