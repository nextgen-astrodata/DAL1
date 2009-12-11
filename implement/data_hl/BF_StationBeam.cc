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

#include <BF_StationBeam.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                               BF_StationBeam

  BF_StationBeam::BF_StationBeam ()
  {
    location_p = 0;
  }

  //_____________________________________________________________________________
  //                                                               BF_StationBeam

  BF_StationBeam::BF_StationBeam (hid_t const &location,
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
  
  BF_StationBeam::~BF_StationBeam ()
  {
    destroy();
  }
  
  void BF_StationBeam::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                      summary
  
  void BF_StationBeam::summary (std::ostream &os)
  {
    os << "[BF_StationBeam] Summary of internal parameters." << std::endl;
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
  
  void BF_StationBeam::setAttributes ()
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
    attributes_p.insert("NOF_PENCIL_BEAMS");
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
  bool BF_StationBeam::open (hid_t const &location,
			     std::string const &name,
			     bool const &create)
  {
    bool status (true);

    /* Set up the list of attributes attached to the root group */
    setAttributes();

    /* Get the list of groups attached to this group */
    std::set<std::string> groups;
    h5get_names (groups,location,H5G_GROUP);

    if (static_cast<bool>(groups.count(name))) {
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
	  std::string grouptype ("StatBeam");
	  std::string mhz ("MHz");
	  std::string tracking ("OFF");
	  std::string second ("s");
	  std::string usecond ("us");
	  std::vector<std::string> stationsList (1,"UNDEFINED");
	  // write the attributes
	  h5set_attribute (location_p,"GROUPTYPE",                grouptype   );
	  h5set_attribute (location_p,"NOF_STATIONS",             int(0)      );
	  h5set_attribute (location_p,"STATIONS_LIST",            stationsList);
	  h5set_attribute (location_p,"POINT_RA",                 float(0.0)  );
	  h5set_attribute (location_p,"POINT_DEC",                float(0.0)  );
	  h5set_attribute (location_p,"TRACKING",                 tracking    );
	  h5set_attribute (location_p,"POINT_ALTITUDE",           float(0.0)  );
	  h5set_attribute (location_p,"POINT_AZIMUTH",            float(0.0)  );
	  h5set_attribute (location_p,"CLOCK_RATE",               int(0)      );
	  h5set_attribute (location_p,"CLOCK_RATE_UNIT",          mhz         );
	  h5set_attribute (location_p,"NOF_SAMPLES",              int(0)      );
	  h5set_attribute (location_p,"SAMPLING_RATE",            float(0.0)  );
	  h5set_attribute (location_p,"SAMPLING_RATE_UNIT",       mhz         );
	  h5set_attribute (location_p,"SAMPLING_TIME",            float(0.0)  );
	  h5set_attribute (location_p,"SAMPLING_TIME_UNIT",       usecond     );
	  h5set_attribute (location_p,"TOTAL_INTEGRATION_TIME",   float(0.0)  );
	  h5set_attribute (location_p,"TOTAL_INTEGRATION_TIME_UNIT", second   );
	  h5set_attribute (location_p,"CHANNELS_PER_SUBBAND",     int(0)      );
	  h5set_attribute (location_p,"SUBBAND_WIDTH",            float(0.0)  );
	  h5set_attribute (location_p,"SUBBAND_WIDTH_UNIT",       mhz         );
	  h5set_attribute (location_p,"CHANNEL_WIDTH",            float(0.0)  );
	  h5set_attribute (location_p,"CHANNEL_WIDTH_UNIT",       mhz         );
	  h5set_attribute (location_p,"NOF_PENCIL_BEAMS",         int(0)      );
	} else {
	  std::cerr << "[BF_StationBeam::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[BF_StationBeam::open] Failed to open group "
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
  
  bool BF_StationBeam::openEmbedded (bool const &create)
  {
    bool status (true);
    std::set<std::string> groupnames;
    
    /* Retrieve the names of the groups attached to the StationBeam group */
    status = h5get_names (groupnames,
			  location_p,
			  H5G_GROUP);

    return status;
  }

  //_____________________________________________________________________________
  //                                                               openPencilBeam
  
  bool BF_StationBeam::openPencilBeam (unsigned int const &pencilID,
				       bool const &create)
  {
    bool status      = true;
    std::string name = BF_PencilBeam::getName (pencilID);
    size_t haveGroup = pencilBeams_p.count (name);

    try {
      // create the PencilBeam group ...
      if (location_p > 0 && haveGroup == 0) {
	pencilBeams_p[name] = BF_PencilBeam (location_p,pencilID,create);
      }
      // ... and do the book-keeping
      int nofBeams = pencilBeams_p.size();
      h5set_attribute (location_p,"NOF_PENCIL_BEAMS",nofBeams);
    } catch (std::string message) {
      std::cerr << "[BF_StationBeam::openPencilBeam] " << message << std::endl;
      status = false;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                      getName
  
  /*!
    \param index -- Index identifying the station beam.

    \return name -- The name of the station beam group,
            <tt>StationBeam<index></tt>
  */
  std::string BF_StationBeam::getName (unsigned int const &index)
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
