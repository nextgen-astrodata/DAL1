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

#include <ITS_ExperimentMeta.h>

// Namespace usage
using DAL::ITS_ExperimentMeta;

/*!
  \file tITS_ExperimentMeta.cc

  \ingroup DAL
  \ingroup data_hl

  \brief A collection of test routines for the DAL::ITS_ExperimentMeta class
 
  \author Lars B&auml;hren
 
  \date 2010/05/15
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new ITS_ExperimentMeta object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (std::string const &infile)
{
  std::cout << "\n[tITS_ExperimentMeta::test_constructors]\n" << std::endl;

  int nofFailedTests (0);
  
  std::cout << "[1] Testing ITS_ExperimentMeta()..." << std::endl;
  try {
    ITS_ExperimentMeta meta;
    //
    meta.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Testing ITS_ExperimentMeta(string)..." << std::endl;
  try {
    ITS_ExperimentMeta meta (infile);
    //
    meta.summary(); 
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc,
          char *argv[])
{
  int nofFailedTests (0);
  bool haveDataset (true);
  std::string infile ("experiment.meta");
  std::string outfile;

  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    // store the name of the input file
    infile      = argv[1];
    haveDataset = true;
    // [optional] store name of output file
  }

  //________________________________________________________
  // Run the tests

  // Test for the constructor(s)
  nofFailedTests += test_constructors (infile);

  return nofFailedTests;
}
