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

  \ingroup DAL
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
    DAL::StokesCoordinate coord;
    //
    coord.summary(); 
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Testing StokesCoordinate (Stokes::Component) ..." << endl;
  try {
    DAL::StokesCoordinate stokesI (DAL::Stokes::I);
    stokesI.summary();
    //
    DAL::StokesCoordinate stokesQ (DAL::Stokes::Q);
    stokesQ.summary();
    //
    DAL::StokesCoordinate stokesU (DAL::Stokes::U);
    stokesU.summary();
    //
    DAL::StokesCoordinate stokesV (DAL::Stokes::V);
    stokesV.summary();
    //
    DAL::StokesCoordinate stokesR (DAL::Stokes::R);
    stokesR.summary();
    //
    DAL::StokesCoordinate stokesL (DAL::Stokes::L);
    stokesL.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[2] Testing StokesCoordinate (Stokes) ..." << endl;
  try {
    DAL::Stokes stokesI (DAL::Stokes::I);
    DAL::Stokes stokesQ (DAL::Stokes::Q);
    DAL::Stokes stokesR (DAL::Stokes::R);
    DAL::Stokes stokesL (DAL::Stokes::L);
    DAL::Stokes stokesX (DAL::Stokes::X);
    DAL::Stokes stokesY (DAL::Stokes::Y);
    //
    DAL::StokesCoordinate coordI (stokesI);
    coordI.summary();
    //
    DAL::StokesCoordinate coordQ (stokesQ);
    coordQ.summary();
    //
    DAL::StokesCoordinate coordR (stokesR);
    coordR.summary();
    //
    DAL::StokesCoordinate coordL (stokesL);
    coordL.summary();
    //
    DAL::StokesCoordinate coordX (stokesX);
    coordX.summary();
    //
    DAL::StokesCoordinate coordY (stokesY);
    coordY.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  cout << "[4] Testing StokesCoordinate(std::vector<DAL::Stokes::Component>) ..." << endl;
  try {
    unsigned int nelem (4);
    std::vector<DAL::Stokes::Component> stokes (nelem);
    //
    stokes[0] = DAL::Stokes::I;
    stokes[1] = DAL::Stokes::Q;
    stokes[2] = DAL::Stokes::U;
    stokes[3] = DAL::Stokes::V;
    //
    DAL::StokesCoordinate coord (stokes);
    coord.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[5] Testing StokesCoordinate(std::vector<DAL::Stokes>) ..." << endl;
  try {
    unsigned int nelem (4);
    std::vector<DAL::Stokes> stokes (nelem);
    //
    stokes[0] = DAL::Stokes(DAL::Stokes::X);
    stokes[1] = DAL::Stokes(DAL::Stokes::Y);
    stokes[2] = DAL::Stokes(DAL::Stokes::R);
    stokes[3] = DAL::Stokes(DAL::Stokes::L);
    //
    DAL::StokesCoordinate coord (stokes);
    coord.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  cout << "[6] Testing StokesCoordinate(StokesCoordinate) ..." << endl;
  try {
    unsigned int nelem (8);
    std::vector<DAL::Stokes> stokes (nelem);
    //
    stokes[0] = DAL::Stokes(DAL::Stokes::I);
    stokes[1] = DAL::Stokes(DAL::Stokes::Q);
    stokes[2] = DAL::Stokes(DAL::Stokes::U);
    stokes[3] = DAL::Stokes(DAL::Stokes::V);
    stokes[4] = DAL::Stokes(DAL::Stokes::X);
    stokes[5] = DAL::Stokes(DAL::Stokes::Y);
    stokes[6] = DAL::Stokes(DAL::Stokes::R);
    stokes[7] = DAL::Stokes(DAL::Stokes::L);
    //
    DAL::StokesCoordinate coord (stokes);
    coord.summary();
    //
    DAL::StokesCoordinate coordCopy (coord);
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
