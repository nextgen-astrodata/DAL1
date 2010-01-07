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

#ifndef BF_STATIONBEAM_H
#define BF_STATIONBEAM_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <CommonInterface.h>
#include <BF_PencilBeam.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_StationBeam
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the station beam of a BF dataset
    
    \author Lars B&auml;hren

    \date 2009/10/28

    \test tBF_StationBeam.cc
    
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

    \verbatim
    StationBeam001
    |-- PencilBeam000
    |   |-- CoordinatesGroup
    |   `-- ProcessingHistory
    |-- PencilBeam001
    |   |-- CoordinatesGroup
    |   `-- ProcessingHistory
    |-- PencilBeam002
    |
    \endverbatim
    
    <h3>Example(s)</h3>
    
  */  
  class BF_StationBeam : public CommonInterface {
    
    //! Station beams
    std::map<std::string,BF_PencilBeam> pencilBeams_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    BF_StationBeam ();
    
    //! Argumented constructor
    BF_StationBeam (hid_t const &location,
		    unsigned int const &index,
		    bool const &create);
    
    // === Destruction ==========================================================
    
    //! Default destructor
    ~BF_StationBeam ();
    
    // --------------------------------------------------------------- Parameters
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, BF_StationBeam.
    */
    inline std::string className () const {
      return "BF_StationBeam";
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

    //! Convert StationBeam index to name of the HDF5 group
    static std::string getName (unsigned int const &index);

    //! Open the file containing the beamformed data.
    bool open (hid_t const &location,
	       std::string const &name,
	       bool const &create=true);
    
    //! Open a pencil beam group
    bool openPencilBeam (unsigned int const &pencilID,
			 bool const &create=true);

    //! Get the number of embedded PencilBeam object/groups
    inline unsigned int nofPencilBeams () {
      return pencilBeams_p.size();
    }
    
  protected:
    
    //! Open the structures embedded within the current one
    bool openEmbedded (bool const &create);
    //! Set up the list of attributes attached to the structure
    void setAttributes ();

  }; // Class BF_StationBeam -- end
  
} // Namespace DAL -- end

#endif /* BF_DATASET_H */
  
