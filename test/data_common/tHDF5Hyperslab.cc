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

#include <HDF5Hyperslab.h>

// Namespace usage
using std::cout;
using std::endl;
using DAL::HDF5Hyperslab;

/*!
  \file tHDF5Hyperslab.cc

  \ingroup DAL
  \ingroup data_common

  \brief A collection of test routines for the HDF5Hyperslab class
 
  \author Lars B&auml;hren
 
  \date 2010/02/09
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new HDF5Hyperslab object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tHDF5Hyperslab::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "[1] HDF5Hyperslab () ..." << endl;
  try {
    HDF5Hyperslab slab;
    //
    slab.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] HDF5Hyperslab (shape) ..." << endl;
  try {
    int rank (2);
    std::vector<hsize_t> shape (rank);
    //
    shape[0] = 1024;
    shape[1] = 10;
    //
    HDF5Hyperslab slab (shape);
    slab.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] HDF5Hyperslab (shape,start,block) ..." << endl;
  try {
    int rank (2);
    std::vector<hsize_t> shape (rank);
    std::vector<int> start (rank);
    std::vector<int> block (rank);
    //
    shape[0] = 1024;
    shape[1] = 10;
    start[0] = 0;
    start[1] = 0;
    block[0] = shape[0]/2;
    block[1] = shape[1]/2;
    //
    HDF5Hyperslab slab (shape,start,block);
    slab.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[4] HDF5Hyperslab (shape,start,stride,count,block) ..." << endl;
  try {
    int rank (2);
    std::vector<hsize_t> shape (rank);
    std::vector<int> start (rank);
    std::vector<int> stride (rank);
    std::vector<int> count (rank);
    std::vector<int> block (rank);
    //
    shape[0]  = 1024;
    shape[1]  = 10;
    start[0]  = 0;
    start[1]  = 2;
    stride[0] = 1;
    stride[1] = 1;
    count[0]  = 1;
    count[1]  = 1;
    block[0]  = shape[0]/2;
    block[1]  = shape[1]/2;
    //
    HDF5Hyperslab slab (shape,start,stride,count,block);
    slab.summary(); 
    //
    count[0]  = 2;
    count[1]  = 2;
    //
    HDF5Hyperslab slab2 (shape,start,stride,count,block);
    slab2.summary(); 
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

  return nofFailedTests;
}
