/*-------------------------------------------------------------------------*
 | $Id:: tHDF5Common.cpp 2022 2008-09-26 12:06:09Z baehren               $ |
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

#include <dalCommon.h>
#include <Enumerations.h>
#include <dalDataset.h>

#ifdef HAVE_CASA
#include <casa/Arrays/Vector.h>
#endif

using std::cout;
using std::cerr;
using std::endl;

// -----------------------------------------------------------------------------

/*!
  \file tdalCommon.cpp

  \ingroup DAL

  \brief A collection of test routines for the routines contained in dalCommon
 
  \author Lars B&auml;hren
 
  \date 2008/10/09
*/

// -----------------------------------------------------------------------------

/*!
  \brief Test of the service routines

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_service_routines ()
{
  cout << "\n[tdalCommon::test_service_routines]\n" << endl;
  
  int nofFailedTests (0);
  uint nelem (10);

  cout << "[1] Testing show(std::ostream&,T const *,uint const &) ..." << endl;
  try {
    int * arr_int;
    float * arr_float;
    double * arr_double;
    std::complex<double> * arr_complex;
    //
    arr_int     = new int [nelem];
    arr_float   = new float [nelem];
    arr_double  = new double [nelem];
    arr_complex = new std::complex<double> [nelem];
    //
    for (uint n(0); n<nelem; n++) {
      arr_int[n]     = int(n);
      arr_float[n]   = float(n);
      arr_double[n]  = double(n);
      arr_complex[n] = std::complex<double>(n);
    }
    //
    show (cout,arr_int,nelem);
    show (cout,arr_float,nelem);
    show (cout,arr_double,nelem);
    show (cout,arr_complex,nelem);
    //
    delete [] arr_int;
    delete [] arr_float;
    delete [] arr_double;
    delete [] arr_complex;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Test overloading of operator<< for std::vector<T> ..." << endl;
  try {
    std::vector<int> vec_int (nelem);
    std::vector<long> vec_long (nelem);
    std::vector<short> vec_short (nelem);
    std::vector<float> vec_float (nelem);
    std::vector<double> vec_double (nelem);
    
    cout << "-- vector<int>    = " << vec_int    << std::endl;
    cout << "-- vector<long>   = " << vec_long   << std::endl;
    cout << "-- vector<short>  = " << vec_short  << std::endl;
    cout << "-- vector<float>  = " << vec_float  << std::endl;
    cout << "-- vector<double> = " << vec_double << std::endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test of the routines providing access to HDF5 attributes

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_hdf5_attributes (std::string const &filename)
{
  cout << "\n[tdalCommon::test_hdf5_attributes]\n" << std::endl;

  int nofFailedTests (0);
  hid_t fileID;

  cout << "[1] Creating HDF5 file for testing ..." << endl;
  try {
    fileID = H5Fcreate ("tdalCommon.h5",
			H5F_ACC_TRUNC,
			H5P_DEFAULT,
			H5P_DEFAULT);
    
  } catch (std::string message) {
    cerr << message << endl;
    return 1;
  }

  cout << "[2] Set attributes of single value ..." << endl;
  try {
    int attr_int (1);
    uint attr_uint (2);
    long attr_long (3);
    float attr_float (4);
    double attr_double (5);
    std::string attr_string ("Hello");
    //
  } catch (std::string message) {
    cerr << message << endl;
    return 1;
  }
  
  return nofFailedTests;
}

// -----------------------------------------------------------------------------

int main (int argc,
	  char *argv[])
{
  int nofFailedTests (0);
  std::string filename;

  /* Check for command-line parameters */
  if (argc > 1) {
    filename = argv[1];
  }
  
  nofFailedTests += test_service_routines ();
  nofFailedTests += test_hdf5_attributes (filename);

  return nofFailedTests;
}
