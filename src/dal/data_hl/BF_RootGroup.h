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

#ifndef BF_ROOTGROUP_H
#define BF_ROOTGROUP_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <data_common/HDF5CommonInterface.h>
#include <data_common/Filename.h>
#include "BF_SubArrayPointing.h"
#include "SysLog.h"

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_RootGroup
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the root-group of a beamformed dataset
    
    \author Lars B&auml;hren

    \date 2009/10/28

    \test tBF_RootGroup.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>LOFAR Data Format ICDs:
      <ul>
      <li>\ref dal_icd_003
      <li>\ref dal_icd_005
      </ul>
      <li>Components of the LOFAR user software:
      <ul>
        <li>Filename -- Class to filenames matching convention
        <li>CommonAttributes -- Collection of attributes common to all LOFAR
	datasets
	<li>HDF5CommonInterface -- Common functionality for the high-level
	interfaces to the datasets
	<li>BF_SubArrayPointing
	<li>BF_StokesDataset
	<li>SysLog
      </ul>
    </ul>
    
    <h3>Synopsis</h3>

    A LOFAR BF file will then comprise <i>System Log Group</i> just below the
    root level which contains logs and parameter files which are relevant to the
    entire BF file. Additionally just below the root level, the BF file will
    contain an arbitrary, observation-dependent number of <i>Sub-Array Pointing
    Groups</i> containing N <i>Beam Groups</i> each with it’s own pointing
    information in the header; a <i>Processing History Group</i> at this level
    keeps track of logs and parameter sets relevant to the <i>Sub-Array
    Pointings</i>. Each <i>Beam Group</i> will contain a <i>Processing History
    Group</i> (relevant to the Beams), a <i>Coordinates Group</i> as well as
    one or four <i>Stokes Datasets</i>. The <i>Stokes Datasets</i> contain the
    array data.

    Basic hierarchical structure used DAL classes:
    \verbatim
    /
    |-- SubArrayPointing000
    |   |-- Beam000
    |   |   |-- Stokes0
    |   |   |-- Stokes1
    |   |   |-- Stokes2
    |   |   `-- Stokes3
    |   |-- Beam001
    |   |
    |
    |-- SubArrayPointing001
    |
    `-- SysLog
    \endverbatim

    <h3>Example(s)</h3>

    <ol>
      <li>Create new BF dataset file:
      \code
      // Construct filename from observation ID and filename extension
      DAL::Filename filename ("1234567890",
                              DAL::Filename::h5);
      // Create BF root-group object for new file
      BF_RootGroup bf (filename);
      \endcode
      <li>Open an existing BF dataset \e filename:
      \code
      std:string name ("data.h5");
      
      BF_RootGroup bf (name);
      \endcode
      Once the dataset has been opened its contents can be accessed; to get a
      basic idea of the contents, use
      \code
      bf.summary();
      \endcode
      In order to retrieve a list of the attributes attached to the root group of
      the file
      \code
      std::set<std::string> attr = bf.attributes();
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
      CommonAttributes common = bf.commonAttributes ();
      \endcode

      <li>Creating a new Beam group (BF_BeamGroup) might require the previous
      creation of Sub-Array Pointing group (BF_SubArrayPointing):
      \code
      unsigned int pointingID (10);
      unsigned int beamID (3);
      
      bf.openBeam (pointingID, beamID);
      \endcode
      The resulting structure within the file will be:
      \verbatim
      /                            ...  Root group
      `-- PrimaryPointing010       ...  Primary pointing direction group
          `-- Beam003              ...  Beam group
      \endverbatim
    </ol>
    
  */  
  class BF_RootGroup : public HDF5CommonInterface {

    //! Name of the data file
    std::string itsFilename;
    //! LOFAR common attributes attached to the root group of the dataset
    CommonAttributes itsCommonAttributes;
    //! Primary Pointing Directions
    std::map<std::string,BF_SubArrayPointing> itsSubarrayPointings;
    //! Container for system-wide logs
    std::map<std::string,SysLog> itsSystemLog;

  public:
    
    // === Construction =========================================================
    
    //! Argumented constructor to open existing file
    BF_RootGroup (std::string const &filename);
    
    //! Argumented constructor
    BF_RootGroup (DAL::Filename &infile,
		bool const &create=true);
    
    //! Argumented constructor
    BF_RootGroup (CommonAttributes const &attributes,
		bool const &create=true);
    
    // === Destruction ==========================================================
    
    //! Default destructor
    ~BF_RootGroup ();
    
    // --------------------------------------------------------------- Parameters

    //! Get the set of common attributes attached to the root group of the file
    CommonAttributes commonAttributes ();

    //! Set the set of common attributes attached to the root group of the file
    bool setCommonAttributes (CommonAttributes const &attributes);

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, BF_RootGroup.
    */
    inline std::string className () const {
      return "BF_RootGroup";
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
	       bool const &create=true);

    //! Open a PrimaryPointing direction group
    bool openPrimaryPointing (unsigned int const &pointingID,
			      bool const &create=true);

    //! Get the number of primary pointing direction objects
    inline unsigned int nofPrimaryPointings () const {
      return itsSubarrayPointings.size();
    }

    //! Get a primary pointing direction group
    BF_SubArrayPointing primaryPointing (unsigned int const &pointingID);

    //! Open a beam group
    bool openBeam (unsigned int const &pointingID,
		   unsigned int const &beamID,
		   bool const &create=true);

    //! Get the SysLog group
    SysLog sysLog ();
    
  protected:
    
    //! Open the structures embedded within the current one
    bool openEmbedded (bool const &create);
    //! Set up the list of attributes attached to the structure
    void setAttributes ();

  private:
    
    //! Initialize the internal settings of the object
    void init (CommonAttributes const &attributes);
    //! Open a system log group
    bool openSysLog (bool const &create=true);
    //! Open a PrimaryPointing direction group
    bool openPrimaryPointing (std::string const &name);
      
  }; // Class BF_RootGroup -- end
  
} // Namespace DAL -- end

#endif /* BF_ROOTGROUP_H */
  
