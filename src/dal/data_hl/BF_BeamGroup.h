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

#ifndef BF_GROUPBEAM_H
#define BF_GROUPBEAM_H

// Standard library header files
#include <iostream>
#include <string>
#include <map>

// DAL header files
#include <core/HDF5Attribute.h>
#include <coordinates/CoordinatesGroup.h>
#include <data_common/HDF5GroupBase.h>
#include <data_hl/BF_ProcessingHistory.h>
#include <data_hl/BF_StokesDataset.h>

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
      <li>\ref dal_icd_003
      <li>\ref dal_icd_005
    </ul>
    
    <h3>Synopsis</h3>

    \verbatim
    .
    |-- BEAM_000                        Group
    |   |-- GROUPTYPE                   Attr.               string
    |   |-- TARGET                      Attr.               array<string,1>
    |   |-- NOF_STATIONS                Attr.               int
    |   |-- STATIONS_LIST               Attr.               array<string,1>
    |   |-- POINT_RA                    Attr.               double
    |   |-- POINT_DEC                   Attr.               double
    |   |-- POSITION_OFFSET_RA          Attr.               double
    |   |-- POSITION_OFFSET_DEC         Attr.               double
    |   |-- PB_DIAMETER_RA              Attr.               double
    |   |-- PB_DIAMETER_DEC             Attr.               double
    |   |-- PB_CENTER_FREQUENCY         Attr.               double
    |   |-- PB_CENTER_FREQUENCY_UNIT    Attr.               string
    |   |-- FOLDED_DATA                 Attr.               bool
    |   |-- FOLD_PERIOD                 Attr.               double
    |   |-- FOLD_PERIOD_UNIT            Attr.               string
    |   |-- DEDISPERSION                Attr.               string
    |   |-- DISPERSION_MEASURE          Attr.               double
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
    |-- BEAM_001                        Group
    |
    \endverbatim
    
    <h3>Example(s)</h3>
    
  */  
  class BF_BeamGroup : public HDF5GroupBase {

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
		  IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
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

    // === Public methods =======================================================

    //! Open the file containing the beamformed data.
    bool open (hid_t const &location,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));

    // Open an existing Stokes dataset from within a beam group
    bool openStokesDataset (unsigned int const &stokesID);

    // Open an existing Stokes dataset from within a beam group
    bool openStokesDataset (std::string const &name);
    
    //! Create a new Stokes dataset from within a beam group
    bool openStokesDataset (unsigned int const &stokesID,
			    unsigned int const &nofSamples,
			    unsigned int const &nofSubbands,
			    unsigned int const &nofChannels,
			    DAL::Stokes::Component const &component=DAL::Stokes::I,
			    hid_t const &datatype=H5T_NATIVE_FLOAT,
			    IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Create a new Stokes dataset from within a beam group
    bool openStokesDataset (unsigned int const &stokesID,
			    unsigned int const &nofSamples,
			    std::vector<unsigned int> const &nofChannels,
			    DAL::Stokes::Component const &component=DAL::Stokes::I,
			    hid_t const &datatype=H5T_NATIVE_FLOAT,
			    IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Retrieve a specific Stokes dataset
    BF_StokesDataset getStokesDataset (unsigned int const &stokesID);
    
    //! Retrieve a specific Stokes dataset
    bool getStokesDataset (BF_StokesDataset *dataset,
			   unsigned int const &stokesID);
    
    /*!
      \brief Write \c data to Stokes dataset identified by \c index.
      \param index   -- Index of the Stokes dataset.
      \param data    -- Array with the data to be written.
      \param start   -- Start position from which on the \c data are supposed to
             be written.
      \param block   -- Shape of the \c data array.
      \return status -- Status of the operation; returns \e false in case an
              error was encountered, e.g. because the Stokes dataset the data 
	      were supposed to be written to does not exit.
    */
    template <class T>
      bool writeData (unsigned int const &index,
		      T const data[],
		      std::vector<int> const &start,
		      std::vector<int> const &block)
      {
	bool status      = true;
	std::string name = getName (index);
	std::map<std::string,BF_StokesDataset>::iterator it;

	/*____________________________________________________________
	  Check if the Stokes dataset exists and is available; if this
	  is the case forward the function call to actually write the
	  data.
	*/

	it = itsStokesDatasets.find(name);
	if (it==itsStokesDatasets.end()) {
	  std::cerr << "[BF_BeamGroup::writeData] No such dataset "
		    << name 
		    << " - unable to write data!"
		    << std::endl;
	  status = false;
	} else {
	  /* write the data through BF_StokesDataset::writeData() */
	  status = it->second.writeData(data,start,block);
	}

	return status;
      }
    
    // === Static methods =======================================================
    
    //! Convert beam index to name of the HDF5 group
    static std::string getName (unsigned int const &index);

  protected:
    
    //! Open the structures embedded within the current one
    bool openEmbedded (IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    //! Set up the list of attributes attached to the structure
    void setAttributes ();

  }; // Class BF_BeamGroup -- end
  
} // Namespace DAL -- end

#endif /* BF_BEAMGROUP_H */
  
