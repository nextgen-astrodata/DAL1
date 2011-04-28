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

#include "CommonAttributesObservation.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  CommonAttributesObservation::CommonAttributesObservation ()
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
    
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  CommonAttributesObservation::~CommonAttributesObservation ()
  {
    destroy();
  }
  
  void CommonAttributesObservation::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another CommonAttributesObservation object from which to make a copy.
  */
  CommonAttributesObservation& CommonAttributesObservation::operator= (CommonAttributesObservation const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void CommonAttributesObservation::copy (CommonAttributesObservation const &other)
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
  void CommonAttributesObservation::summary (std::ostream &os)
  {
    os << "[CommonAttributesObservation] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void CommonAttributesObservation::setAttributes ()
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
    itsAttributes.insert("OBSERVATION_FREQUENCY_MIN");
    itsAttributes.insert("OBSERVATION_FREQUENCY_MAX");
    itsAttributes.insert("OBSERVATION_FREQUENCY_CENTER");
    itsAttributes.insert("OBSERVATION_FREQUENCY_UNIT");
  }
  
  //_____________________________________________________________________________
  //                                                                      h5write
  
  bool CommonAttributesObservation::h5write (hid_t const &groupID)
  {
    bool status (true);
    
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_ID",               itsObservationID);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_START_MJD",        itsStartMJD);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_START_TAI",        itsStartTAI);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_START_UTC",        itsStartUTC);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_END_MJD",          itsEndMJD);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_END_TAI",          itsEndMJD);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_END_UTC",          itsEndMJD);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_NOF_STATIONS",     itsNofStations);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_STATIONS_LIST",    itsStationsList);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_FREQUENCY_MIN",    itsFrequencyMin);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_FREQUENCY_MAX",    itsFrequencyMax);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_FREQUENCY_CENTER", itsFrequencyCenter);
    HDF5Attribute::setAttribute(groupID, "OBSERVATION_FREQUENCY_UNIT",   itsFrequencyUnit);
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                       h5read
  
  bool CommonAttributesObservation::h5read (hid_t const &groupID)
  {
    bool status (true);
    
    DAL::h5get_attribute(groupID, "OBSERVATION_ID",               itsObservationID);
    DAL::h5get_attribute(groupID, "OBSERVATION_START_MJD",        itsStartMJD);
    DAL::h5get_attribute(groupID, "OBSERVATION_START_TAI",        itsStartTAI);
    DAL::h5get_attribute(groupID, "OBSERVATION_START_UTC",        itsStartUTC);
    DAL::h5get_attribute(groupID, "OBSERVATION_END_MJD",          itsEndMJD);
    DAL::h5get_attribute(groupID, "OBSERVATION_END_TAI",          itsEndMJD);
    DAL::h5get_attribute(groupID, "OBSERVATION_END_UTC",          itsEndMJD);
    DAL::h5get_attribute(groupID, "OBSERVATION_NOF_STATIONS",     itsNofStations);
    DAL::h5get_attribute(groupID, "OBSERVATION_STATIONS_LIST",    itsStationsList);
    DAL::h5get_attribute(groupID, "OBSERVATION_FREQUENCY_MIN",    itsFrequencyMin);
    DAL::h5get_attribute(groupID, "OBSERVATION_FREQUENCY_MAX",    itsFrequencyMax);
    DAL::h5get_attribute(groupID, "OBSERVATION_FREQUENCY_CENTER", itsFrequencyCenter);
    DAL::h5get_attribute(groupID, "OBSERVATION_FREQUENCY_UNIT",   itsFrequencyUnit);
    
    return status;
  }


} // Namespace DAL -- end
