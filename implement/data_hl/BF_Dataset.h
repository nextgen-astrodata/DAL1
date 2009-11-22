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
#include <BF_Dataset.h>
#include <BF_StationBeam.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_Dataset
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the root-group of a beamformed dataset
    
    \author Lars B&auml;hren

    \date 2009/10/28

    \test tBF_Dataset.cpp
    
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
      </ul>
    </ul>
    
    <h3>Synopsis</h3>

    
    <h3>Example(s)</h3>
    
  */  
  class BF_Dataset : public CommonInterface {
    
    //! LOFAR common attributes attached to the root group of the dataset
    CommonAttributes commonAttributes_p;
    //! Station beams
    std::map<std::string,BF_StationBeam> stationBeams_p;

  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Argumented constructor
    BF_Dataset (Filename const &filename);
    
    //! Argumented constructor
    BF_Dataset (CommonAttributes const &commonAttributes);
    
    // -------------------------------------------------------------- Destruction

    //! Destructor
    ~BF_Dataset ();
    
    // --------------------------------------------------------------- Parameters

    //! Get the set of common attributes attached to the root group of the file
    inline CommonAttributes commonAttributes () const {
      return commonAttributes_p;
    }

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
    inline void summary () {
      summary (std::cout);
    }
    
    /*!
      \brief Provide a summary of the internal status
      
      \param os -- Output stream to which the summary is written.
    */
    void summary (std::ostream &os);    

    // ------------------------------------------------------------------ Methods

    //! Open a station beam group
    bool stationBeam (unsigned int const &id,
		      bool const &create=true);
    
    //! Get the number of station beam objects attached to the root group
    inline unsigned int nofStationBeams () const {
      return stationBeams_p.size();
    }

    //! Get the names of the groups holding station beam data
    std::vector<std::string> stationBeamGroups ();

    //! Get the station beam objects attached to the root group of the file
    std::vector<BF_StationBeam> stationBeams ();
    
    //! Get the object for a specific station beam
    BF_StationBeam stationBeam (std::string const &beam);

  protected:
    
    //! Open the file containing the beamformed data.
    bool open (hid_t const &location,
	       std::string const &name,
	       bool const &create=true);
    //! Open the structures embedded within the current one
    bool openEmbedded ();
    //! Set up the list of attributes attached to the structure
    void setAttributes ();

  private:
    
    //! Initialize the internal settings of the object
    void init (CommonAttributes const &attributes);

    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class BF_Dataset -- end
  
} // Namespace DAL -- end

#endif /* BF_DATASET_H */
  
