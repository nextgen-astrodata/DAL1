/***************************************************************************
 *   Copyright (C) 2011                                                    *
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

#include <sstream>
#include <core/dalCommon.h>
#include <core/HDF5Attribute.h>

// Namespace usage
using std::cerr;
using std::cout;
using std::endl;
using DAL::HDF5Attribute;
using DAL::HDF5Object;

/*!
  \file tHDF5Attribute.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::HDF5Attribute class
 
  \author Lars B&auml;hren
 
  \date 2011/01/27

  We are using the following naming scheme for attributes attached to a group
  or dataset, in order to reflect the data container they were written from:
  \verbatim
  h5a_short              short           H5T_NATIVE_SHORT
  h5a_ushort             ushort          H5T_NATIVE_USHORT
  h5a_int                int             H5T_NATIVE_INT
  h5a_uint               uint            H5T_NATIVE_UINT
  h5a_long               long            H5T_NATIVE_LONG
  h5a_ulong              ulong           H5T_NATIVE_ULONG
  h5a_llong              llong           H5T_NATIVE_LLONG
  h5a_ullong             ullong          H5T_NATIVE_ULLONG
  h5a_float              float           H5T_NATIVE_FLOAT
  h5a_double             double          H5T_NATIVE_DOUBLE
  h5a_bool               bool            H5T_NATIVE_INT
  h5a_string             string

  h5a_array_bool         bool*
  h5a_array_int          int*
  h5a_array_uint         uint*
  h5a_array_short        short*
  h5a_array_ushort       ushort*
  h5a_array_float        float*
  h5a_array_double       double*
  h5a_array_string       string*

  h5a_vector_bool        vector<bool>
  h5a_vector_int         vector<int>
  h5a_vector_uint        vector<uint>
  h5a_vector_short       vector<short>
  h5a_vector_ushort      vector<ushort>
  h5a_vector_float       vector<float>
  h5a_vector_double      vector<double>
  h5a_vector_string      vector<string>

  h5a_set_bool           set<bool>
  h5a_set_int            set<int>
  h5a_set_uint           set<uint>
  h5a_set_short          set<short>
  h5a_set_ushort         set<ushort>
  h5a_set_float          set<float>
  h5a_set_double         set<double>
  h5a_set_string         set<string>
  \endverbatim
  The first column contains the name of the attribute, whereas the second column
  lists the type of the data variable used as part of the test program.

*/

// ==============================================================================
//
//  Helper routines
//
// ==============================================================================

//_______________________________________________________________________________
//                                                                           show

template <class T>
std::string show (std::vector<T> const &data)
{
  std::ostringstream output;

  if (data.empty()) {
    output << "[ ]";
  } else {
    output << "[";
    
    for (size_t n=0; n<data.size(); ++n) {
      output << " " << data[n];
    }
    
    output << " ]";
  }
  
  return output.str();
}

//_______________________________________________________________________________
//                                                                  readAttribute

bool readAttribute (hid_t const &location,
		    std::string const &name,
		    std::vector<std::string> &data)
{
  bool status  = true;
  
  /*____________________________________________________________
    If attribute has been found at reference location, open it.
    If opening has been successful, extract datatype and
    dataspace.
  */
  
  hid_t attribute = H5Aopen (location,
			     name.c_str(),
			     H5P_DEFAULT);
  
  if (H5Iis_valid(attribute)) {
    
    std::vector<hsize_t> dims;
    std::vector<hsize_t> dimsMax;
    herr_t h5err    = DAL::HDF5Dataspace::shape (attribute,dims,dimsMax);
    hid_t datatype  = H5Aget_type (attribute);
    hid_t dataspace = H5Aget_space(attribute);

    if (dims.size()>0) {

      hid_t memtype  = H5Tcopy (H5T_C_S1);
      /* Adjust the size of the buffer array */
      char **buffer = (char **) malloc (dims[0] * sizeof (char *));
      h5err = H5Tset_size (memtype, H5T_VARIABLE);
      /* Read the attribute into the buffer */
      h5err = H5Aread (attribute, memtype, buffer);

      /* Copy attribute data from buffer to returned array */
      data.resize(dims[0]);
      
      for (hsize_t n=0; n<dims[0]; ++n) {
	data[n] = buffer[n];
      }
    
      /* Release allocated memory */
      h5err = H5Dvlen_reclaim (memtype, dataspace, H5P_DEFAULT, buffer);
      free (buffer);
      
    } else {
      return false;
    }
    
    /* Release HDF5 object identifiers */
    if (H5Iis_valid(datatype))  { H5Tclose (datatype);  };
    if (H5Iis_valid(dataspace)) { H5Sclose (dataspace); };
    if (H5Iis_valid(attribute)) { H5Aclose (attribute); };
  } else {
    std::cerr << "[readAttribute]"
	      << " Failed to open attribute " << name << endl;
    status = false;
  }
  
  return status;
}

