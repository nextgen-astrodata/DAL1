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

#ifndef TBB_TRIGGERTABLE_H
#define TBB_TRIGGERTABLE_H

// Standard library header files
#include <iostream>
#include <string>

#include <CommonInterface.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class TBB_TriggerTable
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Container for the trigger parameter data
    
    \author Lars B&auml;hren

    \date 2010/01/19

    \test tTBB_TriggerTable.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>LOFAR Data Format ICD: TBB Time-series data (LOFAR-USG-ICD-001)
      <li>DAL::CommonInterface
      <li>DAL::TBB_Timeseries
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class TBB_TriggerTable : public CommonInterface {

  protected:
    
    //! Struct definiting the elements of a row within the trigger table
    struct TriggerTableRow {
      //! Source from which the trigger originated (TBB, LCU, CEP)
      std::string triggerSource;
      //! Identifier for the station
      unsigned int stationID;
      //! Identifier for the RSP board
      unsigned int rspID;
      //! Identifier for the RCU
      unsigned int rcuID;
      //! Time instance in full seconds
      unsigned int time;
      //! Number of samples passed since \e time
      unsigned int sampleNumber;
      //! Width of the pulse
      int pulseWidth;
      //! Height of the pulse
      int pulseHeight;
    };
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    TBB_TriggerTable ();
    
    //! Argumented constructor
    TBB_TriggerTable (hid_t const &location,
		      std::string const &name="TriggerTable",
		      bool const &create=true);
    
    // === Destruction ==========================================================

    //! Destructor
    ~TBB_TriggerTable ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another TBB_TriggerTable object from which to make a copy.
    */
    TBB_TriggerTable& operator= (TBB_TriggerTable const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, `TBB_TriggerTable`.
    */
    inline std::string className () const {
      return "TBB_TriggerTable";
    }
    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    //! Open a station group
    bool open (hid_t const &location,
	       std::string const &name,
	       bool const &create=true);

  private:

    //! Set up the list of attributes attached to the structure
    void setAttributes ();
    //! Open the structures embedded within the current one
    bool openEmbedded (bool const &create);
    //! Unconditional copying
    void copy (TBB_TriggerTable const &other);
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class TBB_TriggerTable -- end
  
} // Namespace DAL -- end

#endif /* TBB_TRIGGERTABLE_H */
  
