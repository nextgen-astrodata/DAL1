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

  \brief A collection of test routines for the DAL::BeamGroup class
  \author Lars B&auml;hren

  \date 2008/09/19

  This test program mainly is going through the functionality of the
  DAL::BeamGroup class, but also - at least in an indirect way - some of
  DAL::BeamSubband.
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

  \param filename  -- Name of the input data file
  \param groupName -- Name of the beam group to open and work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors (std::string const &filename,
		       std::string const &groupName)
{
  std::cout << "\n[tBeamGroup::test_constructors]\n" << std::endl;

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
    DAL::dalDataset dataset;
    dataset.open(filename.c_str());
    //
    DAL::BeamGroup group (dataset,groupName);
    //
    group.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Test extraction of the various attributes

  \param filename  -- Name of the input data file
  \param groupName -- Name of the beam group to open and work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_attributes (std::string const &filename,
		     std::string const &groupName)
{
  std::cout << "\n[tBeamGroup::test_attributes]\n" << std::endl;

  int nofFailedTests (0);
  
  DAL::dalDataset dataset;
  dataset.open(filename.c_str());
  DAL::BeamGroup group (dataset,groupName);

  std::cout << "[1] Attributes attached to the beam group ..." << std::endl;
  try {
    std::cout << "-- fileID()      = " << group.fileID()      << std::endl;
    std::cout << "-- groupID()     = " << group.groupID()     << std::endl;
    std::cout << "-- groupName()   = " << group.groupName()   << std::endl;
    std::cout << "-- ra()          = " << group.ra()          << std::endl;
    std::cout << "-- dec()         = " << group.dec()         << std::endl;
    std::cout << "-- nofSubbands() = " << group.nofSubbands() << std::endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Attributes attached to the sub-band tables ..." << std::endl;
  try {
    /*
     *  Center frequencies [MHz] of the sub-bands
     */
    std::vector<int> freq = group.center_frequencies();
    std::cout << "-- Center frequencies = [";
    for (uint n(0); n<freq.size(); n++) {
      std::cout << " " << freq[n];
    }
    std::cout << " ]" << std::endl;
    /*
     * Number of rows in the sub-band tables
     */
    std::vector<long> rows = group.nofTableRows();
    std::cout << "-- nof. table rows    = [";
    for (uint n(0); n<rows.size(); n++) {
      std::cout << " " << rows[n];
    }
    std::cout << " ]" << std::endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Test the various methods for operation on the data

  \param filename  -- Name of the input data file
  \param groupName -- Name of the beam group to open and work with

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_methods (std::string const &filename,
		  std::string const &groupName)
{
  std::cout << "\n[tBeamGroup::test_methods]\n" << std::endl;

  int nofFailedTests (0);
  
  DAL::dalDataset dataset;
  dataset.open(filename.c_str());
  DAL::BeamGroup group (dataset,groupName);
  int nofSubbands (group.nofSubbands());
  std::vector<long> rows = group.nofTableRows();

  std::cout << "[1] Extract Subband objects from beam-group..." << std::endl;
  try {
    if (nofSubbands > 0) {
      DAL::BeamSubband * subband;
      for (int n(0); n<nofSubbands; n++) {
	/* Extract the n-th subband from the beam-group */
	subband = group.getSubband (n);
      }
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  std::cout << "[2] Retrieve data from the subbands ..." << std::endl;
  try {
    int length (20);
    /* Arrays for taking up the returned data */
    std::complex<short> * dataX;
    dataX = new std::complex<short> [length];
    /* Go through the subbands and retrieve the data */
    for (int n(0); n<nofSubbands; n++) {
      dataX = group.getSubbandData_X (n,1,length);
      /* Print some values */
      std::cout << "[ " << dataX[0] << " " << dataX[1] << " " << dataX[2] << " ... ]" << std::endl;
    }
    /* Release allocated memory */
    delete [] dataX;
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  return nofFailedTests;
}

// ------------------------------------------------------------------------------

int main (int argc,char *argv[]) 
{
  int nofFailedTests (0);
  std::string filename;
  std::string groupName ("beam000");

  /* Check command line parameters */

  if (argc > 1) {
    filename = std::string(argv[1]);
  } else {
    std::cout << "[tBeamGroup] Missing name of input test file." << std::endl;
    return(DAL::FAIL);
  }

  /* Test the constructors */
  nofFailedTests += test_constructors(filename,groupName);

  if (nofFailedTests == 0) {
    nofFailedTests += test_attributes(filename,groupName);
    nofFailedTests += test_methods(filename,groupName);
  }

  return nofFailedTests;
}
