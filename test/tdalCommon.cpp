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
  \brief Test overloading of operators

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

  //__________________________________________________________________
  
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

  //__________________________________________________________________
  
  cout << "[2] Set attributes of single value ..." << endl;
  try {
    std::cout << "-- Attribute of type int ..." << std::endl;
    int data_int (1);
    DAL::h5set_attribute( fileID, "ATTRIBUTE_INT", data_int );
    //
    std::cout << "-- Attribute of type uint ..." << std::endl;
    uint data_uint (2);
    DAL::h5set_attribute( fileID, "ATTRIBUTE_UINT", data_uint );
    //
    std::cout << "-- Attribute of type short ..." << std::endl;
    short data_short (3);
    DAL::h5set_attribute( fileID, "ATTRIBUTE_SHORT", data_short );
    //
    std::cout << "-- Attribute of type long ..." << std::endl;
    long data_long (4);
    DAL::h5set_attribute( fileID, "ATTRIBUTE_LONG", data_long );
    //
    std::cout << "-- Attribute of type float ..." << std::endl;
    float data_float (5);
    DAL::h5set_attribute( fileID, "ATTRIBUTE_FLOAT", data_float );
    //
    std::cout << "-- Attribute of type double ..." << std::endl;
    double data_double (6);
    DAL::h5set_attribute( fileID, "ATTRIBUTE_DOUBLE", data_double );
    //
    std::cout << "-- Attribute of type string ..." << std::endl;
    std::string data_string ("Hello");
    DAL::h5set_attribute( fileID, "ATTRIBUTE_STRING", data_string );
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  //__________________________________________________________________
  
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
    DAL::h5set_attribute (fileID,
			 "ATTRIBUTE_INT_ARRAY",
			 data_int,
			 3);
    //
    std::cout << "-- Attribute of type uint[3] ..." << std::endl;
    data_uint = new uint[3];
    data_uint[0] = 1;
    data_uint[1] = 2;
    data_uint[2] = 3;
    DAL::h5set_attribute (fileID,
			 "ATTRIBUTE_UINT_ARRAY",
			 data_uint,
			 3);
    //
    std::cout << "-- Attribute of type short[3] ..." << std::endl;
    data_short = new short[3];
    data_short[0] = 1;
    data_short[1] = 2;
    data_short[2] = 3;
    DAL::h5set_attribute (fileID,
			 "ATTRIBUTE_SHORT_ARRAY",
			 data_short,
			 3);
    //
    std::cout << "-- Attribute of type long[3] ..." << std::endl;
    data_long = new long[3];
    data_long[0] = 1;
    data_long[1] = 2;
    data_long[2] = 3;
    DAL::h5set_attribute (fileID,
			 "ATTRIBUTE_LONG_ARRAY",
			 data_long,
			 3);
    //
    std::cout << "-- Attribute of type float[3] ..." << std::endl;
    data_float = new float[3];
    data_float[0] = 1;
    data_float[1] = 2;
    data_float[2] = 3;
    DAL::h5set_attribute (fileID,
			 "ATTRIBUTE_FLOAT_ARRAY",
			 data_float,
			 3);
    //
    std::cout << "-- Attribute of type double[3] ..." << std::endl;
    data_double = new double[3];
    data_double[0] = 1;
    data_double[1] = 2;
    data_double[2] = 3;
    DAL::h5set_attribute (fileID,
			 "ATTRIBUTE_DOUBLE_ARRAY",
			 data_double,
			 3);
    //
    std::cout << "-- Attribute of type string[3] ..." << std::endl;
    data_string = new std::string[3];
    data_string[0] = "aa";
    data_string[1] = "bb";
    data_string[2] = "cc";
    DAL::h5set_attribute (fileID,
			  "ATTRIBUTE_STRING_ARRAY",
			  data_string,
			  3);
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  
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
    DAL::h5set_attribute (fileID,
			  "ATTRIBUTE_INT_VECTOR",
			  data_int);
    //
    std::cout << "-- Attribute of type std::vector<uint> ..." << std::endl;
    data_uint[0] = 4;
    data_uint[1] = 5;
    data_uint[2] = 6;
    DAL::h5set_attribute (fileID,
			  "ATTRIBUTE_UINT_VECTOR",
			  data_uint);
    //
    std::cout << "-- Attribute of type std::vector<short> ..." << std::endl;
    data_short[0] = 7;
    data_short[1] = 8;
    data_short[2] = 9;
    DAL::h5set_attribute (fileID,
			  "ATTRIBUTE_SHORT_VECTOR",
			  data_short);
    //
    std::cout << "-- Attribute of type std::vector<long> ..." << std::endl;
    data_long[0] = 10;
    data_long[1] = 11;
    data_long[2] = 12;
    DAL::h5set_attribute (fileID,
			  "ATTRIBUTE_LONG_VECTOR",
			  data_long);
    //
    std::cout << "-- Attribute of type std::vector<float> ..." << std::endl;
    data_float[0] = 13;
    data_float[1] = 14;
    data_float[2] = 15;
    DAL::h5set_attribute (fileID,
			  "ATTRIBUTE_FLOAT_VECTOR",
			  data_float);
    //
    std::cout << "-- Attribute of type std::vector<double> ..." << std::endl;
    data_double[0] = 16;
    data_double[1] = 17;
    data_double[2] = 18;
    DAL::h5set_attribute (fileID,
			  "ATTRIBUTE_DOUBLE_VECTOR",
			  data_double);
    //
    std::cout << "-- Attribute of type std::vector<string> ..." << std::endl;
    data_string[0] = "aaa";
    data_string[1] = "bbb";
    data_string[2] = "ccc";
    DAL::h5set_attribute (fileID,
			  "ATTRIBUTE_STRING_VECTOR",
			  data_string);
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  
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

  //__________________________________________________________________
  // Test reading attributes consisting of a single value
  
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

  //__________________________________________________________________
  // Test reading attributes into std::vector<T>
  
  cout << "[7] Read attributes of type std::vector<T> ..." << endl;
  try {
    std::vector<int> data_int(nelem);
    std::vector<uint> data_uint(nelem);
    std::vector<short> data_short(nelem);
    std::vector<long> data_long(nelem);
    std::vector<float> data_float(nelem);
    std::vector<double> data_double(nelem);
    std::vector<std::string> data_string(nelem);
    //
    DAL::h5get_attribute (fileID, "ATTRIBUTE_INT_ARRAY", data_int);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_UINT_ARRAY", data_uint);    
    DAL::h5get_attribute (fileID, "ATTRIBUTE_SHORT_ARRAY", data_short);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_LONG_ARRAY", data_long);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_FLOAT_ARRAY", data_float);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_DOUBLE_ARRAY", data_double);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_STRING_ARRAY", data_string);
    //
    cout << "-- ATTRIBUTE_INT_ARRAY    = " << data_int    << endl;
    cout << "-- ATTRIBUTE_UINT_ARRAY   = " << data_uint   << endl;
    cout << "-- ATTRIBUTE_SHORT_ARRAY  = " << data_short  << endl;
    cout << "-- ATTRIBUTE_LONG_ARRAY   = " << data_long   << endl;
    cout << "-- ATTRIBUTE_FLOAT_ARRAY  = " << data_float  << endl;
    cout << "-- ATTRIBUTE_DOUBLE_ARRAY = " << data_double << endl;
    cout << "-- ATTRIBUTE_STRING_ARRAY = " << data_string << endl;
    //
    DAL::h5get_attribute (fileID, "ATTRIBUTE_INT_VECTOR", data_int);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_UINT_VECTOR", data_uint);    
    DAL::h5get_attribute (fileID, "ATTRIBUTE_SHORT_VECTOR", data_short);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_LONG_VECTOR", data_long);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_FLOAT_VECTOR", data_float);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_DOUBLE_VECTOR", data_double);
    DAL::h5get_attribute (fileID, "ATTRIBUTE_STRING_VECTOR", data_string);
    //
    cout << "-- ATTRIBUTE_INT_VECTOR    = " << data_int    << endl;
    cout << "-- ATTRIBUTE_UINT_VECTOR   = " << data_uint   << endl;
    cout << "-- ATTRIBUTE_SHORT_VECTOR  = " << data_short  << endl;
    cout << "-- ATTRIBUTE_LONG_VECTOR   = " << data_long   << endl;
    cout << "-- ATTRIBUTE_FLOAT_VECTOR  = " << data_float  << endl;
    cout << "-- ATTRIBUTE_DOUBLE_VECTOR = " << data_double << endl;
    cout << "-- ATTRIBUTE_STRING_VECTOR = " << data_string << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Test repeated read access to attributes

  cout << "[8] Test repeated read access to attributes ..." << endl;
  try {
    uint nofLoops (200);
    int data_int (0);
    std::string data_string;
    std::vector<int> vec_int;
    std::vector<std::string> vec_string;
    //
    cout << "-- ATTRIBUTE_INT ...           " << std::flush;
    for (uint n(0); n<nofLoops; n++) {
      DAL::h5get_attribute (fileID, "ATTRIBUTE_INT", data_int);
    }
    cout << nofLoops << " reads finished." << endl;
    //
    cout << "-- ATTRIBUTE_STRING ...        " << std::flush;
    for (uint n(0); n<nofLoops; n++) {
      DAL::h5get_attribute (fileID, "ATTRIBUTE_STRING", data_string);
    }
    cout << nofLoops << " reads finished." << endl;
    //
    cout << "-- ATTRIBUTE_INT_VECTOR    ... " << std::flush;
    for (uint n(0); n<nofLoops; n++) {
      DAL::h5get_attribute (fileID, "ATTRIBUTE_INT_VECTOR", vec_int);
    }
    cout << nofLoops << " reads finished." << endl;
    //
    cout << "-- ATTRIBUTE_STRING_VECTOR ... " << std::flush;
    for (uint n(0); n<nofLoops; n++) {
      DAL::h5get_attribute (fileID, "ATTRIBUTE_STRING_VECTOR", vec_string);
    }
    cout << nofLoops << " reads finished." << endl;
  } catch (std::string message) {
    cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \param filename -- Name of the input HDF5 file on which the access routines are
         tested.

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_datasets (std::string const &filename)
{
  std::cout << "\n[tdalCommon::test_datasets]\n" << std::endl;

  int nofFailedTests (0);
  hid_t file_id (-1);
  hid_t group_id (-1);
  hid_t dataset_id (-1);
  
  //__________________________________________________________________
  // Open the HDF5 dataset
  
  std::cout << "-- opening HDF5 file ..." << std::endl;
  file_id = H5Fopen (filename.c_str(),
		     H5F_ACC_RDONLY,
		     H5P_DEFAULT);

  if (file_id < 0) {
    std::cerr << "Failed to open file " << filename << std::endl;
    return 1;
  }
  
  //__________________________________________________________________
  // Access the attributes attached to the root group of the file

  try {
    std::string name;
    std::string telescope;
    std::string observer;
    std::string project;
    std::string observation_id;
    std::string observation_mode;
    //
    DAL::h5get_name (name,file_id);
    DAL::h5get_attribute (file_id,"TELESCOPE",telescope);
    DAL::h5get_attribute (file_id,"OBSERVER",observer);
    DAL::h5get_attribute (file_id,"PROJECT",project);
    DAL::h5get_attribute (file_id,"OBSERVATION_ID",observation_id);
    DAL::h5get_attribute (file_id,"OBSERVATION_MODE",observation_mode);
    //
    cout << "-- TELESCOPE ...... = " << telescope        << endl;
    cout << "-- OBSERVER ....... = " << observer         << endl;
    cout << "-- PROJECT ........ = " << project          << endl;
    cout << "-- OBSERVATION_ID   = " << observation_id   << endl;
    cout << "-- OBSERVATION_MODE = " << observation_mode << endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  //__________________________________________________________________
  // Open the station group
  
  std::cout << "-- opening station group ..." << std::endl;
  group_id = H5Gopen1 (file_id,"Station001");
  
  if (group_id < 0) {
    std::cerr << "Error opening the station group!" << std::endl;
    return 1;
  } 

  //__________________________________________________________________
  // Access the attributes attached to the station group

  try {
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
    DAL::h5get_attribute (group_id, "TRIGGER_TYPE", trigger_type);
    DAL::h5get_attribute (group_id, "TRIGGER_OFFSET", trigger_offset);
    DAL::h5get_attribute (group_id, "STATION_POSITION_VALUE", station_position_value);
    DAL::h5get_attribute (group_id, "STATION_POSITION_UNIT", station_position_unit);
    DAL::h5get_attribute (group_id, "STATION_POSITION_FRAME", station_position_frame);
    DAL::h5get_attribute (group_id, "BEAM_DIRECTION_VALUE", beam_direction_value);
    DAL::h5get_attribute (group_id, "BEAM_DIRECTION_UNIT", beam_direction_unit);
    DAL::h5get_attribute (group_id, "BEAM_DIRECTION_FRAME", beam_direction_frame);
    //
    cout << "-- TRIGGER_TYPE ......... = " << trigger_type           << endl;
    cout << "-- TRIGGER_OFFSET ....... = " << trigger_offset         << endl;
    cout << "-- STATION_POSITION_VALUE = " << station_position_value << endl;
    cout << "-- STATION_POSITION_UNIT  = " << station_position_unit  << endl;
    cout << "-- STATION_POSITION_FRAME = " << station_position_frame << endl;
    cout << "-- BEAM_DIRECTION_VALUE . = " << beam_direction_value   << endl;
    cout << "-- BEAM_DIRECTION_UNIT .. = " << beam_direction_unit    << endl;
    cout << "-- BEAM_DIRECTION_FRAME . = " << beam_direction_frame   << endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  //__________________________________________________________________
  // Open the dipole dataset

  std::cout << "-- opening dipole dataset ..." << std::endl;
  dataset_id = H5Dopen1 (group_id,
			 "001000001");
  
  if (dataset_id < 0) {
    std::cerr << "Error opening the dipole dataset!" << std::endl;
    return 1;
  } 

  //__________________________________________________________________
  // Access the attributes attached to the dipole dataset
  
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
    DAL::h5get_attribute (dataset_id,"STATION_ID", station_id);
    DAL::h5get_attribute (dataset_id,"RSP_ID", rsp_id);
    DAL::h5get_attribute (dataset_id,"RCU_ID", rcu_id);
    DAL::h5get_attribute (dataset_id,"TIME", time);
    DAL::h5get_attribute (dataset_id,"SAMPLE_FREQUENCY_VALUE", sample_frequency_value);
    DAL::h5get_attribute (dataset_id,"SAMPLE_FREQUENCY_UNIT", sample_frequency_unit);
    DAL::h5get_attribute (dataset_id,"SAMPLE_FREQUENCY_VALUE", sample_frequency_value);
    DAL::h5get_attribute (dataset_id,"SAMPLE_FREQUENCY_UNIT", sample_frequency_unit);
    DAL::h5get_attribute (dataset_id,"ANTENNA_POSITION_VALUE", antenna_position_value);
    DAL::h5get_attribute (dataset_id,"ANTENNA_POSITION_UNIT", antenna_position_unit);
    DAL::h5get_attribute (dataset_id,"ANTENNA_POSITION_FRAME", antenna_position_frame);
    DAL::h5get_attribute (dataset_id,"ANTENNA_ORIENTATION_VALUE", antenna_orientation_value);
    DAL::h5get_attribute (dataset_id,"ANTENNA_ORIENTATION_UNIT", antenna_orientation_unit);
    DAL::h5get_attribute (dataset_id,"ANTENNA_ORIENTATION_FRAME", antenna_orientation_frame);
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
  } catch (std::string message) {
    std::cerr << message << std::endl;
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
  bool have_filename (false);

  /* Check for command-line parameters */
  if (argc > 1) {
    filename = argv[1];
    have_filename = true;
  }
  
  nofFailedTests += test_operators ();
  nofFailedTests += test_hdf5_attributes ();

  if (have_filename) {
    test_datasets (filename);
  }

  return nofFailedTests;
}
