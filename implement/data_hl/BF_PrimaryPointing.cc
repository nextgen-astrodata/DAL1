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

#include <BF_PrimaryPointing.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                           BF_PrimaryPointing
  
  BF_PrimaryPointing::BF_PrimaryPointing ()
  {
    location_p = 0;
  }
  
  //_____________________________________________________________________________
  //                                                           BF_PrimaryPointing
  
  BF_PrimaryPointing::BF_PrimaryPointing (hid_t const &location,
					  unsigned int const &index,
					  bool const &create)
  {
    open (location,getName(index),create);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  BF_PrimaryPointing::~BF_PrimaryPointing ()
  {
    if (location_p > 0) {
      // clear maps with embedded objects
      beams_p.clear();
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
  
  void BF_PrimaryPointing::summary (std::ostream &os)
  {
    os << "[BF_PrimaryPointing] Summary of internal parameters." << std::endl;
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
  
  void BF_PrimaryPointing::setAttributes ()
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
  bool BF_PrimaryPointing::open (hid_t const &location,
				 std::string const &name,
				 bool const &create)
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
      if (create) {
	location_p = H5Gcreate (location,
				name.c_str(),
				H5P_DEFAULT,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* If creation was sucessful, add attributes with default values */
	if (location_p > 0) {
	  std::string grouptype ("PrimaryPointingDirection");
	  std::string mhz ("MHz");
	  std::string tracking ("OFF");
	  std::string second ("s");
	  std::string usecond ("us");
	  std::vector<std::string> stationsList (1,"UNDEFINED");
	  // write the attributes
	  h5set_attribute (location_p,"GROUPTYPE",                grouptype   );
	  h5set_attribute (location_p,"NOF_STATIONS",             int(0)      );
	  h5set_attribute (location_p,"STATIONS_LIST",            stationsList);
	  h5set_attribute (location_p,"POINT_RA",                 double(0.0) );
	  h5set_attribute (location_p,"POINT_DEC",                double(0.0) );
	  h5set_attribute (location_p,"TRACKING",                 tracking    );
	  h5set_attribute (location_p,"POINT_ALTITUDE",           double(0.0) );
	  h5set_attribute (location_p,"POINT_AZIMUTH",            double(0.0) );
	  h5set_attribute (location_p,"CLOCK_RATE",               double(0.0) );
	  h5set_attribute (location_p,"CLOCK_RATE_UNIT",          mhz         );
	  h5set_attribute (location_p,"NOF_SAMPLES",              int(0)      );
	  h5set_attribute (location_p,"SAMPLING_RATE",            double(0.0) );
	  h5set_attribute (location_p,"SAMPLING_RATE_UNIT",       mhz         );
	  h5set_attribute (location_p,"SAMPLING_TIME",            float(0.0)  );
	  h5set_attribute (location_p,"SAMPLING_TIME_UNIT",       usecond     );
	  h5set_attribute (location_p,"TOTAL_INTEGRATION_TIME",   double(0.0) );
	  h5set_attribute (location_p,"TOTAL_INTEGRATION_TIME_UNIT", second   );
	  h5set_attribute (location_p,"CHANNELS_PER_SUBBAND",     int(0)      );
	  h5set_attribute (location_p,"SUBBAND_WIDTH",            double(0.0) );
	  h5set_attribute (location_p,"SUBBAND_WIDTH_UNIT",       mhz         );
	  h5set_attribute (location_p,"CHANNEL_WIDTH",            double(0.0) );
	  h5set_attribute (location_p,"CHANNEL_WIDTH_UNIT",       mhz         );
	  h5set_attribute (location_p,"NOF_BEAMS",                int(0)      );
	} else {
	  std::cerr << "[BF_PrimaryPointing::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[BF_PrimaryPointing::open] Failed to open group "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (create);
    } else {
      std::cerr << "[BF_PrimaryPointing::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool BF_PrimaryPointing::openEmbedded (bool const &create)
  {
    bool status (create);
    std::set<std::string> groupnames;
    std::set<std::string>::iterator it;
    
    /* Retrieve the names of the groups attached to the PrimaryPointing group */
    status = h5get_names (groupnames,
			  location_p,
			  H5G_GROUP);
    
    if (groupnames.size() > 0) {
      for (it=groupnames.begin(); it!=groupnames.end(); ++it) {
	beams_p[*it] = BF_Beam (location_p,*it);
      }
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                     openBeam
  
  /*!
    \param beamID -- Identifier for the Beam within the Primary Pointing 
           Direction group.
    \param create -- 
  */
  bool BF_PrimaryPointing::openBeam (unsigned int const &beamID,
				     bool const &create)
  {
    bool status          = true;
    htri_t validLocation = H5Iis_valid(location_p);
    std::string name     = BF_Beam::getName (beamID);
    
    if (location_p > 0 && validLocation) {
      // open Beam group
      BF_Beam beam (location_p,beamID,create);
      beams_p[name] = beam;
      // book-keeping
      int nofBeams = beams_p.size();
      h5set_attribute (location_p,"NOF_BEAMS",nofBeams);
    } else {
      std::cerr << "[BF_PrimaryPointing::openBeam] Not connected to dataset."
		<< std::endl;
      std::cerr << "-- Location ID       = " << location_p    << std::endl;
      std::cerr << "-- Valid HDF5 object = " << validLocation << std::endl;
      std::cerr << "-- Beam group        = " << name          << std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      getName
  
  /*!
    \param index -- Index identifying the station beam.
    
    \return name -- The name of the station beam group,
            <tt>PrimaryPointing<index></tt>
  */
  std::string BF_PrimaryPointing::getName (unsigned int const &index)
  {
    char uid[10];
    sprintf(uid,
            "%03d",
	    index);
    
    std::string name (uid);
    
    name = "PrimaryPointingDirection" + name;
    
    return name;
  }
  
} // Namespace DAL -- end
