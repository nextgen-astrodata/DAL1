/***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#include <coordinates/StokesCoordinate.h>

using std::cout;
using std::endl;

/*!
  \file tStokesCoordinate.cc

  \ingroup DAL1
  \ingroup coordinates

  \brief A collection of test routines for the StokesCoordinate class
 
  \author Lars B&auml;hen
 
  \date 2009/09/13
*/

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test constructors for a new StokesCoordinate object

  \return nofFailedTests -- The number of failed tests encountered within this
          function.
*/
int test_constructors ()
{
  cout << "\n[tStokesCoordinate::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing StokesCoordinate() ..." << endl;
  try {
    DAL1::StokesCoordinate coord;
    //
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Testing StokesCoordinate (Stokes::Component) ..." << endl;
  try {
    DAL1::StokesCoordinate stokesI (DAL1::Stokes::I);
    stokesI.summary();
    //
    DAL1::StokesCoordinate stokesQ (DAL1::Stokes::Q);
    stokesQ.summary();
    //
    DAL1::StokesCoordinate stokesU (DAL1::Stokes::U);
    stokesU.summary();
    //
    DAL1::StokesCoordinate stokesV (DAL1::Stokes::V);
    stokesV.summary();
    //
    DAL1::StokesCoordinate stokesR (DAL1::Stokes::R);
    stokesR.summary();
    //
    DAL1::StokesCoordinate stokesL (DAL1::Stokes::L);
    stokesL.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Testing StokesCoordinate (Stokes) ..." << endl;
  try {
    DAL1::Stokes stokesI (DAL1::Stokes::I);
    DAL1::Stokes stokesQ (DAL1::Stokes::Q);
    DAL1::Stokes stokesR (DAL1::Stokes::R);
    DAL1::Stokes stokesL (DAL1::Stokes::L);
    DAL1::Stokes stokesX (DAL1::Stokes::X);
    DAL1::Stokes stokesY (DAL1::Stokes::Y);
    //
    DAL1::StokesCoordinate coordI (stokesI);
    coordI.summary();
    //
    DAL1::StokesCoordinate coordQ (stokesQ);
    coordQ.summary();
    //
    DAL1::StokesCoordinate coordR (stokesR);
    coordR.summary();
    //
    DAL1::StokesCoordinate coordL (stokesL);
    coordL.summary();
    //
    DAL1::StokesCoordinate coordX (stokesX);
    coordX.summary();
    //
    DAL1::StokesCoordinate coordY (stokesY);
    coordY.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[4] Testing StokesCoordinate(std::vector<DAL1::Stokes::Component>) ..." << endl;
  try {
    unsigned int nelem (4);
    std::vector<DAL1::Stokes::Component> stokes (nelem);
    //
    stokes[0] = DAL1::Stokes::I;
    stokes[1] = DAL1::Stokes::Q;
    stokes[2] = DAL1::Stokes::U;
    stokes[3] = DAL1::Stokes::V;
    //
    DAL1::StokesCoordinate coord (stokes);
    coord.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[5] Testing StokesCoordinate(std::vector<DAL1::Stokes>) ..." << endl;
  try {
    unsigned int nelem (4);
    std::vector<DAL1::Stokes> stokes (nelem);
    //
    stokes[0] = DAL1::Stokes(DAL1::Stokes::X);
    stokes[1] = DAL1::Stokes(DAL1::Stokes::Y);
    stokes[2] = DAL1::Stokes(DAL1::Stokes::R);
    stokes[3] = DAL1::Stokes(DAL1::Stokes::L);
    //
    DAL1::StokesCoordinate coord (stokes);
    coord.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[6] Testing StokesCoordinate(StokesCoordinate) ..." << endl;
  try {
    unsigned int nelem (8);
    std::vector<DAL1::Stokes> stokes (nelem);
    //
    stokes[0] = DAL1::Stokes(DAL1::Stokes::I);
    stokes[1] = DAL1::Stokes(DAL1::Stokes::Q);
    stokes[2] = DAL1::Stokes(DAL1::Stokes::U);
    stokes[3] = DAL1::Stokes(DAL1::Stokes::V);
    stokes[4] = DAL1::Stokes(DAL1::Stokes::X);
    stokes[5] = DAL1::Stokes(DAL1::Stokes::Y);
    stokes[6] = DAL1::Stokes(DAL1::Stokes::R);
    stokes[7] = DAL1::Stokes(DAL1::Stokes::L);
    //
    DAL1::StokesCoordinate coord (stokes);
    coord.summary();
    //
    DAL1::StokesCoordinate coordCopy (coord);
    coordCopy.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main ()
{
  int nofFailedTests (0);

  // Test for the constructor(s)
  nofFailedTests += test_constructors ();

  return nofFailedTests;
}
