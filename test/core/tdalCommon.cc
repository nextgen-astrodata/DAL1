/*-------------------------------------------------------------------------*
 | $Id:: tHDF5Common.cpp 2022 2008-09-26 12:06:09Z baehren               $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include <dalCommon.h>
#include <Enumerations.h>
#include <dalDataset.h>

#ifdef HAVE_CASA
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
#ifdef HAVE_CASA
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
//                                                                 test_operators

/*!
  \brief Test overloading of operators

  \e dalCommon provides overloading of the output operator (>>) in order to
  allow simple output of std::vector<T>.

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_operators ()
{
  cout << "\n[tdalCommon::test_operators]\n" << endl;

  int nofFailedTests (0);
  uint nelem (10);

  cout << "[1] Testing show(std::ostream&,T const *,uint const &) ..." << endl;
  try {
    bool * arr_bool;
    int * arr_int;
    float * arr_float;
    double * arr_double;
    std::complex<double> * arr_complex;
    //
    arr_bool    = new bool [nelem];
    arr_int     = new int [nelem];
    arr_float   = new float [nelem];
    arr_double  = new double [nelem];
    arr_complex = new std::complex<double> [nelem];
    //
    for (uint n(0); n<nelem; n++) {
      arr_bool[n]    = true;
      arr_int[n]     = int(n);
      arr_float[n]   = float(n);
      arr_double[n]  = double(n);
      arr_complex[n] = std::complex<double>(n);
    }
    //
    show (cout,arr_bool,nelem);
    show (cout,arr_int,nelem);
    show (cout,arr_float,nelem);
    show (cout,arr_double,nelem);
    show (cout,arr_complex,nelem);
    //
    delete [] arr_bool;
    delete [] arr_int;
    delete [] arr_float;
    delete [] arr_double;
    delete [] arr_complex;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[2] Test overloading of operator<< for std::vector<T> ..." << endl;
  try {
    std::vector<bool> vec_bool (nelem);
    std::vector<int> vec_int (nelem);
    std::vector<long> vec_long (nelem);
    std::vector<short> vec_short (nelem);
    std::vector<float> vec_float (nelem);
    std::vector<double> vec_double (nelem);
    
    cout << "-- vector<bool>   = " << vec_bool   << endl;
    cout << "-- vector<int>    = " << vec_int    << endl;
    cout << "-- vector<long>   = " << vec_long   << endl;
    cout << "-- vector<short>  = " << vec_short  << endl;
    cout << "-- vector<float>  = " << vec_float  << endl;
    cout << "-- vector<double> = " << vec_double << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Test overloading of operator<< for std::set<T> ..." << endl;
  try {
    bool array_bool[] = {true,false,true,false,true};
    std::set<bool> set_bool (&array_bool[0], &array_bool[5]);
    //
    int array_int[] = {0,1,2,3,4};
    std::set<int> set_int (&array_int[0], &array_int[5]);
    //
    long array_long[] = {0,1,2,3,4};
    std::set<long> set_long (&array_long[0], &array_long[5]);
    //
    short array_short[] = {0,1,2,3,4};
    std::set<short> set_short (&array_short[0], &array_short[5]);
    //
    float array_float[] = {0.0,0.1,0.2,0.3,0.4};
    std::set<float> set_float (&array_float[0], &array_float[5]);
    //
    double array_double[] = {0.1,0.12,0.123,0.1234,0.12345};
    std::set<double> set_double (&array_double[0], &array_double[5]);
    //
    std::string array_string[] = {"a","b","c","d","e"};
    std::set<std::string> set_string (&array_string[0], &array_string[5]);
    
    cout << "-- set<bool>   = " << set_bool   << endl;
    cout << "-- set<int>    = " << set_int    << endl;
    cout << "-- set<long>   = " << set_long   << endl;
    cout << "-- set<short>  = " << set_short  << endl;
    cout << "-- set<float>  = " << set_float  << endl;
    cout << "-- set<double> = " << set_double << endl;
    cout << "-- set<string> = " << set_string << endl;

  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[3] Test overloading of operator<< for std::map<K,V> ..." << endl;
  try {
    std::map<std::string,int> m;
    std::map<std::string,int>::iterator it;
    //
    m["000000000"] = 0;
    m["000000001"] = 1;
    m["000001000"] = 2;
    m["000001001"] = 3;
    //
    cout << "-- map<string,int> = [";
    for (it=m.begin(); it!=m.end(); ++it) {
      cout << " (" << it->first << "," << it->second << ")";
    }
    cout << " ]" << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

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
//                                                           test_hdf5_attributes

/*!
  \brief Test of the routines providing access to HDF5 attributes
  
  Except atomic attributes -- single <tt>int</tt>, <tt>double</tt>, etc. -- the
  following 1-dimensional arrays should be supported:
  - c-array
  - std::vector<T>
  - std::set<T>
  - casa::Vector<T>
  
  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_hdf5_attributes ()
{
  cout << "\n[tdalCommon::test_hdf5_attributes]\n" << endl;

  int nofFailedTests (0);
  std::string filename ("tdalCommon.h5");
  uint nelem (3);
  hid_t fileID (0);
  herr_t h5error (0);

  //__________________________________________________________________

  cout << "[1] Creating HDF5 file for testing ..." << endl;
  try {
    fileID = H5Fcreate (filename.c_str(),
			H5F_ACC_TRUNC,
			H5P_DEFAULT,
			H5P_DEFAULT);
  }
  catch (std::string message) {
    cerr << message << endl;
    return 1;
  }
  
  //__________________________________________________________________
  // Write atomic-valued attributes
  
  cout << "[2] Set attributes of single value ..." << endl;
  try {
    cout << "-- Attribute of type bool ..." << endl;
    bool data_bool (true);
    DAL::h5set_attribute( fileID, "BOOL", data_bool );
    //
    cout << "-- Attribute of type int ..." << endl;
    int data_int (1);
    DAL::h5set_attribute( fileID, "INT", data_int );
    //
    cout << "-- Attribute of type uint ..." << endl;
    uint data_uint (2);
    DAL::h5set_attribute( fileID, "UINT", data_uint );
    //
    cout << "-- Attribute of type short ..." << endl;
    short data_short (3);
    DAL::h5set_attribute( fileID, "SHORT", data_short );
    //
    cout << "-- Attribute of type long ..." << endl;
    long data_long (4);
    DAL::h5set_attribute( fileID, "LONG", data_long );
    //
    cout << "-- Attribute of type float ..." << endl;
    float data_float (5);
    DAL::h5set_attribute( fileID, "FLOAT", data_float );
    //
    cout << "-- Attribute of type double ..." << endl;
    double data_double (6);
    DAL::h5set_attribute( fileID, "DOUBLE", data_double );
    //
    cout << "-- Attribute of type string ..." << endl;
    std::string data_string ("Hello");
    DAL::h5set_attribute( fileID, "STRING", data_string );
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  //__________________________________________________________________
  // Write attributes from c-array

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
    cout << "-- Attribute of type bool[3] ..." << endl;
    {
      bool *data_bool = new bool[3];
      data_bool[0] = true;
      data_bool[1] = false;
      data_bool[2] = true;
      DAL::h5set_attribute (fileID,
			    "ARRAY_BOOL",
			    data_bool,
			    3);
      delete [] data_bool;
    }
    //
    cout << "-- Attribute of type int[3] ..." << endl;
    data_int = new int[3];
    data_int[0] = 1;
    data_int[1] = 2;
    data_int[2] = 3;
    DAL::h5set_attribute (fileID,
			  "ARRAY_INT",
			  data_int,
			  3);
    //
    cout << "-- Attribute of type uint[3] ..." << endl;
    data_uint = new uint[3];
    data_uint[0] = 1;
    data_uint[1] = 2;
    data_uint[2] = 3;
    DAL::h5set_attribute (fileID,
			  "ARRAY_UINT",
			  data_uint,
			  3);
    //
    cout << "-- Attribute of type short[3] ..." << endl;
    data_short = new short[3];
    data_short[0] = 1;
    data_short[1] = 2;
    data_short[2] = 3;
    DAL::h5set_attribute (fileID,
			  "ARRAY_SHORT",
			  data_short,
			  3);
    //
    cout << "-- Attribute of type long[3] ..." << endl;
    data_long = new long[3];
    data_long[0] = 1;
    data_long[1] = 2;
    data_long[2] = 3;
    DAL::h5set_attribute (fileID,
			  "ARRAY_LONG",
			  data_long,
			  3);
    //
    cout << "-- Attribute of type float[3] ..." << endl;
    data_float = new float[3];
    data_float[0] = 1;
    data_float[1] = 2;
    data_float[2] = 3;
    DAL::h5set_attribute (fileID,
			  "ARRAY_FLOAT",
			  data_float,
			  3);
    //
    cout << "-- Attribute of type double[3] ..." << endl;
    data_double = new double[3];
    data_double[0] = 1;
    data_double[1] = 2;
    data_double[2] = 3;
    DAL::h5set_attribute (fileID,
			  "ARRAY_DOUBLE",
			  data_double,
			  3);
    //
    cout << "-- Attribute of type string[3] ..." << endl;
    data_string = new std::string[3];
    data_string[0] = "aa";
    data_string[1] = "bb";
    data_string[2] = "cc";
    DAL::h5set_attribute (fileID,
			  "ARRAY_STRING",
			  data_string,
			  3);
    // release allocated memory
    delete [] data_int;
    delete [] data_uint;
    delete [] data_short;
    delete [] data_long;
    delete [] data_float;
    delete [] data_double;
    delete [] data_string;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  //__________________________________________________________________
  // Write attributes from std::vector<T>
  
  cout << "[4] Set attributes of type std::vector<T> ..." << endl;
  try {
    std::vector<long> data_long(nelem);
    std::vector<float> data_float(nelem);
    std::vector<double> data_double(nelem);
    std::vector<std::string> data_string(nelem);
    //
    cout << "-- Attribute of type std::vector<bool> ..." << endl;
    std::vector<bool> data_bool(nelem);
    data_bool[0] = true;
    data_bool[1] = false;
    data_bool[2] = true;
    DAL::h5set_attribute (fileID,
			  "VECTOR_BOOL",
			  data_bool);
    //
    cout << "-- Attribute of type std::vector<int> ..." << endl;
    std::vector<int> data_int(nelem);
    data_int[0] = 1;
    data_int[1] = 2;
    data_int[2] = 3;
    DAL::h5set_attribute (fileID,
			  "VECTOR_INT",
			  data_int);
    //
    cout << "-- Attribute of type std::vector<uint> ..." << endl;
    std::vector<uint> data_uint(nelem);
    data_uint[0] = 4;
    data_uint[1] = 5;
    data_uint[2] = 6;
    DAL::h5set_attribute (fileID,
			  "VECTOR_UINT",
			  data_uint);
    //
    cout << "-- Attribute of type std::vector<short> ..." << endl;
    std::vector<short> data_short(nelem);
    data_short[0] = 7;
    data_short[1] = 8;
    data_short[2] = 9;
    DAL::h5set_attribute (fileID,
			  "VECTOR_SHORT",
			  data_short);
    //
    cout << "-- Attribute of type std::vector<long> ..." << endl;
    data_long[0] = 10;
    data_long[1] = 11;
    data_long[2] = 12;
    DAL::h5set_attribute (fileID,
			  "VECTOR_LONG",
			  data_long);
    //
    cout << "-- Attribute of type std::vector<float> ..." << endl;
    data_float[0] = 13;
    data_float[1] = 14;
    data_float[2] = 15;
    DAL::h5set_attribute (fileID,
			  "VECTOR_FLOAT",
			  data_float);
    //
    cout << "-- Attribute of type std::vector<double> ..." << endl;
    data_double[0] = 16;
    data_double[1] = 17;
    data_double[2] = 18;
    DAL::h5set_attribute (fileID,
			  "VECTOR_DOUBLE",
			  data_double);
    //
    cout << "-- Attribute of type std::vector<string> ..." << endl;
    data_string[0] = "aaa";
    data_string[1] = "bbb";
    data_string[2] = "ccc";
    DAL::h5set_attribute (fileID,
			  "VECTOR_STRING",
			  data_string);
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  //__________________________________________________________________
  // Write attributes from std::set<T>
  
  cout << "[4] Set attributes of type std::set<T> ..." << endl;
  try {
    int array_int[] = {0,1,2,3,4};
    std::set<int> set_int (&array_int[0], &array_int[5]);
    DAL::h5set_attribute (fileID,
			  "SET_INT",
			  set_int);
    //
    long array_long[] = {0,1,2,3,4};
    std::set<long> set_long (&array_long[0], &array_long[5]);
    DAL::h5set_attribute (fileID,
			  "SET_LONG",
			  set_long);
    //
    short array_short[] = {0,1,2,3,4};
    std::set<short> set_short (&array_short[0], &array_short[5]);
    DAL::h5set_attribute (fileID,
			  "SET_SHORT",
			  set_short);
    //
    float array_float[] = {0.0,0.1,0.2,0.3,0.4};
    std::set<float> set_float (&array_float[0], &array_float[5]);
    DAL::h5set_attribute (fileID,
			  "SET_FLOAT",
			  set_float);
    //
    double array_double[] = {0.1,0.12,0.123,0.1234,0.12345};
    std::set<double> set_double (&array_double[0], &array_double[5]);
    DAL::h5set_attribute (fileID,
			  "SET_DOUBLE",
			  set_double);
    //
    std::string array_string[] = {"a","s","d","f","g"};
    std::set<std::string> set_string (&array_string[0], &array_string[5]);
    DAL::h5set_attribute (fileID,
			  "SET_STRING",
			  set_string);
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Write attributes from casa::Vector<T>
  
#ifdef HAVE_CASA
  cout << "[5] Set attributes of type casa::Vector<T> ..." << endl;
  try {
    cout << "-- Attribute of type casa::Vector<bool> ..." << endl;
    casa::Vector<bool> data_bool(nelem);
    data_bool[0] = true;
    data_bool[1] = false;
    data_bool[2] = true;
    DAL::h5set_attribute (fileID,
			  "CASA_VECTOR_BOOL",
			  data_bool);
    //
    cout << "-- Attribute of type casa::Vector<int> ..." << endl;
    casa::Vector<int> data_int(nelem);
    data_int[0] = 1;
    data_int[1] = 2;
    data_int[2] = 3;
    DAL::h5set_attribute (fileID,
			  "CASA_VECTOR_INT",
			  data_int);
    //
    cout << "-- Attribute of type casa::Vector<short> ..." << endl;
    casa::Vector<short> data_short(nelem);
    data_short[0] = 1;
    data_short[1] = 2;
    data_short[2] = 3;
    DAL::h5set_attribute (fileID,
			  "CASA_VECTOR_SHORT",
			  data_short);
    //
    cout << "-- Attribute of type casa::Vector<float> ..." << endl;
    casa::Vector<float> data_float(nelem);
    data_float[0] = .1;
    data_float[1] = .2;
    data_float[2] = .3;
    DAL::h5set_attribute (fileID,
			  "CASA_VECTOR_FLOAT",
			  data_float);
    //
    cout << "-- Attribute of type casa::Vector<double> ..." << endl;
    casa::Vector<double> data_double(nelem);
    data_double[0] = .4;
    data_double[1] = .5;
    data_double[2] = .6;
    DAL::h5set_attribute (fileID,
			  "CASA_VECTOR_DOUBLE",
			  data_double);
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
#endif

  //__________________________________________________________________
  
  cout << "[6] Close and reopen HDF5 file for testing ..." << endl;
  try {
    cout << "-- Closing file ... " << endl;
    h5error = H5Fclose (fileID);
    h5error = H5Eclear1 ();
    cout << "-- Opening HDF5 file ..." << endl;
    fileID = H5Fopen (filename.c_str(),
		      H5F_ACC_RDWR,
		      H5P_DEFAULT);
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Test reading attributes consisting of a single value

  cout << "[6] Read attributes of single value ..." << endl;
  try {
    bool data_bool;
    int data_int;
    uint data_uint;
    short data_short;
    long data_long;
    float data_float;
    double data_double;
    std::string data_string;
    //
    DAL::h5get_attribute (fileID, "BOOL", data_bool);
    DAL::h5get_attribute (fileID, "INT", data_int);
    DAL::h5get_attribute (fileID, "UINT", data_uint);
    DAL::h5get_attribute (fileID, "SHORT", data_short);
    DAL::h5get_attribute (fileID, "LONG", data_long);
    DAL::h5get_attribute (fileID, "FLOAT", data_float);
    DAL::h5get_attribute (fileID, "DOUBLE", data_double);
    DAL::h5get_attribute (fileID, "STRING", data_string);
    //
    cout << "-- BOOL   = " << data_bool   << endl;
    cout << "-- INT    = " << data_int    << endl;
    cout << "-- UINT   = " << data_uint   << endl;
    cout << "-- SHORT  = " << data_short  << endl;
    cout << "-- LONG   = " << data_long   << endl;
    cout << "-- FLOAT  = " << data_float  << endl;
    cout << "-- DOUBLE = " << data_double << endl;
    cout << "-- STRING = " << data_string << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  //__________________________________________________________________
  // Test reading attributes into std::vector<T>

  cout << "[7] Read attributes of type std::vector<T> ..." << endl;
  try {
    std::vector<bool> data_bool(nelem);
    std::vector<int> data_int(nelem);
    std::vector<uint> data_uint(nelem);
    std::vector<short> data_short(nelem);
    std::vector<long> data_long(nelem);
    std::vector<float> data_float(nelem);
    std::vector<double> data_double(nelem);
    std::vector<std::string> data_string(nelem);
    //
    DAL::h5get_attribute (fileID, "ARRAY_BOOL",   data_bool);
    DAL::h5get_attribute (fileID, "ARRAY_INT",    data_int);
    DAL::h5get_attribute (fileID, "ARRAY_UINT",   data_uint);
    DAL::h5get_attribute (fileID, "ARRAY_SHORT",  data_short);
    DAL::h5get_attribute (fileID, "ARRAY_LONG",   data_long);
    DAL::h5get_attribute (fileID, "ARRAY_FLOAT",  data_float);
    DAL::h5get_attribute (fileID, "ARRAY_DOUBLE", data_double);
    DAL::h5get_attribute (fileID, "ARRAY_STRING", data_string);
    //
    cout << "-- ARRAY_BOOL   = " << data_bool   << endl;
    cout << "-- ARRAY_INT    = " << data_int    << endl;
    cout << "-- ARRAY_UINT   = " << data_uint   << endl;
    cout << "-- ARRAY_SHORT  = " << data_short  << endl;
    cout << "-- ARRAY_LONG   = " << data_long   << endl;
    cout << "-- ARRAY_FLOAT  = " << data_float  << endl;
    cout << "-- ARRAY_DOUBLE = " << data_double << endl;
    cout << "-- ARRAY_STRING = " << data_string << endl;
    //
    DAL::h5get_attribute (fileID, "VECTOR_BOOL",   data_bool);
    DAL::h5get_attribute (fileID, "VECTOR_INT",    data_int);
    DAL::h5get_attribute (fileID, "VECTOR_UINT",   data_uint);
    DAL::h5get_attribute (fileID, "VECTOR_SHORT",  data_short);
    DAL::h5get_attribute (fileID, "VECTOR_LONG",   data_long);
    DAL::h5get_attribute (fileID, "VECTOR_FLOAT",  data_float);
    DAL::h5get_attribute (fileID, "VECTOR_DOUBLE", data_double);
    DAL::h5get_attribute (fileID, "VECTOR_STRING", data_string);
    //
    cout << "-- VECTOR_BOOL   = " << data_bool   << endl;
    cout << "-- VECTOR_INT    = " << data_int    << endl;
    cout << "-- VECTOR_UINT   = " << data_uint   << endl;
    cout << "-- VECTOR_SHORT  = " << data_short  << endl;
    cout << "-- VECTOR_LONG   = " << data_long   << endl;
    cout << "-- VECTOR_FLOAT  = " << data_float  << endl;
    cout << "-- VECTOR_DOUBLE = " << data_double << endl;
    cout << "-- VECTOR_STRING = " << data_string << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Test repeated read access to attributes
  
  cout << "[8] Test repeated read access to attributes ..." << endl;
  try {
    uint nofLoops (500);
    int data_int (0);
    std::string data_string;
    std::vector<int> vec_int;
    std::vector<std::string> vec_string;
    //
    cout << "-- INT ...           " << std::flush;
    for (uint n(0); n<nofLoops; n++) {
      DAL::h5get_attribute (fileID, "INT", data_int);
    }
    cout << nofLoops << " reads finished." << endl;
    //
    cout << "-- STRING ...        " << std::flush;
    for (uint n(0); n<nofLoops; n++) {
      DAL::h5get_attribute (fileID, "STRING", data_string);
    }
    cout << nofLoops << " reads finished." << endl;
    //
    cout << "-- VECTOR_INT    ... " << std::flush;
    for (uint n(0); n<nofLoops; n++) {
      DAL::h5get_attribute (fileID, "VECTOR_INT", vec_int);
    }
    cout << nofLoops << " reads finished." << endl;
    //
    cout << "-- VECTOR_STRING ... " << std::flush;
    for (uint n(0); n<nofLoops; n++) {
      DAL::h5get_attribute (fileID, "VECTOR_STRING", vec_string);
    }
    cout << nofLoops << " reads finished." << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Test post-creation manipulation of attributes
  
  cout << "[9] Update the value of attributes after their creation ..." << endl;
  try {
    cout << "-- manipulating variable of type double ..." << endl;
    
    double var_double (0);
    DAL::h5set_attribute (fileID,
			  "VAR_DOUBLE",
			  var_double);
    DAL::h5get_attribute (fileID,
			  "VAR_DOUBLE",
			  var_double);
    cout << "--> VAR_DOUBLE = " << var_double << endl;
    // Update the previously written attribute
    var_double = 1;
    DAL::h5set_attribute (fileID,
			  "VAR_DOUBLE",
			  var_double);
    DAL::h5get_attribute (fileID,
			  "VAR_DOUBLE",
			  var_double);
    cout << "--> VAR_DOUBLE = " << var_double << endl;
    // Update the previously written attribute
    var_double = 2;
    DAL::h5set_attribute (fileID,
			  "VAR_DOUBLE",
			  var_double);
    DAL::h5get_attribute (fileID,
			  "VAR_DOUBLE",
			  var_double);
    cout << "--> VAR_DOUBLE = " << var_double << endl;
    
    //____________________________________________
    
    cout << "-- manipulating variable of type string ..." << endl;
    
    std::string var_string ("a");
    DAL::h5set_attribute (fileID,
			  "VAR_STRING",
			  var_string);
    DAL::h5get_attribute (fileID,
			  "VAR_STRING",
			  var_string);
    cout << "--> VAR_STRING = " << var_string << endl;
    // Update the previously written attribute
    var_string = "b";
    DAL::h5set_attribute (fileID,
			  "VAR_STRING",
			  var_string);
    DAL::h5get_attribute (fileID,
			  "VAR_STRING",
			  var_string);
    cout << "--> VAR_STRING = " << var_string << endl;
    // Update the previously written attribute
    var_string = "c";
    DAL::h5set_attribute (fileID,
			  "VAR_STRING",
			  var_string);
    DAL::h5get_attribute (fileID,
			  "VAR_STRING",
			  var_string);
    cout << "--> VAR_STRING = " << var_string << endl;
    
    //____________________________________________
    
    cout << "-- manipulating variable of type vector<double> ..." << endl;
    
    std::vector<double> var_vector_double (3);
    // write the first set of values
    var_vector_double[0] = 0;
    var_vector_double[1] = 1;
    var_vector_double[2] = 2;
    DAL::h5set_attribute (fileID,
			  "VAR_VECTOR_DOUBLE",
			  var_vector_double);
    DAL::h5get_attribute (fileID,
			  "VAR_VECTOR_DOUBLE",
			  var_vector_double);
    cout << "--> VAR_VECTOR_DOUBLE = " << var_vector_double << endl;
    // write the second set of values
    var_vector_double[0] = 3;
    var_vector_double[1] = 4;
    var_vector_double[2] = 5;
    DAL::h5set_attribute (fileID,
			  "VAR_VECTOR_DOUBLE",
			  var_vector_double);
    DAL::h5get_attribute (fileID,
			  "VAR_VECTOR_DOUBLE",
			  var_vector_double);
    cout << "--> VAR_VECTOR_DOUBLE = " << var_vector_double << endl;
    // write the second set of values
    var_vector_double[0] = 6;
    var_vector_double[1] = 7;
    var_vector_double[2] = 8;
    DAL::h5set_attribute (fileID,
			  "VAR_VECTOR_DOUBLE",
			  var_vector_double);
    DAL::h5get_attribute (fileID,
			  "VAR_VECTOR_DOUBLE",
			  var_vector_double);
    cout << "--> VAR_VECTOR_DOUBLE = " << var_vector_double << endl;
    
    //____________________________________________
    
    cout << "-- manipulating variable of type vector<std::string> ..." << endl;
    
    std::vector<std::string> var_vector_string (3);
    // write the first set of values
    var_vector_string[0] = "a";
    var_vector_string[1] = "b";
    var_vector_string[2] = "c";
    DAL::h5set_attribute (fileID,
			  "VAR_VECTOR_STRING",
			  var_vector_string);
    DAL::h5get_attribute (fileID,
			  "VAR_VECTOR_STRING",
			  var_vector_string);
    cout << "--> VAR_VECTOR_STRING = " << var_vector_string << endl;
    // write the second set of values
    var_vector_string[0] = "aa";
    var_vector_string[1] = "bb";
    var_vector_string[2] = "cc";
    DAL::h5set_attribute (fileID,
			  "VAR_VECTOR_STRING",
			  var_vector_string);
    DAL::h5get_attribute (fileID,
			  "VAR_VECTOR_STRING",
			  var_vector_string);
    cout << "--> VAR_VECTOR_STRING = " << var_vector_string << endl;
    // write the second set of values
    var_vector_string[0] = "aaa";
    var_vector_string[1] = "bbb";
    var_vector_string[2] = "ccc";
    DAL::h5set_attribute (fileID,
			  "VAR_VECTOR_STRING",
			  var_vector_string);
    DAL::h5get_attribute (fileID,
			  "VAR_VECTOR_STRING",
			  var_vector_string);
    cout << "--> VAR_VECTOR_STRING = " << var_vector_string << endl;
    
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                test_timeseries

/*!
  \brief Test access to a TBB time-series dataset

  \param filename -- Name of the input HDF5 file on which the access routines are
         tested.

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_timeseries (std::string const &filename)
{
  cout << "\n[tdalCommon::test_timeseries]\n" << endl;

  int nofFailedTests = 0;
  bool status        = true;
  hid_t fileID       = -1;
  hid_t groupID      = -1;
  hid_t datasetID    = -1;

  //__________________________________________________________________
  // Open the HDF5 dataset

  cout << "[1] opening HDF5 file ..." << endl;
  fileID = H5Fopen (filename.c_str(),
                     H5F_ACC_RDWR,
                     H5P_DEFAULT);

  if (fileID < 0) {
    cerr << "Failed to open file " << filename << endl;
    return 1;
  }
  
  //__________________________________________________________________
  // Access the attributes attached to the root group of the file

  cout << "[2] Access the attributes attached to the root group of the file"
       << endl;
  try {
    std::string name;
    std::string telescope;
    std::string observer;
    std::string project;
    std::string observation_id;
    std::string observation_mode;
    //
    DAL::h5get_name (name,fileID);
    DAL::h5get_attribute (fileID,"TELESCOPE",telescope);
    DAL::h5get_attribute (fileID,"OBSERVER",observer);
    DAL::h5get_attribute (fileID,"PROJECT",project);
    DAL::h5get_attribute (fileID,"OBSERVATION_ID",observation_id);
    DAL::h5get_attribute (fileID,"OBSERVATION_MODE",observation_mode);
    //
    cout << "-- Filename ....... = " << name             << endl;
    cout << "-- TELESCOPE ...... = " << telescope        << endl;
    cout << "-- OBSERVER ....... = " << observer         << endl;
    cout << "-- PROJECT ........ = " << project          << endl;
    cout << "-- OBSERVATION_ID   = " << observation_id   << endl;
    cout << "-- OBSERVATION_MODE = " << observation_mode << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  std::cout << "[3] repeated read access to attributes ..." << endl;
  try {
    uint nofTrials (500);
    std::string telescope;
    std::string observer;
    std::string project;
    std::string observation_id;
    std::string observation_mode;
    
    for (uint n(0); n<nofTrials; n++) {
      DAL::h5get_attribute (fileID,"TELESCOPE",telescope);
      DAL::h5get_attribute (fileID,"OBSERVER",observer);
      DAL::h5get_attribute (fileID,"PROJECT",project);
      DAL::h5get_attribute (fileID,"OBSERVATION_ID",observation_id);
      DAL::h5get_attribute (fileID,"OBSERVATION_MODE",observation_mode);
    }
    cout << "-- " << nofTrials << " read completed." << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  //__________________________________________________________________
  // Write acces to attributes in the root group

  std::cout << "[4] write access to attributes in the root group ..." << endl;
  try {
    std::string observer;
    std::string project;
    //
    DAL::h5get_attribute (fileID,"OBSERVER",observer);
    DAL::h5get_attribute (fileID,"PROJECT",project);
    cout << "-- original values:" << endl;
    cout << "--> OBSERVER = " << observer << endl;
    cout << "--> PROJECT  = " << project  << endl;
    //
    observer = "Lars Baehren";
    project  = "CR";
    status = DAL::h5set_attribute (fileID,"OBSERVER",observer);
    status = DAL::h5set_attribute (fileID,"PROJECT",project);
    if (status) {
      DAL::h5get_attribute (fileID,"OBSERVER",observer);
      DAL::h5get_attribute (fileID,"PROJECT",project);
      cout << "-- new values:" << endl;
      cout << "--> OBSERVER = " << observer << endl;
      cout << "--> PROJECT  = " << project  << endl;
    }
    //
    observer = "UNDEFINED";
    project  = "UNDEFINED";
    status = DAL::h5set_attribute (fileID,"OBSERVER",observer);
    status = DAL::h5set_attribute (fileID,"PROJECT",project);
    cout << "-- reverted to original values" << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  //__________________________________________________________________
  // Open the station group
  
  cout << "[5] opening station group ..." << endl;
  try {
    std::vector<std::string> names;
    // Get the names of the groups attached to the root group of the file
    status = DAL::h5get_names (names,fileID,H5G_GROUP);
    // Open up the first station group
    if (status) {
      cout << "-- Station groups = " << names << endl;
      groupID = H5Gopen1 (fileID,names[0].c_str());
    }
    // If opening of group failed, do not continue
    if (groupID < 0) {
      cerr << "Error opening the station group!" << endl;
      return 1;
    }
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Access the attributes attached to the station group
  
  cout << "[6] Reading attributes attached to station group ..." << endl;
  try {
    std::string name;
    std::string trigger_type;
    double trigger_offset;
    std::vector<uint> triggered_antennas;
    std::vector<double> station_position_value;
    std::vector<std::string> station_position_unit;
    std::string station_position_frame;
    std::vector<double> beam_direction_value;
    std::vector<std::string> beam_direction_unit;
    std::string beam_direction_frame;
    //
    DAL::h5get_name (name,groupID);
    DAL::h5get_attribute (groupID, "TRIGGER_TYPE", trigger_type);
    DAL::h5get_attribute (groupID, "TRIGGER_OFFSET", trigger_offset);
    DAL::h5get_attribute (groupID, "STATION_POSITION_VALUE", station_position_value);
    DAL::h5get_attribute (groupID, "STATION_POSITION_UNIT", station_position_unit);
    DAL::h5get_attribute (groupID, "STATION_POSITION_FRAME", station_position_frame);
    DAL::h5get_attribute (groupID, "BEAM_DIRECTION_VALUE", beam_direction_value);
    DAL::h5get_attribute (groupID, "BEAM_DIRECTION_UNIT", beam_direction_unit);
    DAL::h5get_attribute (groupID, "BEAM_DIRECTION_FRAME", beam_direction_frame);
    //
    cout << "-- Group name             = " << name                   << endl;
    cout << "-- TRIGGER_TYPE           = " << trigger_type           << endl;
    cout << "-- TRIGGER_OFFSET         = " << trigger_offset         << endl;
    cout << "-- STATION_POSITION_VALUE = " << station_position_value << endl;
    cout << "-- STATION_POSITION_UNIT  = " << station_position_unit  << endl;
    cout << "-- STATION_POSITION_FRAME = " << station_position_frame << endl;
    cout << "-- BEAM_DIRECTION_VALUE   = " << beam_direction_value   << endl;
    cout << "-- BEAM_DIRECTION_UNIT    = " << beam_direction_unit    << endl;
    cout << "-- BEAM_DIRECTION_FRAME   = " << beam_direction_frame   << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Open the dipole dataset

  cout << "[7] opening dipole dataset ..." << endl;
  try {
    std::vector<std::string> names;
    // Get the names of the groups attached to the root group of the file
    status = DAL::h5get_names (names, groupID, H5G_DATASET);
    // Open up the first station group
    if (status) {
      cout << "-- Dipole datasets = " << names << endl;
      datasetID = H5Dopen1 (groupID, names[0].c_str());
    }
    // Check if dataset was opened successfully
    if (datasetID < 0) {
      cerr << "Error opening the dipole dataset!" << endl;
      return 1;
    }
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Access the attributes attached to the dipole dataset
  
  cout << "[6] Reading attributes attached to dipole dataset ..." << endl;
  try {
    uint station_id;
    uint rsp_id;
    uint rcu_id;
    uint time;
    double sample_frequency_value;
    std::string sample_frequency_unit;
    std::vector<double> antenna_position_value;
    std::vector<std::string> antenna_position_unit;
    std::string antenna_position_frame;
    std::vector<double> antenna_orientation_value;
    std::vector<std::string> antenna_orientation_unit;
    std::string antenna_orientation_frame;
    //
    DAL::h5get_attribute (datasetID,"STATION_ID", station_id);
    DAL::h5get_attribute (datasetID,"RSP_ID", rsp_id);
    DAL::h5get_attribute (datasetID,"RCU_ID", rcu_id);
    DAL::h5get_attribute (datasetID,"TIME", time);
    DAL::h5get_attribute (datasetID,"SAMPLE_FREQUENCY_VALUE", sample_frequency_value);
    DAL::h5get_attribute (datasetID,"SAMPLE_FREQUENCY_UNIT", sample_frequency_unit);
    DAL::h5get_attribute (datasetID,"SAMPLE_FREQUENCY_VALUE", sample_frequency_value);
    DAL::h5get_attribute (datasetID,"SAMPLE_FREQUENCY_UNIT", sample_frequency_unit);
    DAL::h5get_attribute (datasetID,"ANTENNA_POSITION_VALUE", antenna_position_value);
    DAL::h5get_attribute (datasetID,"ANTENNA_POSITION_UNIT", antenna_position_unit);
    DAL::h5get_attribute (datasetID,"ANTENNA_POSITION_FRAME", antenna_position_frame);
    DAL::h5get_attribute (datasetID,"ANTENNA_ORIENTATION_VALUE", antenna_orientation_value);
    DAL::h5get_attribute (datasetID,"ANTENNA_ORIENTATION_UNIT", antenna_orientation_unit);
    DAL::h5get_attribute (datasetID,"ANTENNA_ORIENTATION_FRAME", antenna_orientation_frame);
    //
    cout << "-- STATION_ID .............. = " << station_id << endl;
    cout << "-- RSP_ID .................. = " << rsp_id     << endl;
    cout << "-- RCU_ID .................. = " << rcu_id     << endl;
    cout << "-- TIME .................... = " << time       << endl;
    cout << "-- SAMPLE_FREQUENCY_VALUE .. = " << sample_frequency_value << endl;
    cout << "-- SAMPLE_FREQUENCY_UNIT ... = " << sample_frequency_unit  << endl;
    cout << "-- ANTENNA_POSITION_VALUE .. = " << antenna_position_value << endl;
    cout << "-- ANTENNA_POSITION_UNIT ... = " << antenna_position_unit  << endl;
    cout << "-- ANTENNA_POSITION_FRAME .. = " << antenna_position_frame << endl;
    cout << "-- ANTENNA_ORIENTATION_VALUE = " << antenna_orientation_value << endl;
    cout << "-- ANTENNA_ORIENTATION_UNIT  = " << antenna_orientation_unit  << endl;
    cout << "-- ANTENNA_ORIENTATION_FRAME = " << antenna_orientation_frame << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Test retrieval of positions and directions as Measures

#ifdef HAVE_CASA
  try {
    casa::MPosition station_position;
    casa::MPosition antenna_position;
    casa::MDirection beam_direction;
    //
    station_position = DAL::h5get_position (groupID,
					    "STATION_POSITION_VALUE",
					    "STATION_POSITION_UNIT",
					    "STATION_POSITION_FRAME");
    beam_direction = DAL::h5get_direction (groupID,
					   "BEAM_DIRECTION_VALUE",
					   "BEAM_DIRECTION_UNIT",
					   "BEAM_DIRECTION_FRAME");
    antenna_position = DAL::h5get_position (datasetID,
					    "ANTENNA_POSITION_VALUE",
					    "ANTENNA_POSITION_UNIT",
					    "ANTENNA_POSITION_FRAME");
    //
    cout << "-- STATION_POSITION = " << station_position << endl;
    cout << "-- ANTENNA_POSITION = " << antenna_position << endl;
    cout << "-- BEAM_DIRECTION   = " << beam_direction   << endl;
  }
  catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
#endif

  //__________________________________________________________________
  // Release HDF5 object identifiers

  if (H5Iis_valid(fileID))    { H5Fclose(fileID);    }
  if (H5Iis_valid(groupID))   { H5Gclose(groupID);   }
  if (H5Iis_valid(datasetID)) { H5Dclose(datasetID); }
  
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
    DAL::h5get_attribute (fileID,"NUMBER_OF_STATIONS",nofStations);
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
//                                                           test_hdf5_inspection

/*!
  \brief Test the various functions for inspection of a HDF5 file
  
  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_hdf5_inspection ()
{
  cout << "\n[tdalCommon::test_hdf5_inspection]\n" << endl;

  int nofFailedTests (0);
  std::string filename ("tdalCommon.h5");
  std::set<std::string> names;

  hid_t fileID = H5Fopen (filename.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
  
  if (fileID < 0) {
    cerr << "Failed to open file " << filename << endl;
    return 1;
  }
  
  cout << "[1] Get names of the groups attached to the root group ..." << endl;
  try {
    DAL::h5get_names(names,fileID);
  }
  catch (std::string message) {
    cerr << message << endl;
    ++nofFailedTests;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc,
          char *argv[])
{
  int nofFailedTests (0);
  std::string data_timeseries;
  std::string data_beamformed;
  bool have_datasets (false);
  
  /* Check for command-line parameters */
  if (argc > 2) {
    data_timeseries = argv[1];
    data_beamformed = argv[2];
    have_datasets   = true;
  }
  
#ifdef HAVE_CASA
  nofFailedTests += test_casacore_variables ();
#endif
  nofFailedTests += test_operators ();
  nofFailedTests += test_iterators ();
  nofFailedTests += test_hdf5_attributes ();
  // Test the various functions for inspection of a HDF5 file
  nofFailedTests += test_hdf5_inspection ();
  
  if (have_datasets) {
    test_timeseries (data_timeseries);
//     test_beamformed (data_beamformed);
  }
  
  return nofFailedTests;
}
