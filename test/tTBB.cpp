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

#ifndef TBB_H
#include "TBB.h"
#endif

// Namespace usage
using std::cerr;
using std::cout;
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
  cout << "\n[tTBB::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing default constructor ..." << std::endl;
  try {
    TBB newTBB (filename);
    //
    newTBB.summary(); 
  } catch (std::string message) {
    cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test the functions providing access to internal parameters

  \return nofFailedTests -- The number of failed tests within this function.
*/
int test_parameters (std::string const &filename)
{
  int nofFailedTests (0);
  TBB tbb (filename);

  tbb.setTelescope ("a");
  tbb.setObserver ("b");
  tbb.setProject ("c");
  tbb.setObservation_id ("d");
  tbb.setObservation_mode ("e");
  
  tbb.summary();

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

int main (int argc, char *argv[])
{
  int nofFailedTests (0);
  std::string rawfile;
  std::string outfile;
  std::string calfile;
  bool have_rawfile (false);
  bool have_outfile (false);
  bool have_calfile (false);

  //__________________________________________________________________
  // Check the input parameters provided form the command line

  if (argc < 2) {
    cerr << "[tTBB] Missing name of output file!" << std::endl;
    cerr << std::endl;
    cerr << " tTBB <HDF5 outfile>" << std::endl;
    cerr << " tTBB <HDF5 outfile> <TBB raw file>" << std::endl;
    cerr << " tTBB <HDF5 outfile> <TBB raw file> <Cal. file>" << std::endl;
    cerr << std::endl;
  }

  if (argc == 2) {
    outfile      = argv[1];
    have_outfile = true;
  }

  if (argc == 3) {
    rawfile      = argv[2];
    have_rawfile = true;
  }

  if (argc == 4) {
    calfile      = argv[3];
    have_calfile = true;
  }
  
  //__________________________________________________________________
  // Run the tests

  // Test for the constructor(s)
  nofFailedTests += test_constructors (outfile);

  if (nofFailedTests == 0) {

    nofFailedTests += test_parameters (outfile);

  }

  return nofFailedTests;
}
