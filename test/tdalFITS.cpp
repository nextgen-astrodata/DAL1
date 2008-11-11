/*-------------------------------------------------------------------------*
 | $Id:: tHDF5Common.cpp 2022 2008-09-26 12:06:09Z baehren               $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Sven Duscha                                                           *
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

#include <dalFITS.h>

using std::cout;
using std::cerr;
using std::endl;

// -----------------------------------------------------------------------------

/*!
  \file tdalFITS.cpp

  \ingroup DAL

  \brief A collection of test routines for the routines contained in dalFITS
 
  \author Sven Duscha
 
  \date 2008/11/11
*/

// -----------------------------------------------------------------------------

int test_constructors ()
{
  int nofFailedTests (0);

  return nofFailedTests;
}

// -----------------------------------------------------------------------------

int main (int argc,
	  char *argv[])
{
  int nofFailedTests (0);
  std::string filename;

  //__________________________________________________________________
  // Check the input parameters provided form the command line
  
  if (argc < 2) {
    std::cerr << "[tdalFITS] Missing name of test input file!" << std::endl;
    std::cerr << "" << std::endl;
    std::cerr << "  tdalFITS <FITS file>" << std::endl;
    std::cerr << "" << std::endl;
    return 1;
  } else {
    filename = argv[1];
  }
  
  //__________________________________________________________________
  // Run the tests
  
  nofFailedTests += test_constructors ();
  
  return nofFailedTests;
}
