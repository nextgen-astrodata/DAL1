/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
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

#include <iostream>
#include <string>

#ifndef ENUMERATIONS_H
#include <Enumerations.h>
#endif

using std::cout;
using std::endl;

/*!
  \file tEnumerations.cpp

  \ingroup DAL

  \brief A collection of test routines for the Enumerations class
 
  \author Lars B&auml;hren
 
  \date 2008/01/31
*/

// -----------------------------------------------------------------------------

/*!
  \brief Test the output of the routines to provide the attribute names

  Retrieve the names of the various attributes:
  \verbatim
  -- TELESCOPE        = TELESCOPE
  -- OBSERVER         = OBSERVER
  -- PROJECT          = PROJECT
  -- OBSERVATION_ID   = OBS_ID
  -- OBSERVATION_MODE = OBS_MODE
  -- TIME             = TIME
  \endverbatim

  \return nofFailedTests -- The number of failed tests.
*/
int test_id2name ()
{
  int nofFailedTests (0);
  
  cout << "\n[tEnumerations::test_id2name]\n" << endl;

  try {
    cout << "-- TELESCOPE        = "
	 << DAL::attribute_name (DAL::TELESCOPE) << endl;
    cout << "-- OBSERVER         = "
	 << DAL::attribute_name (DAL::OBSERVER) << endl;
    cout << "-- PROJECT          = "
	 << DAL::attribute_name (DAL::PROJECT) << endl;
    cout << "-- OBSERVATION_ID   = "
	 << DAL::attribute_name (DAL::OBSERVATION_ID) << endl;
    cout << "-- OBSERVATION_MODE = "
	 << DAL::attribute_name (DAL::OBSERVATION_MODE) << endl;
    cout << "-- TIME             = "
	 << DAL::attribute_name (DAL::TIME) << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

int main ()
{
  int nofFailedTests (0);
  
  nofFailedTests += test_id2name ();

  return nofFailedTests;
}
