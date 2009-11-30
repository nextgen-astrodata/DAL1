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

#include <BF_Dataset.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                   BF_Dataset

  /*!
    \param filename -- Name of the dataset to open.
  */
  BF_Dataset::BF_Dataset (std::string const &filename)
  {
    if (!open (0,filename,false)) {
      std::cerr << "[BF_Dataset::BF_Dataset] Failed to open file "
		<< filename
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                   BF_Dataset

  /*!
    \param filename -- Filename object from which the actual file name of the
           dataset is derived.
  */
  BF_Dataset::BF_Dataset (Filename const &filename)
  {
    CommonAttributes attr;
    attr.setFilename (filename);
    init (attr);
  }
  
  //_____________________________________________________________________________
  //                                                                   BF_Dataset

  BF_Dataset::BF_Dataset (CommonAttributes const &commonAttributes)
  {
    init (commonAttributes);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  BF_Dataset::~BF_Dataset ()
  {
    destroy();
  }
  
  void BF_Dataset::destroy ()
  {
    if (location_p > 0) {
      herr_t h5error = H5Fclose (location_p);
      if (h5error != 0) {
	std::cerr << "Error closing HDF5 dataset." << std::endl;
      }
    }
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                          setCommonAttributes
  
  /*!
    \param attributes -- Set of LOFAR common attributes to be attached to the 
           root group of the beamformed dataset.

    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool BF_Dataset::setCommonAttributes (CommonAttributes const &attributes)
  {
    bool status (true);

    try {
      commonAttributes_p = attributes;
    } catch (std::string message) {
      std::cerr << "[BF_Dataset::setCommonAttributes] " << message << std::endl;
      status = false;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  void BF_Dataset::summary (std::ostream &os)
  {
    os << "[BF_Dataset] Summary of internal parameters." << std::endl;
    os << "-- Filename        = " << commonAttributes_p.filename() << std::endl;
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
  //                                                                         init
  
  void BF_Dataset::init (CommonAttributes const &attributes)
  {
    /* Store the common attributes */
    setCommonAttributes (attributes);
    /* Set up the list of attributes attached to the root group */
    setAttributes();
    /* Try opening the file holding the dataset */
    if (!open (0,commonAttributes_p.filename(),true)) {
      std::cerr << "[BF_Dataset::init] Initialization of object failed!"
		<< " Unable to access file holding the dataset!"
		<< std::endl;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached; parameter inherited through CommonInterface,
	   but not evaluated here.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
    \param create -- Create the corresponding data structure, if it does not 
           exist yet?
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool BF_Dataset::open (hid_t const &location,
			 std::string const &name,
			 bool const &create)
  {
    bool status (true);

    /* Initialize private variables*/
    location_p = location;
    stationBeams_p.clear();
    sysLog_p.clear();

    /* Try to open the file */
    location_p = H5Fopen (name.c_str(),
			  H5F_ACC_RDWR,
			  H5P_DEFAULT);
    
    if (location_p > 0) {
      // Read the common LOFAR attributes
      commonAttributes_p.h5read(location_p);
    } else {
      /* If failed to open file, check if we are supposed to create one */
      if (create) {
	location_p = H5Fcreate (name.c_str(),
				H5F_ACC_TRUNC,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* Write the common attributes attached to the root group */
	commonAttributes_p.h5write(location_p);
	/* Write the additional attributes attached to the root group */
	std::string undefined ("UNDEFINED");
	std::vector<float> vect (1,0.0);
	//
 	h5set_attribute (location_p,"CREATE_OFFLINE_ONLINE",true);
 	h5set_attribute (location_p,"BF_FORMAT",undefined);
	h5set_attribute (location_p,"BF_VERSION",undefined);
	h5set_attribute (location_p,"EXPTIME_START_UTC",undefined);
	h5set_attribute (location_p,"EXPTIME_STOP_UTC",undefined);
	h5set_attribute (location_p,"TOTAL_INTEGRATION_TIME",float(0.0));
	h5set_attribute (location_p,"OBS_DATATYPE",undefined);
	h5set_attribute (location_p,"STATION_BEAM_DIAMETER",int(0));
	h5set_attribute (location_p,"BANDWIDTH",float(0.0));
	h5set_attribute (location_p,"PENCIL_BEAM_DIAMETER",int(0));
 	h5set_attribute (location_p,"WEATHER_TEMPERATURE",vect);
	h5set_attribute (location_p,"WEATHER_HUMIDITY",vect);
 	h5set_attribute (location_p,"SYSTEM_TEMPERATURE",vect);
 	h5set_attribute (location_p,"PHASE_CENTER",vect);
	h5set_attribute (location_p,"NOF_BEAMS",int(0));
      } else {
	std::cerr << "[BF_Dataset::open] Failed to open file "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    openEmbedded (create);
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void BF_Dataset::setAttributes ()
  {
    attributes_p.clear();
    
    attributes_p.insert("CREATE_OFFLINE_ONLINE");
    attributes_p.insert("BF_FORMAT");
    attributes_p.insert("BF_VERSION");
    attributes_p.insert("EXPTIME_START_UTC");
    attributes_p.insert("EXPTIME_STOP_UTC");
    attributes_p.insert("TOTAL_INTEGRATION_TIME");
    attributes_p.insert("OBS_DATATYPE");
    attributes_p.insert("BANDWIDTH");
    attributes_p.insert("STATION_BEAM_DIAMETER");
    attributes_p.insert("PENCIL_BEAM_DIAMETER");
    attributes_p.insert("WEATHER_TEMPERATURE");
    attributes_p.insert("WEATHER_HUMIDITY");
    attributes_p.insert("SYSTEM_TEMPERATURE");
    attributes_p.insert("PHASE_CENTER");
    attributes_p.insert("NOF_BEAMS");
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  /*!
    \return status -- Status of the operation; returns <tt>False</tt> in case
            no station groups were found.
   */
  bool BF_Dataset::openEmbedded (bool const &create)
  {
    bool status (true);
    std::set<std::string> groupnames;
    
    /* Retrieve the names of the groups attached to the root group */
    status = h5get_names (groupnames,
			  location_p,
			  H5G_GROUP);

    std::cout << "[BF_Dataset::openEmbedded]" << std::endl;
    std::cout << "-- Location ID = " << location_p        << std::endl;
    std::cout << "-- nof. groups = " << groupnames.size() << std::endl;
    
    if (status && groupnames.size()>0) {
      /* Open system log */
      status = openSysLog (create);
      /* Open StationBeam groups */
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                   openSysLog
  
  bool BF_Dataset::openSysLog (bool const &create)
  {
    bool status (true);

    if (sysLog_p.size() == 0 && location_p > 0) {
      sysLog_p["SysLog"] = BF_SysLog (location_p,"SysLog",create);
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                            stationBeamGroups
  
  std::vector<std::string> BF_Dataset::stationBeamGroups ()
  {
    std::vector<std::string> names (nofStationBeams());
    std::map<std::string,BF_StationBeam>::iterator n;
    
    /* Iterate through the map containing the station beam groups and extract the
       group name.
    */
    n = stationBeams_p.begin();
    
    return names;
  }
  
  //_____________________________________________________________________________
  //                                                                 stationBeams

  std::vector<BF_StationBeam> BF_Dataset::stationBeams ()
  {
    std::vector<BF_StationBeam> beams (nofStationBeams());
    std::map<std::string,BF_StationBeam>::iterator n;

    return beams;
  }

  //_____________________________________________________________________________
  //                                                              stationBeamName
  
  std::string BF_Dataset::stationBeamName (unsigned int const &index)
  {
    char uid[10];
    sprintf(uid,
            "%03d",
	    index);

    std::string name (uid);

    name = "StationBeam" + name;

    return name;
  }

} // Namespace DAL -- end
