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

#ifndef BF_BEAM_H
#define BF_BEAM_H

// Standard library header files
#include <iostream>
#include <string>
#include <map>

// DAL header files
#include <CommonInterface.h>
#include <CoordinatesGroup.h>
#include <BF_ProcessingHistory.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_Beam
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the station beam of a BF dataset
    
    \author Lars B&auml;hren

    \date 2009/10/28

    \test tBF_Beam.cpp
    
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
	<li>CoordinatesGroup
	<li>BF_ProcessingHistory
      </ul>
    </ul>
    
    <h3>Synopsis</h3>

    \verbatim
    Beam000
    |-- CoordinatesGroup
    `-- ProcessingHistory
    \endverbatim
    
    <h3>Example(s)</h3>
    
  */  
  class BF_Beam : public CommonInterface {

    //! Procesing history group
    std::map<std::string,BF_ProcessingHistory> processingHistory_p;
    //! Coordinates group
    std::map<std::string,CoordinatesGroup> coordinates_p;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    BF_Beam ();
    //! Argumented constructor
    BF_Beam (hid_t const &location,
	     std::string const &name);
    //! Argumented constructor
    BF_Beam (hid_t const &location,
	     unsigned int const &index,
	     bool const &create);
    
    // === Destruction =========================================================
    
    //! Default destructor
    ~BF_Beam ();
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, BF_Beam.
    */
    inline std::string className () const {
      return "BF_Beam";
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

    //! Convert beam index to name of the HDF5 group
    static std::string getName (unsigned int const &index);

    //! Open the file containing the beamformed data.
    bool open (hid_t const &location,
	       std::string const &name,
	       bool const &create=true);

    //! Open the processing history group
    bool openProcessingHistory (bool const &create=true);

    //! Open the coordinates group
    bool openCoordinatesGroup (bool const &create=true);
    
  protected:
    
    //! Open the structures embedded within the current one
    bool openEmbedded (bool const &create);
    //! Set up the list of attributes attached to the structure
    void setAttributes ();

  }; // Class BF_Beam -- end
  
} // Namespace DAL -- end

#endif /* BF_DATASET_H */
  
