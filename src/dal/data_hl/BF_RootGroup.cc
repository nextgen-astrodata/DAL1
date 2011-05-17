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

#include "BF_RootGroup.h"

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
    : HDF5CommonInterface()
  {
    if (!open (0,filename,false)) {
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
    \param create   -- Create the corresponding data structure, if it does not 
           exist yet?
  */
  BF_RootGroup::BF_RootGroup (DAL::Filename &infile,
			  bool const &create)
    : HDF5CommonInterface()
  {
    if (!open (0,infile.filename(),create)) {
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
    \param create -- Create the corresponding data structure, if it does not 
           exist yet?
  */
  BF_RootGroup::BF_RootGroup (CommonAttributes const &attributes,
			  bool const &create)
  {
    if (!open (0,attributes.filename(),create)) {
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
  
  void BF_RootGroup::summary (std::ostream &os,
			      bool const &showAttributes)
  {
    os << "[BF_RootGroup] Summary of internal parameters." << std::endl;
    os << "-- Filename                  = " << itsFilename           << std::endl;
    os << "-- Location ID               = " << location_p            << std::endl;
    os << "-- nof. attributes           = " << attributes_p.size()   << std::endl;
    os << "-- nof. SysLog groups        = " << itsSystemLog.size()   << std::endl;
    os << "-- nof. primary pointings    = " << nofPrimaryPointings() << std::endl;
    
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
    attributes_p.insert("OBS_DATATYPE");
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
           structure is attached; parameter inherited through HDF5CommonInterface,
	   but not evaluated here.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
    \param create -- Create the corresponding data structure, if it does not 
           exist yet?
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool BF_RootGroup::open (hid_t const &location,
			 std::string const &name,
			 bool const &create)
  {
    bool status (true);

    // Initialize private variables ________________________
    location_p  = location;
    itsFilename = name;
    setAttributes();
    itsSubarrayPointings.clear();
    itsSystemLog.clear();
    
    // Try to open the file ________________________________

    std::ifstream infile;
    infile.open (name.c_str(), std::ifstream::in);

    if (infile.is_open() && infile.good()) {
      // If the file already exists, close it ...
      infile.close();
      // ... and open it as HDF5 file
      location_p = H5Fopen (name.c_str(),
			    H5F_ACC_RDWR,
			    H5P_DEFAULT);
    } else {
      infile.close();
      location_p = 0;
    }
    
    if (location_p > 0) {
      // Read the common LOFAR attributes
      itsCommonAttributes.h5read(location_p);
    } else {
      /* If failed to open file, check if we are supposed to create one */
      if (create) {
	location_p = H5Fcreate (name.c_str(),
				H5F_ACC_TRUNC,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* Write LOFAR common attribute to the root group of the file */
	itsCommonAttributes.h5write(location_p);
	/* Write the additional attributes attached to the root group */
	std::string undefined ("UNDEFINED");
	std::vector<float> vectF (1,0.0);
	std::vector<double> vectD (1,0.0);
	bool trueBool (true);
	//
	HDF5Attribute::write (location_p,"FILENAME",                  name        );
 	HDF5Attribute::write (location_p,"CREATE_OFFLINE_ONLINE",     trueBool    );
 	HDF5Attribute::write (location_p,"BF_FORMAT",                 undefined   );
	HDF5Attribute::write (location_p,"BF_VERSION",                undefined   );
	HDF5Attribute::write (location_p,"EXPTIME_START_UTC",         undefined   );
	HDF5Attribute::write (location_p,"EXPTIME_STOP_UTC",          undefined   );
	HDF5Attribute::write (location_p,"EXPTIME_START_MJD",         undefined   );
	HDF5Attribute::write (location_p,"EXPTIME_STOP_MJD",          undefined   );
	HDF5Attribute::write (location_p,"EXPTIME_START_TAI",         undefined   );
	HDF5Attribute::write (location_p,"EXPTIME_STOP_TAI",          undefined   );
	HDF5Attribute::write (location_p,"TOTAL_INTEGRATION_TIME",    float(0.0)  );
	HDF5Attribute::write (location_p,"OBS_DATATYPE",              undefined   );
	HDF5Attribute::write (location_p,"PRIMARY_POINTING_DIAMETER", float(0.0)  );
	HDF5Attribute::write (location_p,"BANDWIDTH",                 double(0.0) );
	HDF5Attribute::write (location_p,"BEAM_DIAMETER",             float(0.0)  );
	HDF5Attribute::write (location_p,"WEATHER_TEMPERATURE",       vectF       );
	HDF5Attribute::write (location_p,"WEATHER_HUMIDITY",          vectF       );
	HDF5Attribute::write (location_p,"SYSTEM_TEMPERATURE",        vectF       );
	HDF5Attribute::write (location_p,"NOF_PRIMARY_BEAMS",         int(0)      );
	/* Read back in the common attributes after storing default values */
	itsCommonAttributes.h5read(location_p);
      } else {
	std::cerr << "[BF_RootGroup::open] Failed to open file "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (create);
    } else {
      std::cerr << "[BF_RootGroup::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  /*!
    \return status -- Status of the operation; returns <tt>False</tt> in case
            no primary pointing direction groups were found.
   */
  bool BF_RootGroup::openEmbedded (bool const &create)
  {
    bool status (true);
    std::set<std::string> groups;
    std::set<std::string>::iterator it;

    /* Retrieve the names of the groups attached to the root group */
    status = h5get_names (groups,
			  location_p,
			  H5G_GROUP);
    
    /* Open system log group ... */
    status = openSysLog (create);
    /* ... and remove its name from the previously retrieved list */
    groups.erase("SysLog");

    /* Open the PrimaryPointingDirection groups */
    for (it=groups.begin(); it!=groups.end(); ++it) {
      status *= openPrimaryPointing (*it);
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                   openSysLog
  
  bool BF_RootGroup::openSysLog (bool const &create)
  {
    bool status (true);

    if (itsSystemLog.size() == 0 && location_p > 0) {
      itsSystemLog["SysLog"] = SysLog (location_p,create);
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                          openPrimaryPointing
  
  /*!
    \param pointingID -- ID of the sub-array pointing group.
    \param create     -- Create new group, if not existing yet?
  */
  bool BF_RootGroup::openPrimaryPointing (unsigned int const &pointingID,
					  bool const &create)
  {
    bool status (true);
    htri_t validLocation = H5Iis_valid(location_p);
    std::string name = BF_SubArrayPointing::getName (pointingID);

    if (location_p > 0 && validLocation) {
      int nofPrimaryBeams;
      std::map<std::string,BF_SubArrayPointing>::iterator it;
      // convert station ID to group name
      it   = itsSubarrayPointings.find(name);
      // check if the station beam group indeed exists
      if (it == itsSubarrayPointings.end()) {
	// open/create the station beam group
	itsSubarrayPointings[name] = BF_SubArrayPointing (location_p,
							pointingID,
							create);
	// attributes for book-keeping
	nofPrimaryBeams = itsSubarrayPointings.size();
	HDF5Attribute::write (location_p,
			      "NOF_PRIMARY_BEAMS",
			      nofPrimaryBeams);
      }
    }
    else {
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                          openPrimaryPointing
  
  /*!
    \param name   -- 
    \param create -- 
  */
  bool BF_RootGroup::openPrimaryPointing (std::string const &name)
  {
    bool status (true);

    if (location_p > 0 && H5Iis_valid(location_p)) {
      std::map<std::string,BF_SubArrayPointing>::iterator it;
      // convert station ID to group name
      it = itsSubarrayPointings.find(name);
      // check if the station beam group indeed exists
      if (it == itsSubarrayPointings.end()) {
	// open the primary pointing direction group
	itsSubarrayPointings[name] = BF_SubArrayPointing (location_p,name);
      }
    }
    else {
      std::cerr << "[BF_RootGroup::openPrimaryPointing] Not attached to file!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                     openBeam
  
  /*!
    \param pointingID -- Identifier for the primary pointing direction.
    \param beamID     -- Identifier for the beam within the primary pointing
           direction.
    \param create     -- Create the group if it does not exist yet?
  */
  bool BF_RootGroup::openBeam (unsigned int const &pointingID,
			     unsigned int const &beamID,
			     bool const &create)
  {
    bool status (true);
    htri_t validLocation = H5Iis_valid(location_p);

    if (location_p > 0 && validLocation) {
      /* Open PrimaryPointing group */
      status = openPrimaryPointing (pointingID, create);

      /* Open Beam group */
      if (status) {
	std::string name;
	std::map<std::string,BF_SubArrayPointing>::iterator it;
	// get pointer to PrimaryPointing object
	name = BF_SubArrayPointing::getName (pointingID);
	it   = itsSubarrayPointings.find(name);
	// forward function call to open beam
	if ( it != itsSubarrayPointings.end() ) {
	  it->second.openBeam(beamID,create);
	}
	else {
	  std::cout << "[BF_RootGroup::openBeam] " << BF_BeamGroup::getName(beamID)
		    << " already exists."
		    << std::endl;
	}
      } else {
	std::cerr << "[BF_RootGroup::openBeam] Failed to open StationGroup!"
		  << std::endl;
	status = false;
      }
    }  //  end -- (location_p>0)
    else {
      std::cerr << "[BF_RootGroup::openBeam] No connection to dataset!"
		<< std::endl;
      std::cerr << "-- Location ID       = " << location_p    << std::endl;
      std::cerr << "-- Valid HDF5 object = " << validLocation << std::endl;
      status = false;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                              primaryPointing
  
  /*!
    \param pointingID -- Identifier for the primary pointing direction.
  */
  BF_SubArrayPointing BF_RootGroup::primaryPointing (unsigned int const &pointingID)
  {
    BF_SubArrayPointing pointing;      
      
    if (H5Iis_valid(location_p)) {
      std::string name;
      std::map<std::string,BF_SubArrayPointing>::iterator it;
      
      name = BF_SubArrayPointing::getName (pointingID);
      it   = itsSubarrayPointings.find(name);

      if (it != itsSubarrayPointings.end()) {
	pointing = it->second;
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
  //                                                                       sysLog
  
  SysLog BF_RootGroup::sysLog ()
  {
    std::map<std::string,SysLog>::iterator it;
    it = itsSystemLog.begin();
    return it->second;
  }


} // Namespace DAL -- end
