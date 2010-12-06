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
  {
    itsObservationID   = "UNDEFINED";
    itsStartMJD        = "UNDEFINED";
    itsStartTAI        = "UNDEFINED";
    itsStartUTC        = "UNDEFINED";
    itsEndMJD          = "UNDEFINED";
    itsEndTAI          = "UNDEFINED";
    itsEndUTC          = "UNDEFINED";
    itsNofStations     = 0;
    itsStationsList.clear();
    itsFrequencyMin    = 0.0;
    itsFrequencyMax    = 0.0;
    itsFrequencyCenter = 0.0;
    itsFrequencyUnit   = "UNDEFINED";

    setAttributes();
  }
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  AttributesObservation::AttributesObservation (AttributesObservation const &other)
    : AttributesInterface (other)
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
  {
    itsStationsList.clear();

    itsObservationID   = other.itsObservationID;
    itsStartMJD        = other.itsStartMJD;
    itsStartTAI        = other.itsStartTAI;
    itsStartUTC        = other.itsStartUTC;
    itsEndMJD          = other.itsEndMJD;
    itsEndTAI          = other.itsEndTAI;
    itsEndUTC          = other.itsEndUTC;
    itsNofStations     = other.itsNofStations;
    itsStationsList    = other.itsStationsList;
    itsFrequencyMin    = other.itsFrequencyMin;
    itsFrequencyMax    = other.itsFrequencyMax;
    itsFrequencyCenter = other.itsFrequencyCenter;
    itsFrequencyUnit   = other.itsFrequencyUnit;
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

    itsAttributes.insert("OBSERVATION_ID");
    itsAttributes.insert("OBSERVATION_START_MJD");
    itsAttributes.insert("OBSERVATION_START_TAI");
    itsAttributes.insert("OBSERVATION_START_UTC");
    itsAttributes.insert("OBSERVATION_END_MJD");
    itsAttributes.insert("OBSERVATION_END_TAI");
    itsAttributes.insert("OBSERVATION_END_UTC");
    itsAttributes.insert("OBSERVATION_NOF_STATIONS");
    itsAttributes.insert("OBSERVATION_STATIONS_LIST");
    itsAttributes.insert("OBSERVATION_FREQUENCY_MAX");
    itsAttributes.insert("OBSERVATION_FREQUENCY_MIN");
    itsAttributes.insert("OBSERVATION_FREQUENCY_CENTER");
    itsAttributes.insert("OBSERVATION_FREQUENCY_UNIT");
  }

} // Namespace DAL -- end
