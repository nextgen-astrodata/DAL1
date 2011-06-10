/***************************************************************************
 *   Copyright (C) 2009-2011                                               *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#ifndef SYSLOG_H
#define SYSLOG_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <data_common/HDF5GroupBase.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class SysLog
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the system logs attached to a beamformed dataset
    
    \author Lars B&auml;hren

    \date 2009/11/26

    \test tSysLog.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>LOFAR Data Format ICDs:
      <ul>
	<li>Beam-Formed Data (LOFAR-USG-ICD-003)
	<li>Naming conventions (LOFAR-USG-ICD-005)
      </ul>
      <li>Components of the LOFAR user software:
      <ul>
        <li>Filename -- Class to filenames matching convention
        <li>CommonAttributes -- Collection of attributes common to all LOFAR
	datasets
	<li>HDF5GroupBase -- Common functionality for the high-level
	interfaces to the datasets
      </ul>
    </ul>
    
    <h3>Synopsis</h3>

    
    <h3>Example(s)</h3>
    
  */  
  class SysLog : public HDF5GroupBase {
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    SysLog ();
    
    //! Argumented constructor
    SysLog (hid_t const &location,
	    IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    // === Destruction ==========================================================
    
    //! Default destructor
    ~SysLog ();
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, SysLog.
    */
    inline std::string className () const {
      return "SysLog";
    }
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }

    /*!
      \brief Provide a summary of the internal status

      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // === Methods ==============================================================

    //! Open the file containing the beamformed data.
    bool open (hid_t const &location,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Get the name of the group
    static std::string getName () {
      return "SYS_LOG";
    }

  protected:
    
    //! Open the structures embedded within the current one
    bool openEmbedded (IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate)) {
      return flags.flags();
    }
    //! Set up the list of attributes attached to the structure
    void setAttributes ();

  }; // Class SysLog -- end
  
} // Namespace DAL -- end

#endif /* SYSLOG_H */
  
