/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007                                                    *
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

#ifndef TBB_STATIONGROUP_H
#define TBB_STATIONGROUP_H

// Standard library header files
#include <iostream>
#include <string>

#include <casa/aips.h>
#include <casa/Arrays/Matrix.h>
#include <casa/Arrays/Vector.h>
#include <casa/Containers/Record.h>
#include <measures/Measures/MDirection.h>
#include <measures/Measures/MPosition.h>

#ifndef TBB_DIPOLEDATASET_H
#include <TBB_DipoleDataset.h>
#endif

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class TBB_StationGroup
    
    \ingroup DAL
    
    \brief Container for the data in the StationGroup of LOFAR times-series data
    
    \author Lars B&auml;hren
    
    \date 2007/12/10
    
    \test tTBB_StationGroup.cpp
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>Definition of the LOFAR time-series data format
      <li>DAL::TBB_Timeseries
      <li>DAL::TBB_DipoleDataset
      <li>CR::LOFAR_TBB -- Interface between Data Access Library (DAL) and
      CR::DataReader framework
    </ul>
    
    <h3>Synopsis</h3>
    
    <ol>
      <li>Structure of the \ref dal_formats_tbb
    </ol>

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

      casa::Matrix<short> data = group.fx (start,
                                           nofSamples);
      \endcode
    </ol>
    
  */
  class TBB_StationGroup {
    
    //! Identifier for this group within the HDF5 file
    hid_t groupID_p;
    //! Datasets contained within this group
    std::vector<TBB_DipoleDataset> datasets_p;
    //! Number of triggered antennas at this station
    uint nofTriggeredAntennas_p;
    
  public:
    
    // ==========================================================================
    //
    //  Construction / Destruction
    //
    // ==========================================================================
    
    //! Default constructor
    TBB_StationGroup ();
    
    //! Argumented constructor
    TBB_StationGroup (std::string const &filename,
		      std::string const &group);
    
    //! Argumented constructor
    TBB_StationGroup (hid_t const &location,
		      std::string const &group);
    
    //! Argumented constructor
    TBB_StationGroup (hid_t const &groupID);
    
    //! Copy constructor
    TBB_StationGroup (TBB_StationGroup const &other);
    
    //! Destructor
    ~TBB_StationGroup ();
    
    // ==========================================================================
    //
    //  Operators
    //
    // ==========================================================================
    
    //! Overloading of the copy operator
    TBB_StationGroup& operator= (TBB_StationGroup const &other); 
    
    // ==========================================================================
    //
    //  Parameter access - station group
    //
    // ==========================================================================
    
    /*!
      \brief Get the identifier for this group within the HDF5 file
      
      \return groupID -- The identifier for this group within the HDF5 file
    */
    inline hid_t group_id () const { return groupID_p; }

    //! Get the name for this group within the HDF5 file
    std::string group_name (bool const &stripPath=false);
    
    //! Get the trigger type which cause recording this data
    std::string trigger_type ();

    //! Get the trigger offset
    double trigger_offset ();
    
    //! Get the list of triggered antennas
#ifdef HAVE_CASA
    casa::Vector<uint> triggered_antennas ();
#else
   std::vector<uint> triggered_antennas ();
#endif

   //! Get the number of triggered antennas at this station
   inline uint nofTriggeredAntennas () const {
     return nofTriggeredAntennas_p;
   }
    
#ifdef HAVE_CASA
    //! Get the numerical value of the beam pointing direction.
    casa::Vector<double> beam_direction_value ();
    //! Get the physical units for the beam direction
    casa::Vector<casa::String> beam_direction_unit ();
    //! Get the numerical values of the station position
    casa::Vector<double> station_position_value ();
    //! Get the physical units for the station position
    casa::Vector<casa::String> station_position_unit ();
#else 
    //! Get the numerical value of the beam pointing direction.
    std::vector<double> beam_direction_value ();
    //! Get the physical units for the beam direction
    std::vector<std::string> beam_direction_unit ();
    //! Get the numerical values of the station position
    std::vector<double> station_position_value ();
    //! Get the physical units for the station position
    std::vector<std::string> station_position_unit ();
#endif
    
    //! Get the coordinate frame identifier for the beam direction
    std::string beam_direction_frame ();
    
    //! Get the identifier for the station position reference frame
    std::string station_position_frame ();

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, TBB_StationGroup.
    */
    std::string className () const { return "TBB_StationGroup"; }

    /*!
      \brief Provide a summary of the internal status
    */
    inline void summary () { summary (std::cout); }

    /*!
      \brief Provide a summary of the internal status
    */
    void summary (std::ostream &os);    

    // ------------------------------------------------------------------ Methods

    /*!
      \brief Get the number of dipole datasets within this station group
      
      \return nofDipoleDatasets -- The number of dipole datasets contained with
              this station group.
    */
    inline uint nofDipoleDatasets () { return datasets_p.size(); }

#ifdef HAVE_CASA
    //! Get the direction of the station beam as casa::Measure
    casa::MDirection beam_direction ();

    //! Get the position of the station as a casa::Measure
    casa::MPosition station_position ();
#endif

    // ==========================================================================
    //
    //  Parameter access - dipole dataset
    //
    // ==========================================================================
    
    /*!
      \brief Retrieve the station IDs from the antenna datasets within this group
      
      \return stationIDs -- The station IDs from the antenna datasets within this
              group
    */
#ifdef HAVE_CASA
    casa::Vector<uint> station_id ();
#else
    std::vector<uint> station_id ();
#endif
    
    /*!
      \brief Retrieve the RSP IDs from the antenna datasets within this group

      \return rspIDs -- The RSP IDs from the antenna datasets within this
              group
    */
#ifdef HAVE_CASA
    casa::Vector<uint> rsp_id ();
#else
    std::vector<uint> rsp_id ();
#endif
    
    /*!
      \brief Retrieve the RCU IDs from the antenna datasets within this group

      \return rcuIDs -- The RCU IDs from the antenna datasets within this
              group
    */
#ifdef HAVE_CASA
    casa::Vector<uint> rcu_id ();
#else
    std::vector<uint> rcu_id ();
#endif
    
    //! Get the numerical value of the sample frequencies
#ifdef HAVE_CASA
    casa::Vector<double> sample_frequency_value ();
#else
    std::vector<double> sample_frequency_value ();
#endif

    //! Get the physical unit of the sample frequencies
#ifdef HAVE_CASA
    casa::Vector<casa::String> sample_frequency_unit ();
#else
    std::vector<std::string> sample_frequency_unit ();
#endif

    //! Get the sample freuqncies as casa::Measure
#ifdef HAVE_CASA
    casa::Vector<casa::MFrequency> sample_frequency ();
#endif
    
    /*!
      \brief Get the Nyquist zone in which the ADC is performed
      
      \return zone -- The Nyquist zone in which the analog-to-digital conversion
              (ADC) is performed
    */
#ifdef HAVE_CASA
    casa::Vector<uint> nyquist_zone ();
#else
    std::vector<uint> nyquist_zone ();
#endif
    
#ifdef HAVE_CASA
    //! Get the values of TIME for all present datasets
    casa::Vector<uint> time ();
    //! Get the offset in number of samples from the last full second
    casa::Vector<uint> sample_number ();
    //! Get the number of samples contained in the original data frames
    casa::Vector<uint> samples_per_frame ();
    //! Time offset between the individual antennas in units of samples
    casa::Vector<int> sample_offset (uint const &refAntenna=0);
    //! Get the values of DATA_LENGTH for all present datasets
    casa::Vector<uint> data_length ();
    //! Get the type of the feeds.
    casa::Vector<casa::String> feed ();
    //! Get the numerical values of the antenna positions within this station.    
    casa::Matrix<double> antenna_position_value ();
    //! Get the physical units for the antenna positions within this station.
    casa::Matrix<casa::String> antenna_position_unit ();
    //! Get the positions of the antennas within this station as Measure.
    casa::Vector<casa::MPosition> antenna_position ();
#else
    //! Get the values of TIME for all present datasets
    std::vector<uint> time ();
    //! Get the offset in number of samples from the last full second
    std::vector<uint> sample_number ();
    //! Get the number of samples contained in the original data frames
    std::vector<uint> samples_per_frame ();
    //! Time offset between the individual antennas in units of samples
    std::vector<int> sample_offset (uint const &refAntenna=0);
    //! Get the values of DATA_LENGTH for all present datasets
    std::vector<uint> data_length ();
    //! Get the type of the feeds.
    std::vector<std::string> feed ();
#endif
    
    //! Retrieve the list of channels IDs contained within this group
#ifdef HAVE_CASA
    casa::Vector<int> channelID ();
#else
    std::vector<int> channelID ();
#endif
    
    //! Retrieve the list of channels names contained within this group
#ifdef HAVE_CASA
    casa::Vector<casa::String> channelNames ();
#else
    std::vector<std::string> channelNames ();
#endif
    
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
				  unsigned int const &rcu_id) {
      char uid[10];
      sprintf(uid, "%03d%03d%03d",
	      station_id,rsp_id,rcu_id);
      std::string channel_id (uid);
      return channel_id;
    }
    
    //! Get identifiers to the datasets within the station group
