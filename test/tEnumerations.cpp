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

#include <Data/LOFAR_Attributes.h>

using std::cout;
using std::endl;

/*!
  \file tLOFAR_Attributes.cc

  \ingroup LOFAR

  \brief A collection of test routines for the LOFAR_Attributes class
 
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
int test_LOFAR_Attributes ()
{
  int nofFailedTests (0);
  
  cout << "\n[test_LOFAR_Attributes]\n" << endl;

  try {
    cout << "-- TELESCOPE        = "
	 << LOFAR::attribute_name (LOFAR::TELESCOPE) << endl;
    cout << "-- OBSERVER         = "
	 << LOFAR::attribute_name (LOFAR::OBSERVER) << endl;
    cout << "-- PROJECT          = "
	 << LOFAR::attribute_name (LOFAR::PROJECT) << endl;
    cout << "-- OBSERVATION_ID   = "
	 << LOFAR::attribute_name (LOFAR::OBSERVATION_ID) << endl;
    cout << "-- OBSERVATION_MODE = "
	 << LOFAR::attribute_name (LOFAR::OBSERVATION_MODE) << endl;
    cout << "-- TIME             = "
	 << LOFAR::attribute_name (LOFAR::TIME) << endl;
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
  
  nofFailedTests += test_LOFAR_Attributes ();

  return nofFailedTests;
}
