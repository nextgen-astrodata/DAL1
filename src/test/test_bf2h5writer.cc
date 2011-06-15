//# test_bf2h5writer: Test DAL HDF5 routines
//#
//#  Copyright (C) 2011
//#  ASTRON (Netherlands Foundation for Research in Astronomy)
//#  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
//#
//#  This program is free software; you can redistribute it and/or modify
//#  it under the terms of the GNU General Public License as published by
//#  the Free Software Foundation; either version 2 of the License, or
//#  (at your option) any later version.
//#
//#  This program is distributed in the hope that it will be useful,
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#  GNU General Public License for more details.
//#
//#  You should have received a copy of the GNU General Public License
//#  along with this program; if not, write to the Free Software
//#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*!
  \file test_bf2h5writer.cc

  \ingroup DAL
  \ingroup data_hl

  \brief Integrated test for the creation of BF data sets.
 
  \author Jan David Mol
  \author Lars B&auml;hren

  <h3>Prerequisite</h3>
  
  <ul>
    <li>DAL::BF_RootGroup -- High-level interface to the root-group of
        Beam-Formed data.
    <li>DAL::BF_SubArrayPointing -- High-level interface to the Sub-Array
        pointing group of Beam-Formed data.
    <li>DAL::BF_BeamGroup -- High-level interface to the Beam group of
        Beam-Formed Data.
    <li>DAL::BF_StokesDataset -- High-level interface to the Stokes dataset
        of Beam-Formed Data.
  </ul>
*/

#ifdef DAL_WITH_LOFAR
#include <lofar_config.h>
#endif

#define SAMPLES         1008
#define CHANNELS        64
#define SUBBANDS        60

#include <data_hl/BF_RootGroup.h>
#include <data_hl/BF_StokesDataset.h>

#include <iostream>
#include <boost/format.hpp>
#include <boost/multi_array.hpp>

using namespace std;
using namespace boost;
using boost::format;

//_______________________________________________________________________________
//                                                            setCommonAttributes

DAL::CommonAttributes commonAttributes (DAL::Filename const &filename)
{
  DAL::CommonAttributes attributes (filename);

  attributes.setTelescope      ("LOFAR");
  attributes.setObserver       ("Pulsar observer");
  attributes.setProjectID      ("Pulsar");
  attributes.setProjectTitle   ("Pulsar survey");
  attributes.setProjectPI      ("Mr. Pulsar");
  attributes.setProjectCoI     ("Mrs. Pulsar");
  attributes.setProjectContact ("pulsar@lofar.org");

  return attributes;
}

//_______________________________________________________________________________
//                                                                           main

int main()
{
  unsigned int nofPointings = 5;
  unsigned int nofBeams     = 10;
  unsigned int subArrayID   = 0;
  unsigned int beamID       = 0;
  
  /*__________________________________________________________________
    Create DAL::Filename object for generation of proper filename,
    matching the rules as  defined in ICD-005.
  */
  
  std::string observationID ("1234567890");
  DAL::Filename filename (observationID,
			  "test",
			  DAL::Filename::bf,
			  DAL::Filename::h5);
  
  /*__________________________________________________________________
    Set up the LOFAR Common Attributes, which will be attached to the
    root group of the newly created file.
  */

  std::cout << "\n[1] Setting up LOFAR Common Attributes ..." << std::endl;

  DAL::CommonAttributes attributes = commonAttributes (filename);

  /*__________________________________________________________________
    Create new BF file; we are using "IO_Mode::Truncate" in order to
    always start with a freshly created file - otherwise previously 
    created structure elements will remain in place.
  */
  
  std::cout << "\n[2] Creating new file " << filename.filename() << endl;

  DAL::BF_RootGroup bfRoot (attributes,
			    DAL::IO_Mode(DAL::IO_Mode::Create));
  
  /*__________________________________________________________________
    Create primary array pointing groups with embedded beam groups.
  */
  
  std::cout << "\n[3] Creating sub-array groups and beam groups ..." << endl;

  for (unsigned int numPointing=0; numPointing<nofPointings; ++numPointing) {
    for (unsigned int numBeam=0; numBeam<nofBeams; ++numBeam) {
      // status message
      std::cout << "-- Creating Sub-Array / Beam [ " 
		<< numPointing << " , " << numBeam << " ]"
		<< std::endl;
      // recursively open beam group
      bfRoot.openBeam ( numPointing, numBeam );
    }
  }
  
  /*__________________________________________________________________
    Create Stokes datasets
  */
  
  std::cout << "\n[4] Creating Stokes datasets ..." << endl;

  bfRoot.openStokesDataset (subArrayID,        // ID of sub-array pointing
			    beamID,            // ID of beam group
			    0,                 // ID of Stokes dataset
			    SAMPLES,
			    SUBBANDS,
			    CHANNELS,
			    DAL::Stokes::I);
  bfRoot.openStokesDataset (subArrayID,        // ID of sub-array pointing
			    beamID,            // ID of beam group
			    1,                 // ID of Stokes dataset
			    SAMPLES,
			    SUBBANDS,
			    CHANNELS,
			    DAL::Stokes::Q);
  
  /*__________________________________________________________________
    Inspection of the previously created hierarchical structure.
  */

  std::cout << "\n[5] Inspection of hierarchical structure ..." << std::endl;

  {
    DAL::BF_SubArrayPointing bfSubArray = bfRoot.getSubArrayPointing (0);
    bfSubArray.summary();
  }

  /*__________________________________________________________________
    Write data to Stokes dataset
  */

  std::cout << "\n[6] Write data to Stokes dataset ..." << std::endl;

  {
    unsigned int stokesID   = 0;
    hid_t fileID            = bfRoot.locationID();
    std::string name = DAL::BF_SubArrayPointing::getName(subArrayID)
      + "/" + DAL::BF_BeamGroup::getName(beamID) 
      + "/" + DAL::BF_StokesDataset::getName(stokesID);

    std::cout << "-- Opening Stokes dataset " << name << std::endl;
    
    DAL::BF_StokesDataset stokesDataset(fileID,
					name);
    stokesDataset.summary();
    
    cout << "-- Creating sample multiarray of [ 1" 
	 << " x " << SUBBANDS
	 << " x " << CHANNELS << " ]" << endl;
    
    boost::multi_array<float,3> samples(extents[1][SUBBANDS][CHANNELS]);
    
    std::vector<int> start (2,0);
    std::vector<int> count;
    std::vector<int> block (2,0);
    unsigned int nofDatapoints (0);

    /* Iterate through the various time steps */
    for (unsigned int t=0; t<SAMPLES/4; ++t) {

      /* Update hyperslab parameters */
      start[0]      = t;
      start[1]      = 0;
      block[0]      = 1;
      block[1]      = int(SUBBANDS)*int(CHANNELS);
      nofDatapoints = DAL::HDF5Hyperslab::nofDatapoints (count,block);
      
      std::cout << "-- Writing data for timestep " << t << std::endl;
      std::cout << "--> start /block = " << start << " / " << block << std::endl;
      std::cout << "--> nof. datapoints = " << nofDatapoints << std::endl;
      
      /* Update the data array written to file */
      for (unsigned s = 0; s < SUBBANDS; s++) {
      	for (unsigned c = 0; c < CHANNELS; c++) {
      	  samples[0][s][c] = 1e06*(t+1) + 1e03*s + c;
     	}
      }
      
      stokesDataset.writeData( samples.origin(), start, block );
      
    } // END for (unsigned int t=0; t<10; ++t)

  }
  
  return 0;
}
