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
      <li>\ref dal_file_formats
      <li>Classes handling the entities further down in the hierarchy of the
      dataset:
      <ul>
        <li>DAL::TBB_StationGroup
	<li>DAL::TBB_DipoleDataset
      </ul>
      Some of the direct interaction with a HDF5 dataset is implemented in
      HDF5Common.h
    </ul>
    
    <h3>Synopsis</h3>

    <ol>
      <li><a href="#Structure of the data-set">Structure of the data-set</a>
      <li><a href="#Signal channel ID">Signal channel ID</a>
      <li><a href="#Handling of physical quanities">Handling of physical
      quanities</a>
    </ol>

    This class provides the top-level interface to a standard dataset of LOFAR
    time-series data, as obtained from dumping the contents of the cyclic 
    buffers of the transient buffer boards (TBB).
    
    <ol>
      <li><b><a name="Structure of the data-set">Structure of the
      data-set</a></b> <br>
      \verbatim
      /                             ... Group
      |-- TELESCOPE                 ... Attribute       ... string
      |-- OBSERVER                  ... Attribute       ... string
      |-- PROJECT                   ... Attribute       ... string
      |-- Station001                ... Group
      |   |-- OBSERVATION_ID        ... Attribute       ... string
      |   |-- OBSERVATION_MODE      ... Attribute       ... string
      |   |-- TRIGGER_TYPE          ... Attribute       ... string
      |   |-- TRIGGER_OFFSET        ... Attribute       ... string
      |   |-- TRIG_ANTS             ... Attribute       ... array<int,1>
      |   |-- BEAM_DIRECTION        ... Attribute       ... array<double,2>
      |   |-- 001000000             ... Dataset         ... array<uint,1>
      |   |   |-- STATION_ID        ... Attribute       ... uint
      |   |   |-- RSP_ID            ... Attribute       ... uint 
      |   |   |-- RCU_ID            ... Attribute       ... uint
      |   |   |-- SAMPLE_FREQ       ... Attribute       ... double
      |   |   |-- TIME              ... Attribute       ... uint
      |   |   |-- SAMPLE_NR         ... Attribute       ... uint
      |   |   |-- SAMPLES_PER_FRAME ... Attribute       ... uint
      |   |   |-- DATA_LENGTH       ... Attribute       ... uint
      |   |   |-- NYQUIST_ZONE      ... Attribute       ... uint
      |   |   |-- FEED              ... Attribute       ... string
      |   |   |-- ANT_POSITION      ... Attribute       ... array<double,1>
      |   |   `-- ANT_ORIENTATION   ... Attribute       ... array<double,1>
      |   |-- 001000001
      |   |-- 001000002
      |   |-- 001000003
      |   |-- 001000004
      |   |
      |   |-- 001001000
      |   |-- 001001001
      |   |
      |
      \endverbatim
      As can be read from the above listing, the internal structure of a LOFAR
      time-series dataset is made up of a number of sub-components, each of them
      representing a hierarchical level in the setup of the telecope.
      <ol>
        <li>Root group
	  <table>
	    <tr>
	    <td class="indexkey">Field Name
	    <td class="indexkey">Type
	    <td class="indexkey">Status
	    <td class="indexkey">Default
	    <td class="indexkey">Desctription
	    </tr>
	    <tr>
	      <td>TELESCOPE
	      <td>string
	      <td>mandatory
	      <td>LOFAR
	      <td>Name of the telescope with which the observation has been 
	      performed.
	    </tr>
	    <tr>
	      <td>OBSERVER
	      <td>string
	      <td>mandatory
	      <td>LOFAR/CR
	      <td>Name of the person either responsible for carrying out the 
	      observation or having requested the observation; combination of 
	      both possible.
	    </tr>
	    <tr>
	      <td>PROJECT
	      <td>string
	      <td>mandatory
	      <td>LOFAR/CR
	      <td>Name of the project for which this observation has been 
	      carried out.
	    </tr>
	    <tr>
	      <td>OBSERVATION_ID
	      <td>string
	      <td>mandatory
	      <td>
	      <td>Unique identifier for this observation.
	    </tr>
	  </table>
	  <br>
        <li>Station group
	  <table>
	    <tr>
	    <td class="indexkey">Field Name
	    <td class="indexkey">Type
	    <td class="indexkey">Status
	    <td class="indexkey">Default
	    <td class="indexkey">Desctription
	    </tr>
	    <tr>
	      <td>STATION_ID
	      <td>uint
	      <td>mandatory
	      <td>
	      <td>Identifier of the LOFAR station at which this dipole is
	      located.
	    </tr>
	    <tr>
	      <td>STATION_POSITION
	      <td>uint
	      <td>mandatory
	      <td>
	      <td>Position of the LOFAR station in the coordinate reference frame
	      specified by <tt>REFERENCE_FRAME</tt>
	    </tr>
	    <tr>
	      <td>REFERENCE_FRAME
	      <td>uint
	      <td>mandatory
	      <td>
	      <td>Reference frame for the position cordinates of this station.
	    </tr>
	    <tr>
	      <td>TELESCOPE
	      <td>string
	      <td>optional
	      <td>LOFAR
	      <td>Name of the telescope with which the observation has been 
	      performed.
	    </tr>
	    <tr>
	      <td>OBSERVER
	      <td>string
	      <td>optional
	      <td>LOFAR/CR
	      <td>Name of the person either responsible for carrying out the 
	      observation or having requested the observation; combination of 
	      both possible.
	    </tr>
	    <tr>
	      <td>PROJECT
	      <td>string
	      <td>optional
	      <td>LOFAR/CR
	      <td>Name of the project for which this observation has been 
	      carried out.
	    </tr>
	    <tr>
	      <td>OBSERVATION_ID
	      <td>string
	      <td>optional
	      <td>
	      <td>Unique identifier for this observation.
	    </tr>
	    <tr>
	      <td>OBSERVATION_MODE
	      <td>string
	      <td>mandatory
	      <td>
	      <td>Description of the telescope mode in which this observation
	      was carried out.
	    </tr>
	    <tr>
	      <td>TRIGGER_TYPE
	      <td>string
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	    <tr>
	      <td>TRIGGER_OFFSET
	      <td>string
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	    <tr>
	      <td>TRIGGERED_ANTENNAS
	      <td>array<int,1>
	      <td>optional
	      <td>
	      <td>
	    </tr>
	  </table>
	  <br>
	<li>Dipole dataset
	  <table>
	    <tr>
	    <td class="indexkey">Field Name
	    <td class="indexkey">Type
	    <td class="indexkey">Status
	    <td class="indexkey">Default
	    <td class="indexkey">Desctription
	    </tr>
	    <tr>
	      <td>STATION_ID
	      <td>uint
	      <td>optional
	      <td>
	      <td>Identifier of the LOFAR station at which this dipole is
	      located.
	    </tr>
	    <tr>
	      <td>RSP_ID
	      <td>uint
	      <td>mandatory
	      <td>
	      <td>Identifier of the RSP board, via which this dipole is connected.
	    </tr>
	    <tr>
	      <td>RCU_ID
	      <td>uint
	      <td>mandatory
	      <td>
	      <td>Identifier of the receiver unit (RCU), to which this dipole
	      is connected.
	    </tr>
	    <tr>
	      <td>TIME
	      <td>uint
	      <td>mandatory
	      <td>
	      <td>Date of the observation. Standard unix date i.e. (GMT-)seconds
	      since 1.1.1970
	    </tr>
	    <tr>
	      <td>SAMPLE_NUMBER
	      <td>uint
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	    <tr>
	      <td>SAMPLE_FREQUENCY
	      <td>uint
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	    <tr>
	      <td>NYQUIST_ZONE
	      <td>uint
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	    <tr>
	      <td>ANTENNA_POSITION_VALUE
	      <td>array<double,1>
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	    <tr>
	      <td>ANTENNA_POSITION_UNIT
	      <td>string
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	    <tr>
	      <td>ANTENNA_POSITION_FRAME
	      <td>string
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	    <tr>
	      <td>ANTENNA_ORIENTATION_VALUE
	      <td>array<double,1>
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	    <tr>
	      <td>ANTENNA_ORIENTATION_UNIT
	      <td>string
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	    <tr>
	      <td>ANTENNA_ORIENTATION_FRAME
	      <td>string
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	    <tr>
	      <td>FEED
	      <td>string
	      <td>mandatory
	      <td>
	      <td>
	    </tr>
	  </table>
	  <br>
      </ol>
      <li><b><a name="Signal channel ID">Signal channel ID</a></b> <br> 
      The signal channel for an individual reception element (dipole) can be
      uniquely identified through a combination of the following three separate
      identifiers:
      <ol>
        <li>STATION_ID  -- Identification number of the LOFAR station within the
	complete array.
	<li>RSP_ID -- Identification number of a Remote Station Processing (RSP)
	board within a given LOFAR station.
	<li>RCU_ID -- Identification number of a Receiver Unit (RCU) attached to
	a given RSP board.
      </ol>
      <li><b><a name="Handling of physical quanities">Handling of physical
      quanities</a></b>
    </ol>

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
    
    /*!
      \brief Default constructor

      A minimal setup of the internal dataspace is performed, but since no
      data file is provided, no inspection of the data structure is carried out.
    */
    TBB_Timeseries ();
    
    /*!
      \brief Default constructor

      \param filename -- Name of the data file
    */
    TBB_Timeseries (std::string const &filename);
    
    /*!
      \brief Copy constructor
      
      \param other -- Another TBB_Timeseries object from which to create
             this new one.
    */
    TBB_Timeseries (TBB_Timeseries const &other);
    
    // -------------------------------------------------------------- Destruction

    /*!
      \brief Destructor
    */
    ~TBB_Timeseries ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another TBB_Timeseries object from which to make a
             copy.
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

    /*!
      \brief Get station groups embedded within the dataset

      \return stationGroups -- Vector with a set of TBB_StationGroup objects,
              encapsulating the contents of the groups within the dataset.
    */
    inline std::vector<TBB_StationGroup> stationGroups () const {
      return groups_p;
    }

    /*!
      \brief Get the name of the telescope
      
      \return telescope -- The name of the telescope with which the data were
              recorded; returns an empty string in case no keyword value could
	      be extracted.
    */
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

    /*!
      \brief Provide a summary of the internal status
    */
    inline void summary () {
      summary (std::cout);
    }

    /*!
      \brief Provide a summary of the internal status
    */
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
    
    // ==========================================================================
    //
    //  Parameter access - station group
    //
    // ==========================================================================
    
#ifdef HAVE_CASA
    casa::Vector<std::string> trigger_type ();
#else
    std::vector<std::string> trigger_type ();
#endif
    
#ifdef HAVE_CASA
    casa::Vector<double> trigger_offset ();
#else
    std::vector<double> trigger_offset ();
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
    casa::Vector<uint> times ();
#else
    std::vector<uint> times ();
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

    /*!
      \brief Get a casa::Record containing the values of the attributes

      \param addRecursive -- Recursively add information from embedded HDF5
             objects, such as groups and datasets? 

      \return record -- A casa::Record container holding the values of the 
              attributes attached to the dataset for this dipole
    */
    casa::Record attributes2record (bool const &addRecursive=false);

    //! Set up the record which is used as member of the CR::DataReader class
    casa::Record attributes2headerRecord ();
    void attributes2headerRecord (casa::Record &rec);
    
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
  
