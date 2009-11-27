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
  
  BF_StationBeam::BF_StationBeam ()
  {;}
  
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

    /* Try to open the group */
    location_p = H5Gopen2 (location,
			   name.c_str(),
			   H5P_DEFAULT);
    
    if (location_p > 0) {
      status = true;
    } else {
      /* If failed to open the group, check if we are supposed to create one */
      if (create) {
	location_p = H5Gcreate (location,
				name.c_str(),
				H5F_ACC_TRUNC,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* If creation was sucessful, add attributes with default values */
	if (location_p > 0) {
	  std::vector<std::string> vect (1,"UNDEFINED");
	  std::string string_group ("StatBeam");
	  std::string string_mhz ("MHz");
	  std::string string_off ("OFF");
	  std::string string_s ("s");
	  std::string string_us ("us");
	  // write the attributes
	  h5set_attribute (location_p,"GROUPTYPE",                string_group );
	  h5set_attribute (location_p,"NOF_STATIONS",             int(0)     );
	  h5set_attribute (location_p,"STATIONS_LIST",            vect       );
	  h5set_attribute (location_p,"POINT_RA",                 float(0.0) );
	  h5set_attribute (location_p,"POINT_DEC",                float(0.0) );
	  h5set_attribute (location_p,"TRACKING",                 string_off );
	  h5set_attribute (location_p,"POINT_ALTITUDE",           float(0.0) );
	  h5set_attribute (location_p,"POINT_AZIMUTH",            float(0.0) );
	  h5set_attribute (location_p,"CLOCK_RATE",               int(0)     );
	  h5set_attribute (location_p,"CLOCK_RATE_UNIT",          string_mhz );
	  h5set_attribute (location_p,"NOF_SAMPLES",              int(0)     );
	  h5set_attribute (location_p,"SAMPLING_RATE",            float(0.0) );
	  h5set_attribute (location_p,"SAMPLING_RATE_UNIT",       string_mhz );
	  h5set_attribute (location_p,"SAMPLING_TIME",            float(0.0) );
	  h5set_attribute (location_p,"SAMPLING_TIME_UNIT",       string_us  );
	  h5set_attribute (location_p,"TOTAL_INTEGRATION_TIME",   float(0.0) );
	  h5set_attribute (location_p,"TOTAL_INTEGRATION_TIME_UNIT", string_s );
	  h5set_attribute (location_p,"CHANNELS_PER_SUBBAND",     int(0)     );
	  h5set_attribute (location_p,"SUBBAND_WIDTH",            float(0.0) );
	  h5set_attribute (location_p,"SUBBAND_WIDTH_UNIT",       string_mhz );
	  h5set_attribute (location_p,"CHANNEL_WIDTH",            float(0.0) );
	  h5set_attribute (location_p,"CHANNEL_WIDTH_UNIT",       string_mhz );
	  h5set_attribute (location_p,"NOF_PENCIL_BEAMS",         int(0)     );
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
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool BF_StationBeam::openEmbedded (bool const &create)
  {
    return false;
  }
  
  
} // Namespace DAL -- end
