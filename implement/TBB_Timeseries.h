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

// Standard library header files
#include <iostream>
#include <string>
#include <vector>

// casacore header files
#include <casa/Arrays/Matrix.h>
#include <casa/Arrays/Vector.h>
#include <casa/BasicSL/String.h>

#include <dalCommon.h>
#include <TBB_StationGroup.h>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class TBB_Timeseries
    
    \ingroup DAL
    
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
        <li>DAL::TBB_StationGroup
	<li>DAL::TBB_DipoleDataset
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
    std::string telescope ();

    /*!
      \brief Get the name of the observer
      
      \return observer -- The name of the observer; returns an empty string in
              case no keyword value could be extracted.
    */
    std::string observer ();
    
    /*!
      \brief Get the project name/description
      
      \return project -- Name/Description of the project for which this 
              observation was carried out; returns an empty string in case no
	      keyword value could be extracted.
    */
    std::string project ();

    /*!
      \brief Get the observation ID

      \return observation_id -- The observation ID; returns an empty string in
              case no keyword value could be extracted.
    */
    std::string observation_id ();
    
    /*!
      \brief Get the description of the observation mode

      \return observation_mode -- Description/type of observation mode
    */
    std::string observation_mode ();
    
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

    //! Get the type of trigger causing the dump of the TBB data
#ifdef HAVE_CASA
    casa::Vector<casa::String> trigger_type ();
#else
    std::vector<std::string> trigger_type ();
#endif

    //! Time offset from the trigger reference time
#ifdef HAVE_CASA
    casa::Vector<double> trigger_offset ();
#else
    std::vector<double> trigger_offset ();
#endif

#ifdef HAVE_CASA
    //! Get the position of the station as a casa::Measure
    casa::Vector<casa::MPosition> station_position ();
    //! Get the direction of the station beam as casa::Measure
    casa::Vector<casa::MDirection> beam_direction ();
#endif
    
    // ==========================================================================
    //
    //  Parameter access - dipole dataset
    //
    // ==========================================================================
    
    /*!
      \brief Retrieve the list of channel names (i.e. as string)
      
      \return names -- A list of the channel IDs for all the dipoles within
              this data set.
    */
#ifdef HAVE_CASA
    casa::Vector<casa::String> channelNames ();
#else
    std::vector<std::string> channelNames ();
#endif

    /*!
      \brief Retrieve the list of channel IDs
      
      \return channelID -- A list of the channel IDs for all the dipoles within
              this data set.
     */
#ifdef HAVE_CASA
    casa::Vector<int> channelID ();
#else
    std::vector<int> channelID ();
#endif
    
    /*!
      \brief Get the values of TIME for all present datasets
      
      \return times -- Values of the TIME attribute for all datasets present in
              this station group
    */
#ifdef HAVE_CASA
    casa::Vector<uint> time ();
#else
    std::vector<uint> time ();
#endif

    /*!
      \brief Get the values of SAMPLE_FREQUENCY for all present datasets
      
      \return sample_frequencies -- Values of the SAMPLE_FREQUENCY attribute
              for all datasets present in this station group
    */
#ifdef HAVE_CASA
    casa::Vector<double> sample_frequency ();
#else
    std::vector<double> sample_frequency ();
#endif

    /*!
      \brief Get the values of DATA_LENGTH for all present datasets
      
      \return times -- Values of the DATA_LENGTH attribute for all datasets present in
              this station group
    */
#ifdef HAVE_CASA
    casa::Vector<uint> data_length ();
#else
    std::vector<uint> data_length ();
#endif

    /*!
      \brief Retrieve a block of ADC values per dipole

      \param start      -- Number of the sample at which to start reading
      \param nofSamples -- Number of samples to read, starting from the position
             given by <tt>start</tt>.

      \return fx -- [nofSamples,dipole] Array of raw ADC samples representing
              the electric field strength as function of time.
    */
    casa::Matrix<double> fx (int const &start=0,
			     int const &nofSamples=1);

    /*!
      \brief Retrieve a block of ADC values per dipole

      \param start      -- Number of the sample at which to start reading; w.r.t. 
             to the variant where just a single value is provided, this function
	     allows to set the starting point for each dipole individually
      \param nofSamples -- Number of samples to read, starting from the position
             given by <tt>start</tt>.

      \return fx -- [nofSamples,dipole] Array of raw ADC samples representing
              the electric field strength as function of time.
    */
    casa::Matrix<double> fx (std::vector<int> const &start,
			     int const &nofSamples=1);

#ifdef HAVE_CASA
    //! Get a casa::Record containing the values of the attributes
    casa::Record attributes2record (bool const &recursive=false);
    
    //! Create casa::Record used as header record for the CR::DataReader class
    casa::Record attributes2headerRecord ();
    
#endif
    
  private:
    
    /*!
      \brief Unconditional copying

      \param other -- Another TBB_Timeseries object from which to create
             this new one.
    */
    void copy (TBB_Timeseries const &other);
    
    /*!
      \brief Initialize the internal dataspace of the object
    */
    void init ();

    /*!
      \brief Initialize the internal dataspace of the object

      \param filename -- Name of the data file
    */
    void init (std::string const &filename);

    /*!
      \brief Locate and register the station groups contained within the file

      \return status -- Status of the operation; returns <tt>false</tt> in case
              an error was encountered.
    */
    bool setStationGroups ();
    
    /*!
      \brief Unconditional deletion 
    */
    void destroy(void);
    
  };
  
} // Namespace DAL -- end

#endif /* TBB_TIMESERIES_H */
  
