/***************************************************************************
 *   Copyright (C) 2008                                                    *
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

#include <core/dalCommon.h>
#include <core/dalDataset.h>

#ifdef DAL_WITH_CASA
#include <casa/Arrays/Vector.h>
#include <casa/BasicSL/String.h>
#endif

using std::cout;
using std::cerr;
using std::endl;

// -----------------------------------------------------------------------------

/*!
  \file tdalCommon.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the routines contained in dalCommon

  \author Lars B&auml;hren

  \date 2008/10/09

  <h3>Examples</h3>

  Calling the test program without any further command-line parameters will 
  result in a number of tests which do not require any existing data-sets.
*/

//_______________________________________________________________________________
//                                                        test_casacore_variables

/*!
  \brief Test operation performed with/on casacore variable types

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
#ifdef DAL_WITH_CASA
int test_casacore_variables ()
{
  cout << "\n[tdalCommon::test_operators]\n" << endl;

  int nofFailedTests (0);

  cout << "[1] Assign std::string -> casa::String" << endl;
  try {
    std::string string_std ("LOFAR");
    casa::String string_casa;
    //
    string_casa = string_std;
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Assign std::string -> casa::Vector<casa::String>" << endl;
  try {
    uint nelem (20);
    std::string string_std ("LOFAR");
    casa::Vector<casa::String> string_casa (nelem);
    //
    for (uint n(0); n<nelem; n++) {
      string_casa(n) = string_std;
    }
  }
  catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}
#endif

//_______________________________________________________________________________
//                                                                 test_iterators

/*!
  \brief Test working with C++ iterators on various types of arrays

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_iterators ()
{
  cout << "\n[tdalCommon::test_iterators]\n" << endl;

  int nofFailedTests (0);
  unsigned int nelem (10);
  unsigned int n(0);
  std::vector<double>::iterator it;

  // Create 1-dim arrays of different types
  double * arr = new double [nelem];
  std::vector<double> vec (nelem);

  // Assign values to the c-array
  for (n=0; n<nelem; ++n) {
    arr[n] = 0.5*n;
  }

  for (it=vec.begin(), n=0; it!=vec.end(); ++it, ++n) {
    *it = arr[n];
  }

  std::cout << "-- STL vector<T> = " << vec                   << std::endl;
  std::cout << "-- (end-begin)   = " << vec.end()-vec.begin() << std::endl;
  std::cout << "-- nof. elements = " << vec.size()            << std::endl;

  delete [] arr;
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_beamformed

/*!
  \brief Test access to a beam-formed dataset

  \param infile -- Name of the input HDF5 file on which the access routines are
         tested.

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_beamformed (std::string const &infile)
{
  cout << "\n[tdalCommon::test_beamformed]\n" << endl;

  int nofFailedTests (0);

  hid_t fileID (-1);

  //__________________________________________________________________
  // Open the HDF5 dataset

  cout << "-- opening HDF5 file " << infile << " ..." << endl;
  fileID = H5Fopen (infile.c_str(),
                    H5F_ACC_RDWR,
                    H5P_DEFAULT);

  if (fileID < 0) {
    cerr << "Failed to open file " << infile << endl;
    return 1;
  }
  
  //__________________________________________________________________
  // Access the attributes attached to the root group of the file

  cout << "-- Access the attributes attached to the root group of the file"
       << endl;
  try {
    std::string name;
    std::string filename;
    std::string telescope;
    int nofStations;
    //
    DAL::h5get_name (name,fileID);
    DAL::HDF5Attribute::read (fileID,"NUMBER_OF_STATIONS",nofStations);
    //
    cout << "-- FILENAME ......... = " << filename     << endl;
    cout << "-- TELESCOPE ........ = " << telescope    << endl;
    cout << "-- NUMBER_OF_STATIONS = " << nofStations  << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    ++nofFailedTests;
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
  
#ifdef DAL_WITH_CASA
  nofFailedTests += test_casacore_variables ();
#endif
  
  // Test usage of iterators on STL containers
  nofFailedTests += test_iterators ();
  
  return nofFailedTests;
}
