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
  
  CoordinatesGroup::CoordinatesGroup ()
  {;}
  
  CoordinatesGroup::CoordinatesGroup (CoordinatesGroup const &other)
  {
    copy (other);
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
  
  //_____________________________________________________________________________
  //                                                                      destroy

  void CoordinatesGroup::destroy ()
  {
    /* Release memory allocated for the embedded coordinate objects */
    if (coordinates_p.size()) {
      for (unsigned int n(0); n<coordinates_p.size(); n++) {
	delete [] coordinates_p[n];
      }
    }
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  CoordinatesGroup& CoordinatesGroup::operator= (CoordinatesGroup const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy

  void CoordinatesGroup::copy (CoordinatesGroup const &other)
  {
    grouptype_p    = other.grouptype_p;
    equinox_p      = other.equinox_p;
    system_radec_p = other.system_radec_p;

    /* Check if tere are already some coordinate objects stored; if this is the
     * case, we need to purge them.
     */
    if (coordinates_p.size()) {
      for (unsigned int n(0); n<coordinates_p.size(); n++) {
	delete [] coordinates_p[n];
      }
    }
    // adjust the size of the vector storing the coordinate objects
    coordinates_p.resize(other.coordinates_p.size());
    // copy coordinate objects from the other instance
    for (unsigned int n(0); n<coordinates_p.size(); n++) {
      coordinates_p[n] = other.coordinates_p[n];
    }
  }
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      nofAxes
  
  int CoordinatesGroup::nofAxes ()
  {
    int naxes (0);

    for (unsigned int n(0); n<coordinates_p.size(); n++) {
      naxes += coordinates_p[n]->nofAxes();
    }

    return naxes;
  }

  //_____________________________________________________________________________
  //                                                              coordinateTypes
  
  std::vector<Coordinate::Type> CoordinatesGroup::coordinateTypes ()
  {
    std::vector<Coordinate::Type> types;

    for (unsigned int n(0); n<coordinates_p.size(); n++) {
      types.push_back(coordinates_p[n]->type());
    }
    return types;
  }

  //_____________________________________________________________________________
  //                                                              coordinateNames
  
  std::vector<std::string> CoordinatesGroup::coordinateNames ()
  {
    std::vector<std::string> names;
    
    for (unsigned int n(0); n<coordinates_p.size(); n++) {
      names.push_back(coordinates_p[n]->name());
    }
    return names;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  void CoordinatesGroup::summary (std::ostream &os)
  {
    os << "[CoordinatesGroup] Summary of internal parameters."   << std::endl;
    os << "-- Equinox                  = " << equinox_p          << std::endl;
    os << "-- System Ra/Dec            = " << system_radec_p     << std::endl;
    os << "-- Reference location value = " << refLocationValue_p << std::endl;
    os << "-- Reference location unit  = " << refLocationUnit_p  << std::endl;
    os << "-- nof. coordinates         = " << nofCoordinates()   << std::endl;
    os << "-- nof. coordinate axes     = " << nofAxes()          << std::endl;
    os << "-- Coordinate types         = " << coordinateNames()  << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init
  
  void CoordinatesGroup::init ()
  {
    grouptype_p    = "Coordinates";
    equinox_p      = "UNDEFINED";
    system_radec_p = "UNDEFINED";

    refLocationValue_p.resize(3);
    refLocationUnit_p.resize(3);
    
    refLocationValue_p = std::vector<double> (3,0.0);
    refLocationUnit_p  = std::vector<std::string> (3,"UNDEFINED");
    refLocationFrame_p = "UNDEFINED";
  }

  //_____________________________________________________________________________
  //                                                                         init
  
  void CoordinatesGroup::h5write (hid_t const &groupID)
  {
    std::string attrGrouptype = "Coordinates";
    int attrNofCoordinates    = nofCoordinates();
    int attrNofAxes           = nofAxes();

    /* Write the attributes to the group */
    DAL::h5set_attribute( groupID,
			  "GROUPTYPE",       attrGrouptype       );
    DAL::h5set_attribute( groupID,
			  "EQUINOX",         equinox_p           );
    DAL::h5set_attribute( groupID,
			  "SYSTEM_RADEC",    system_radec_p      );
    DAL::h5set_attribute( groupID,
			  "REFERENCE_LOCATION_VALUE", refLocationValue_p );
    DAL::h5set_attribute( groupID,
			  "REFERENCE_LOCATION_UNIT", refLocationUnit_p );
    DAL::h5set_attribute( groupID,
			  "REFERENCE_LOCATION_FRAME", refLocationFrame_p );
    DAL::h5set_attribute( groupID,
			  "NOF_COORDINATES", attrNofCoordinates  );
    DAL::h5set_attribute( groupID,
			  "NOF_AXES",        attrNofAxes         );

    /* Write the cordinate objects themselves */
    for (unsigned int n(0); coordinates_p.size(); n++) {
      // name of the group holding the coordinate object
      std::string groupName = "COORDINATE"+to_string(n);
      // write coordinate object
      coordinates_p[n]->h5write(groupID,groupName);
    }
  }

} // Namespace DAL -- end
