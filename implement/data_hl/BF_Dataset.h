/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 2286 2009-02-03 10:50:48Z baehren                    $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#ifndef BF_DATASET_H
#define BF_DATASET_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <CommonInterface.h>
#include <Filename.h>
#include <BF_Dataset.h>
#include <BF_PrimaryPointing.h>
#include <BF_SysLog.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_Dataset
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the root-group of a beamformed dataset
    
    \author Lars B&auml;hren

    \date 2009/10/28

    \test tBF_Dataset.cc
    
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
	<li>CommonInterface -- Common functionality for the high-level
	interfaces to the datasets
	<li>BF_PrimaryPointing
	<li>BF_SysLog
      </ul>
    </ul>
    
    <h3>Synopsis</h3>

    Basic hierarchical structure used DAL classes:
    \verbatim
    /
    |-- PrimaryPointing000
    |   |-- Beam000
    |   |-- Beam000
    |   |
    |
    |-- PrimaryPointing001
    |
    `-- SysLog
    \endverbatim
    
    <h3>Example(s)</h3>
    
  */  
  class BF_Dataset : public CommonInterface {

    //! Name of the data file
    std::string filename_p;
    //! LOFAR common attributes attached to the root group of the dataset
    CommonAttributes commonAttributes_p;
    //! Primary Pointing Directions
    std::map<std::string,BF_PrimaryPointing> primaryPointings_p;
    //! Container for system-wide logs
    std::map<std::string,BF_SysLog> sysLog_p;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    BF_Dataset (std::string const &filename);
    
    //! Argumented constructor
    BF_Dataset (DAL::Filename &infile,
		bool const &create=true);
    
    //! Argumented constructor
    BF_Dataset (CommonAttributes const &attributes,
		bool const &create=true);
    
    // === Destruction ==========================================================
    
    //! Default destructor
    ~BF_Dataset ();
    
    // --------------------------------------------------------------- Parameters

    //! Get the set of common attributes attached to the root group of the file
    CommonAttributes commonAttributes ();

    //! Set the set of common attributes attached to the root group of the file
    bool setCommonAttributes (CommonAttributes const &attributes);

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, BF_Dataset.
    */
    inline std::string className () const {
      return "BF_Dataset";
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
      return primaryPointings_p.size();
    }

    //! Get a primary pointing direction group
    BF_PrimaryPointing primaryPointing (unsigned int const &pointingID);

    //! Open a beam group
    bool openBeam (unsigned int const &pointingID,
		   unsigned int const &beamID,
		   bool const &create=true);

    //! Open a system log group
    bool openSysLog (bool const &create=true);

    //! Get the SysLog group
    BF_SysLog sysLog ();
    
  protected:
    
    //! Open the structures embedded within the current one
    bool openEmbedded (bool const &create);
    //! Set up the list of attributes attached to the structure
    void setAttributes ();

  private:
    
    //! Initialize the internal settings of the object
    void init (CommonAttributes const &attributes);
    //! Open a PrimaryPointing direction group
    bool openPrimaryPointing (std::string const &name);
      
  }; // Class BF_Dataset -- end
  
} // Namespace DAL -- end

#endif /* BF_DATASET_H */
  
