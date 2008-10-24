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
int test_hdf5_attributes ()
{
  cout << "\n[tdalCommon::test_hdf5_attributes]\n" << std::endl;

  int nofFailedTests (0);
  std::string filename ("tdalCommon.h5");
  uint nelem (3);
  hid_t fileID (0);
  herr_t h5error (0);

  cout << "[1] Creating HDF5 file for testing ..." << endl;
  try {
    fileID = H5Fcreate (filename.c_str(),
			H5F_ACC_TRUNC,
			H5P_DEFAULT,
			H5P_DEFAULT);
    
  } catch (std::string message) {
    cerr << message << endl;
    return 1;
  }

  cout << "[2] Set attributes of single value ..." << endl;
  try {
    std::cout << "-- Attribute of type int ..." << std::endl;
    int *data_int;
    data_int = new int[1];
    data_int[0] = 1;
    DAL::h5setAttribute( H5T_NATIVE_INT, fileID, "ATTRIBUTE_INT", data_int, 1 );
    //
    std::cout << "-- Attribute of type uint ..." << std::endl;
    uint *data_uint;
    data_uint = new uint[1];
    data_uint[0] = 2;
    DAL::h5setAttribute( H5T_NATIVE_UINT, fileID, "ATTRIBUTE_UINT", data_uint, 1 );
    //
    std::cout << "-- Attribute of type short ..." << std::endl;
    short *data_short;
    data_short = new short[1];
    data_short[0] = 3;
    DAL::h5setAttribute( H5T_NATIVE_SHORT, fileID, "ATTRIBUTE_SHORT", data_short, 1 );
    //
    std::cout << "-- Attribute of type long ..." << std::endl;
    long *data_long;
    data_long = new long[1];
    data_long[0] = 4;
    DAL::h5setAttribute( H5T_NATIVE_LONG, fileID, "ATTRIBUTE_LONG", data_long, 1 );
    //
    std::cout << "-- Attribute of type float ..." << std::endl;
    float *data_float;
    data_float = new float[1];
    data_float[0] = 5;
    DAL::h5setAttribute( H5T_NATIVE_FLOAT, fileID, "ATTRIBUTE_FLOAT", data_float, 1 );
    //
    std::cout << "-- Attribute of type double ..." << std::endl;
    double *data_double;
    data_double = new double[1];
    data_double[0] = 6;
    DAL::h5setAttribute( H5T_NATIVE_DOUBLE, fileID, "ATTRIBUTE_DOUBLE", data_double, 1 );
    //
    std::cout << "-- Attribute of type string ..." << std::endl;
    std::string  *data_string;
    data_string = new std::string[1];
    data_string[0] = "Hello";
    DAL::h5setAttribute_string( fileID, "ATTRIBUTE_STRING", data_string, 1 );
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[3] Set attributes of type array ..." << endl;
  try {
    int *data_int;
    uint *data_uint;
    short *data_short;
    long *data_long;
    float *data_float;
    double *data_double;
    std::string  *data_string;
    //
    std::cout << "-- Attribute of type int[3] ..." << std::endl;
    data_int = new int[3];
    data_int[0] = 1;
    data_int[1] = 2;
    data_int[2] = 3;
    DAL::h5setAttribute (H5T_NATIVE_INT,
			 fileID,
			 "ATTRIBUTE_INT_ARRAY",
			 data_int,
			 3);
    //
    std::cout << "-- Attribute of type uint[3] ..." << std::endl;
    data_uint = new uint[3];
    data_uint[0] = 1;
    data_uint[1] = 2;
    data_uint[2] = 3;
    DAL::h5setAttribute (H5T_NATIVE_UINT,
			 fileID,
			 "ATTRIBUTE_UINT_ARRAY",
			 data_uint,
			 3);
    //
    std::cout << "-- Attribute of type short[3] ..." << std::endl;
    data_short = new short[3];
    data_short[0] = 1;
    data_short[1] = 2;
    data_short[2] = 3;
    DAL::h5setAttribute (H5T_NATIVE_SHORT,
			 fileID,
			 "ATTRIBUTE_SHORT_ARRAY",
			 data_short,
			 3);
    //
    std::cout << "-- Attribute of type long[3] ..." << std::endl;
    data_long = new long[3];
    data_long[0] = 1;
    data_long[1] = 2;
    data_long[2] = 3;
    DAL::h5setAttribute( H5T_NATIVE_LONG,
			 fileID,
			 "ATTRIBUTE_LONG_ARRAY",
			 data_long,
			 3);
    //
    std::cout << "-- Attribute of type float[3] ..." << std::endl;
    data_float = new float[3];
    data_float[0] = 1;
    data_float[1] = 2;
    data_float[2] = 3;
    DAL::h5setAttribute( H5T_NATIVE_FLOAT,
			 fileID,
			 "ATTRIBUTE_FLOAT_ARRAY",
			 data_float,
			 3);
    //
    std::cout << "-- Attribute of type double[3] ..." << std::endl;
    data_double = new double[3];
    data_double[0] = 1;
    data_double[1] = 2;
    data_double[2] = 3;
    DAL::h5setAttribute( H5T_NATIVE_DOUBLE,
			 fileID,
			 "ATTRIBUTE_DOUBLE_ARRAY",
			 data_double,
			 3);
    //
    std::cout << "-- Attribute of type string[3] ..." << std::endl;
    data_string = new std::string[3];
    data_string[0] = "a";
    data_string[1] = "b";
    data_string[2] = "c";
    DAL::h5setAttribute_string (fileID,
				"ATTRIBUTE_STRING_ARRAY",
				data_string,
				3);
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[4] Set attributes of type std::vector<T> ..." << endl;
  try {
    std::vector<int> data_int(nelem);
    std::vector<uint> data_uint(nelem);
    std::vector<short> data_short(nelem);
    std::vector<long> data_long(nelem);
    std::vector<float> data_float(nelem);
    std::vector<double> data_double(nelem);
    std::vector<std::string> data_string(nelem);
    //
    std::cout << "-- Attribute of type std::vector<int> ..." << std::endl;
    data_int[0] = 1;
    data_int[1] = 2;
    data_int[2] = 3;
    DAL::h5set_attribute (H5T_NATIVE_INT,
			  fileID,
			  "ATTRIBUTE_INT_VECTOR",
			  data_int);
    //
    std::cout << "-- Attribute of type std::vector<uint> ..." << std::endl;
    data_uint[0] = 4;
    data_uint[1] = 5;
    data_uint[2] = 6;
    DAL::h5set_attribute (H5T_NATIVE_UINT,
			  fileID,
			  "ATTRIBUTE_UINT_VECTOR",
			  data_uint);
    //
    std::cout << "-- Attribute of type std::vector<short> ..." << std::endl;
    data_short[0] = 7;
    data_short[1] = 8;
    data_short[2] = 9;
    DAL::h5set_attribute (H5T_NATIVE_SHORT,
			  fileID,
			  "ATTRIBUTE_SHORT_VECTOR",
			  data_short);
    //
    std::cout << "-- Attribute of type std::vector<long> ..." << std::endl;
    data_long[0] = 10;
    data_long[1] = 11;
    data_long[2] = 12;
    DAL::h5set_attribute (H5T_NATIVE_LONG,
			  fileID,
			  "ATTRIBUTE_LONG_VECTOR",
			  data_long);
    //
    std::cout << "-- Attribute of type std::vector<float> ..." << std::endl;
    data_float[0] = 13;
    data_float[1] = 14;
    data_float[2] = 15;
    DAL::h5set_attribute (H5T_NATIVE_FLOAT,
			  fileID,
			  "ATTRIBUTE_FLOAT_VECTOR",
			  data_float);
    //
    std::cout << "-- Attribute of type std::vector<double> ..." << std::endl;
    data_double[0] = 16;
    data_double[1] = 17;
    data_double[2] = 18;
    DAL::h5set_attribute (H5T_NATIVE_DOUBLE,
			  fileID,
			  "ATTRIBUTE_DOUBLE_VECTOR",
			  data_double);
    //
    std::cout << "-- Attribute of type std::vector<string> ..." << std::endl;
    data_string[0] = "a";
    data_string[1] = "b";
    data_string[2] = "c";
//     DAL::h5set_attribute (H5T_STRING,
// 			  fileID,
// 			  "ATTRIBUTE_STRING_VECTOR",
// 			  data_string);
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[5] Close and reopen HDF5 file for testing ..." << endl;
  try {
    cout << "-- Closing file ... " << endl;
    h5error = H5Fclose (fileID);
    h5error = H5Eclear1 ();
    cout << "-- Opening HDF5 file ..." << endl;
    fileID = H5Fopen (filename.c_str(),
		      H5F_ACC_RDONLY,
		      H5P_DEFAULT);  
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[6] Read attributes of single value ..." << endl;
  try {
    int data_int;
    uint data_uint;
    short data_short;
    long data_long;
    float data_float;
    double data_double;
    std::string data_string;
    //
    DAL::h5get_attribute (fileID, "ATTRIBUTE_INT", data_int);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_UINT", data_uint);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_SHORT", data_short);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_LONG", data_long);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_FLOAT", data_float);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_DOUBLE", data_double);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_STRING", data_string);
    //
    cout << "-- ATTRIBUTE_INT    = " << data_int    << endl;
    cout << "-- ATTRIBUTE_UINT   = " << data_uint   << endl;
    cout << "-- ATTRIBUTE_SHORT  = " << data_short  << endl;
    cout << "-- ATTRIBUTE_LONG   = " << data_long   << endl;
    cout << "-- ATTRIBUTE_FLOAT  = " << data_float  << endl;
    cout << "-- ATTRIBUTE_DOUBLE = " << data_double << endl;
    cout << "-- ATTRIBUTE_STRING = " << data_string << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[7] Read attributes of type std::vector<T> ..." << endl;
  try {
    std::vector<int> data_int(nelem);
    std::vector<uint> data_uint(nelem);
    std::vector<short> data_short(nelem);
    std::vector<long> data_long(nelem);
    std::vector<float> data_float(nelem);
    std::vector<double> data_double(nelem);
    //
    DAL::h5get_attribute (fileID, "ATTRIBUTE_INT_VECTOR", data_int);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_UINT_VECTOR", data_uint);    
    DAL::h5get_attribute (fileID, "ATTRIBUTE_SHORT_VECTOR", data_short);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_LONG_VECTOR", data_long);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_FLOAT_VECTOR", data_float);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_DOUBLE_VECTOR", data_double);
    //
    cout << "-- ATTRIBUTE_INT_VECTOR    = " << data_int    << endl;
    cout << "-- ATTRIBUTE_UINT_VECTOR   = " << data_uint   << endl;
    cout << "-- ATTRIBUTE_SHORT_VECTOR  = " << data_short  << endl;
    cout << "-- ATTRIBUTE_LONG_VECTOR   = " << data_long   << endl;
    cout << "-- ATTRIBUTE_FLOAT_VECTOR  = " << data_float  << endl;
    cout << "-- ATTRIBUTE_DOUBLE_VECTOR = " << data_double << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
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
  nofFailedTests += test_hdf5_attributes ();

  return nofFailedTests;
}
