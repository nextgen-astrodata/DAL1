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

#include <coordinates/Coordinate.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  Coordinate::Coordinate (Coordinate::Type const &type)
  {
    init (type);
  }

  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  Coordinate::Coordinate (Coordinate const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  Coordinate::~Coordinate ()
  {
    destroy();
  }
  
  void Coordinate::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another Coordinate object from which to make a copy.
  */
  Coordinate& Coordinate::operator= (Coordinate const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void Coordinate::copy (Coordinate const &other)
  {
    // copy the list of recognized components
    coordTypes_p.clear();
    coordTypes_p = other.coordTypes_p;
    // copy information which component has been selected
    std::map<Coordinate::Type,std::string>::iterator it = other.coord_p;
    setType(it->first);
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void Coordinate::summary (std::ostream &os)
  {
    os << "[Coordinate] Summary of internal parameters." << std::endl;
    os << "-- Coordinate type = " << name()              << std::endl;
    os << "-- Has projection? = " << hasProjection()     << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init
  
  /*!
    \param type -- A Stokes component.
  */
  void Coordinate::init (Coordinate::Type const &type)
  {
    bool status (true);

    /* Set up the map for the available Stokes components */
    coordTypes_p.clear();
    coordTypes_p = coordinatesMap ();

    /* Set the type of Stokes component */
    status = setType (type);
  }

  //_____________________________________________________________________________
  //                                                                      setType

  /*!
    \param type    -- Identifier for the type of coordinate.
    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
   */
  bool Coordinate::setType (Coordinate::Type const &type)
  {
    bool status (true);
    std::map<Coordinate::Type,std::string>::iterator it = coordTypes_p.find(type);

    /* If the provided type does not match any of the recognized types, set the
       coordinate to UNDEFINED. */
    if (it == coordTypes_p.end()) {
      status = false;
      it     = coordTypes_p.find(Coordinate::UNDEFINED);
      // Error message 
      std::cerr << "[Coordinate::setType] Undefined coordinate type "
		<< type << std::endl;
    }

    coord_p = it;

    return status;
  }

  //_____________________________________________________________________________
  //                                                                      setType

  /*!
    \param type    -- Identifier for the type of coordinate.
    \return status -- Status of the operation; returns \e false in case an error 
            was encountered.
   */
  bool Coordinate::setType (std::string const &type)
  {
    bool status (false);
    std::map<Coordinate::Type,std::string>::iterator it;

    for (it=coordTypes_p.begin(); it!=coordTypes_p.end(); ++it) {
      if (it->second == type) {
	coord_p = it;
	status = true;
      }
    }

    // Provide some feedback in case the provided coordinate ID was rejected
    if (!status) {
      std::cerr << "[Coordinate::setType] Invalid type of coordinate: " << type
		<< " is not a recognized value! Keeping current settings."
		<< std::endl;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                               coordinatesMap

  std::map<DAL::Coordinate::Type,std::string> Coordinate::coordinatesMap ()
  {
    std::map<DAL::Coordinate::Type,std::string> coordMap;

    coordMap[DAL::Coordinate::DIRECTION] = "DIRECTION";
    coordMap[DAL::Coordinate::LINEAR]    = "LINEAR";
    coordMap[DAL::Coordinate::TABULAR]   = "TABULAR";
    coordMap[DAL::Coordinate::STOKES]    = "STOKES";
    coordMap[DAL::Coordinate::SPECTRAL]  = "SPECTRAL";
    coordMap[DAL::Coordinate::UNDEFINED] = "UNDEFINED";

    return coordMap;
  }

  //_____________________________________________________________________________
  //                                                              coordinatesType

  std::vector<DAL::Coordinate::Type> Coordinate::coordinatesType ()
  {
    std::map<DAL::Coordinate::Type,std::string> coordMap = coordinatesMap();
    std::map<DAL::Coordinate::Type,std::string>::iterator it;
    std::vector<DAL::Coordinate::Type> vec;

    for (it=coordMap.begin(); it!=coordMap.end(); ++it) {
      vec.push_back(it->first);
    }
    
    return vec;
  }
  
  //_____________________________________________________________________________
  //                                                              coordinatesName
  
  std::vector<std::string> Coordinate::coordinatesName ()
  {
    std::map<Coordinate::Type,std::string> coordMap = coordinatesMap();
    std::map<Coordinate::Type,std::string>::iterator it;
    std::vector<std::string> names;

    for (it=coordMap.begin(); it!=coordMap.end(); ++it) {
      names.push_back(it->second);
    }
    
    return names;
  }

  //_____________________________________________________________________________
  //                                                                      getType

  Coordinate::Type Coordinate::getType (std::string const &name)
  {
    std::map<Coordinate::Type,std::string> coordMap = coordinatesMap();
    std::map<Coordinate::Type,std::string>::iterator it;
    Coordinate::Type type (Coordinate::UNDEFINED);

    for (it=coordMap.begin(); it!=coordMap.end(); ++it) {
      if (it->second == name) {
	type = it->first;
      }
    }
    
    return type;
  }
  
  //_____________________________________________________________________________
  //                                                                      getName
  
  std::string Coordinate::getName (Coordinate::Type const &type)
  {
    std::map<Coordinate::Type,std::string> coordMap     = coordinatesMap();
    std::map<Coordinate::Type,std::string>::iterator it = coordMap.find(type);

    if (it == coordMap.end()) {
      return "UNDEFINED";
    } else {
      return it->second;
    }
  }

  //_____________________________________________________________________________
  //                                                                hasProjection
  
  bool Coordinate::hasProjection (Coordinate::Type const &type)
  {
    bool status (true);

    switch (type) {
    case Coordinate::DIRECTION:
      status = true;
      break;
    default:
      status = false;
      break;
    };

    return status;
  }

  //_____________________________________________________________________________
  //                                                                    isTabular
  
  bool Coordinate::isTabular (Coordinate::Type const &type)
  {
    bool status (true);

    switch (type) {
    case Coordinate::TABULAR:
      status = true;
      break;
    default:
      status = false;
      break;
    };

    return status;
  }

} // Namespace DAL -- end
