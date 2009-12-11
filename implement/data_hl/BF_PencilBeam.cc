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

#include <BF_PencilBeam.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                BF_PencilBeam

  BF_PencilBeam::BF_PencilBeam ()
  {
    location_p = 0;
  }

  //_____________________________________________________________________________
  //                                                                BF_PencilBeam

  BF_PencilBeam::BF_PencilBeam (hid_t const &location,
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
  
  BF_PencilBeam::~BF_PencilBeam ()
  {
    destroy();
  }
  
  void BF_PencilBeam::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                      summary
  
  void BF_PencilBeam::summary (std::ostream &os)
  {
    os << "[BF_PencilBeam] Summary of internal parameters." << std::endl;
  }
  
  
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void BF_PencilBeam::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("TARGET");
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
  bool BF_PencilBeam::open (hid_t const &location,
			    std::string const &name,
			    bool const &create)
  {
    bool status (true);

    /* Set up the list of attributes attached to the PencilBeam group */
    setAttributes();

    /* Try to open the group: get list of groups attached to 'location' and
       check if 'name' is part of it.
    */
    std::set<std::string> groups;
    h5get_names (groups,location,H5G_GROUP);
    if (static_cast<bool>(groups.count(name))) {
      location_p = H5Gopen2 (location,
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
	  std::string grouptype ("PencilBeam");
	  std::string undefined ("UNDEFINED");
	  bool ok (false);
	  std::vector<std::string> stokes (1,undefined);
	  // write the attributes
	  h5set_attribute (location_p,"GROUPTYPE",                grouptype  );
	  h5set_attribute (location_p,"TARGET",                   undefined  );
	  h5set_attribute (location_p,"POINT_RA",                 float(0.0) );
	  h5set_attribute (location_p,"POINT_DEC",                float(0.0) );
	  h5set_attribute (location_p,"POSITION_OFFSET_RA",       float(0.0) );
	  h5set_attribute (location_p,"POSITION_OFFSET_DEC",      float(0.0) );
	  h5set_attribute (location_p,"PB_DIAMETER_RA",           float(0.0) );
	  h5set_attribute (location_p,"PB_DIAMETER_DEC",          float(0.0) );
	  h5set_attribute (location_p,"PB_CENTER_FREQUENCY",      float(0.0) );
	  h5set_attribute (location_p,"PB_CENTER_FREQUENCY_UNIT", undefined  );
	  h5set_attribute (location_p,"FOLDED_DATA",              ok         );
	  h5set_attribute (location_p,"FOLD_PERIOD",              float(0.0) );
	  h5set_attribute (location_p,"FOLD_PERIOD_UNIT",         undefined  );
	  h5set_attribute (location_p,"DEDISPERSION",             undefined  );
	  h5set_attribute (location_p,"DISPERSION_MEASURE",       float(0.0) );
 	  h5set_attribute (location_p,"DISPERSION_MEASURE_UNIT",  undefined  );
 	  h5set_attribute (location_p,"BARYCENTER",               ok         );
 	  h5set_attribute (location_p,"STOKES_COMPONENTS",        stokes     );
 	  h5set_attribute (location_p,"COMPLEX_VOLTAGE",          ok         );
	  h5set_attribute (location_p,"SIGNAL_SUM",               undefined  );
	} else {
	  std::cerr << "[BF_PencilBeam::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[BF_PencilBeam::open] Failed to open group "
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
  
  bool BF_PencilBeam::openEmbedded (bool const &create)
  {
    bool status (true);
    std::set<std::string> groupnames;
    
    /* Retrieve the names of the groups attached to the StationBeam group */
    status = h5get_names (groupnames,
			  location_p,
			  H5G_GROUP);

    /* Open the processing history group */
    status = openProcessingHistory (create);
    /* Open coordinates group */
    status = openCoordinatesGroup (create);

    return status;
  }

  //_____________________________________________________________________________
  //                                                        openProcessingHistory
  
  bool BF_PencilBeam::openProcessingHistory (bool const &create)
  {
    bool status (true);

    if (processingHistory_p.size() == 0 && location_p > 0) {
      processingHistory_p["ProcessingHistory"] = BF_ProcessingHistory (location_p,create);
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                         openCoordinatesGroup
  
  bool BF_PencilBeam::openCoordinatesGroup (bool const &create)
  {
    bool status (true);

    if (coordinates_p.size() == 0 && location_p > 0) {
      coordinates_p["CoordinatesGroup"] = CoordinatesGroup (location_p,create);
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                      getName
  
  /*!
    \param index -- Index identifying the pencil beam.

    \return name -- The name of the pencil beam group,
            <tt>PencilBeam<index></tt>
  */
  std::string BF_PencilBeam::getName (unsigned int const &index)
  {
    char uid[10];
    sprintf(uid,
            "%03d",
	    index);
    
    std::string name (uid);
    
    name = "PencilBeam" + name;
    
    return name;
  }
  
} // Namespace DAL -- end
