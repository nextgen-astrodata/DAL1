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

#ifndef ATTRIBUTESOBSERVATION_H
#define ATTRIBUTESOBSERVATION_H

// Standard library header files
#include <iostream>
#include <string>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class AttributesObservation
    
    \ingroup DAL
    \ingroup data_common
    
    \brief Common LOFAR attributes for description of observation
    
    \author Lars B&auml;hren

    \date 2010/12/03

    \test tAttributesObservation.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class AttributesObservation {

    //! Attributes names
    std::set<std::string> itsAttributes;

    //! Unique identifier for the observation
    std::string itsObservationID;
    //! Start date of the observation (MJD)
    std::string itsStartMJD;
    //! Start date of the observation (TAI)
    std::string itsStartTAI;
    //! Start date of the observation (UTC)
    std::string itsStartUTC;
    //! End date of the observation (MJD)
    std::string itsEndMJD;
    //! End date of the observation (TAI)
    std::string itsEndTAI;
    //! End date of the observation (UTC)
    std::string itsEndUTC;
    //! nof. stations used during the observation
    int itsNofStations;
    //! List of stations used during the observation
    std::vector<std::string> itsStationsList;
    //! Observation minimum frequency
    double itsFrequencyMin;
    //! Observation maximum frequency
    double itsFrequencyMax;
    //! Observation center frequency
    double itsFrequencyCenter;
    //! Observation frequency physical units
    std::string itsFrequencyUnit;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    AttributesObservation ();
    
    //! Copy constructor
    AttributesObservation (AttributesObservation const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~AttributesObservation ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    AttributesObservation& operator= (AttributesObservation const &other); 
    
    // === Parameter access =====================================================
    
    //! Unique identifier for the observation
    inline std::string observationID () const {
      return itsObservationID;
    }
    //! Unique identifier for the observation
    inline void setObservationID (std::string const &observationID) {
      itsObservationID = observationID;
    }

    //! Start date of the observation (MJD)
    inline std::string startMJD () const {
      return itsStartMJD;
    }
    //! Start date of the observation (MJD)
    inline void setStartMJD (std::string const &startMJD) {
      itsStartMJD = startMJD;
    }

    //! Start date of the observation (TAI)
    inline std::string startTAI () const {
      return itsStartTAI;
    }
    //! Start date of the observation (UTC)
    inline std::string startUTC () const {
      return itsStartUTC;
    }
    //! End date of the observation (MJD)
    inline std::string endMJD () const {
      return itsEndMJD;
    }
    //! End date of the observation (TAI)
    inline std::string endTAI () const {
      return itsEndTAI;
    }
    //! End date of the observation (UTC)
    inline std::string endUTC () const {
      return itsEndUTC;
    }
    //! nof. stations used during the observation
    inline int nofStations () const {
      return itsNofStations;
    }
    //! List of stations used during the observation
    inline std::vector<std::string> stationsList () const {
      return itsStationsList;
    }
    //! Observation minimum frequency
    inline double frequencyMin () const {
      return itsFrequencyMin;
    }
    //! Observation maximum frequency
    inline double frequencyMax () const {
      return itsFrequencyMax;
    }
    //! Observation center frequency
    inline double frequencyCenter () const {
      return itsFrequencyCenter;
    }
    //! Observation frequency physical units
    inline std::string frequencyUnit () const {
      return itsFrequencyUnit;
    }

    // === Methods ==============================================================
    
    /*!
      \brief Get the name of the class
      \return className -- The name of the class, AttributesObservation.
    */
    inline std::string className () const {
      return "AttributesObservation";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    
    
#ifdef HAVE_HDF5
    //! Write the attributes to a HDF5 file
    void h5write (hid_t const &groupID);
    
    //! Read the attributes from a HDF5 file
    void h5read (hid_t const &groupID);
#endif    

  private:
    
    //! Set the atttribute names
    void setAttributes ();

    //! Unconditional copying
    void copy (AttributesObservation const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class AttributesObservation -- end
  
} // Namespace DAL -- end

#endif /* ATTRIBUTESOBSERVATION_H */
  
