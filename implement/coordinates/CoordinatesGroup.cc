/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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

#include <CoordinatesGroup.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                             CoordinatesGroup
  
  CoordinatesGroup::CoordinatesGroup ()
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                             CoordinatesGroup
  
  CoordinatesGroup::CoordinatesGroup (hid_t const &location,
				      bool const &create)
  {
    init ();
    open (location,
	  groupType_p,
	  create);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  CoordinatesGroup::~CoordinatesGroup ()
  {
    destroy();
  }
  
  void CoordinatesGroup::destroy ()
  {
    if (location_p > 0) {
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
  
  void CoordinatesGroup::summary (std::ostream &os)
  {
    os << "[CoordinatesGroup] Summary of internal parameters." << std::endl;
    os << "-- Location ID         = " << location_p            << std::endl;
    os << "-- Ref. location value = " << refLocationValue_p    << std::endl;
    os << "-- Ref. location unit  = " << refLocationUnit_p     << std::endl;
    os << "-- Ref. location frame = " << refLocationFrame_p    << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  void CoordinatesGroup::init ()
  {
    std::string undefined ("UNDEFINED");

    /* Variables declared by the base class*/
    location_p = 0;

    /* Internal variables*/
    groupType_p        = "CoordinatesGroup";
    refLocationValue_p = std::vector<double> (3, 0.0);
    refLocationUnit_p  = std::vector<std::string> (3, undefined);
    refLocationFrame_p = undefined;
    nofCoordinates_p   = 0;
    nofAxes_p          = 0;
    coordinateTypes_p  = std::vector<std::string> (1, undefined);
  }

  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void CoordinatesGroup::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("REF_LOCATION_VALUE");
    attributes_p.insert("REF_LOCATION_UNIT");
    attributes_p.insert("REF_LOCATION_FRAME");
    attributes_p.insert("NOF_COORDINATES");
    attributes_p.insert("NOF_AXES");
    attributes_p.insert("COORDINATE_TYPES");
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
  bool CoordinatesGroup::open (hid_t const &location,
			       std::string const &name,
			       bool const &create)
  {
    bool status (true);

    /* Set up the list of attributes attached to the root group */
    setAttributes();

    /* Try to open the group: get list of groups attached to 'location' and
       check if 'name' is part of it.
    */
    std::set<std::string> groups;
    h5get_names (groups,location,H5G_GROUP);
    if (static_cast<bool>(attributes_p.count(name))) {
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
	  std::string undefined ("UNDEFINED");
	  std::vector<std::string> types (1,undefined);
	  // write the attributes
	  h5set_attribute (location_p, "GROUPTYPE",          groupType_p       );
	  h5set_attribute (location_p, "REF_LOCATION_VALUE", refLocationValue_p);
	  h5set_attribute (location_p, "REF_LOCATION_UNIT",  refLocationUnit_p );
	  h5set_attribute (location_p, "REF_LOCATION_FRAME", refLocationFrame_p);
	  h5set_attribute (location_p, "NOF_COORDINATES",    nofCoordinates_p  );
	  h5set_attribute (location_p, "NOF_AXES",           nofAxes_p         );
	  h5set_attribute (location_p, "COORDINATE_TYPES",   types             );
	} else {
	  std::cerr << "[CoordinatesGroup::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[CoordinatesGroup::open] Failed to open group "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool CoordinatesGroup::openEmbedded (bool const &create)
  {
    return false;
  }
  
  //_____________________________________________________________________________
  //                                                                    groupType
  
  std::string CoordinatesGroup::groupType ()
  {
    if (location_p>0) {
      getAttribute ("GROUPTYPE", groupType_p);
    }

    return groupType_p;
  }
  
  //_____________________________________________________________________________
  //                                                             refLocationValue
  
  std::vector<double> CoordinatesGroup::refLocationValue ()
  {
    if (location_p>0) {
      getAttribute ("REF_LOCATION_VALUE", refLocationValue_p);
    }

    return refLocationValue_p;
  }
  
  //_____________________________________________________________________________
  //                                                              refLocationUnit
  
  std::vector<std::string> CoordinatesGroup::refLocationUnit ()
  {
    if (location_p>0) {
      getAttribute ("REF_LOCATION_UNIT", refLocationUnit_p);
    }

    return refLocationUnit_p;
  }
  
  //_____________________________________________________________________________
  //                                                             refLocationFrame
  
  std::string CoordinatesGroup::refLocationFrame ()
  {
    if (location_p>0) {
      getAttribute ("REF_LOCATION_FRAME", refLocationFrame_p);
    }

    return refLocationFrame_p;
  }

  
} // Namespace DAL -- end
