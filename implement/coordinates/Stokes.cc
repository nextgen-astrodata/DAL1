/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include <Stokes.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                       Stokes
  
  Stokes::Stokes ()
  {
    init();
  }
  
  //_____________________________________________________________________________
  //                                                                       Stokes
  
  /*!
    \param type -- A Stokes component.
  */
  Stokes::Stokes (Stokes::Component const &type)
  {
    init (type);
  }

  //_____________________________________________________________________________
  //                                                                       Stokes
  
  /*!
    \param other -- Another Stokes object from which to create this new
           one.
  */
  Stokes::Stokes (Stokes const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      ~Stokes
  
  Stokes::~Stokes ()
  {
    destroy();
  }
  
  //_____________________________________________________________________________
  //                                                                      destroy
  
  void Stokes::destroy ()
  {
    components_p.clear();
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another Stokes object from which to make a copy.
  */
  Stokes& Stokes::operator= (Stokes const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  /*!
    \param other -- Other Stokes component object from which to make a copy.
  */
  void Stokes::copy (Stokes const &other)
  {
    // copy the list of recognized components
    components_p.clear();
    components_p = other.components_p;
    // copy information which component has been selected
    std::map<Stokes::Component,std::string>::iterator it = other.component_p;
    setType(it->first);
  }

  // ============================================================================
  //
  //  Parameter access
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      setType
  
  /*!
    \param type    -- A Stokes component.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
   */
  bool Stokes::setType (Stokes::Component const &type)
  {
    bool status (true);
    std::map<Stokes::Component,std::string>::iterator it;
    
    it = components_p.find(type);
    
    if (it == components_p.end()) {
      status = false;
    } else {
      component_p = it;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                      setType
  
  /*!
    \param type    -- A Stokes component.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool Stokes::setType (std::string const &name)
  {
    bool status (true);

    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void Stokes::summary (std::ostream &os)
  {
    os << "[Stokes] Summary of internal parameters."      << std::endl;
    os << "-- Stokes component        = " << name()       << std::endl;
    os << "-- Component is linear     = " << isLinear()   << std::endl;
    os << "-- Component is circular   = " << isCircular() << std::endl;
    os << "-- Components are parallel = " << isParallel() << std::endl;
    os << "-- Component are cross     = " << isCross()    << std::endl;
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
  void Stokes::init (Stokes::Component const &type)
  {
    bool status (true);

    /* Set up the map for the available Stokes components */
    components_p.clear();
    components_p[Stokes::I]  = "I";
    components_p[Stokes::Q]  = "Q";
    components_p[Stokes::U]  = "U";
    components_p[Stokes::V]  = "V";
    components_p[Stokes::R]  = "R";
    components_p[Stokes::L]  = "L";
    components_p[Stokes::RR] = "RR";
    components_p[Stokes::LL] = "LL";
    components_p[Stokes::RL] = "RL";
    components_p[Stokes::LR] = "LR";
    components_p[Stokes::X]  = "X";
    components_p[Stokes::Y]  = "Y";
    components_p[Stokes::XX] = "XX";
    components_p[Stokes::YY] = "YY";
    components_p[Stokes::XY] = "XY";
    components_p[Stokes::YX] = "YX";

    /* Set the type of Stokes component */
    status = setType (type);
  }

  //_____________________________________________________________________________
  //                                                               componentTypes
  
  std::vector<Stokes::Component> Stokes::componentTypes ()
  {
    std::map<Stokes::Component,std::string>::iterator it;
    std::vector<Stokes::Component> types;

    for (it=components_p.begin(); it!=components_p.end(); ++it) {
      types.push_back(it->first);
    }

    return types;
  }

  //_____________________________________________________________________________
  //                                                               componentNames
  
  std::vector<std::string> Stokes::componentNames ()
  {
    std::map<Stokes::Component,std::string>::iterator it;
    std::vector<std::string> names;

    for (it=components_p.begin(); it!=components_p.end(); ++it) {
      names.push_back(it->second);
    }

    return names;
  }

  //_____________________________________________________________________________
  //                                                                     isLinear
  
  /*!
    \return isLinear -- Is the Stokes component linear? Returns \e false if this 
            is not the case.
  */
  bool Stokes::isLinear ()
  {
    return isLinear(component_p->first);
  }

  //_____________________________________________________________________________
  //                                                                     isLinear
  
  /*!
    \param type      -- A Stokes component.
    \return isLinear -- Is the Stokes component linear? Returns \e false if this 
            is not the case.
  */
  bool Stokes::isLinear (Stokes::Component const &type)
  {
    bool status;

    switch (type) {
    case Stokes::Q:
    case Stokes::U:
    case Stokes::X:
    case Stokes::Y:
    case Stokes::XX:
    case Stokes::YY:
    case Stokes::XY:
    case Stokes::YX:
      status = true;
      break;
    default:
      status = false;
      break;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                   isCircular
  
  /*!
    \return isCircular -- Is the Stokes component circular? Returns \e false if
            this is not the case.
  */
  bool Stokes::isCircular ()
  {
    return isCircular(component_p->first);
  }

  //_____________________________________________________________________________
  //                                                                   isCircular
  
  /*!
    \param type        -- A Stokes component.
    \return isCircular -- Is the Stokes component circular? Returns \e false if
            this is not the case.
  */
  bool Stokes::isCircular (Stokes::Component const &type)
  {
    bool status;

    switch (type) {
    case Stokes::V:
    case Stokes::R:
    case Stokes::L:
    case Stokes::RR:
    case Stokes::LL:
    case Stokes::RL:
    case Stokes::LR:
      status = true;
      break;
    default:
      status = false;
      break;
    }

    return status;
  }

  //_____________________________________________________________________________
  //                                                                   isParallel
  
  /*!
    \return isParallel -- Is this a parallel combination of Stokes components?
            Returns \e false if this is not the case.
  */
  bool Stokes::isParallel ()
  {
    return isParallel(component_p->first);
  }
  
  //_____________________________________________________________________________
  //                                                                   isParallel
  
  bool Stokes::isParallel (Stokes::Component const &type)
  {
    bool status;

    switch (type) {
    case Stokes::RR:
    case Stokes::LL:
    case Stokes::XX:
    case Stokes::YY:
      status = true;
      break;
    default:
      status = false;
      break;
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      isCross

  /*!  
    \return isCross -- Is this a cross combination of Stokes components?
            Returns \e false if this is not the case.
  */
  bool Stokes::isCross ()
  {
    return isCross(component_p->first);
  }
  
  //_____________________________________________________________________________
  //                                                                      isCross
  
  bool Stokes::isCross (Stokes::Component const &type)
  {
    bool status;

    switch (type) {
    case Stokes::RL:
    case Stokes::LR:
    case Stokes::XY:
    case Stokes::YX:
      status = true;
      break;
    default:
      status = false;
      break;
    }

    return status;
  }
  
} // Namespace DAL -- end
