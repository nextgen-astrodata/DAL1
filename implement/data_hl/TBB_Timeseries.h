/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
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

#ifndef TBB_TIMESERIES_H
#define TBB_TIMESERIES_H

// casacore header files
#ifdef HAVE_CASA
#include <casa/BasicSL/String.h>
#endif

#include <CommonAttributes.h>
#include <TBB_StationGroup.h>

namespace DAL {  // Namespace DAL -- begin

  /*!
    \class TBB_Timeseries

    \ingroup DAL
    \ingroup data_hl

    \brief Brief description for class TBB_Timeseries

    \author Lars B&auml;hren

    \date 2008/02/06

    \test tTBB_Timeseries.cpp

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>Structure of the \ref dal_formats_tbb
      <li>Classes handling the entities further down in the hierarchy of the
      dataset:
      <ul>
        <li>DAL::CommonAttributes - Collection of attributes common to all
	LOFAR datasets.
        <li>DAL::TBB_StationGroup - Container for the data in the StationGroup
	of LOFAR times-series data.
	<li>DAL::TBB_DipoleDataset - Container for dipole-based data in a LOFAR
	TBB time-series dataset.
      </ul>
      <li>Some of the direct interaction with a HDF5 dataset is implemented in
      dalCommon.h
    </ul>

    <h3>Synopsis</h3>

    This class provides the top-level interface to a standard dataset of LOFAR
    time-series data, as obtained from dumping the contents of the cyclic
    buffers of the transient buffer boards (TBB). Since the time-series data
    are are tailored towards the HDF5 data format, the implementation within this
    class for the most part of it does not go through the additional abstraction
    layers - such as dalDataset or dalGroup - but directly performs the required
    operations through the HDF5 library.

    <h3>Example(s)</h3>

  */
  class TBB_Timeseries {
    
  protected:

    //! LOFAR common root attributes
    CommonAttributes attributes_p;
    //! Name of the data file
    std::string filename_p;
    //! File handle identifier
    hid_t fileID_p;
    //! Station groups contained within this file
    std::vector<TBB_StationGroup> groups_p;
    
  public:
    
    // ------------------------------------------------------------- Construction
    
    //! Default constructor
    TBB_Timeseries ();
    //! Construction using filename of dataset
    TBB_Timeseries (std::string const &filename);
    //! Copy constructor
    TBB_Timeseries (TBB_Timeseries const &other);
    
    // -------------------------------------------------------------- Destruction
    
    //! Destructor
    ~TBB_Timeseries ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another TBB_Timeseries object from which to make a copy.
    */
    TBB_Timeseries& operator= (TBB_Timeseries const &other);
    
    // --------------------------------------------------------------- Parameters
    
    //! Get the common attributes attached to the root group
    inline CommonAttributes commonAttributes () const {
      return attributes_p;
    }
    
    /*!
      \brief Get the name of the data file
      
      \return filename -- The name of the data file.
    */
    inline std::string filename () const {
      return filename_p;
    }
    
    /*!
      \brief Get the object identifier for the data file
      
      \return file_id -- The object identifier for the data file
    */
    inline hid_t file_id () const {
      return fileID_p;
    }
    
    //! Get the name of the telescope
    inline std::string telescope () const {
      return attributes_p.telescope();
    }
    
    //! Get the name of the observer
    inline std::string observer () const {
      return attributes_p.observer();
    }
    
    /*!
      \brief Get the project name/description
      
      \return project -- Name/Description of the project for which this
              observation was carried out; returns an empty string in case no
	      keyword value could be extracted.
    */
    std::string project ();
    
    //! Get the observation ID
    inline std::string observation_id () const {
      return attributes_p.observationID();
    };
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, TBB_Timeseries.
    */
    std::string className () const {
      return "TBB_Timeseries";
    }
    
    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    
    //! Provide a summary of the internal status
    void summary (std::ostream &os);
    
    // ==========================================================================
    //
    //  Parameter access - TBB time-series
    //
    // ==========================================================================
    
    /*!
      \brief Get the number of station groups collected into this file
      
      \return nofStationGroups -- The number of station groups collected into
      this file
    */
    inline uint nofStationGroups () const {
      return groups_p.size();
    }
    
    /*!
      \brief Get the number of dipole datasets collected into this file
      
      \return nofDipoleDatasets -- The number of dipole datasets collected into
      this file.
    */
    inline uint nofDipoleDatasets () {
      uint nofDatasets (0);
      for (uint n(0); n<groups_p.size(); n++) {
	nofDatasets += groups_p[n].nofDipoleDatasets();
      }
      return nofDatasets;
    }
    
    /*!
      \brief Get station groups embedded within the dataset
      
      \return stationGroups -- Vector with a set of TBB_StationGroup objects,
      encapsulating the contents of the groups within the dataset.
    */
    inline std::vector<TBB_StationGroup> stationGroups () const {
      return groups_p;
    }
    
