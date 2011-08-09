/***************************************************************************
 *   Copyright (C) 2010                                                    *
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

#include <data_hl/TBB_StationTrigger.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                             TBB_StationTrigger

  TBB_StationTrigger::TBB_StationTrigger ()
  {
    location_p = 0;
  }

  //_____________________________________________________________________________
  //                                                             TBB_StationTrigger

  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached.
    \param name -- Name of the structure (file, group, dataset, etc.) to be
           opened.
  */
  TBB_StationTrigger::TBB_StationTrigger (hid_t const &location,
					  std::string const &name,
					  IO_Mode const &flags)
  {
    open (location, name, flags);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  TBB_StationTrigger::~TBB_StationTrigger ()
  {
    destroy();
  }
  
  void TBB_StationTrigger::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  TBB_StationTrigger& TBB_StationTrigger::operator= (TBB_StationTrigger const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void TBB_StationTrigger::copy (TBB_StationTrigger const &other)
  {
    location_p  = other.location_p;
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void TBB_StationTrigger::summary (std::ostream &os)
  {
    os << "[TBB_StationTrigger] Summary of internal parameters." << std::endl;
    os << "-- Trigger table ID = " << location_p               << std::endl;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void TBB_StationTrigger::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("TRIGGER_TYPE");
    attributes_p.insert("TRIGGER_SOURCE");
    attributes_p.insert("TRIGGER_TIME");
    attributes_p.insert("TRIGGER_SAMPLE_NUMBER");
    attributes_p.insert("PARAM_COINCIDENCE_CHANNELS");
    attributes_p.insert("PARAM_COINCIDENCE_TIME");
    attributes_p.insert("PARAM_DIRECTION_FIT");
    attributes_p.insert("PARAM_ELEVATION_MIN");
    attributes_p.insert("PARAM_FIT_VARIANCE_MAX");
    attributes_p.insert("COINCIDENCE_CHANNELS");
    attributes_p.insert("RCU_ID");
    attributes_p.insert("TIME");
    attributes_p.insert("SAMPLE_NUMBER");
    attributes_p.insert("PULSE_SUM");
    attributes_p.insert("PULSE_WIDTH");
    attributes_p.insert("PULSE_PEAK");
    attributes_p.insert("PULSE_POWER_PRE");
    attributes_p.insert("PULSE_POWER_POST");
    attributes_p.insert("NOF_MISSED_TRIGGERS");
    attributes_p.insert("DIRECTION_AZIMUTH");
    attributes_p.insert("DIRECTION_ELEVATION");
    attributes_p.insert("DIRECTION_DISTANCE");
    attributes_p.insert("DIRECTION_VARIANCE");
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
  bool TBB_StationTrigger::open (hid_t const &location,
				 std::string const &name,
				 IO_Mode const &flags)
  {
    bool status (true);
    
    /* Internal setup */
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
      if ( (flags.flags() & IO_Mode::Create) ||
	   (flags.flags() & IO_Mode::CreateNew) ) {
	location_p = H5Gcreate (location,
				name.c_str(),
				H5P_DEFAULT,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* If creation was sucessful, add attributes with default values */
	if (location_p > 0) {
	  std::string grouptype ("StationTrigger");
	  std::string triggerType ("VHECR");
	  std::string triggerSource ("LCU");
	  std::vector<int> vecInt (1,0);
	  std::string simple ("simple");
	  // write the attributes
	  HDF5Attribute::write (location_p, "GROUPTYPE",                  grouptype     );
	  HDF5Attribute::write (location_p, "TRIGGER_TYPE",               triggerType   );
	  HDF5Attribute::write (location_p, "TRIGGER_SOURCE",             triggerSource );
	  HDF5Attribute::write (location_p, "TRIGGER_TIME",               int(0)        );
	  HDF5Attribute::write (location_p, "TRIGGER_SAMPLE_NUMBER",      int(0)        );
	  HDF5Attribute::write (location_p, "PARAM_COINCIDENCE_CHANNELS", int(0)        );
	  HDF5Attribute::write (location_p, "PARAM_COINCIDENCE_TIME",     float(0)      );
	  HDF5Attribute::write (location_p, "PARAM_DIRECTION_FIT",        simple        );
	  HDF5Attribute::write (location_p, "RCU_ID",                     vecInt        );
	  HDF5Attribute::write (location_p, "TIME",                       vecInt        );
	  HDF5Attribute::write (location_p, "SAMPLE_NUMBER",              vecInt        );
	  HDF5Attribute::write (location_p, "PULSE_SUM",                  vecInt        );
	  HDF5Attribute::write (location_p, "PULSE_WIDTH",                vecInt        );
	  HDF5Attribute::write (location_p, "PULSE_PEAK",                 vecInt        );
	  HDF5Attribute::write (location_p, "PULSE_POWER_PRE",            vecInt        );
	  HDF5Attribute::write (location_p, "PULSE_POWER_POST",           vecInt        );
	  HDF5Attribute::write (location_p, "NOF_MISSED_TRIGGERS",        vecInt        );
	} else {
#ifdef DAL_DEBUGGING_MESSAGES
	  std::cerr << "[TBB_StationTrigger::open] Failed to create group "
		    << name
		    << std::endl;
#endif
	  status = false;
	}
      } else {
#ifdef DAL_DEBUGGING_MESSAGES
	std::cerr << "[TBB_StationTrigger::open] Failed to open group "
		  << name
		  << std::endl;
#endif
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded ();
    }
#ifdef DAL_DEBUGGING_MESSAGES
    else {
      std::cerr << "[TBB_StationTrigger::open] Skip opening embedded groups!"
		<< std::endl;
    }
#endif
 
    return status;
  }
  
} // Namespace DAL -- end
