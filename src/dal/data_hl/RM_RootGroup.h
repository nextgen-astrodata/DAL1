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

#ifndef RM_ROOTGROUP_H
#define RM_ROOTGROUP_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <data_common/HDF5GroupBase.h>
#include <data_common/Filename.h>
#include <data_hl/SysLog.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class RM_RootGroup
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the root-group of a beamformed dataset
    
    \author Lars B&auml;hren

    \date 2009/10/28

    \test tRM_RootGroup.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>LOFAR Data Format ICDs:
      <ul>
	<li>Rotation-Measure Synthesis Cube (LOFAR-USG-ICD-008)
	<li>Naming conventions (LOFAR-USG-ICD-005)
      </ul>
      <li>Components of the LOFAR user software:
      <ul>
        <li>Filename -- Class to filenames matching convention
        <li>CommonAttributes -- Collection of attributes common to all LOFAR
	datasets
	<li>HDF5GroupBase -- Common functionality for the high-level
	interfaces to the datasets
	<li>SysLog
      </ul>
    </ul>
    
    <h3>Synopsis</h3>

    Basic hierarchical structure used DAL classes:
    \verbatim
    /
    |-- Image000
    |   |-- Coordinates
    |   |-- Data
    |   |-- Source
    |   `-- ProcessingHistory
    |
    |-- Image001
    |
    `-- SysLog
    \endverbatim
    
    <h3>Example(s)</h3>

    <ol>
      <li>Open an existing RM Synthesis Cube \e filename:
      \code
      std:string filename ("data.h5");
      
      RM_RootGroup rmsc (filename);
      \endcode
      Once the dataset has been opened its contents can be accessed; to get a
      basic idea of the contents, use
      \code
      rmsc.summary();
      \endcode
      In order to retrieve a list of the attributes attached to the root group of
      the file
      \code
      std::set<std::string> attr = rmsc.attributes();
      \endcode

      <li>Retrieve the value of an individual attribute:
      \code
      std::string lofar;
      std::string projectID;
      std::string projectTitle;
      
      bf.getAttribute ("TELESCOPE",     lofar);
      bf.getAttribute ("PROJECT_ID",    projectID);
      bf.getAttribute ("PROJECT_TITLE", projectTitle);
      \endcode
      Retrieve the collection of LOFAR Common Attributes:
      \code
      CommonAttributes common = rmsc.commonAttributes ();
      \endcode
    </ol>
    
  */  
  class RM_RootGroup : public HDF5GroupBase {

    //! Name of the data file
    std::string filename_p;
    //! LOFAR common attributes attached to the root group of the dataset
    CommonAttributes commonAttributes_p;
    //! Container for system-wide logs
    std::map<std::string,SysLog> sysLog_p;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    RM_RootGroup (std::string const &filename);
    
    //! Argumented constructor
    RM_RootGroup (DAL::Filename &infile,
		  IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Argumented constructor
    RM_RootGroup (CommonAttributes const &attributes,
		  IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    // === Destruction ==========================================================
    
    //! Default destructor
    ~RM_RootGroup ();
    
    // === Parameter access =====================================================

    //! Get the set of common attributes attached to the root group of the file
    CommonAttributes commonAttributes ();

    //! Set the set of common attributes attached to the root group of the file
    bool setCommonAttributes (CommonAttributes const &attributes);

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, RM_RootGroup.
    */
    inline std::string className () const {
      return "RM_RootGroup";
    }

    //! Provide a summary of the internal status
    inline void summary (bool const &showAttributes=false) {
      summary (std::cout,showAttributes);
    }
    
    /*!
      \brief Provide a summary of the internal status
      
      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os,
		  bool const &showAttributes=false);    

    // === Methods ==============================================================

    //! Open the file containing the beamformed data.
    bool open (hid_t const &location,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));

    //! Get the SysLog group
    SysLog sysLog ();
    
  protected:
    
    //! Open the structures embedded within the current one
    bool openEmbedded (IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    //! Set up the list of attributes attached to the structure
    void setAttributes ();

  private:
    
    //! Initialize the internal settings of the object
    void init (CommonAttributes const &attributes);
    //! Open a system log group
    bool openSysLog (IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
      
  }; // Class RM_RootGroup -- end
  
} // Namespace DAL -- end

#endif /* RM_ROOTGROUP_H */
  
