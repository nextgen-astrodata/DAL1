/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include <dalConversions.h>

/*!
  \file tdalConversions.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the dalConversions class
 
  \author Lars B&auml;hren
 
  \date 2010/08/27
*/

//_______________________________________________________________________________
//                                                             test_convertVector

/*!
  \brief Test conversion of vectors

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_convertVector ()
{
  std::cout << "\n[tdalConversions::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    // dalConversions newObject;
    // newObject.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
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
  nofFailedTests += test_convertVector ();

  return nofFailedTests;
}
