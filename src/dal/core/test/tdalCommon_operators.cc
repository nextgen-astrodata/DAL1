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

#include <complex>
#include <core/dalCommon.h>

using std::cout;
using std::cerr;
using std::endl;

// -----------------------------------------------------------------------------

/*!
  \file tdalCommon_operators.cc

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
  unsigned int nelem (10);

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
    for (unsigned int n(0); n<nelem; n++) {
      arr_bool[n]    = true;
      arr_int[n]     = int(n);
      arr_float[n]   = float(n);
      arr_double[n]  = double(n);
      arr_complex[n] = std::complex<double>(n);
    }
    //
    std::cout << "-- bool*    = " << DAL::toString (arr_bool,nelem)    << endl;
    std::cout << "-- int*     = " << DAL::toString (arr_int,nelem)     << endl;
    std::cout << "-- float*   = " << DAL::toString (arr_float,nelem)   << endl;
    std::cout << "-- double*  = " << DAL::toString (arr_double,nelem)  << endl;
    std::cout << "-- complex* = " << DAL::toString (arr_complex,nelem) << endl;
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
//                                                                           main

/*!
  \brief Main routine of the test program

  \return nofFailedTests -- The number of failed tests encountered within and
          identified by this test program.
*/
int main (int argc, char *argv[])
{
  int nofFailedTests   = 0;
  std::string filename = "tdalCommon.h5";

  if (argc>1) {
    filename = argv[1];
  }

  nofFailedTests += test_operators ();

  return nofFailedTests;
}
