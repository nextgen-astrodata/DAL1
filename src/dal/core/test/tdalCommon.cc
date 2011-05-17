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
#include <core/Enumerations.h>
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
  
#ifdef DAL_WITH_CASA
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
  // Test access to HDF5 attributes
  nofFailedTests += test_hdf5_attributes ();
  
  return nofFailedTests;
}