    //! Get one of the embedded station group objects
    TBB_StationGroup stationGroup (uint const &station);
    
    // ==========================================================================
    //
    //  Parameter access - station group
    //
    // ==========================================================================
    
#ifdef HAVE_CASA
    //! Get the type of trigger causing the dump of the TBB data
    casa::Vector<casa::String> trigger_type ();
    //! Time offset from the trigger reference time
    casa::Vector<double> trigger_offset ();
    //! Get the values of the station position
    casa::Matrix<double> station_position_value ();
    //! Get the physical units of the station positions
    casa::Matrix<casa::String> station_position_unit ();
    //! Get the reference codes for the frame of the station positions
    casa::Vector<casa::String> station_position_frame ();
    //! Get the position of the station as a casa::Measure
    casa::Vector<casa::MPosition> station_position ();
    //! Get the values of the beam directions
    casa::Matrix<double> beam_direction_value ();
    //! Get the physical units of the beam directions
    casa::Matrix<casa::String> beam_direction_unit ();
    //! Get the reference codes for the frame of the beam direction
    casa::Vector<casa::String> beam_direction_frame ();
    //! Get the direction of the station beam as casa::Measure
    casa::Vector<casa::MDirection> beam_direction ();
#else
    //! Get the type of trigger causing the dump of the TBB data
    std::vector<std::string> trigger_type ();
    //! Time offset from the trigger reference time
    std::vector<double> trigger_offset ();
    //! Get the reference codes for the frame of the station positions
    std::vector<std::string> station_position_frame ();
#endif
    
    // ==========================================================================
    //
    //  Parameter access - dipole dataset
    //
    // ==========================================================================
    
#ifdef HAVE_CASA
    //! Retrieve the list of channel names (i.e. as string)
    casa::Vector<casa::String> channelNames ();
    //! Retrieve the list of channel IDs
    casa::Vector<int> channelID ();
    //! Get the values of TIME for all present datasets
    casa::Vector<uint> time ();
    //! Get the number of samples elapsed since the last full second
    casa::Vector<uint> sample_number ();
    //! Get the Nyquist zone for the A/D conversion
    casa::Vector<uint> nyquist_zone ();
    //! Get the values of the ADC sample frequency
    casa::Vector<double> sample_frequency_value ();
    //! Get the unit of the ADC sample frequency
    casa::Vector<casa::String> sample_frequency_unit ();
    //! Get the sample frequency as vector of casa::MFrequency
    casa::Vector<casa::MFrequency> sample_frequency ();
    //! Get the values of DATA_LENGTH for all present datasets
    casa::Vector<uint> data_length ();
#else
    //! Retrieve the list of channel names (i.e. as string)
    std::vector<std::string> channelNames ();
    //! Retrieve the list of channel IDs
    std::vector<int> channelID ();
    //! Get the values of TIME for all present datasets
    std::vector<uint> time ();
    //! Get the number of samples elapsed since the last full second
    std::vector<uint> sample_number ();
    //! Get the Nyquist zone for the A/D conversion
    std::vector<uint> nyquist_zone ();
    //! Get the values of the ADC sample frequency
    std::vector<double> sample_frequency_value ();
    //! Get the unit of the ADC sample frequency
    std::vector<std::string> sample_frequency_unit ();
    //! Get the values of DATA_LENGTH for all present datasets
    std::vector<uint> data_length ();
#endif
    
    // ==========================================================================
    //
    //  High-level access to data and attributes
    //
    // ==========================================================================
    
#ifdef HAVE_CASA
    //! Time offset between the individual antennas in units of samples
    casa::Vector<int> sample_offset (uint const &refAntenna=0);
    //! Retrieve a block of ADC values per dipole
    casa::Matrix<double> fx (int const &start=0,
			     int const &nofSamples=1);
    //! Retrieve a block of ADC values per dipole
    casa::Matrix<double> fx (std::vector<int> const &start,
			     int const &nofSamples=1);
    //! Get a casa::Record containing the values of the attributes
    casa::Record attributes2record (bool const &recursive=false);
    //! Create casa::Record used as header record for the CR::DataReader class
    casa::Record attributes2headerRecord ();
#endif
    
  private:
    
    //! Unconditional copying
    void copy (TBB_Timeseries const &other);
    //! Initialize the internal dataspace of the object
    void init ();
    //! Initialize the internal dataspace of the object
    void init (std::string const &filename);
    //! Locate and register the station groups contained within the file
    bool setStationGroups ();
    //! Unconditional deletion
    void destroy(void);
    
  };
  
} // Namespace DAL -- end

#endif /* TBB_TIMESERIES_H */