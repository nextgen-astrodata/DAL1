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
#define BLOCKS          10

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
  const unsigned nofPointings = 5;
  const unsigned nofBeams     = 10;
  
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

  std::cout << "-- Setting up LOFAR Common Attributes ..." << std::endl;

  DAL::CommonAttributes attributes = commonAttributes (filename);

  /*__________________________________________________________________
    Create new BF file; we are using "IO_Mode::Truncate" in order to
    always start with a freshly created file - otherwise previously 
    created structure elements will remain in place.
  */
  
  std::cout << "-- Creating new file " << filename.filename() << endl;

  DAL::BF_RootGroup bfRoot (attributes,
			    DAL::IO_Mode(DAL::IO_Mode::Create));
  
  /*__________________________________________________________________
    Create primary array pointing groups with embedded beam groups.
  */
  
  for (unsigned numPointing=0; numPointing<nofPointings; ++numPointing) {
    std::cout << "-- Pointing " <<  numPointing << ": [";
    for (unsigned numBeam=0; numBeam<nofBeams; ++numBeam) {
      // progress message
      std::cout << " " << numBeam;
      // recursively open beam group
      bfRoot.openBeam ( numPointing, numBeam );
    }
    std::cout << " ]" << std::endl;
  }
  
  std::cout << "--> Finished opening sub-groups." << std::endl;
  
  /*__________________________________________________________________
    Create Stokes datasets
  */
  
  const unsigned nofSamples  = SAMPLES;
  const unsigned nofChannels = SUBBANDS * CHANNELS;
  
  std::cout << "-- Creating Stokes::I dataset 0 ..." << std::endl;
  bfRoot.openStokesDataset (0,                 // ID of sub-array pointing
			    0,                 // ID of beam group
			    0,                 // ID of Stokes dataset
			    nofSamples,
			    SUBBANDS,
			    CHANNELS,
			    DAL::Stokes::I);
  std::cout << "-- Creating Stokes::Q dataset 1 ..." << std::endl;
  bfRoot.openStokesDataset (0,                 // ID of sub-array pointing
			    0,                 // ID of beam group
			    1,                 // ID of Stokes dataset
			    nofSamples,
			    SUBBANDS,
			    CHANNELS,
			    DAL::Stokes::Q);
  
  /*__________________________________________________________________
    Retrieve objects embedded in the HDF5 file hierarchy
  */

  /* Variant 1: recursive retrieval of embedded objects. */

  {
    std::cout << "-- BF_RootGroup::getSubArrayPointing() ..." << std::endl;
    
    DAL::BF_SubArrayPointing bfSubArray = bfRoot.getSubArrayPointing (0);
    bfSubArray.summary();
    
    std::cout << "-- BF_SubArrayPointing::getBeamGroup() ..." << std::endl;

    DAL::BF_BeamGroup bfBeam = bfSubArray.getBeamGroup (0);
    bfBeam.summary();

    std::cout << "-- BF_BeamGroup::getStokesDataset() ..." << std::endl;

    DAL::BF_StokesDataset bfStokes = bfBeam.getStokesDataset (0);
    bfStokes.summary();
  }

  /* Variant 2: direct retrieval across herarchical levels. */
  
  {
    std::cout << "-- BF_RootGroup::getBeamGroup() ..." << std::endl;
    
    DAL::BF_BeamGroup bfBeam = bfRoot.getBeamGroup (0,0);
    bfBeam.summary();
  }
  
  /*__________________________________________________________________
    Write data to Stokes dataset
  */

  return 0;

  {
    hid_t fileID = bfRoot.locationID();
    cout << "Creating stokes set 0" << endl;
    DAL::BF_StokesDataset stokesDataset(fileID,
					0,
					nofSamples,
					SUBBANDS,
					CHANNELS,
					DAL::Stokes::I );
    
    cout << "Creating sample multiarray of " << (SAMPLES|2) << " x " << SUBBANDS << " x " << CHANNELS << endl;
    typedef multi_array<float,3> array;
    
    array samples(extents[SAMPLES|2][SUBBANDS][CHANNELS]);
    
    for (unsigned t = 0; t < SAMPLES; t++)
      for (unsigned s = 0; s < SUBBANDS; s++)
        for (unsigned c = 0; c < CHANNELS; c++)
          samples[t][s][c] = t * s * c;
    
    cout << "Creating hyperslab of " << nofSamples << " x " << nofChannels << endl;
    
    vector<int> start(2), stride(2), count(2), block(2);
    
    start[0] = 0;
    stride[0] = samples.strides()[0]; // the sample dimension could be |2
    count[0] = nofSamples;
    block[0] = 1;
    
    start[1] = 0;
    stride[1] = 1;
    count[1] = nofChannels;
    block[1] = 1;
    
    DAL::HDF5Hyperslab hyperslab ( start, stride, count, block );
    hyperslab.summary( cout );
    
    for (unsigned i = 0; i < BLOCKS; i++ ) {
      cout << "Writing data block " << i << endl;
      stokesDataset.writeData( samples.origin(), hyperslab );
    }
  }

  return 0;
}
