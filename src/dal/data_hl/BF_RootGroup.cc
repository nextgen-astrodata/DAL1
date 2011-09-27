/***************************************************************************
 *   Copyright (C) 2009-2011                                               *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#include <data_hl/BF_RootGroup.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                 BF_RootGroup

  /*!
    \param filename -- Name of the dataset to open.
  */
  BF_RootGroup::BF_RootGroup (std::string const &filename)
    : HDF5GroupBase()
  {
    if (!open (0,filename,IO_Mode(itsFlags))) {
      std::cerr << "[BF_RootGroup::BF_RootGroup] Failed to open file "
		<< filename
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                 BF_RootGroup
  
  /*!
    \param filename -- Filename object from which the actual file name of the
           dataset is derived.
    \param flags    -- I/O mode flags.
  */
  BF_RootGroup::BF_RootGroup (DAL::Filename &infile,
			      IO_Mode const &flags)
    : HDF5GroupBase(flags)
  {
    if (!open (0,infile.filename(),itsFlags)) {
      std::cerr << "[BF_RootGroup::BF_RootGroup] Failed to open file "
		<< infile.filename()
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                 BF_RootGroup

  /*!
    \param attributes -- CommonAttributes object from which the actual file name
           of the dataset is extracted.
    \param flags      -- I/O mode flags.
  */
  BF_RootGroup::BF_RootGroup (CommonAttributes const &attributes,
			      IO_Mode const &flags)
    : HDF5GroupBase(flags)
  {
    if (!open (0,attributes.filename(),itsFlags)) {
      std::cerr << "[BF_RootGroup::BF_RootGroup] Failed to open file "
		<< attributes.filename()
		<< std::endl;
    }
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  BF_RootGroup::~BF_RootGroup ()
  {
    if (location_p > 0) {
      // clear maps with embedded objects
      itsSubarrayPointings.clear();
      itsSystemLog.clear();
      // release HDF5 object
      herr_t h5error;
      H5I_type_t object_type = H5Iget_type(location_p);
      if (object_type == H5I_FILE) {
	h5error = H5Fclose(location_p);
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
  //                                                             commonAttributes
  
  CommonAttributes BF_RootGroup::commonAttributes ()
  {
    if (location_p > 0) {
      itsCommonAttributes.h5read (location_p);
    }

    return itsCommonAttributes;
  }

  //_____________________________________________________________________________
  //                                                          setCommonAttributes
  
  /*!
    \param attributes -- Set of LOFAR common attributes to be attached to the 
           root group of the beamformed dataset.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool BF_RootGroup::setCommonAttributes (CommonAttributes const &attributes)
  {
    bool status (true);

    try {
      itsCommonAttributes = attributes;
    } catch (std::string message) {
      std::cerr << "[BF_RootGroup::setCommonAttributes] " << message << std::endl;
      status = false;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary

  /*!
    \param os -- Output stream to which the summary is written.
    \param showAttributes -- Show detailed information on the attributes attached
           to the root group of the file?
  */
  void BF_RootGroup::summary (std::ostream &os,
			      bool const &showAttributes)
  {
    os << "[BF_RootGroup] Summary of internal parameters." << std::endl;
    os << "-- Filename                = " << itsFilename            << std::endl;
    os << "-- I/O mode flags          = " << itsFlags.names()       << std::endl;
    os << "-- Location ID             = " << location_p             << std::endl;
    os << "-- nof. attributes         = " << attributes_p.size()    << std::endl;
    os << "-- nof. SysLog groups      = " << itsSystemLog.size()    << std::endl;
    os << "-- nof. primary pointings  = " << nofSubArrayPointings() << std::endl;
    
    if (showAttributes) {
      os << "-- Attributes              = " << attributes_p          << std::endl;
    }
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init
  
  void BF_RootGroup::init (CommonAttributes const &attributes)
  {
    /* Store the common attributes */
    setCommonAttributes (attributes);
    /* Set up the list of attributes attached to the root group */
    setAttributes();
    /* Try opening the file holding the dataset */
    if (!open (0,itsCommonAttributes.filename(),true)) {
      std::cerr << "[BF_RootGroup::init] Initialization of object failed!"
		<< " Unable to access file holding the dataset!"
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void BF_RootGroup::setAttributes ()
  {
    attributes_p.clear();

    attributes_p = itsCommonAttributes.attributes();
    
    attributes_p.insert("CREATE_OFFLINE_ONLINE");
    attributes_p.insert("BF_FORMAT");
    attributes_p.insert("BF_VERSION");
    attributes_p.insert("EXPTIME_START_UTC");
    attributes_p.insert("EXPTIME_STOP_UTC");
    attributes_p.insert("EXPTIME_START_MJD");
    attributes_p.insert("EXPTIME_STOP_MJD");
    attributes_p.insert("EXPTIME_START_TAI");
    attributes_p.insert("EXPTIME_STOP_TAI");
    attributes_p.insert("TOTAL_INTEGRATION_TIME");
    attributes_p.insert("OBSERVATION_DATATYPE");
    attributes_p.insert("PRIMARY_POINTING_DIAMETER");
    attributes_p.insert("BANDWIDTH");
    attributes_p.insert("BEAM_DIAMETER");
    attributes_p.insert("WEATHER_TEMPERATURE");
    attributes_p.insert("WEATHER_HUMIDITY");
    attributes_p.insert("SYSTEM_TEMPERATURE");
    attributes_p.insert("NOF_PRIMARY_BEAMS");
  }
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached; parameter inherited through HDF5GroupBase,
	   but not evaluated here.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool BF_RootGroup::open (hid_t const &location,
			   std::string const &name,
			   IO_Mode const &flags)
  {
    // Initialize private variables ________________________

    location_p  = location;
    itsFilename = name;
    itsFlags    = flags;
    setAttributes();
    itsSubarrayPointings.clear();
    itsSystemLog.clear();

    // Open or create file _________________________________

    bool fileTruncated = HDF5Object::openFile (location_p,
					       name,
					       itsFlags);

    // Set attributes ______________________________________
    
    if (fileTruncated) {
      itsCommonAttributes.h5write(location_p);
      /* Write the additional attributes attached to the root group */
      bool valBool          = true;
      double valDouble      = 0.0;
      std::string undefined = "UNDEFINED";
      std::vector<double> vectD (1,0.0);
      //
      HDF5Attribute::write (location_p,"FILENAME",                  name       );
      HDF5Attribute::write (location_p,"CREATE_OFFLINE_ONLINE",     valBool    );
      HDF5Attribute::write (location_p,"BF_FORMAT",                 undefined  );
      HDF5Attribute::write (location_p,"BF_VERSION",                undefined  );
      HDF5Attribute::write (location_p,"EXPTIME_START_UTC",         undefined  );
      HDF5Attribute::write (location_p,"EXPTIME_STOP_UTC",          undefined  );
      HDF5Attribute::write (location_p,"EXPTIME_START_MJD",         undefined  );
      HDF5Attribute::write (location_p,"EXPTIME_STOP_MJD",          undefined  );
      HDF5Attribute::write (location_p,"EXPTIME_START_TAI",         undefined  );
      HDF5Attribute::write (location_p,"EXPTIME_STOP_TAI",          undefined  );
      HDF5Attribute::write (location_p,"TOTAL_INTEGRATION_TIME",    valDouble  );
      HDF5Attribute::write (location_p,"OBSERVATION_DATATYPE",      undefined  );
      HDF5Attribute::write (location_p,"PRIMARY_POINTING_DIAMETER", valDouble  );
      HDF5Attribute::write (location_p,"BANDWIDTH",                 valDouble  );
      HDF5Attribute::write (location_p,"BEAM_DIAMETER",             valDouble  );
      HDF5Attribute::write (location_p,"WEATHER_TEMPERATURE",       vectD      );
      HDF5Attribute::write (location_p,"WEATHER_HUMIDITY",          vectD      );
      HDF5Attribute::write (location_p,"SYSTEM_TEMPERATURE",        vectD      );
      HDF5Attribute::write (location_p,"NOF_PRIMARY_BEAMS",         int(0)     );
    }

    // Read common atributes _______________________________

    itsCommonAttributes.h5read(location_p);
    
    // Open embedded groups ________________________________

    return openEmbedded ();
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  /*!
    \return status -- Status of the operation; returns <tt>False</tt> in case
            no primary pointing direction groups were found.
   */
  bool BF_RootGroup::openEmbedded (IO_Mode const &flags)
  {
    bool status = flags.flags();

    if (H5Iis_valid(location_p)) {

      std::set<std::string> groups;
      std::set<std::string>::iterator it;

      /* Retrieve the names of the groups attached to the root group */
      status = h5get_names (groups,
			    location_p,
			    H5G_GROUP);
      
      /* Open system log group ... */
      status = openSysLog ();
      /* ... and remove its name from the previously retrieved list */
      groups.erase("SysLog");
      
      /* Open the SubArrayPointingDirection groups */
      for (it=groups.begin(); it!=groups.end(); ++it) {
	status *= openSubArrayPointing (*it);
      }
      
    } else {
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                   openSysLog
  
  bool BF_RootGroup::openSysLog (IO_Mode const &flags)
  {
    bool status (true);

    if (itsSystemLog.size() == 0 && location_p > 0) {
      itsSystemLog["SysLog"] = SysLog (location_p,flags);
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                         openSubArrayPointing
  
  /*!
    \param pointingID -- ID of the sub-array pointing group.
    \param flags      -- I/O mode flags.
  */
  bool BF_RootGroup::openSubArrayPointing (unsigned int const &pointingID,
					  IO_Mode const &flags)
  {
    bool status      = true;
    std::string name = BF_SubArrayPointing::getName (pointingID);

    /*______________________________________________________
      Check if the group has been opened already.
    */

    if ( itsSubarrayPointings.find(name) == itsSubarrayPointings.end() ) {
      // open/create group
      itsSubarrayPointings[name] = BF_SubArrayPointing (location_p,
							pointingID,
							flags);
      // internal book-keeping
      int nofPrimaryBeams = itsSubarrayPointings.size();
      HDF5Attribute::write (location_p,
			    "NOF_PRIMARY_BEAMS",
			    nofPrimaryBeams);
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                         openSubArrayPointing
  
  /*!
    \param name -- Name of the primary array pointing group to be opened.
  */
  bool BF_RootGroup::openSubArrayPointing (std::string const &name)
  {
    bool status = true;

    /*______________________________________________________
      Check if the group has been opened already.
    */

    if ( itsSubarrayPointings.find(name) == itsSubarrayPointings.end() ) {
      // open/create group
      itsSubarrayPointings[name] = BF_SubArrayPointing (location_p,
							name);
      // internal book-keeping
      int nofPrimaryBeams = itsSubarrayPointings.size();
      HDF5Attribute::write (location_p,
			    "NOF_PRIMARY_BEAMS",
			    nofPrimaryBeams);
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                     openBeam
  
  /*!
    \param pointingID -- Identifier for the sub-array pointing direction.
    \param beamID     -- Identifier for the beam within the primary pointing
           direction.
    \param create     -- Create the group if it does not exist yet?
  */
  bool BF_RootGroup::openBeam (unsigned int const &pointingID,
			       unsigned int const &beamID,
			       IO_Mode const &flags)
  {
    std::map<std::string,BF_SubArrayPointing>::iterator it;
    
    /*______________________________________________________
      Open/Create primary array pointing group
    */
    
    std::string name = BF_SubArrayPointing::getName (pointingID);
    bool status      = openSubArrayPointing (pointingID,
					     flags);
    
    /*______________________________________________________
      Forward the function call to the next-lower
      hierarchical level structure.
    */

    it = itsSubarrayPointings.find(name);
    
    if (it==itsSubarrayPointings.end()) {
      std::cerr << "[BF_RootGroup::openBeam]"
		<< " Unable to open sub-array direction group " << name
		<< std::endl;
      status = false;
    } else {
      it->second.openBeam (beamID, flags);
    }

    return true;
  }

  //_____________________________________________________________________________
  //                                                          getSubArrayPointing
  
  /*!
    \param pointingID -- Identifier for the primary pointing direction.
  */
  BF_SubArrayPointing BF_RootGroup::getSubArrayPointing (unsigned int const &pointingID)
  {
    BF_SubArrayPointing pointing;

    if (H5Iis_valid(location_p)) {
      std::string name = BF_SubArrayPointing::getName (pointingID);
      std::map<std::string,BF_SubArrayPointing>::iterator it;
      
      it   = itsSubarrayPointings.find(name);

      if (it != itsSubarrayPointings.end()) {
	return it->second;
      } else {
	std::cerr << "[BF_RootGroup::primaryPointing] No such group "
		  << "\"" << name << "\""
		  << std::endl;
      }
    }
    else {
      std::cerr << "[BF_RootGroup::primaryPointing] Not connected to file!"
		<< std::endl;
    }
    
    return pointing;
  }

  //_____________________________________________________________________________
  //                                                                 getBeamGroup
  
  BF_BeamGroup BF_RootGroup::getBeamGroup (unsigned int const &pointingID,
					   unsigned int const &beamID)
  {
    BF_SubArrayPointing pointing = getSubArrayPointing(pointingID);
    return pointing.getBeamGroup(beamID);
  }
  
  //_____________________________________________________________________________
  //                                                            openStokesDataset
  
  bool BF_RootGroup::openStokesDataset (unsigned int const &pointingID,
					unsigned int const &beamID,
					unsigned int const &stokesID,
					IO_Mode const &flags)
  {
    bool status = true;
    
    /*______________________________________________________
      Open/Create primary array pointing group
    */

    status = openSubArrayPointing (pointingID,
				   flags);
    
    /*______________________________________________________
      Forward the function call to the next-lower
      hierarchical level structure.
    */
    
    std::string name;
    std::map<std::string,BF_SubArrayPointing>::iterator it;
    
    name = BF_SubArrayPointing::getName (pointingID);
    it   = itsSubarrayPointings.find(name);
    
    if (it==itsSubarrayPointings.end()) {
      status = false;
    } else {
      it->second.openStokesDataset (beamID, stokesID);
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                            openStokesDataset

  /*!
    \param pointingID  -- ID of the sub-array pointing group.
    \param beamID      -- ID of the beam group.
    \param stokesID    -- ID of the Stokes dataset.
    \param nofSamples  -- Number of bins along the time axis.
    \param nofSubbands -- Number of sub-bands.
    \param nofChannels -- Number of channels within the subbands.
    \param component   -- Stokes component stored within the dataset
    \param datatype    -- Datatype for the elements within the Dataset
    \param flags       -- I/o mode flags.
    \return status     -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool BF_RootGroup::openStokesDataset (unsigned int const &pointingID,
					unsigned int const &beamID,
					unsigned int const &stokesID,
					unsigned int const &nofSamples,
					unsigned int const &nofSubbands,
					unsigned int const &nofChannels,
					DAL::Stokes::Component const &component,
					hid_t const &datatype,
					IO_Mode const &flags)
  {
    std::vector<unsigned int> channels (nofSubbands,nofChannels);
    
    return openStokesDataset (pointingID,
			      beamID,
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
    \param pointingID  -- ID of the sub-array pointing group.
    \param beamID      -- ID of the beam group.
    \param stokesID    -- ID of the Stokes dataset.
    \param nofSamples  -- Number of bins along the time axis.
    \param nofChannels -- Number of channels within the subbands.
    \param component   -- Stokes component stored within the dataset
    \param datatype    -- Datatype for the elements within the Dataset
    \param flags       -- I/o mode flags.
    \return status     -- Status of the operation; returns \e false in case an
            error was encountered.
  */
  bool BF_RootGroup::openStokesDataset (unsigned int const &pointingID,
					unsigned int const &beamID,
					unsigned int const &stokesID,
					unsigned int const &nofSamples,
					std::vector<unsigned int> const &nofChannels,
					DAL::Stokes::Component const &component,
					hid_t const &datatype,
					IO_Mode const &flags)
  {
    bool status = true;
    
    /*______________________________________________________
      Open/Create sub-array pointing direction group.
    */
    
    status = openSubArrayPointing (pointingID, flags);
    
    /*______________________________________________________
      Forward the function call to the next-lower
      hierarchical level structure.
    */
    
    std::string name;
    std::map<std::string,BF_SubArrayPointing>::iterator it;
    
    name = BF_SubArrayPointing::getName (pointingID);
    it   = itsSubarrayPointings.find(name);
    
    if (it==itsSubarrayPointings.end()) {
      status = false;
    } else {
      status = it->second.openStokesDataset (beamID,
					     stokesID,
					     nofSamples,
					     nofChannels,
					     component,
					     datatype,
					     flags);
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                       sysLog
  
  SysLog BF_RootGroup::sysLog ()
  {
    std::map<std::string,SysLog>::iterator it;
    it = itsSystemLog.begin();
    return it->second;
  }


} // Namespace DAL -- end
