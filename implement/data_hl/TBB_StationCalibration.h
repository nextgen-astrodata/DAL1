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

#ifndef TBB_STATIONCALIBRATION_H
#define TBB_STATIONCALIBRATION_H

// Standard library header files
#include <iostream>
#include <string>

#include <CoordinatesGroup.h>
#include <CommonInterface.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class TBB_StationCalibration
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Brief description for class TBB_StationCalibration
    
    \author Lars B&auml;hren

    \date 2010/01/26

    \test tTBB_StationCalibration.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class TBB_StationCalibration : public CommonInterface {
    
    //! Complex electronic gain as function of frequency
    hid_t gainCurve_p;
    //! Coordinates attached to the gain curve data
    CoordinatesGroup gainCurveCoordinates_p;
    //! Complex system noice as function of frequency
    hid_t noiseCurve_p;
    //! Coordinates attached to the noise curve data
    CoordinatesGroup noiseCurveCoordinates_p;
    //! Complex beam pattern as function of direction and frequency
    hid_t beamShape_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    TBB_StationCalibration ();
    
    // === Destruction ==========================================================

    //! Destructor
    ~TBB_StationCalibration ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another TBB_StationCalibration object from which to make a copy.
    */
    TBB_StationCalibration& operator= (TBB_StationCalibration const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, TBB_StationCalibration.
    */
    inline std::string className () const {
      return "TBB_StationCalibration";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    //! Open a station calibration group
    bool open (hid_t const &location,
	       bool const &create=true);

  private:
    
    //! Set up the list of attributes attached to the structure
    void setAttributes ();

    //! Open a station calibration group
    bool open (hid_t const &location,
	       std::string const &name,
	       bool const &create=true);

    //! Open the structures embedded within the current one
    bool openEmbedded (bool const &create);

    //! Unconditional copying
    void copy (TBB_StationCalibration const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class TBB_StationCalibration -- end
  
} // Namespace DAL -- end

#endif /* TBB_STATIONCALIBRATION_H */
  
