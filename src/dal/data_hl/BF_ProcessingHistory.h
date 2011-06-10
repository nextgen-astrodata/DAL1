/***************************************************************************
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

#ifndef BF_PROCESSINGHISTORY_H
#define BF_PROCESSINGHISTORY_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <data_common/HDF5GroupBase.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_ProcessingHistory
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the processing history attached to a BF dataset
    
    \author Lars B&auml;hren

    \date 2009/11/26

    \test tBF_ProcessingHistory.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>\ref dal_icd_003
      <li>Filename -- Class to filenames matching convention
      <li>CommonAttributes -- Collection of attributes common to all LOFAR
          datasets
      <li>HDF5GroupBase -- Common functionality for the high-level
          interfaces to the datasets
    </ul>
    
    <h3>Synopsis</h3>

    
    <h3>Example(s)</h3>
    
  */  
  class BF_ProcessingHistory : public HDF5GroupBase {
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    BF_ProcessingHistory ();
    
    //! Argumented constructor
    BF_ProcessingHistory (hid_t const &location,
			  IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));

    // === Destruction ==========================================================
    
    //! Default destructor
    ~BF_ProcessingHistory ();
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, BF_ProcessingHistory.
    */
    inline std::string className () const {
      return "BF_ProcessingHistory";
    }
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the internal status
    void summary (std::ostream &os);    

    // === Public methods =======================================================

    //! Open the file containing the beamformed data.
    bool open (hid_t const &location,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));

    // === Static methods =======================================================
    
    //! Get the name of the HDF5 group.
    static std::string getName ();
    
  private:

    //! Initialize internal paramaters to default values
    void init ();
    //! Open the structures embedded within the current one
    bool openEmbedded (IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate)) {
      return flags.flags();
    }
    //! Set up the list of attributes attached to the structure
    void setAttributes ();

  }; // Class BF_ProcessingHistory -- end
  
} // Namespace DAL -- end

#endif /* BF_PROCESSINGHISTORY_H */
  
