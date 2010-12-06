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

#include <AttributesObservation.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  AttributesObservation::AttributesObservation ()
  {;}
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  AttributesObservation::AttributesObservation (AttributesObservation const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  AttributesObservation::~AttributesObservation ()
  {
    destroy();
  }
  
  void AttributesObservation::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another AttributesObservation object from which to make a copy.
  */
  AttributesObservation& AttributesObservation::operator= (AttributesObservation const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void AttributesObservation::copy (AttributesObservation const &other)
  {;}

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
  void AttributesObservation::summary (std::ostream &os)
  {
    os << "[AttributesObservation] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  void AttributesObservation::setAttributes ()
  {
    itsAttributes.clear();

    attributes_p.insert("OBSERVATION_ID");
    attributes_p.insert("OBSERVATION_START_MJD");
    attributes_p.insert("OBSERVATION_START_TAI");
    attributes_p.insert("OBSERVATION_START_UTC");
    attributes_p.insert("OBSERVATION_END_MJD");
    attributes_p.insert("OBSERVATION_END_TAI");
    attributes_p.insert("OBSERVATION_END_UTC");
    attributes_p.insert("OBSERVATION_NOF_STATIONS");
    attributes_p.insert("OBSERVATION_STATIONS_LIST");
    attributes_p.insert("OBSERVATION_FREQUENCY_MAX");
    attributes_p.insert("OBSERVATION_FREQUENCY_MIN");
    attributes_p.insert("OBSERVATION_FREQUENCY_CENTER");
    attributes_p.insert("OBSERVATION_FREQUENCY_UNIT");
  }

} // Namespace DAL -- end
