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

#ifndef BF_ROOTGROUP_H
#define BF_ROOTGROUP_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <data_common/HDF5GroupBase.h>
#include <data_common/Filename.h>
#include <data_hl/BF_SubArrayPointing.h>
#include <data_hl/SysLog.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_RootGroup
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the root-group of a beamformed dataset
    
    \author Lars B&auml;hren

    \date 2009/10/28

    \test tBF_RootGroup.cc
    \test test_bf2h5writer.cc
    
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
	<li>HDF5GroupBase -- Common functionality for the high-level
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
    /                                   ...  BF_RootGroup
    |-- SubArrayPointing000             ...  BF_SubArrayPointing
    |   |-- Beam000                     ...  BF_BeamGroup
    |   |   |-- CoordinatesGroup
    |   |   |-- ProcessingHistory
    |   |   |-- Stokes0                 ...  BF_StokesDataset
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
      `-- SubArrayPointing010      ...  Sub-array pointing group
          `-- Beam003              ...  Beam group
      \endverbatim
    </ol>
    
  */  
  class BF_RootGroup : public HDF5GroupBase {

    //! Name of the data file
    std::string itsFilename;
    //! LOFAR common attributes attached to the root group of the dataset
    CommonAttributes itsCommonAttributes;
    //! Sub-array pointing directions
    std::map<std::string,BF_SubArrayPointing> itsSubarrayPointings;
    //! Container for system-wide logs
    std::map<std::string,SysLog> itsSystemLog;

  public:
    
    // === Construction =========================================================
    
    //! Argumented constructor to open existing file
    BF_RootGroup (std::string const &filename);
    
    //! Argumented constructor
    BF_RootGroup (DAL::Filename &infile,
		  IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Argumented constructor
    BF_RootGroup (CommonAttributes const &attributes,
		  IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    // === Destruction ==========================================================
    
    //! Default destructor
    ~BF_RootGroup ();
    
    // === Parameter access =====================================================
    
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
    
    //! Provide a summary of the internal status
    void summary (std::ostream &os,
		  bool const &showAttributes=false);    

    // === Methods ==============================================================

    //! Open the file containing the beamformed data.
    bool open (hid_t const &location,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Open a SubArrayPointing direction group
    bool openSubArrayPointing (unsigned int const &pointingID,
			       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Open a beam group
    bool openBeam (unsigned int const &pointingID,
		   unsigned int const &beamID,
		   IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Open a Stokes dataset
    bool openStokesDataset (unsigned int const &pointingID,
			    unsigned int const &beamID,
			    unsigned int const &stokesID,
			    IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Create a new Stokes dataset.
    bool openStokesDataset (unsigned int const &pointingID,
			    unsigned int const &beamID,
			    unsigned int const &stokesID,
			    unsigned int const &nofSamples,
			    unsigned int const &nofSubbands,
			    unsigned int const &nofChannels,
			    DAL::Stokes::Component const &component=DAL::Stokes::I,
			    hid_t const &datatype=H5T_NATIVE_FLOAT,
			    IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Create a new Stokes dataset.
    bool openStokesDataset (unsigned int const &pointingID,
			    unsigned int const &beamID,
			    unsigned int const &stokesID,
			    unsigned int const &nofSamples,
			    std::vector<unsigned int> const &nofChannels,
			    DAL::Stokes::Component const &component=DAL::Stokes::I,
			    hid_t const &datatype=H5T_NATIVE_FLOAT,
			    IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Get the number of primary pointing direction objects
    inline unsigned int nofSubArrayPointings () const {
      return itsSubarrayPointings.size();
    }
    
    //! Get the SysLog group
    SysLog sysLog ();
    
    //! Get a primary pointing direction group
    BF_SubArrayPointing getSubArrayPointing (unsigned int const &pointingID);

    //! Get a beam group
    BF_BeamGroup getBeamGroup (unsigned int const &pointingID,
			       unsigned int const &beamID);

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
    //! Open a SubArrayPointing direction group
    bool openSubArrayPointing (std::string const &name);
      
  }; // Class BF_RootGroup -- end
  
} // Namespace DAL -- end

#endif /* BF_ROOTGROUP_H */
  
