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
  \file tBeamFormed.cpp

  \ingroup DAL

  \brief Test program for beam-formed functionality.

  \author Joseph Masters

  \date 12-04-07
*/

#ifndef BEAMFORMED_H
#include <BeamFormed.h>
#endif

using std::endl;
using std::vector;

// ------------------------------------------------------------ test_constructors

/*!
  \brief Test the various constructors for an object of type BeamFormed

  \param filename -- Name of the input data file

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors (std::string const &filename)
{
  std::cout << "[tBeamFormed::test_constructors]" << endl;

  int nofFailedTests (0);

  std::cout << "[1] Test default constructor ..." << endl;
  try {
    DAL::BeamFormed bf;
    //
    bf.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[2] Test argumented constructor ..." << endl;
  try {
    DAL::BeamFormed bf (filename);
    //
    bf.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// -------------------------------------------------------------- test_attributes

/*!
  \brief Test the various methods for the retrieval of the attributes
  
  \param filename -- Name of the input data file
  
  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_attributes (std::string const &filename)
{
  std::cout << "\n[tBeamFormed::test_attributes]\n" << endl;
  
  int nofFailedTests (0);
  DAL::BeamFormed bf (filename);
  int nofBeams = bf.number_of_beams();

  std::cout << "[1] Attributes of the root group..." << endl;
  try {
    std::cout << "-- Filename       = " << bf.filename()        << endl;
    std::cout << "-- Telescope      = " << bf.telescope()       << endl;
    std::cout << "-- nof. stations  = " << bf.nofStations()     << endl;
    std::cout << "-- Datatype       = " << bf.datatype()        << endl;
    std::cout << "-- EM band        = " << bf.emband()          << endl;
    std::cout << "-- Observation ID = " << bf.observation_id()  << endl;
    std::cout << "-- Project ID     = " << bf.proj_id()         << endl;
    std::cout << "-- nof. beams     = " << bf.number_of_beams() << endl;
    std::cout << "-- Epoch (MJD)    = " << bf.epoch_mjd()       << endl;
    std::cout << "-- Epoch (Date)   = " << bf.epoch_date()      << endl;
    std::cout << "-- Epoch (UTC)    = " << bf.epoch_utc()       << endl;
    std::cout << "-- Epoch (LST)    = " << bf.epoch_lst()       << endl;
    std::cout << "-- Sampling time  = " << bf.sampling_time()   << endl;
    //
    std::vector<std::string> sources = bf.sources();
    std::cout << "-- Sources        = [";
    for (unsigned int idx=0; idx<sources.size(); idx++) {
      std::cout << " " << sources[ idx ];
    }
    std::cout << " ]" << endl;
    
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  if (nofBeams > 0) {
    std::cout << "[2] Attributes of the beam groups..." << endl;
    DAL::BeamGroup *group;
    for (int beam(0); beam<nofBeams; beam++) {
      try {
	/* Retrieve the beam group */
	group = bf.getBeam(beam);
	/* Display the attribute of this group */
	std::cout << "-- Group number    = " << beam                 << endl;
	std::cout << "-- HDF5 file ID    = " << group->fileID()      << endl;
	std::cout << "-- HDF5 group ID   = " << group->groupID()     << endl;
	std::cout << "-- HDF5 group name = " << group->groupName()   << endl;
	std::cout << "-- RA of the beam  = " << group->ra()          << endl;
	std::cout << "-- Dec of the beam = " << group->dec()         << endl;
	std::cout << "-- nof. subbands   = " << group->nofSubbands() << endl;
      } catch (std::string message) {
	std::cerr << message << endl;
	nofFailedTests++;
      }
    }
  }

  return nofFailedTests;
}

// ---------------------------------------------------------------- test_getData

/*!
  \brief Test retrieval of the actual subband data

  \param filename -- Name of the input data file

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_getData (std::string const &filename)
{
  std::cout << "\n[tBeamFormed::test_getData]\n" << endl;

  int nofFailedTests (0);
  DAL::BeamFormed * file = NULL;
  DAL::BeamGroup * beam  = NULL;
  int subband            = 0;
  int start              = 0;
  int length             = 20; //441344;
  
  std::cout << "[1] Extract BeamGroup ..." << endl;
  try {
    file = new DAL::BeamFormed(filename.c_str());
    beam = file->getBeam(0);
    //
    beam->summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[2] Read in data from X-component of subband..." << endl;
  try {
    std::vector< std::complex<short> > xx;
    xx.clear();
    
    beam->getSubbandData_X( subband, start, length, xx );

    std::cout << "-- First " << length << " xx values for subband "
	      << subband << " = [";
    
    for (int ii=0; ii < length; ii++ ) {
      std::cout << " (" << xx[ii].real() << "," << xx[ii].imag() << ")";
      }
    std::cout << " ]" << endl;
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }

  std::cout << "[3] Read in data from (X,Y) components of subband..." << endl;
  try {
    std::vector< std::complex<short> > xvals;
    std::vector< std::complex<short> > yvals;
    xvals.clear();
    yvals.clear();
    
    // step through the data for subband 0, 10 samples at a time, 10 times
    for (unsigned int count=0; count < 10; count++ )
      {
	beam->getSubbandData_XY( subband, start, length, xvals, yvals );
	
	printf( "Values %d through %d\n", start, start + length );
	for (unsigned int ii=0; ii < xvals.size(); ii++ )
	  {
	    printf( "(%d,%d),(%d,%d)\n" , xvals[ii].real(),
		    xvals[ii].imag(),
		    yvals[ii].real(),
		    yvals[ii].imag() );
	  }
	printf("\n");
	xvals.clear();
	yvals.clear();
	start += length;
      }
  } catch (std::string message) {
    std::cerr << message << endl;
    nofFailedTests++;
  }
  
  return nofFailedTests;
}

// ------------------------------------------------------------------------- main

int main(int argc,char *argv[])
{
  int nofFailedTests (0);
  
  std::string filename;
  
  if (argc > 1) {
    filename = std::string(argv[1]);
  } else {
    std::cerr << "Please provide a HDF5 filename.\n";
    return(DAL::FAIL);
  }

  /* Test for the constructors */
  nofFailedTests += test_constructors(filename);

  if (nofFailedTests == 0) {
    nofFailedTests += test_attributes (filename);
    nofFailedTests += test_getData (filename);
  }

  return nofFailedTests;
}
