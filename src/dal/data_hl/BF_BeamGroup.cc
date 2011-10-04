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

#include <data_hl/BF_BeamGroup.h>

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
  
  /*!
    \param location -- Identifier of the object the beam group is attached to.
    \param name     -- Name of the beam group to be opened.
  */
  BF_BeamGroup::BF_BeamGroup (hid_t const &location,
			      std::string const &name)
  {
    open (location,name,IO_Mode(IO_Mode::Open));
  }
  
  //_____________________________________________________________________________
  //                                                                 BF_BeamGroup
  
  /*!
    \param location -- Identifier of the object the beam group is attached to.
    \param index    -- Index of the beam group.
    \param flags    -- I/O mode flags.
  */
  BF_BeamGroup::BF_BeamGroup (hid_t const &location,
			      unsigned int const &index,
			      IO_Mode const &flags)
  {
    std::string name = getName(index);
    open (location,name,flags);
  }
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                ~BF_BeamGroup

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
    os << "-- Location ID     = " << location_p                   << std::endl;
    os << "-- Group name      = " << HDF5Object::name(location_p) << std::endl;
    os << "-- nof. attributes = " << attributes_p.size()          << std::endl;
    os << "-- Attributes      = " << attributes_p                 << std::endl;
    os << "-- nof. datasets   = " << itsStokesDatasets.size()     << std::endl;
    
  }
  
  // ============================================================================
  //
  //  Public methods
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
    attributes_p.insert("TRACKING");
    attributes_p.insert("POINT_RA");
    attributes_p.insert("POINT_DEC");
    attributes_p.insert("POSITION_OFFSET_RA");
    attributes_p.insert("POSITION_OFFSET_DEC");
    attributes_p.insert("BEAM_DIAMETER_RA");
    attributes_p.insert("BEAM_DIAMETER_DEC");
    attributes_p.insert("BEAM_FREQUENCY_CENTER");
    attributes_p.insert("BEAM_FREQUENCY_CENTER_UNIT");
    attributes_p.insert("FOLDED_DATA");
    attributes_p.insert("FOLD_PERIOD");
    attributes_p.insert("FOLD_PERIOD_UNIT");
    attributes_p.insert("DEDISPERSION");
    attributes_p.insert("DISPERSION_MEASURE");
    attributes_p.insert("DISPERSION_MEASURE_UNIT");
    attributes_p.insert("BARYCENTER");
    attributes_p.insert("NOF_STOKES");
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
    \param flags  -- I/O mode flags.
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool BF_BeamGroup::open (hid_t const &location,
			   std::string const &name,
			   IO_Mode const &flags)
  {
    bool status    = true;
    bool truncated = HDF5GroupBase::open (location_p,
					  location,
					  name,
					  flags);
    
    if ( H5Iis_valid(location_p) ) {

      // List of recognized attributes _____________________
      setAttributes();
      
      if (truncated) {

	// Initial values for the attributes _______________
	
	bool valBool           = false;
	int valInt             = 0;
	double valDouble       = 0.0;
	std::string grouptype  = "Beam";
	std::string undefined  = "UNDEFINED";
	std::string MHz        = "MHz";
	std::string foldPeriod = "pc/cm^3";
	std::vector<std::string> vectString (1,undefined);
	
	// Write attribute values __________________________
	
	HDF5Attribute::write (location_p,"GROUPTYPE",                  grouptype  );
	HDF5Attribute::write (location_p,"TARGET",                     vectString );
	HDF5Attribute::write (location_p,"NOF_STATIONS",               valInt     );
	HDF5Attribute::write (location_p,"STATIONS_LIST",              vectString );
	HDF5Attribute::write (location_p,"TRACKING",                   undefined  );
	HDF5Attribute::write (location_p,"POINT_RA",                   valDouble  );
	HDF5Attribute::write (location_p,"POINT_DEC",                  valDouble  );
	HDF5Attribute::write (location_p,"POSITION_OFFSET_RA" ,        valDouble  );
	HDF5Attribute::write (location_p,"POSITION_OFFSET_DEC",        valDouble  );
	HDF5Attribute::write (location_p,"BEAM_DIAMETER_RA",           valDouble  );
	HDF5Attribute::write (location_p,"BEAM_DIAMETER_DEC",          valDouble  );
	HDF5Attribute::write (location_p,"BEAM_FREQUENCY_CENTER",      valDouble  );
	HDF5Attribute::write (location_p,"BEAM_FREQUENCY_CENTER_UNIT", MHz        );
	HDF5Attribute::write (location_p,"FOLDED_DATA",                valBool    );
	HDF5Attribute::write (location_p,"FOLD_PERIOD",                valDouble  );
	HDF5Attribute::write (location_p,"FOLD_PERIOD_UNIT",           foldPeriod );
	HDF5Attribute::write (location_p,"DEDISPERSION",               undefined  );
	HDF5Attribute::write (location_p,"DISPERSION_MEASURE",         valDouble  );
	HDF5Attribute::write (location_p,"DISPERSION_MEASURE_UNIT",    undefined  );
	HDF5Attribute::write (location_p,"BARYCENTER",                 valBool    );
	HDF5Attribute::write (location_p,"NOF_STOKES",                 valInt     );
	HDF5Attribute::write (location_p,"STOKES_COMPONENTS",          vectString );
	HDF5Attribute::write (location_p,"COMPLEX_VOLTAGE",            valBool    );
	HDF5Attribute::write (location_p,"SIGNAL_SUM",                 undefined  );
      }
      
      // Open embedded groups ______________________________
      
      status = openEmbedded (flags);
      
    } else {
      std::cerr << "[BF_BeamGroup::open]"
		<< " Failed to open/create group " << name
		<< std::endl;
      return false;
    }
    
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                            openStokesDataset

  bool BF_BeamGroup::openStokesDataset (unsigned int const &stokesID)
  {
    /* Convert ID to name ... */
    std::string name = BF_StokesDataset::getName (stokesID);
    /* ... and try to open the dataset */
    return openStokesDataset (name);
  }

  //_____________________________________________________________________________
  //                                                            openStokesDataset

  bool BF_BeamGroup::openStokesDataset (std::string const &name)
  {
    bool status (true);
    std::map<std::string,BF_StokesDataset>::iterator it;

    it = itsStokesDatasets.find(name);

    /*
     *  Check internal book-keeping; if the dataset has been opened already, we
     *  can skip the additional tests.
     */
    if (it==itsStokesDatasets.end()) {
      /* Dataset not yet opened */
      if (H5Lexists (location_p, name.c_str(), H5P_DEFAULT)) {
	/* Dataset exists, but not yet has been opened */
	itsStokesDatasets[name] = BF_StokesDataset (location_p, name);
      } else {
	/* Dataset does not exist */
	status = false;
      }
    } else {
      /* Dataset exits and is opened already */
      status = true;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                            openStokesDataset

  /*!
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
  bool BF_BeamGroup::openStokesDataset (unsigned int const &stokesID,
					unsigned int const &nofSamples,
					unsigned int const &nofSubbands,
					unsigned int const &nofChannels,
					DAL::Stokes::Component const &component,
					hid_t const &datatype,
					IO_Mode const &flags)
  {
    /* Put input parameters into proper format to be forwarded */
    std::vector<unsigned int> channels (nofSubbands,nofChannels);
    /* Open/create Stokes dataset. */
    return openStokesDataset (stokesID,
			      nofSamples,
			      channels,
			      component,
			      datatype,
			      flags);
  }
  
  //_____________________________________________________________________________
  //                                                            openStokesDataset
  
  /*!
    \param stokesID    -- ID of the Stokes dataset to be created.
    \param nofSamples  -- Number of bins along the time axis.
    \param nofChannels -- Number of channels within the subbands.
    \param component   -- Stokes component stored within the dataset
    \param datatype    -- Datatype for the elements within the Dataset
    \param flags       -- I/o mode flags.
    \return status     -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool BF_BeamGroup::openStokesDataset (unsigned int const &stokesID,
					unsigned int const &nofSamples,
					std::vector<unsigned int> const &nofChannels,
					DAL::Stokes::Component const &component,
					hid_t const &datatype,
					IO_Mode const &flags)
  {
    bool status = true;
    std::string name = BF_StokesDataset::getName(stokesID);
    std::map<std::string,BF_StokesDataset>::iterator it = itsStokesDatasets.find(name);
    
    /*________________________________________________________________
      Check input parameters.
    */    
    
    if (nofChannels.empty()) {
      std::cerr << "[BF_BeamGroup::createStokesDataset]"
		<< " Empty array of frequency channels!"
		<< std::endl;
      return false;
    }

    /*________________________________________________________________
      Check if dataset of given name does exist already; if this is
      the case, also take into account the 'truncate' parameter which
      allows over-writing an existing dataset -- otherwise return
      error.
    */

    if (it==itsStokesDatasets.end()) {
      /* Dataset not yet opened */
      status = true;
    } else {
      if ( (flags.flags() & IO_Mode::OpenOrCreate) ||
	   (flags.flags() & IO_Mode::Create) ||
	   (flags.flags() & IO_Mode::Truncate) ) {
	/* Removing dataset from internal book-keeping */
	itsStokesDatasets.erase(it);
	/* Delete dataset */
	H5Ldelete (location_p,
		   name.c_str(),
		   H5P_DEFAULT);
	/* Update status for subsequent operations */
	status = true;
      } else {
	status = false;
      }
    }
    
    /*________________________________________________________________
      Create new Stokes dataset.
    */    
    
    itsStokesDatasets[name] = BF_StokesDataset (location_p,
						stokesID,
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
  //                                                             getStokesDataset

  /*!
    \param stokesID -- ID of the Stokes dataset to be created.
    \return dataset -- BF_StokesDataset object corresponding to \c stokesID;
            returns empty object (as created by default constructor) in case
	    there is no dataset corresponding to the provided \c stokesID.
  */
  BF_StokesDataset BF_BeamGroup::getStokesDataset (unsigned int const &stokesID)
  {
    BF_StokesDataset stokes;

    if (H5Iis_valid(location_p)) {
      std::string name = BF_StokesDataset::getName (stokesID);
      std::map<std::string,BF_StokesDataset>::iterator it;
      
      it = itsStokesDatasets.find(name);

      if (it != itsStokesDatasets.end()) {
	return BF_StokesDataset(it->second);
      } else {
	std::cerr << "[BF_BeamGroup::getStokesDataset] No such dataset "
		  << "\"" << name << "\""
		  << std::endl;
      }
    }
    else {
      std::cerr << "[BF_BeamGroup::getStokesDataset] Not connected to file!"
		<< std::endl;
    }
    
    return stokes;
  }

  //_____________________________________________________________________________
  //                                                             getStokesDataset

  /*!
    \retval dataset -- Pointer to BF_StokesDataset object corresponding to
            \c stokesID.
    \param stokesID -- ID of the Stokes dataset to be created.
    \return status  -- Status of the operation; returns \e false in case an error
            was encountered, e.g. because there is no dataset corresponding to
	    the provided \c stokesID.
  */
  bool BF_BeamGroup::getStokesDataset (BF_StokesDataset *dataset,
				       unsigned int const &stokesID)
  {
    /* Convert ID to name */
    std::string name = BF_StokesDataset::getName (stokesID);
    /* Search for requested dataset */
    std::map<std::string,BF_StokesDataset>::iterator it = itsStokesDatasets.find(name);
    
    if (it==itsStokesDatasets.end()) {
      std::cerr << "[BF_BeamGroup::getStokesDataset]"
		<< " Unable to find Stokes dataset " << name << std::endl;
      return false;
    } else {
      dataset = &(it->second);
      return true;
    }
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
    sprintf (uid, "%03d", index);
    
    std::string name (uid);
    
    name = "BEAM_" + name;
    
    return name;
  }
  
  // ============================================================================
  //
  //  Private methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool BF_BeamGroup::openEmbedded (IO_Mode const &flags)
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
      itsProcessingHistory["ProcessingHistory"] = BF_ProcessingHistory (location_p,
									flags);
    }

    /*________________________________________________________________
      Open Stokes datasets.
    */

    for (it=datasets.begin(); it!=datasets.end(); ++it) {
      status *= openStokesDataset (*it);
    }
    
    return status;
  }

} // Namespace DAL -- end
