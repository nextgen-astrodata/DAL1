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

#ifndef BF_PRIMARYPOINTING_H
#define BF_PRIMARYPOINTING_H

// Standard library header files
#include <iostream>
#include <string>

// DAL header files
#include <data_common/HDF5GroupBase.h>
#include <data_hl/BF_BeamGroup.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class BF_SubArrayPointing
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief High-level interface to the Sub-Array pointing group of Beam-Formed Data
    
    \author Lars B&auml;hren

    \date 2009/10/28

    \test tBF_SubArrayPointing.cc
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
      </ul>
    </ul>
    
    <h3>Synopsis</h3>

    \verbatim
    SubArrayPointing000             ...  BF_SubArrayPointing
    |-- Beam000                     ...  BF_BeamGroup
    |   |-- CoordinatesGroup
    |   |-- ProcessingHistory
    |   |-- Stokes0                 ...  BF_StokesDataset
    |   |-- Stokes1
    |   |-- Stokes2
    |   `-- Stokes3
    |-- Beam001
    |
    \endverbatim
    
    <h3>Example(s)</h3>
    
  */  
  class BF_SubArrayPointing : public HDF5GroupBase {
    
    //! Station beams
    std::map<std::string,BF_BeamGroup> itsBeams;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    BF_SubArrayPointing ();
    
    //! Default constructor
    BF_SubArrayPointing (hid_t const &location,
			 std::string const &name,
			 IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Argumented constructor
    BF_SubArrayPointing (hid_t const &location,
			 unsigned int const &index,
			 IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Copy constructor
    BF_SubArrayPointing (BF_SubArrayPointing const &other);
    
    // === Destruction ==========================================================
    
    //! Default destructor
    ~BF_SubArrayPointing ();
    
    // --------------------------------------------------------------- Parameters
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, BF_SubArrayPointing.
    */
    inline std::string className () const {
      return "BF_SubArrayPointing";
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

    // === Public methods =======================================================

    //! Convert Sub-array pointing direction index to name of the HDF5 group
    static std::string getName (unsigned int const &index);

    //! Open the file containing the beamformed data.
    bool open (hid_t const &location,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Open a beam group
    bool openBeam (unsigned int const &beamID,
		   IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Get the number of embedded Beam object/groups
    inline unsigned int nofBeams () {
      return itsBeams.size();
    }
    
    //! Retrieve a specific Beam group
    BF_BeamGroup getBeamGroup (unsigned int const &beamID);
    
    //! Retrieve a specific Beam group
    bool getBeamGroup (BF_BeamGroup *beam,
		       unsigned int const &beamID);


    //! Open an existing Stokes dataset
    bool openStokesDataset (unsigned int const &beamID,
			    unsigned int const &stokesID,
			    IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Create a new Stokes dataset from within a beam group
    bool openStokesDataset (unsigned int const &beamID,
			    unsigned int const &stokesID,
			    unsigned int const &nofSamples,
			    unsigned int const &nofSubbands,
			    unsigned int const &nofChannels,
			    DAL::Stokes::Component const &component=DAL::Stokes::I,
			    hid_t const &datatype=H5T_NATIVE_FLOAT,
			    IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Create a new Stokes dataset from within a beam group
    bool openStokesDataset (unsigned int const &beamID,
			    unsigned int const &stokesID,
			    unsigned int const &nofSamples,
			    std::vector<unsigned int> const &nofChannels,
			    DAL::Stokes::Component const &component=DAL::Stokes::I,
			    hid_t const &datatype=H5T_NATIVE_FLOAT,
			    IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Retrieve a specific Stokes dataset
    BF_StokesDataset getStokesDataset (unsigned int const &beamID,
				       unsigned int const &stokesID);
    
    //! Retrieve a specific Stokes dataset
    bool getStokesDataset (BF_StokesDataset *dataset,
			   unsigned int const &beamID,
			   unsigned int const &stokesID);
    
  protected:
    
    //! Set up the list of attributes attached to the structure
    void setAttributes ();
    //! Open the structures embedded within the current one
    bool openEmbedded (IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));

  }; // Class BF_SubArrayPointing -- end
  
} // Namespace DAL -- end

#endif /* BF_PRIMARYPOINTING_H */
  