#ifdef HAVE_CASA
    casa::Vector<hid_t> datasetIDs ();
#else
    std::vector<hid_t> datasetIDs ();
#endif

    //! Get the positions of all antennas in the station
#ifdef HAVE_CASA
    casa::Vector<casa::MPosition> antennaPositions ();
#endif
    
    //! Retrieve a block of ADC values for the dipoles in this station
    casa::Matrix<double> fx (int const &start=0,
			     int const &nofSamples=1);
    
    //! Retrieve a block of ADC values for the dipoles in this station
    casa::Matrix<double> fx (int const &start,
			     int const &nofSamples,
			     std::vector<uint> const &dipoleSelection);
    
    //! Get a casa::Record containing the values of the attributes
#ifdef HAVE_CASA
    casa::Record attributes2record (bool const &recursive=false);
    bool attributes2record (casa::Record &rec,
			    bool const &recursive=false);
#endif
    
  private:
    
    //! Initialize the internal dataspace
    void init ();
    
    //! Initialize the internal dataspace
    void init (hid_t const &group_id);
    
    //! Initialize the internal dataspace
    void init (std::string const &filename,
	       std::string const &group);
    
    //! Initialize the internal dataspace
    void init (hid_t const &location,
	       std::string const &group);
    
    //! Unconditional copying
    void copy (TBB_StationGroup const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
    //! Set up the list of dipole datasets contained within this group
    bool setDipoleDatasets ();

  };
  
} // Namespace DAL -- end

#endif /* TBB_STATIONGROUP_H */
  
