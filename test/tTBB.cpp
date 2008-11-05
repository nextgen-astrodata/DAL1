/*-------------------------------------------------------------------------*
 | $Id:: tNewClass.cc 1964 2008-09-06 17:52:38Z baehren                  $ |
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

#include <dal.h>
#include <TBB.h>

// Namespace usage
using DAL::TBB;

/*!
  \file tTBB.cc

  \ingroup DAL

  \brief A collection of test routines for the TBB class
 
  \author Lars B&auml;hren
 
  \date 2008/11/05
*/

// -----------------------------------------------------------------------------

/*!
  \brief Test constructors for a new TBB object

  \return nofFailedTests -- The number of failed tests within this function.
*/
int test_constructors (std::string const &filename)
{
  int nofFailedTests (0);
  
  std::cout << "\n[tTBB::test_constructors]\n" << std::endl;

  std::cout << "[1] Testing default constructor ..." << std::endl;
  try {
    TBB newTBB (filename);
    //
    newTBB.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

int main ()
{
  int nofFailedTests (0);
  std::string outfile;

  // Test for the constructor(s)
  nofFailedTests += test_constructors (outfile);

  return nofFailedTests;
}
