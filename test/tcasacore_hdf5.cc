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

/* Standard header files */
#include <iostream>
#include <string>

/* casacore header files */
#include <casa/Arrays.h>
#include <casa/HDF5.h>

using std::cout;
using std::cerr;
using std::endl;

/*!
  \file tcasacore_hdf5.cc

  \ingroup DAL

  \brief A collection of tests to work with the casacore HDF5 module

  \author Lars B&auml;hren

  \date 2009/05/30

  <h3>Synopsis</h3>

  The casacore libraries contain a module to interface the with the HDF5 library,
  providing the necessary machinery to map some of its main storage containers --
  such as arrays, records and tables -- onto the corresponding structures in the
  HDF5 context.
*/

//_______________________________________________________________________________
//                                                                  test_HDF5File

/*!
  \brief Test working with the casa::HDF5File class

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_HDF5File ()
{
  cout << "\n[tcasacore_hdf5::test_HDF5File]\n" << endl;

  int nofFailedTests (0);

  cout << "[1] Creating new HDF5 file ..." << endl;
  try
    {
      // create new file
      casa::HDF5File f ("testfile1.h5", casa::ByteIO::New);
      // show file properties
      cout << "-- File is writable          = " << f.isWritable()        << endl;
      cout << "-- File is opened for delete = " << f.isOpenedForDelete() << endl;
      cout << "-- File temporarily closed   = " << f.isClosed()          << endl;
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[2] Reopen previously created HDF5 file as RO ..." << endl;
  try
    {
      // open the file
      casa::HDF5File f ("testfile1.h5", casa::ByteIO::Old);
      // show file properties
      cout << "-- File is writable          = " << f.isWritable()        << endl;
      cout << "-- File is opened for delete = " << f.isOpenedForDelete() << endl;
      cout << "-- File temporarily closed   = " << f.isClosed()          << endl;
    }
  catch (std::string message)
    {
      cerr << message << endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

//_______________________________________________________________________________
//

int test_HDF5Record ()
{
  int nofFailedTests (0);

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc,
          char *argv[])
{
  int nofFailedTests (0);

  nofFailedTests += test_HDF5File ();

  return nofFailedTests;
}
