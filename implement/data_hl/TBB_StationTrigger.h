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

#ifndef TBB_STATIONTRIGGER_H
#define TBB_STATIONTRIGGER_H

// Standard library header files
#include <iostream>
#include <string>

#include <CommonInterface.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class TBB_StationTrigger
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Container for the parameters generated from the station trigger
    
    \author Lars B&auml;hren

    \date 2010/01/19

    \test tTBB_StationTrigger.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>LOFAR Data Format ICD: TBB Time-series data (LOFAR-USG-ICD-001)
      <li>DAL::CommonInterface
      <li>DAL::TBB_Timeseries
    </ul>
    
    <h3>Synopsis</h3>
    
    <h3>Example(s)</h3>
    
  */  
  class TBB_StationTrigger : public CommonInterface {
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    TBB_StationTrigger ();
    
    //! Argumented constructor
    TBB_StationTrigger (hid_t const &location,
		      std::string const &name="StationTrigger",
		      bool const &create=true);
    
    // === Destruction ==========================================================

    //! Destructor
    ~TBB_StationTrigger ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another TBB_StationTrigger object from which to make a copy.
    */
    TBB_StationTrigger& operator= (TBB_StationTrigger const &other); 
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, `TBB_StationTrigger`.
    */
    inline std::string className () const {
      return "TBB_StationTrigger";
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
    void copy (TBB_StationTrigger const &other);
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class TBB_StationTrigger -- end
  
} // Namespace DAL -- end

#endif /* TBB_STATIONTRIGGER_H */
  
