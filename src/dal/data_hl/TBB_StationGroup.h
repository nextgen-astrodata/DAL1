/***************************************************************************
 *   Copyright (C) 2007                                                    *
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

#ifndef TBB_STATIONGROUP_H
#define TBB_STATIONGROUP_H

// Standard library header files
#include <iostream>
#include <map>
#include <string>

#ifdef DAL_WITH_CASA
#include <casa/Arrays/ArrayIO.h>
#include <casa/Arrays/Matrix.h>
#include <measures/Measures/MDirection.h>
#endif

#include <data_common/HDF5GroupBase.h>
#include <data_hl/TBB_DipoleDataset.h>
#include <data_hl/TBB_StationTrigger.h>

namespace DAL {   // Namespace DAL -- begin

  /*!
    \class TBB_StationGroup

    \ingroup DAL
    \ingroup data_hl

    \brief Container for the data in the StationGroup of LOFAR times-series data

    \author Lars B&auml;hren

    \date 2007/12/10

    \test tTBB_StationGroup.cc

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>\ref dal_icd_001
      <li>DAL::TBB_Timeseries
      <li>DAL::TBB_StationTrigger
      <li>DAL::TBB_DipoleDataset
    </ul>

    <h3>Example(s)</h3>

    <ol>
      <li>Object construction using filename and full name/path of the group
      within the file:
      \code
      std::string filename = "data.h5";
      std::string group    = "Station001";

      TBB_StationGroup group (filename,
                                group);
      \endcode
      <li>Retrieve the list of channels IDs contained within this group:
      \code
      std::string <std::vector> channelID = group.channelID();
      \endcode
      which will return something along the lines of
      \verbatim
      [ 001000000 001000001 .. 001002003 ]
      \endverbatim
      <li>Retrieve a block of samples from the data channels (i.e. dipoles)
      belonging to this station:
      \code
      // TBB_StationGroup constructed somewhere above...

      // set sample at which to start reading
      int start      = 0;
      // set the number of samples to read per data channel (dipole)
      int nofSamples = 1024;

      casa::Matrix<short> data = group.readData (start,
                                           nofSamples);
      \endcode
    </ol>

  */
  class TBB_StationGroup : public HDF5GroupBase {
    
    //! Typedef for the iterator on the map holding the TBB_DipoleDataset
    typedef std::map<std::string,TBB_DipoleDataset>::iterator iterDipoleDataset;
    
    //! Station identifier
    unsigned int stationID_p;
    //! Station trigger group
    TBB_StationTrigger stationTrigger_p;
    //! Number of triggered antennas at this station
    uint nofTriggeredAntennas_p;
    //! Datasets contained within this group
    std::map<std::string,TBB_DipoleDataset> datasets_p;
    //! Selected dipoles
    std::map<std::string,iterDipoleDataset> selectedDatasets_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    TBB_StationGroup ();
    
    //! Argumented constructor
    TBB_StationGroup (hid_t const &location,
		      std::string const &group,
          IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Argumented constructor
    TBB_StationGroup (hid_t const &location,
		      unsigned int const &stationID,
          IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Argumented constructor
    TBB_StationGroup (hid_t const &groupID);
    
    // === Destruction ==========================================================

    //! Destructor
    ~TBB_StationGroup ();
    
    // === Operators ============================================================
    
    //! Overloading of the copy operator
    TBB_StationGroup& operator= (TBB_StationGroup const &other);
    
    // === Parameter access =====================================================
    
    //! Get the name for this group within the HDF5 file
    std::string group_name (bool const &stripPath=false);

    //! Get the set of selected dipoles
    std::set<std::string> selectedDipoles ();

    //! Get the map constaining the actual dipole dataset selection
    inline std::map<std::string,iterDipoleDataset> dipoleSelection () const {
      return selectedDatasets_p;
    }
    
    //! Set the set of selected dipoles
    bool selectDipoles (std::set<std::string> const &selection);

    //! Selected all dipoles from the station
    bool selectAllDipoles ();

    //! Get the number of triggered antennas at this station
    inline uint nofTriggeredAntennas () const {
      return nofTriggeredAntennas_p;
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, TBB_StationGroup.
    */
    std::string className () const {
      return "TBB_StationGroup";
    }
    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);
    
    // === Methods ==============================================================
    
    //! Open a structure (file, group, dataset, etc.)
    bool open (hid_t const &location);

    //! Open a station group
    bool open (hid_t const &location,
	       std::string const &name,
	       IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));
    
    //! Open a dipole dataset
    bool openDipoleDataset (unsigned int const &rspID,
			    unsigned int const &rcuID,
			    bool const &create=true);
    
    /*!
      \brief Get the number of dipole datasets within this station group
      
      \return nofDipoleDatasets -- The number of dipole datasets contained with
      this station group.
    */
    inline uint nofDipoleDatasets () {
      return datasets_p.size();
    }
    
    /*!
      \brief Get the number of selected dipole datasets
      
      \return nofSelectedDatasets -- The number of selected dipole datasets within this station group.
    */
    inline uint nofSelectedDatasets() {
      return selectedDatasets_p.size();
    }

    //! Get the groupname for a station identified by <tt>index</tt>
    static std::string getName (unsigned int const &index);
    
    // ==========================================================================
    //
    //  Parameter access - dipole dataset
    //
    // ==========================================================================

    //! Get attributes <tt>name</tt> from the embedded dipole datasets
    template <typename T>
      bool getAttributes (std::string const &name,
			  std::vector<T> &result)
      {
	bool status (true);

	if (location_p > 0) {
	  T tmp;
	  std::map<std::string,iterDipoleDataset>::iterator it;
	  
    // Clear output vector
    result.clear();
	  
	  for (it=selectedDatasets_p.begin(); it!=selectedDatasets_p.end(); ++it) {
	    it->second->second.getAttribute(name,tmp);
	    result.push_back(tmp);
	  }
	} else {
	  status = false;
	}

	return status;
      }
    
    //! Get attributes <tt>name</tt> from the embedded dipole datasets
#ifdef DAL_WITH_CASA
    template <typename T>
      bool getAttributes (std::string const &name,
			  casa::Vector<T> &result)
      {
	bool status (true);
	
	if (location_p > 0) {
	  uint n(0);
	  T tmp;
	  std::map<std::string,iterDipoleDataset>::iterator it;
	  
	  result.resize(selectedDatasets_p.size());
	  
	  for (it=selectedDatasets_p.begin(); it!=selectedDatasets_p.end(); ++it) {
	    it->second->second.getAttribute(name,tmp);
	    result(n) = tmp;
	    ++n;
	  }
	} else {
	  status = false;
	}
	return status;
      }
    //! Get the sample frequencies as casa::Quantity
    bool sample_frequency (casa::Vector<casa::Quantity> &freq);
    //! Get the sample frequencies as casa::Measure
    bool sample_frequency (casa::Vector<casa::MFrequency> &freq);
    //! Time offset between the individual antennas in units of samples
    casa::Vector<int> sample_offset (uint const &refAntenna=0);
    //! Get the numerical values of the antenna positions within this station.
    casa::Matrix<double> antenna_position_value ();
    //! Get the physical units for the antenna positions within this station.
    casa::Matrix<casa::String> antenna_position_unit ();
    //! Get the positions of the antennas within this station as Measure.
    casa::Vector<casa::MPosition> antenna_position ();
    //! Get identifiers to the datasets within the station group
    casa::Vector<hid_t> datasetIDs ();
#else
    //! Time offset between the individual antennas in units of samples
    std::vector<int> sample_offset (uint const &refAntenna=0);
    //! Get identifiers to the datasets within the station group
    std::vector<hid_t> datasetIDs ();
#endif

    //! Retrieve the list of channels names contained within this group
    std::vector<std::string> dipoleNames ();
    //! Retrieve the list of channels IDs contained within this group
    std::vector<int> dipoleNumbers ();
    
    /*!
      \brief Convert individual ID number to joint unique ID
      
      \param station_id -- Identification number of the LOFAR station within the
             complete array.
      \param rsp_id -- Identification number of a Remote Station Processing
             (RSP) board within a given LOFAR station.
      \param rcu_id -- Identification number of a Receiver Unit (RCU) attached to
             a given RSP board.
      
      \return channel_id -- Unique identifier string for an individual dipole
              within the full LOFAR array.
    */
    inline std::string channelID (unsigned int const &station_id,
				  unsigned int const &rsp_id,
				  unsigned int const &rcu_id)
    {
      return TBB_DipoleDataset::dipoleName(station_id,
					   rsp_id,
					   rcu_id);
    }
    
    // ============================================================================
    //
    //  Methods using casacore
    //
    // ============================================================================
    
#ifdef DAL_WITH_CASA
    
    //! Get the positions of all antennas in the station
    casa::Vector<casa::MPosition> antennaPositions ();
    
    //! Get the direction of the station beam as casa::Measure
    casa::MDirection beam_direction ();
    
    //! Get the position of the station as a casa::Measure
    casa::MPosition station_position ();
    
    //! Retrieve a block of ADC values for the dipoles in this station
    bool readData (casa::Matrix<double> &data,
		   int const &start,
		   int const &nofSamples);
    
    //! Retrieve a block of ADC values for the dipoles in this station
    bool readData (casa::Matrix<double> &data,
		   casa::Vector<int> const &start,
		   int const &nofSamples);
    
    //! Get a casa::Record containing the values of the attributes
    casa::Record attributes2record (bool const &recursive=false);
    
    //! Get a casa::Record containing the values of the attributes
    bool attributes2record (casa::Record &rec,
			    bool const &recursive=false);
#endif
    
  protected:
    
    //! Set up the list of attributes attached to the structure
    void setAttributes ();
    //! Open the structures embedded within the current one
    bool openEmbedded (IO_Mode const &flags=IO_Mode(IO_Mode::OpenOrCreate));

  private:
    
    //! Unconditional copying
    void copy (TBB_StationGroup const &other);
    
    //! Unconditional deletion
    void destroy(void);
    
  };
  
} // Namespace DAL -- end

#endif /* TBB_STATIONGROUP_H */

