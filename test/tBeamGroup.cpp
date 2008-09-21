/***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
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

/*!
  \file tBeamGroup.cpp

  \ingroup DAL

  \brief Test program for beam-formed functionality.

  \author Lars B&auml;hren

  \date 2008/09/19
*/

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#ifndef BEAMFORMED_H
#include <BeamFormed.h>
#endif

#ifndef BEAMGROUP_H
#include <BeamGroup.h>
#endif

// ------------------------------------------------------------------------------

/*!
  \brief Test the various constructors for an object of type BeamGroup

  \param filename -- Name of the input data file

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors (std::string const &filename)
{
  int nofFailedTests (0);

  std::cout << "[1] Default constructor..." << std::endl;
  try {
    DAL::BeamGroup group;
    //
    group.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  std::cout << "[2] Argumented constructor..." << std::endl;
  try {
    DAL::dalDataset dataset (filename.c_str(),"HDF5");
    std::string groupName ("beam000");
    //
    DAL::BeamGroup group (dataset,groupName);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests;
  }

  return nofFailedTests;
}

// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------

int main (int argc,char *argv[]) 
{
  int nofFailedTests (0);
  std::string filename;

  /* Check command line parameters */

  if (argc > 1) {
    filename = std::string(argv[1]);
  } else {
    std::cout << "[tBeamGroup] Missing name of input test file." << std::endl;
    return(DAL::FAIL);
  }

  /* Run the tests */

  nofFailedTests += test_constructors(filename);

  return nofFailedTests;
}
