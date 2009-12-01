/*-------------------------------------------------------------------------*
 | $Id:: tUseMeasures.cc 511 2007-08-05 13:14:48Z baehren                $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007                                                    *
 *   Lars Baehren (bahren@astron.nl)                                       *
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

#include <iostream>
#include <string>

// --- required for test_arrays
#include <casa/Arrays/Array.h>
#include <casa/Arrays/Cube.h>
#include <casa/Arrays/IPosition.h>
#include <casa/Arrays/Matrix.h>
#include <casa/Arrays/Vector.h>
// --- required for test_tables
#include <tables/Tables/TableDesc.h>
#include <tables/Tables/SetupNewTab.h>
#include <tables/Tables/Table.h>
#include <tables/Tables/TableLock.h>
#include <tables/Tables/ScaColDesc.h>
#include <tables/Tables/ArrColDesc.h>
#include <tables/Tables/ScalarColumn.h>
#include <tables/Tables/ArrayColumn.h>
#include <tables/Tables/StManAipsIO.h>
#include <tables/Tables/ExprNode.h>
#include <tables/Tables/ExprNodeSet.h>

/*!
  \file tMeasurementSets.cc

  \ingroup DAL

  \brief Test building against the "ms" modules of casacore

  \author Lars B&auml;hren

  \date 2007/08/13

  This test program contains a number of routines which make use of various
  modules and classes of the casacore libraries; rather than providing a full
  test and demonstration of all the available functionality, this program collects
  a number of very simple test such creating some basic objects and using their
  APIs.
*/

using std::cout;
using std::endl;

// casa/Arrays
using casa::Array;
using casa::DComplex;
using casa::Cube;
using casa::IPosition;
using casa::Matrix;
using casa::Slice;
using casa::Slicer;
using casa::uInt;
using casa::Vector;
// tables/Tables
using casa::ColumnDesc;
using casa::TableDesc;
using casa::ScalarColumnDesc;
using casa::ArrayColumnDesc;

// -----------------------------------------------------------------------------

/*!
	\brief Test working with the CASA Array classes

	[1] Construction of arrays, as well as addressing individual elements or
	sub-array is done via an IPosition - a vector of integer numbers describing
	the shape of the array.

	[2] For low-dimensional array there exist specialized classes: Vector (1D),
	Matrix (2D) and Cube (3D). As these classes are derived from Array, we can
	create e.g. a Matrix from an Array and vice versa (the latter of course
	requires the array to be of correct dimensionality).

	[3] Slicing - i.e. the addressing of a sub-region of an array - is done
	using a set of IPosition objects. The sub-region typically is defined by
	its lower left corner, its upper right corner and the strides parallel to
	each axis.

	\return nofFailedTests -- Number of failed tests within this function
*/
int test_Arrays ()
{
  cout << "\n[test_Arrays]\n" << endl;

  int nofFailedTests (0);
  int nelem (10);

  cout << "[1] Construct Array via IPosition shape ..." << endl;
  try
    {
      Array<double> arr1D (IPosition(1,nelem));
      cout << " --> Array<double> = " << arr1D.shape() << endl;
      Array<double> arr2D (IPosition(2,nelem));
      cout << " --> Array<double> = " << arr2D.shape() << endl;
      Array<double> arr3D (IPosition(3,nelem));
      cout << " --> Array<double> = " << arr3D.shape() << endl;
      Array<double> arr4D (IPosition(4,nelem));
      cout << " --> Array<double> = " << arr4D.shape() << endl;
      Array<double> arr5D (IPosition(5,nelem));
      cout << " --> Array<double> = " << arr5D.shape() << endl;
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[2] Construct Array from specialized objects ..." << endl;
  try
    {
      Vector<double> vector (nelem);
      Matrix<double> matrix (nelem,nelem);
      Cube<double> cube (IPosition(3,nelem));
      cout << " -- Creating Array from Vector ..." << endl;
      Array<double> arr1D (vector);
      cout << " -- Creating Array from Matrix ..." << endl;
      Array<double> arr2D (matrix);
      cout << " -- Creating Array from Cube ..." << endl;
      Array<double> arr3D (cube);
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  cout << "[3] Construct Array from slicing another one ..." << endl;
  try
    {
      // the original array
      Array<double> arr5D (IPosition(5,nelem));
      // the slicer
      Slicer slice (IPosition(5,0),
                    IPosition(5,1,1,nelem,nelem,nelem),
                    IPosition(5,1),
                    Slicer::endIsLength);
      // the new array obtained from slicing through the original
      Array<double> slice5D(arr5D(slice));
      Array<double> slice3D(arr5D(slice).nonDegenerate(0));
      // Summary of the array properties
      cout << " -- original array                     = " << arr5D.shape()   << endl;
      cout << " -- slicing result w/o axis removal    = " << slice5D.shape() << endl;
      cout << " -- slicing result w/o degenerate axes = " << slice3D.shape() << endl;
    }
  catch (std::string message)
    {
      std::cerr << message << endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
	\brief Test working with the classes in the CASA "tables" module

	\return nofFaildTests -- The number of failed tests within this function
 */
int test_tables ()
{
  cout << "\n[test_tables]\n" << endl;

  int nofFailedTests (0);

  std::cout << "[1] Build the table description..." << std::endl;
  try
    {
      TableDesc td("", "1", casa::TableDesc::Scratch);
      td.comment() = "A test of class Table";
      td.addColumn (ScalarColumnDesc<int>("ab","Comment for column ab"));
      td.addColumn (ScalarColumnDesc<uInt>("ad","comment for ad"));
      td.addColumn (ScalarColumnDesc<DComplex>("ag"));
      td.addColumn (ArrayColumnDesc<float>("arr1",IPosition(3,2,3,4),
                                           ColumnDesc::Direct));
      td.addColumn (ArrayColumnDesc<float>("arr2",0));
      td.addColumn (ArrayColumnDesc<float>("arr3",0,ColumnDesc::Direct));
    }
  catch (std::string message)
    {
      std::cerr << message << std::endl;
      nofFailedTests++;
    }

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
	\brief Test working with the classes in the CASA "ms" module

	\return nofFaildTests -- The number of failed tests within this function
 */
int test_ms ()
{
  cout << "\n[test_ms]\n" << endl;

  int nofFailedTests (0);

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

/*!
  \brief Test working with the classes in the CASA "measures" module

  \return nofFaildTests -- The number of failed tests within this function
 */
int test_Measures ()
{
  cout << "\n[test_Measures]\n" << endl;

  int nofFailedTests (0);

  return nofFailedTests;
}


/*!
  \brief Main routine

  \return nofFailedTests -- The number of failed tests
*/
int main ()
{
  int nofFailedTests (0);

  nofFailedTests += test_Arrays();
  nofFailedTests += test_Measures();

  return nofFailedTests;
}
