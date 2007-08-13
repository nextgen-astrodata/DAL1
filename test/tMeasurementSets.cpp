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
#include <cmath>
#include <limits>

#include <casa/Arrays/Array.h>
#include <casa/Arrays/IPosition.h>
#include <casa/Arrays/Matrix.h>
#include <casa/Arrays/Vector.h>

/*!
  \file tUseMeasurementSets.cpp
  
  \ingroup DAL

  \brief Test building against te "ms" modules of casacore

  \author Lars B&auml;hren

  \date 2007/08/13
*/

using std::cout;
using std::endl;

using casa::Array;
using casa::IPosition;

// -----------------------------------------------------------------------------

int test_Arrays ()
{
	cout << "\n[test_Arrays]\n" << endl;
	
	int nofFailedTests (0);
	int nelem (10);
	
	cout << "[1] Construct Array via IPosition shape ..." << endl;
	try {
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
	} catch (std::string message) {
		std::cerr << message << endl;
		nofFailedTests++;
	}
	
	return nofFailedTests;
}

// -----------------------------------------------------------------------------

int test_Measures ()
{
	cout << "\n[test_Measures]\n" << endl;
	
	int nofFailedTests (0);
	
	return nofFailedTests;
}


/*!
  \brief main routine
  
  \return nofFailedTests -- The number of failed tests
*/
int main () 
{
	int nofFailedTests (0);
	
	nofFailedTests += test_Arrays();
	nofFailedTests += test_Measures();
	
	return nofFailedTests;
}
