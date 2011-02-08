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

#ifndef BF_GROUPBEAM_H
#define BF_GROUPBEAM_H

// Standard library header files
#include <iostream>
#include <string>
#include <map>

// DAL header files
#include <HDF5Attribute.h>
#include <HDF5CommonInterface.h>
#include <CoordinatesGroup.h>
#include <BF_ProcessingHistory.h>
#include <BF_StokesDataset.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_BeamGroup
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the Beam group of Beam-Formed Data
    
    \author Lars B&auml;hren

    \date 2009/10/28

    \test tBF_BeamGroup.cc
    
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
	<li>CoordinatesGroup
	<li>BF_ProcessingHistory
      </ul>
    </ul>
    
    <h3>Synopsis</h3>

    \verbatim
    .
    |-- Beam000                         Group
    |   |-- GROUPTYPE                   Attr.               string
    |   |-- TARGET                      Attr.               array<string,1>
    |   |-- NOF_STATIONS                Attr.               int
    |   |-- STATIONS_LIST               Attr.               array<string,1>
    |   |-- POINT_RA                    Attr.               double
    |   |-- POINT_DEC                   Attr.               double
    |   |-- POSITION_OFFSET_RA          Attr.               double
    |   |-- POSITION_OFFSET_DEC         Attr.               double
    |   |-- PB_DIAMETER_RA              Attr.               float
    |   |-- PB_DIAMETER_DEC             Attr.               float
    |   |-- PB_CENTER_FREQUENCY         Attr.               double
    |   |-- PB_CENTER_FREQUENCY_UNIT    Attr.               string
    |   |-- FOLDED_DATA                 Attr.               bool
    |   |-- FOLD_PERIOD                 Attr.               float
    |   |-- FOLD_PERIOD_UNIT            Attr.               string
    |   |-- DEDISPERSION                Attr.               string
    |   |-- DISPERSION_MEASURE          Attr.               float
    |   |-- DISPERSION_MEASURE_UNIT     Attr.               string
    |   |-- BARYCENTER                  Attr.               bool
    |   |-- NOF_STOKES                  Attr.               int
    |   |-- STOKES_COMPONENTS           Attr.               array<string,1>
    |   |-- COMPLEX_VOLTAGE             Attr.               bool
    |   |-- SIGNAL_SUM                  Attr.               string
    |   |-- COORDINATES                 Group
    |   |-- PROCESSING_HISTORY          Group
    |   |-- STOKES_0                    Dataset
    |   |-- STOKES_1                    Dataset
    |   |-- STOKES_2                    Dataset
    |   `-- STOKES_3                    Dataset
    |-- Beam001                         Group
    |
    \endverbatim
    
    <h3>Example(s)</h3>
    
  */  
  class BF_BeamGroup : public HDF5CommonInterface {

    //! Procesing history group
    std::map<std::string,BF_ProcessingHistory> itsProcessingHistory;
    //! Coordinates group
    std::map<std::string,CoordinatesGroup> itsCoordinates;
    //! Stokes datasets
    std::map<std::string,BF_StokesDataset> itsStokesDatasets;

  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    BF_BeamGroup ();
    //! Argumented constructor
    BF_BeamGroup (hid_t const &location,
		  std::string const &name);
    //! Argumented constructor
    BF_BeamGroup (hid_t const &location,
		  unsigned int const &index,
		  bool const &create);
    
    // === Destruction =========================================================
    
    //! Default destructor
    ~BF_BeamGroup ();
    
    // === Parameter access =====================================================

    //! Get the number of Stokes datasets embedded inside this group
    inline unsigned int nofStokesDatasets () const {
      return itsStokesDatasets.size();
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, BF_BeamGroup.
    */
    inline std::string className () const {
      return "BF_BeamGroup";
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

    //! Convert beam index to name of the HDF5 group
    static std::string getName (unsigned int const &index);

    //! Open the file containing the beamformed data.
    bool open (hid_t const &location,
	       std::string const &name,
	       bool const &create=true);

    //! Open an existing Stokes dataset
    bool openStokesDataset (std::string const &name,
			    unsigned int const &nofSamples,
			    unsigned int const &nofSubbands,
			    unsigned int const &nofChannels,
			    DAL::Stokes::Component const &component=DAL::Stokes::I,
			    hid_t const &datatype=H5T_NATIVE_FLOAT);

    //! Open an existing Stokes dataset
    bool openStokesDataset (std::string const &name,
			    unsigned int const &nofSamples,
			    std::vector<unsigned int> const &nofChannels,
			    DAL::Stokes::Component const &component=DAL::Stokes::I,
			    hid_t const &datatype=H5T_NATIVE_FLOAT);
    
  protected:
    
    //! Open the structures embedded within the current one
    bool openEmbedded (bool const &create);
    //! Set up the list of attributes attached to the structure
    void setAttributes ();

  }; // Class BF_BeamGroup -- end
  
} // Namespace DAL -- end

#endif /* BF_BEAMGROUP_H */
  
