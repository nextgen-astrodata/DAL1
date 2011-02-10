/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 1964 2008-09-06 17:52:38Z baehren                   $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include <BF_BeamGroup.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                BF_BeamGroup
  
  BF_BeamGroup::BF_BeamGroup ()
  {
    location_p = 0;
    itsProcessingHistory.clear();
    itsCoordinates.clear();
  }
  
  //_____________________________________________________________________________
  //                                                                 BF_BeamGroup
  
  BF_BeamGroup::BF_BeamGroup (hid_t const &location,
			      std::string const &name)
  {
    open (location,name,false);
  }
  
  //_____________________________________________________________________________
  //                                                                 BF_BeamGroup
  
  BF_BeamGroup::BF_BeamGroup (hid_t const &location,
			      unsigned int const &index,
			      bool const &create)
  {
    open (location,getName(index),create);
  }
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                     ~BF_BeamGroup

  BF_BeamGroup::~BF_BeamGroup ()
  {
    if (location_p > 0) {
      herr_t h5error;
      H5I_type_t object_type = H5Iget_type(location_p);
      // clear maps with embedded objects
      itsProcessingHistory.clear();
      itsCoordinates.clear();
      // release HDF5 object
      if (object_type == H5I_GROUP) {
	h5error = H5Gclose(location_p);
	location_p = 0;
      }
    }
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void BF_BeamGroup::summary (std::ostream &os)
  {
    std::map<std::string,BF_ProcessingHistory>::iterator hist;
    std::map<std::string,CoordinatesGroup>::iterator coord;

    hist  = itsProcessingHistory.begin();
    coord = itsCoordinates.begin();

    os << "[BF_BeamGroup] Summary of internal parameters." << std::endl;
    os << "-- Location ID   = " << location_p << std::endl;
    os << "-- Group name    = " << HDF5Object::name(location_p) << std::endl;
    os << "-- nof. datasets = " << itsStokesDatasets.size()     << std::endl;
    
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void BF_BeamGroup::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("TARGET");
    attributes_p.insert("NOF_STATIONS");
    attributes_p.insert("STATIONS_LIST");
    attributes_p.insert("POINT_RA");
    attributes_p.insert("POINT_DEC");
    attributes_p.insert("POSITION_OFFSET_RA");
    attributes_p.insert("POSITION_OFFSET_DEC");
    attributes_p.insert("PB_DIAMETER_RA");
    attributes_p.insert("PB_DIAMETER_DEC");
    attributes_p.insert("PB_CENTER_FREQUENCY");
    attributes_p.insert("PB_CENTER_FREQUENCY_UNIT");
    attributes_p.insert("FOLDED_DATA");
    attributes_p.insert("FOLD_PERIOD");
    attributes_p.insert("FOLD_PERIOD_UNIT");
    attributes_p.insert("DEDISPERSION");
    attributes_p.insert("DISPERSION_MEASURE");
    attributes_p.insert("DISPERSION_MEASURE_UNIT");
    attributes_p.insert("BARYCENTER");
    attributes_p.insert("STOKES_COMPONENTS");
    attributes_p.insert("COMPLEX_VOLTAGE");
    attributes_p.insert("SIGNAL_SUM");
  }

  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
    \param create -- Create the corresponding data structure, if it does not 
           exist yet?
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool BF_BeamGroup::open (hid_t const &location,
			   std::string const &name,
			   bool const &create)
  {
    bool status (true);

    /* Set up the list of attributes attached to the Beam group */
    setAttributes();

    /* Try to open the group: get list of groups attached to 'location' and
       check if 'name' is part of it.
    */
    if (H5Lexists (location, name.c_str(), H5P_DEFAULT)) {
      location_p = H5Gopen (location,
			    name.c_str(),
			    H5P_DEFAULT);
    } else {
      location_p = 0;
    }
    
    if (location_p > 0) {
      status = true;
    } else {
      /* If failed to open the group, check if we are supposed to create one */
      if (create) {
	location_p = H5Gcreate (location,
				name.c_str(),
				H5P_DEFAULT,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* If creation was sucessful, add attributes with default values */
	if (location_p > 0) {
	  std::string grouptype ("Beam");
	  std::string undefined ("UNDEFINED");
	  bool ok (false);
	  std::vector<std::string> stations (1,undefined);
	  std::vector<std::string> stokes (1,undefined);
	  /* double-type attributes */
	  std::vector<std::string> attributesDouble;
	  attributesDouble.push_back("POINT_RA");
	  attributesDouble.push_back("POINT_DEC");
	  attributesDouble.push_back("POSITION_OFFSET_RA");
	  attributesDouble.push_back("POSITION_OFFSET_DEC");
	  attributesDouble.push_back("PB_DIAMETER_RA");
	  attributesDouble.push_back("PB_DIAMETER_DEC");
	  attributesDouble.push_back("PB_CENTER_FREQUENCY");
	  /* string-type attributes */
	  std::vector<std::string> attributesString;
	  attributesString.push_back("TARGET");
	  attributesString.push_back("FOLD_PERIOD_UNIT");
	  attributesString.push_back("DEDISPERSION");
	  attributesString.push_back("DISPERSION_MEASURE_UNIT");
	  attributesString.push_back("SIGNAL_SUM");
	  attributesString.push_back("PB_CENTER_FREQUENCY_UNIT");
	  // write the attributes
	  HDF5Attribute::setAttribute (location_p,"GROUPTYPE",          grouptype   );
	  HDF5Attribute::setAttribute (location_p,"NOF_STATIONS",       int(0)      );
	  HDF5Attribute::setAttribute (location_p,"STATIONS_LIST",      stations    );
	  HDF5Attribute::setAttribute (location_p, attributesDouble,    double(0.0) );
	  HDF5Attribute::setAttribute (location_p, attributesString,    undefined   );
	  HDF5Attribute::setAttribute (location_p,"FOLDED_DATA",        ok          );
	  HDF5Attribute::setAttribute (location_p,"FOLD_PERIOD",        float(0.0)  );
	  HDF5Attribute::setAttribute (location_p,"DISPERSION_MEASURE", float(0.0)  );
 	  HDF5Attribute::setAttribute (location_p,"BARYCENTER",         ok          );
 	  HDF5Attribute::setAttribute (location_p,"STOKES_COMPONENTS",  stokes      );
 	  HDF5Attribute::setAttribute (location_p,"COMPLEX_VOLTAGE",    ok          );
	} else {
	  std::cerr << "[BF_BeamGroup::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[BF_BeamGroup::open] Failed to open group "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (create);
    } else {
      std::cerr << "[BF_StationBeam::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }

  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool BF_BeamGroup::openEmbedded (bool const &create)
  {
    bool status = true;
    std::set<std::string>::iterator it;
    std::set<std::string> groups;
    std::set<std::string> datasets;
    
    /*________________________________________________________________
      Extract the names of the groups and datasets attached to this 
      beam group.
    */
    
    if (H5Iis_valid(location_p)) {
      status = h5get_names (groups,   location_p, H5G_GROUP   );
      status = h5get_names (datasets, location_p, H5G_DATASET );
    } else {
      std::cerr << "[BF_BeamGroup::openEmbedded]"
		<< " No connection to valid HDF5 object!"
		<< std::endl;
      return false;
    }
    
    /*________________________________________________________________
      Open groups with processing history and coordinate information.
    */

    if (itsProcessingHistory.size() == 0 && location_p > 0) {
      itsProcessingHistory["ProcessingHistory"] = BF_ProcessingHistory (location_p,create);
    }

    /*________________________________________________________________
      Open Stokes datasets.
    */

    if (!datasets.empty()) {
      for (it=datasets.begin(); it!=datasets.end(); ++it) {
	itsStokesDatasets[*it] = BF_StokesDataset (location_p, *it);
      }
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                            openStokesDataset

  /*!
    \param name        -- Name of the dataset.
    \param nofSamples  -- Number of bins along the time axis.
    \param nofSubbands -- Number of sub-bands.
    \param nofChannels -- Number of channels within the subbands.
    \param component   -- Stokes component stored within the dataset
    \param datatype    -- Datatype for the elements within the Dataset
    \return status     -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool BF_BeamGroup::openStokesDataset (std::string const &name,
					unsigned int const &nofSamples,
					unsigned int const &nofSubbands,
					unsigned int const &nofChannels,
					DAL::Stokes::Component const &component,
					hid_t const &datatype)
  {
    /* Put input parameters into proper format to be forwarded */
    std::vector<unsigned int> channels (nofSubbands,nofChannels);
    /* Open/create Stokes dataset. */
    return openStokesDataset (name,
			      nofSamples,
			      channels,
			      component,
			      datatype);
  }

  //_____________________________________________________________________________
  //                                                            openStokesDataset

  /*!
    \param name        -- Name of the dataset.
    \param nofSamples  -- Number of bins along the time axis.
    \param nofChannels -- Number of channels within the subbands.
    \param component   -- Stokes component stored within the dataset
    \param datatype    -- Datatype for the elements within the Dataset
    \return status     -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool BF_BeamGroup::openStokesDataset (std::string const &name,
					unsigned int const &nofSamples,
					std::vector<unsigned int> const &nofChannels,
					DAL::Stokes::Component const &component,
					hid_t const &datatype)
  {
    bool status = true;
    std::map<std::string,BF_StokesDataset>::iterator it = itsStokesDatasets.find(name);

    /*________________________________________________________________
      Check input parameters.
    */    

    if (nofChannels.empty()) {
      std::cerr << "[BF_BeamGroup::openStokesDataset]"
		<< " Empty array of frequency channels!"
		<< std::endl;
      return false;
    }

    /*________________________________________________________________
      Check if dataset of given name does exist already; if this is
      is the case, abort in order to avoid corrupting dataset.
    */

    if (it==itsStokesDatasets.end()) {
      status = true;
    } else {
      std::cout << "[BF_BeamGroup::openStokesDataset]"
		<< " Found existing dataset " << name << "!"
		<< std::endl;
      return false;
    }
    
    /*________________________________________________________________
      Create new Stokes dataset.
    */    

    itsStokesDatasets[name] = BF_StokesDataset (location_p,
						name,
						nofSamples,
						nofChannels,
						component,
						datatype);

    /*________________________________________________________________
      Check if creation of dataset was successful; is this was not the
      case, make sure no entry is made in the internal map.
    */    

    return status;
  } 
  
  //_____________________________________________________________________________
  //                                                                      getName
  
  /*!
    \param index -- Index identifying the beam.

    \return name -- The name of the beam group, <tt>Beam<index></tt>
  */
  std::string BF_BeamGroup::getName (unsigned int const &index)
  {
    char uid[10];
    sprintf(uid,
            "%03d",
	    index);
    
    std::string name (uid);
    
    name = "Beam" + name;
    
    return name;
  }
  
} // Namespace DAL -- end
