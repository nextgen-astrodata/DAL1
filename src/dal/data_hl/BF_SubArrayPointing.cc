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

#include <data_hl/BF_SubArrayPointing.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                          BF_SubArrayPointing
  
  BF_SubArrayPointing::BF_SubArrayPointing ()
  {
    location_p   = 0;
    itsGroupType = "SubArrayPointing";
  }
  
  //_____________________________________________________________________________
  //                                                          BF_SubArrayPointing
  
  BF_SubArrayPointing::BF_SubArrayPointing (hid_t const &location,
					    std::string const &name)
  {
    itsGroupType = "SubArrayPointing";
    open (location,name,IO_Mode(IO_Mode::Open));
  }
  
  //_____________________________________________________________________________
  //                                                          BF_SubArrayPointing
  
  BF_SubArrayPointing::BF_SubArrayPointing (hid_t const &location,
					    unsigned int const &index,
					    IO_Mode const &flags)
  {
    itsGroupType = "SubArrayPointing";
    open (location,getName(index),flags);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  BF_SubArrayPointing::~BF_SubArrayPointing ()
  {
    if (location_p > 0) {
      // clear maps with embedded objects
      itsBeams.clear();
      // release HDF5 object
      herr_t h5error;
      H5I_type_t object_type = H5Iget_type(location_p);
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
  
  void BF_SubArrayPointing::summary (std::ostream &os)
  {
    os << "[BF_SubArrayPointing] Summary of internal parameters." << std::endl;
    os << "-- Location ID     = " << location_p          << std::endl;
    os << "-- nof. attributes = " << attributes_p.size() << std::endl;
    os << "-- Attributes      = " << attributes_p        << std::endl;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void BF_SubArrayPointing::setAttributes ()
  {
    attributes_p.clear();
    
    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("NOF_STATIONS");
    attributes_p.insert("STATIONS_LIST");
    attributes_p.insert("POINT_RA");
    attributes_p.insert("POINT_DEC");
    attributes_p.insert("TRACKING");
    attributes_p.insert("POINT_ALTITUDE");
    attributes_p.insert("POINT_AZIMUTH");
    attributes_p.insert("CLOCK_RATE");
    attributes_p.insert("CLOCK_RATE_UNIT");
    attributes_p.insert("NOF_SAMPLES");
    attributes_p.insert("SAMPLING_RATE");
    attributes_p.insert("SAMPLING_RATE_UNIT");
    attributes_p.insert("SAMPLING_TIME");
    attributes_p.insert("SAMPLING_TIME_UNIT");
    attributes_p.insert("TOTAL_INTEGRATION_TIME");
    attributes_p.insert("TOTAL_INTEGRATION_TIME_UNIT");
    attributes_p.insert("CHANNELS_PER_SUBBAND");
    attributes_p.insert("SUBBAND_WIDTH");
    attributes_p.insert("SUBBAND_WIDTH_UNIT");
    attributes_p.insert("CHANNEL_WIDTH");
    attributes_p.insert("CHANNEL_WIDTH_UNIT");
    attributes_p.insert("NOF_BEAMS");
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
  bool BF_SubArrayPointing::open (hid_t const &location,
				  std::string const &name,
				  IO_Mode const &flags)
  {
    bool status (true);
    
    /* Set up the list of attributes attached to the root group */
    setAttributes();
    
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
      if ( (flags.flags() & IO_Mode::OpenOrCreate) ||
	   (flags.flags() & IO_Mode::Create) ||
	   (flags.flags() & IO_Mode::CreateNew) ) {
	location_p = H5Gcreate (location,
				name.c_str(),
				H5P_DEFAULT,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* If creation was sucessful, add attributes with default values */
	if (location_p > 0) {
	  std::string mhz ("MHz");
	  std::string tracking ("OFF");
	  std::string second ("s");
	  std::string usecond ("us");
	  std::vector<std::string> stationsList (1,"UNDEFINED");
	  // write the attributes
	  HDF5Attribute::write (location_p,"GROUPTYPE",                itsGroupType);
	  HDF5Attribute::write (location_p,"NOF_STATIONS",             int(0)      );
	  HDF5Attribute::write (location_p,"STATIONS_LIST",            stationsList);
	  HDF5Attribute::write (location_p,"POINT_RA",                 double(0.0) );
	  HDF5Attribute::write (location_p,"POINT_DEC",                double(0.0) );
	  HDF5Attribute::write (location_p,"TRACKING",                 tracking    );
	  HDF5Attribute::write (location_p,"POINT_ALTITUDE",           double(0.0) );
	  HDF5Attribute::write (location_p,"POINT_AZIMUTH",            double(0.0) );
	  HDF5Attribute::write (location_p,"CLOCK_RATE",               double(0.0) );
	  HDF5Attribute::write (location_p,"CLOCK_RATE_UNIT",          mhz         );
	  HDF5Attribute::write (location_p,"NOF_SAMPLES",              int(0)      );
	  HDF5Attribute::write (location_p,"SAMPLING_RATE",            double(0.0) );
	  HDF5Attribute::write (location_p,"SAMPLING_RATE_UNIT",       mhz         );
	  HDF5Attribute::write (location_p,"SAMPLING_TIME",            float(0.0)  );
	  HDF5Attribute::write (location_p,"SAMPLING_TIME_UNIT",       usecond     );
	  HDF5Attribute::write (location_p,"TOTAL_INTEGRATION_TIME",   double(0.0) );
	  HDF5Attribute::write (location_p,"TOTAL_INTEGRATION_TIME_UNIT", second   );
	  HDF5Attribute::write (location_p,"CHANNELS_PER_SUBBAND",     int(0)      );
	  HDF5Attribute::write (location_p,"SUBBAND_WIDTH",            double(0.0) );
	  HDF5Attribute::write (location_p,"SUBBAND_WIDTH_UNIT",       mhz         );
	  HDF5Attribute::write (location_p,"CHANNEL_WIDTH",            double(0.0) );
	  HDF5Attribute::write (location_p,"CHANNEL_WIDTH_UNIT",       mhz         );
	  HDF5Attribute::write (location_p,"NOF_BEAMS",                int(0)      );
	} else {
	  std::cerr << "[BF_SubArrayPointing::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[BF_SubArrayPointing::open] Failed to open group "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (flags);
    } else {
      std::cerr << "[BF_SubArrayPointing::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool BF_SubArrayPointing::openEmbedded (IO_Mode const &flags)
  {
    bool status = flags.flags();
    std::set<std::string> groupnames;
    std::set<std::string>::iterator it;
    
    /* Retrieve the names of the groups attached to the sub-array pointing group */
    status = h5get_names (groupnames,
			  location_p,
			  H5G_GROUP);
    
    if (groupnames.size() > 0) {
      for (it=groupnames.begin(); it!=groupnames.end(); ++it) {
	itsBeams[*it] = BF_BeamGroup (location_p,*it);
      }
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                     openBeam
  
  /*!
    \param beamID -- Identifier for the Beam within the Primary Pointing 
           Direction group.
    \param flags  -- I/O mode flags.
  */
  bool BF_SubArrayPointing::openBeam (unsigned int const &beamID,
				      IO_Mode const &flags)
  {
    bool status      = true;
    bool groupIsOpen = false;
    bool groupExists = false;
    std::string name = BF_BeamGroup::getName (beamID);

    /*______________________________________________________
      Check object ID for sub-array pointing direction group
    */
    
    if ( H5Iis_valid(location_p) ) {
      /* Check if the group has been opened already */
      if ( itsBeams.find(name) == itsBeams.end() ) {
	groupIsOpen = false;
	/* Check if the group at least exists */
	htri_t h5err = H5Lexists (location_p,
				  name.c_str(),
				  H5P_DEFAULT);
	/* Inspect return value */
	if (h5err>0) {
	  groupExists = true;
	} else {
	  groupExists = false;
	}
      } else {
	groupIsOpen = true;
      }
    } else {
      std::cerr << "[BF_SubArrayPointing::openBeam]" 
		<< " Invalid object ID!"
		<< std::endl;
      return false;
    }
    
    /*______________________________________________________
      If group is not opened yet, check if it exists; if yes
      then open it.
    */
    
    if ( !groupIsOpen ) {
      if (groupExists) {
	itsBeams[name] = BF_BeamGroup (location_p,
				       name);   
      } else if (flags.flags() & IO_Mode::OpenOrCreate) {
	itsBeams[name] = BF_BeamGroup (location_p,
				       beamID,
				       flags);   
      }
    }
    
    /*______________________________________________________
      Internal book-keeping
    */
    
    int nofBeams = itsBeams.size();
    HDF5Attribute::write (location_p,
			  "NOF_BEAMS",
			  nofBeams);
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                 getBeamGroup
  
  BF_BeamGroup BF_SubArrayPointing::getBeamGroup (unsigned int const &beamID)
  {
    std::string name = BF_BeamGroup::getName (beamID);
    std::map<std::string,BF_BeamGroup>::iterator it = itsBeams.find(name);
    
    if (it==itsBeams.end()) {
      std::cerr << "[BF_SubArrayPointing::getBeamGroup]"
		<< " Unable to find Beam group " << name << std::endl;
      return BF_BeamGroup();
    } else {
      return it->second;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                 getBeamGroup
  
  bool BF_SubArrayPointing::getBeamGroup (BF_BeamGroup *beam,
					  unsigned int const &beamID)
  {
    std::string name = BF_BeamGroup::getName (beamID);
    std::map<std::string,BF_BeamGroup>::iterator it = itsBeams.find(name);

    if (it==itsBeams.end()) {
      std::cerr << "[BF_SubArrayPointing::getBeamGroup]"
		<< " Unable to find Beam group " << name << std::endl;
      return false;
    } else {
      beam = &(it->second);
      return true;
    }
  }

  //_____________________________________________________________________________
  //                                                            openStokesDataset

  /*!
    \param beamID   -- Beam group index.
    \param stokesID -- Stokes dataset index.
    \param flags    -- I/O mode flags.
    \return status  -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool BF_SubArrayPointing::openStokesDataset (unsigned int const &beamID,
					       unsigned int const &stokesID,
					       IO_Mode const &flags)
  {
    bool status = true;
    
    /*______________________________________________________
      Open/Create beam group
    */
    
    status = openBeam (beamID, flags);
    
    /*______________________________________________________
      Forward the function call to the next-lower
      hierarchical level structure.
    */
    
    std::string name;
    std::map<std::string,BF_BeamGroup>::iterator it;
    
    name = BF_BeamGroup::getName (beamID);
    it   = itsBeams.find(name);
    
    if (it==itsBeams.end()) {
      status = false;
    } else {
      it->second.openStokesDataset (stokesID);
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                            openStokesDataset

  /*!
    \param beamID      -- ID of the beam group to be created.
    \param stokesID    -- ID of the Stokes dataset to be created.
    \param nofSamples  -- Number of bins along the time axis.
    \param nofSubbands -- Number of sub-bands.
    \param nofChannels -- Number of channels within the subbands.
    \param component   -- Stokes component stored within the dataset
    \param datatype    -- Datatype for the elements within the Dataset
    \param flags       -- I/o mode flags.
    \return status     -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool BF_SubArrayPointing::openStokesDataset (unsigned int const &beamID,
					       unsigned int const &stokesID,
					       unsigned int const &nofSamples,
					       unsigned int const &nofSubbands,
					       unsigned int const &nofChannels,
					       DAL::Stokes::Component const &component,
					       hid_t const &datatype,
					       IO_Mode const &flags)
  {
    std::vector<unsigned int> channels (nofSubbands,nofChannels);
    
    return openStokesDataset (beamID,
			      stokesID,
			      nofSamples,
			      channels,
			      component,
			      datatype,
			      flags);
  }
  
  //_____________________________________________________________________________
  //                                                            openStokesDataset
  
  /*!
    \param beamID      -- ID of the beam group to be created.
    \param stokesID    -- ID of the Stokes dataset to be created.
    \param nofSamples  -- Number of bins along the time axis.
    \param nofChannels -- Number of channels within the subbands.
    \param component   -- Stokes component stored within the dataset
    \param datatype    -- Datatype for the elements within the Dataset
    \param flags       -- I/o mode flags.
    \return status     -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool BF_SubArrayPointing::openStokesDataset (unsigned int const &beamID,
					       unsigned int const &stokesID,
					       unsigned int const &nofSamples,
					       std::vector<unsigned int> const &nofChannels,
					       DAL::Stokes::Component const &component,
					       hid_t const &datatype,
					       IO_Mode const &flags)
  {
    bool status = true;
    
    /*______________________________________________________
      Open/Create beam group
    */
    
    status = openBeam (beamID, flags);
    
    /*______________________________________________________
      Forward the function call to the next-lower
      hierarchical level structure.
    */
    
    std::string name;
    std::map<std::string,BF_BeamGroup>::iterator it;
    
    name = BF_BeamGroup::getName (beamID);
    it   = itsBeams.find(name);
    
    if (it==itsBeams.end()) {
      status = false;
    } else {
      status = it->second.openStokesDataset (stokesID,
					     nofSamples,
					     nofChannels,
					     component,
					     datatype,
					     flags);
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                             getStokesDataset
  
  BF_StokesDataset BF_SubArrayPointing::getStokesDataset (unsigned int const &beamID,
							  unsigned int const &stokesID)
  {
    std::string name = BF_BeamGroup::getName (beamID);
    std::map<std::string,BF_BeamGroup>::iterator it = itsBeams.find(name);
    
    if (it==itsBeams.end()) {
      std::cerr << "[BF_SubArrayPointing::getStokesDataset]"
		<< " Unable to find Beam group " << name << std::endl;
      return BF_StokesDataset();
    } else {
      return it->second.getStokesDataset(stokesID);
    }
  }
  
  //_____________________________________________________________________________
  //                                                             getStokesDataset
  
  bool BF_SubArrayPointing::getStokesDataset (BF_StokesDataset *dataset,
					      unsigned int const &beamID,
					      unsigned int const &stokesID)
  {
    std::string name = BF_BeamGroup::getName (beamID);
    std::map<std::string,BF_BeamGroup>::iterator it = itsBeams.find(name);

    if (it==itsBeams.end()) {
      std::cerr << "[BF_SubArrayPointing::getStokesDataset]"
		<< " Unable to find Beam group " << name << std::endl;
      return false;
    } else {
      return it->second.getStokesDataset(dataset,stokesID);
    }
  }
  
  //_____________________________________________________________________________
  //                                                                      getName
  
  /*!
    \param index -- Index identifying the station beam.
    
    \return name -- The name of the station beam group,
            <tt>SubArrayPointing<index></tt>
  */
  std::string BF_SubArrayPointing::getName (unsigned int const &index)
  {
    char uid[10];
    sprintf(uid,
            "%03d",
	    index);
    
    std::string name (uid);
    
    name = "SUB_ARRAY_POINTING_" + name;
    
    return name;
  }
  
} // Namespace DAL -- end