// ==============================================================================
//
//  Test routines
//
// ==============================================================================

//_______________________________________________________________________________
//                                                              test_static_write

/*!
  \brief Test static functions to write attributes

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_write (hid_t const &location)
{
  std::cout << "\n[tHDF5Attribute::test_static_write]" << endl;

  int nofFailedTests = 0;
  
  /*__________________________________________________________________
    Test 1: Create/Set attribute storing atomic value.
  */
  
  std::cout << "\n[1] Testing write(hid_t,string,T) ..." << endl;
  try {
    bool valBool             = false;
    short valShort           = 1;
    unsigned short valUshort = 2;
    int valInt               = 3;
    unsigned int valUint     = 4;
    long valLong             = 5;
    unsigned long valUlong   = 6;
    float valFloat           = 0.5;
    double valDouble         = 0.25;
    std::string valString    = "bla";

    std::cout << "-- Start writing attributes." << endl << std::flush;
    HDF5Attribute::write (location, "h5a_bool",    valBool);
    HDF5Attribute::write (location, "h5a_short",   valShort);
    HDF5Attribute::write (location, "h5a_ushort",  valUshort);
    HDF5Attribute::write (location, "h5a_int",     valInt);
    HDF5Attribute::write (location, "h5a_uint",    valUint);
    HDF5Attribute::write (location, "h5a_long",    valLong);
    HDF5Attribute::write (location, "h5a_ulong",   valUlong);
    HDF5Attribute::write (location, "h5a_float",   valFloat);
    HDF5Attribute::write (location, "h5a_double",  valDouble);
    HDF5Attribute::write (location, "h5a_string",  valString);
    std::cout << "-- Finished writing attributes." << endl << std::flush;
  } catch (std::string message) {
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 2: Create/Set attribute storing array-type value, using
            interface for std::vector<T>
  */

  cout << "\n[2] Testing write(hid_t,string,vector<T>) ..." << endl;
  try {
    unsigned int nelem = 3;
    std::vector<bool> valBool             (nelem, false);
    std::vector<short> valShort           (nelem, 1);
    std::vector<unsigned short> valUshort (nelem, 2);
    std::vector<int> valInt               (nelem, 3);
    std::vector<unsigned int> valUint     (nelem, 4);
    std::vector<long> valLong             (nelem, 5);
    std::vector<unsigned long> valUlong   (nelem, 6);
    std::vector<float> valFloat           (nelem, 0.5);
    std::vector<double> valDouble         (nelem, 0.25);
    std::vector<std::string> valString    (nelem, "bla");
    
    std::cout << "-- Start writing attributes." << endl << std::flush;
    HDF5Attribute::write (location, "h5a_vector_bool",    valBool);
    HDF5Attribute::write (location, "h5a_vector_short",   valShort);
    HDF5Attribute::write (location, "h5a_vector_ushort",  valUshort);
    HDF5Attribute::write (location, "h5a_vector_int",     valInt);
    HDF5Attribute::write (location, "h5a_vector_uint",    valUint);
    HDF5Attribute::write (location, "h5a_vector_long",    valLong);
    HDF5Attribute::write (location, "h5a_vector_ulong",   valUlong);
    HDF5Attribute::write (location, "h5a_vector_float",   valFloat);
    HDF5Attribute::write (location, "h5a_vector_double",  valDouble);
    HDF5Attribute::write (location, "h5a_vector_string",  valString);
    std::cout << "-- Finished writing attributes." << endl << std::flush;
  } catch (std::string message) {
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 3: Create/Set attribute storing array-type value
  */
  
  cout << "\n[3] Testing write(hid_t,string,T*,uint) ..." << endl;
  try {
    unsigned int nelem      = 5;
    int valBool[]           = {false,true,false,true,false};
    int valInt[]            = {1,1,1,1,1};
    unsigned int valUint[]  = {2,2,2,2,2};
    short valShort[]        = {3,3,3,3,3};
    long valLong[]          = {4,4,4,4,4};
    float valFloat[]        = {0.5,0.5,0.5,0.5,0.5};
    double valDouble[]      = {0.25,0.25,0.25,0.25,0.25};
    std::string valString[] = {"a","bb","ccc","dddd","eeeee"};

    std::cout << "-- Start writing attributes." << endl << std::flush;
    HDF5Attribute::write (location, "h5a_array_bool",   valBool,   nelem);
    HDF5Attribute::write (location, "h5a_array_int",    valInt,    nelem);
    HDF5Attribute::write (location, "h5a_array_uint",   valUint,   nelem);
    HDF5Attribute::write (location, "h5a_array_short",  valShort,  nelem);
    HDF5Attribute::write (location, "h5a_array_long",   valLong,   nelem);
    HDF5Attribute::write (location, "h5a_array_float",  valFloat,  nelem);
    HDF5Attribute::write (location, "h5a_array_double", valDouble, nelem);
    HDF5Attribute::write (location, "h5a_array_string", valString, nelem);
    std::cout << "-- Finished writing attributes." << endl << std::flush;
  } catch (std::string message) {
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 4: Set/Update attributes
  */

  cout << "\n[4] Updating attributes via write(hid_t,string,T) ..." << endl;
  try {
    bool valBool             = true;
    short valShort           = 10;
    unsigned short valUshort = 20;
    int valInt               = 30;
    unsigned int valUint     = 40;
    long valLong             = 50;
    unsigned long valUlong   = 60;
    float valFloat           = 0.05;
    double valDouble         = 0.025;
    std::string valString    = "bladibla";

    std::cout << "-- Start updating attributes." << endl << std::flush;
    HDF5Attribute::write (location, "h5a_bool",    valBool);
    HDF5Attribute::write (location, "h5a_short",   valShort);
    HDF5Attribute::write (location, "h5a_ushort",  valUshort);
    HDF5Attribute::write (location, "h5a_int",     valInt);
    HDF5Attribute::write (location, "h5a_uint",    valUint);
    HDF5Attribute::write (location, "h5a_long",    valLong);
    HDF5Attribute::write (location, "h5a_ulong",   valUlong);
    HDF5Attribute::write (location, "h5a_float",   valFloat);
    HDF5Attribute::write (location, "h5a_double",  valDouble);
    HDF5Attribute::write (location, "h5a_string",  valString);
    std::cout << "-- Finished updating attributes." << endl << std::flush;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 5: Set/Update attributes
  */

  cout << "\n[5] Updating attributes via write(hid_t,string,vector<T>) ..." << endl;
  try {
    unsigned int nelem = 3;
    std::vector<int> valInt            (nelem, 10);
    std::vector<unsigned int> valUint  (nelem, 20);
    std::vector<short> valShort        (nelem, 30);
    std::vector<long> valLong          (nelem, 40);
    std::vector<float> valFloat        (nelem, 0.05);
    std::vector<double> valDouble      (nelem, 0.025);
    std::vector<std::string> valString (nelem, "bladibla");
    
    std::cout << "-- Start updating attributes." << endl << std::flush;
    HDF5Attribute::write (location, "h5a_vector_int",    valInt);
    HDF5Attribute::write (location, "h5a_vector_uint",   valUint);
    HDF5Attribute::write (location, "h5a_vector_short",  valShort);
    HDF5Attribute::write (location, "h5a_vector_long",   valLong);
    HDF5Attribute::write (location, "h5a_vector_float",  valFloat);
    HDF5Attribute::write (location, "h5a_vector_double", valDouble);
    HDF5Attribute::write (location, "h5a_vector_string", valString);
    std::cout << "-- Finished updating attributes." << endl << std::flush;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 6: Set/Update attributes
  */

  cout << "\n[6] Updating attributes via write(hid_t,string,T*,uint) ..." << endl;
  try {
    unsigned int nelem      = 5;
    int valInt[]            = {10,10,10,10,10};
    unsigned int valUint[]  = {20,20,20,20,20};
    short valShort[]        = {30,30,30,30,30};
    long valLong[]          = {40,40,40,40,40};
    float valFloat[]        = {0.05,0.05,0.05,0.05,0.05};
    double valDouble[]      = {0.025,0.025,0.025,0.025,0.025};
    std::string valString[] = {"A","BB","CCC","DDDD","EEEEE"};

    HDF5Attribute::write (location, "h5a_array_int",    valInt,    nelem);
    HDF5Attribute::write (location, "h5a_array_uint",   valUint,   nelem);
    HDF5Attribute::write (location, "h5a_array_short",  valShort,  nelem);
    HDF5Attribute::write (location, "h5a_array_long",   valLong,   nelem);
    HDF5Attribute::write (location, "h5a_array_float",  valFloat,  nelem);
    HDF5Attribute::write (location, "h5a_array_double", valDouble, nelem);
    HDF5Attribute::write (location, "h5a_array_string", valString, nelem);
  } catch (std::string message) {
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 7: Create / Set attributes from casa::Vector<T>
  */

#ifdef DAL_WITH_CASA
  cout << "\n[7] Testing write(hid_t,string,casa::Vector<T>) ..." << endl;
  try {
    unsigned int nelem (10);
    casa::Vector<int> valInt (nelem, 1);
    casa::Vector<short> valShort (nelem, 2);
    casa::Vector<long> valLong (nelem, 3);
    casa::Vector<float> valFloat (nelem, 0.5);
    casa::Vector<double> valDouble (nelem, 0.25);
    casa::Vector<std::string> valString (nelem, "bla");

    HDF5Attribute::write (location, "AttributeCASAInt",    valInt);
    HDF5Attribute::write (location, "AttributeCASAShort",  valShort);
    HDF5Attribute::write (location, "AttributeCASALong",   valLong);
    HDF5Attribute::write (location, "AttributeCASAFloat",  valFloat);
    HDF5Attribute::write (location, "AttributeCASADouble", valDouble);
    HDF5Attribute::write (location, "AttributeCASAString", valString);
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
#endif

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                               test_static_read

/*!
  \brief Test static functions to read attributes

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_static_read (hid_t const &location)
{
  std::cout << "\n[tHDF5Attribute::test_static_read]" << endl;

  int nofFailedTests = 0;
  
  /*__________________________________________________________________
    Test 1: Get number, names and types of attributes
  */
  
  cout << "\n[1] Get number, names and types of attributes ..." << endl;
  try {
    cout << "-- nof attributes  = " << HDF5Object::nofAttributes(location) << endl;
    // cout << "-- Attribute names = " << HDF5Attribute::attributes(location) << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 2: Retrieve previously set attributes.
  */

  cout << "\n[2] Testing read(hid_t,string,T) ..." << endl;
  try {
    short valShort;
    unsigned short valUshort;
    int valInt;
    unsigned int valUint;
    long valLong;
    unsigned long valUlong;
    float valFloat;
    double valDouble;
    std::string valString;

    HDF5Attribute::read (location, "h5a_short",   valShort);
    HDF5Attribute::read (location, "h5a_ushort",  valUshort);
    HDF5Attribute::read (location, "h5a_int",     valInt);
    HDF5Attribute::read (location, "h5a_uint",    valUint);
    HDF5Attribute::read (location, "h5a_long",    valLong);
    HDF5Attribute::read (location, "h5a_ulong",   valUlong);
    HDF5Attribute::read (location, "h5a_float",   valFloat);
    HDF5Attribute::read (location, "h5a_double",  valDouble);
    HDF5Attribute::read (location, "h5a_string",  valString);

    std::cout << "-- h5a_short     = " << valShort  << endl;
    std::cout << "-- h5a_ushort    = " << valUshort << endl;
    std::cout << "-- h5a_int       = " << valInt    << endl;
    std::cout << "-- h5a_uint      = " << valUint   << endl;
    std::cout << "-- h5a_long      = " << valLong   << endl;
    std::cout << "-- h5a_ulong     = " << valUlong  << endl;
    std::cout << "-- h5a_float     = " << valFloat  << endl;
    std::cout << "-- h5a_double    = " << valDouble << endl;
    std::cout << "-- h5a_string    = " << valString << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  /*__________________________________________________________________
    Test 3: Retrieve previously set attributes.
  */

  cout << "\n[3] Testing read(hid_t,string,vector<T>) ..." << endl;
  try {
    std::vector<short> valShort;
    std::vector<unsigned short> valUshort;
    std::vector<int> valInt;
    std::vector<unsigned int> valUint;
    std::vector<long> valLong;
    std::vector<unsigned long> valUlong;
    std::vector<float> valFloat;
    std::vector<double> valDouble;
    std::vector<std::string> valString;

    /* Read single-value attributes */

    HDF5Attribute::read (location, "h5a_short",   valShort);
    HDF5Attribute::read (location, "h5a_ushort",  valUshort);
    HDF5Attribute::read (location, "h5a_int",     valInt);
    HDF5Attribute::read (location, "h5a_uint",    valUint);
    HDF5Attribute::read (location, "h5a_long",    valLong);
    HDF5Attribute::read (location, "h5a_ulong",   valUlong);
    HDF5Attribute::read (location, "h5a_float",   valFloat);
    HDF5Attribute::read (location, "h5a_double",  valDouble);
    HDF5Attribute::read (location, "h5a_string",  valString);

    std::cout << "-- h5a_short         = " << show(valShort)  << endl;
    std::cout << "-- h5a_ushort        = " << show(valUshort) << endl;
    std::cout << "-- h5a_int           = " << show(valInt)    << endl;
    std::cout << "-- h5a_uint          = " << show(valUint)   << endl;
    std::cout << "-- h5a_long          = " << show(valLong)   << endl;
    std::cout << "-- h5a_ulong         = " << show(valUlong)  << endl;
    std::cout << "-- h5a_float         = " << show(valFloat)  << endl;
    std::cout << "-- h5a_double        = " << show(valDouble) << endl;
    std::cout << "-- h5a_string        = " << show(valString) << endl;

    /* Read vector-value attributes */

    HDF5Attribute::read (location, "h5a_vector_short",   valShort);
    HDF5Attribute::read (location, "h5a_vector_ushort",  valUshort);
    HDF5Attribute::read (location, "h5a_vector_int",     valInt);
    HDF5Attribute::read (location, "h5a_vector_uint",    valUint);
    HDF5Attribute::read (location, "h5a_vector_long",    valLong);
    HDF5Attribute::read (location, "h5a_vector_ulong",   valUlong);
    HDF5Attribute::read (location, "h5a_vector_float",   valFloat);
    HDF5Attribute::read (location, "h5a_vector_double",  valDouble);
    HDF5Attribute::read (location, "h5a_vector_string",  valString);

    std::cout << "-- h5a_vector_short  = " << show(valShort)  << endl;
    std::cout << "-- h5a_vector_ushort = " << show(valUshort) << endl;
    std::cout << "-- h5a_vector_int    = " << show(valInt)    << endl;
    std::cout << "-- h5a_vector_uint   = " << show(valUint)   << endl;
    std::cout << "-- h5a_vector_long   = " << show(valLong)   << endl;
    std::cout << "-- h5a_vector_ulong  = " << show(valUlong)  << endl;
    std::cout << "-- h5a_vector_float  = " << show(valFloat)  << endl;
    std::cout << "-- h5a_vector_double = " << show(valDouble) << endl;
    std::cout << "-- h5a_vector_string = " << show(valString) << endl;

    /* Read array-value attributes */

    HDF5Attribute::read (location, "h5a_array_int",    valInt);
    HDF5Attribute::read (location, "h5a_array_uint",   valUint);
    HDF5Attribute::read (location, "h5a_array_short",  valShort);
    HDF5Attribute::read (location, "h5a_array_long",   valLong);
    HDF5Attribute::read (location, "h5a_array_float",  valFloat);
    HDF5Attribute::read (location, "h5a_array_double", valDouble);
    HDF5Attribute::read (location, "h5a_array_string", valString);

    std::cout << "-- h5a_array_int     = " << show(valInt)    << endl;
    std::cout << "-- h5a_array_uint    = " << show(valUint)   << endl;
    std::cout << "-- h5a_array_short   = " << show(valShort)  << endl;
    std::cout << "-- h5a_array_long    = " << show(valLong)   << endl;
    std::cout << "-- h5a_array_float   = " << show(valFloat)  << endl;
    std::cout << "-- h5a_array_double  = " << show(valDouble) << endl;
    std::cout << "-- h5a_array_string  = " << show(valString) << endl;

  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }

  return nofFailedTests;
}
  
//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new HDF5Attribute object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors (hid_t const &location)
{
  std::cout << "\n[tHDF5Attribute::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "-- nof. attributes = " << HDF5Object::nofAttributes(location) << endl;

  /*__________________________________________________________________
    Test 1: Test for the default constructor
  */

  std::cout << "[1] Testing HDF5Attribute() ..." << endl;
  try {
    HDF5Attribute attr;
    //
    attr.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  /*__________________________________________________________________
    Test 2: Testing argumented constructor
  */

  std::cout << "[2] Testing HDF5Attribute(hid_t,string) ..." << endl;
  try {
    HDF5Attribute attrInt (location, "AttributeInt");
    attrInt.summary(); 
    //
    HDF5Attribute attrFloat (location, "AttributeFloat");
    attrFloat.summary(); 
    //
    HDF5Attribute attrDouble (location, "AttributeDouble");
    attrDouble.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
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
int main (int argc, char *argv[])
{
  int nofFailedTests   = 0;
  hid_t fileID         = 0;
  std::string filename = "tHDF5Attribute.h5";
  bool haveDataset     = false;

  //________________________________________________________
  // Process command line parameters

  if (argc>1) {
    filename    = argv[1];
    haveDataset = true;
  }

  //________________________________________________________
  // Open/create HDF5 file to work with
  
  if (haveDataset) {
    fileID = H5Fopen (filename.c_str(),
		      H5F_ACC_RDWR,
		      H5P_DEFAULT);
  } else {
    fileID = H5Fcreate (filename.c_str(),
			H5F_ACC_TRUNC,
			H5P_DEFAULT,
			H5P_DEFAULT);
  }
  
  //________________________________________________________
  // Run the tests
  
  if (H5Iis_valid(fileID)) {

    // Test static functions to write attributes
    nofFailedTests += test_static_write (fileID);

    // Test static functions to read attributes
    nofFailedTests += test_static_read (fileID);

//     // Test for the constructor(s)
//     nofFailedTests += test_constructors (fileID);
    
  } else {
    std::cerr << "[tHDF5Attribute] Failed to open file " << filename << endl;
    return -1;
  }
    
  //________________________________________________________
  // Release HDF5 objects
  
  if (H5Iis_valid(fileID)) {
    H5Fclose(fileID);
  }
  
  return nofFailedTests;
}
