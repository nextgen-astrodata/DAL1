/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
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

#ifndef TBB_TIMESERIES_H
#define TBB_TIMESERIES_H

// casacore header files
#ifdef HAVE_CASA
#include <casa/BasicSL/String.h>
#endif

#include <CommonAttributes.h>
#include <CommonInterface.h>
#include <SysLog.h>
#include <TBB_StationGroup.h>
#include <TBB_StationTrigger.h>

namespace DAL {  // Namespace DAL -- begin

  /*!
    \class TBB_Timeseries

    \ingroup DAL
    \ingroup data_hl

    \brief Brief description for class TBB_Timeseries

    \author Lars B&auml;hren

    \date 2008/02/06

    \test tTBB_Timeseries.cc

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>Structure of the \ref dal_formats_tbb
      <li>Classes handling the entities further down in the hierarchy of the
      dataset:
      <ul>
        <li>DAL::CommonAttributes - Collection of attributes common to all
	LOFAR datasets.
	<li>DAL::TBB_TriggerTable -- Container for the trigger parameter data.
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

    \verbatim
    OBSERVATION
    |-- Station000                            ...  Group
    |   |-- 000000000                         ...  Dataset
    |   |   |-- STATION_ID                    ...  Attribute
    |   |   |-- RSP_ID                        ...  Attribute
    |   |   |-- RCU_ID                        ...  Attribute
    |   |   |
    |   |
    |   |-- 000000001                         ...  Dataset
    |   |
    |   `-- StationCalibration                ...  Group
    |       |-- GainCurve                     ...  Dataset
    |       |   `-- CoordinatesGroup          ...  Group
    |       |       `-- FrequencyCoordinate   ...  Group
    |       |-- NoiseCurve                    ...  Dataset
    |       |   `-- CoordinatesGroup          ...  Group
    |       |       `-- FrequencyCoordinate   ...  Group
    |       `-- BeamShape                     ...  Dataset
    |           `-- CoordinatesGroup          ...  Group
    |               |-- DirectionCoordinate   ...  Group
    |               `-- FrequencyCoordinate   ...  Group
    |
    |-- Station001                            ...  Group
    |   |-- 001000000                         ...  Dataset
    |   |-- 001000001                         ...  Dataset
    |   |
    |   `-- Calibration                       ...  Group
    |
    `-- TriggerTable                          ...  Group
    \endverbatim

    In order to provide the positions of the antennas/dipoles for which data
    where received from the TBBs the following set of metadata is required:
    \verbatim
    STATION  NAME    STATION_ID  RSP_ID  RCU_ID  POLARIZATION  POSITION  ORIENTATION
    CS001    center  001         -1      -1      -1            [x,y,z]   [a,b,c]
    CS001    lba0    001         000     000      x            [x,y,z]   [a,b,c]
    CS001    lba0    001         000     001      y            [x,y,z]   [a,b,c]
    CS001    lba1    001         000     002      x            [x,y,z]   [a,b,c]
    CS001    lba1    001         000     003      y            [x,y,z]   [a,b,c]
    CS001    lba2    001         000     004      x            [x,y,z]   [a,b,c]
    CS001    lba2    001         000     005      y            [x,y,z]   [a,b,c]
    CS001    lba3    001         000     006      x            [x,y,z]   [a,b,c]
    CS001    lba3    001         000     007      y            [x,y,z]   [a,b,c]
    CS001    lba4    001         001     008      x            [x,y,z]   [a,b,c]
    CS001    lba4    001         001     009      y            [x,y,z]   [a,b,c]
    \endverbatim
    Notes:
    <ul>
      <li>Even though in principle one might want/need to account for changes in
      orientation of the dipoles w.r.t. the underlying reference frame, the
      additional values almost certainly will not be used (at least not for a
      long time).
    </ul>
    
    <h3>Example(s)</h3>

    <ol>
      <li>Access to attributes attached to the dipole datasets
      \code
      // Get the values of DATA_LENGTH for all present datasets
      std::vector<uint> dataength = ts.data_length ();
      \endcode
    </ol>
    
  */
  class TBB_Timeseries : public CommonInterface {
    
    //! Typedef for the iterator on the map holding the TBB_DipoleDataset
    typedef std::map<std::string,TBB_DipoleDataset>::iterator iterDipoleDataset;
    
  protected:
    
    //! Name of the data file
    std::string filename_p;
    //! Container for system-wide logs attached to the root group of the file
    std::map<std::string,SysLog> sysLog_p;
    //! Station groups attached to the root group of the file
    std::map<std::string,TBB_StationGroup> stationGroups_p;
    //! Selected dipoles
    std::map<std::string,iterDipoleDataset> selectedDatasets_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    TBB_Timeseries ();
    //! Argumented constructor
    TBB_Timeseries (std::string const &filename);
    //! Create a new dataset from LOFAR common attributes
    TBB_Timeseries (CommonAttributes const &attributes);
    //! Copy constructor
    TBB_Timeseries (TBB_Timeseries const &other);
    
    // === Destruction ==========================================================
    
    //! Destructor
    ~TBB_Timeseries ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another TBB_Timeseries object from which to make a copy.
    */
    TBB_Timeseries& operator= (TBB_Timeseries const &other);
    
    // === Parameter access =====================================================
    
    /*!
      \brief Get the name of the data file
      
      \return filename -- The name of the data file.
    */
    inline std::string filename () const {
      return filename_p;
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, TBB_Timeseries.
    */
    std::string className () const {
      return "TBB_Timeseries";
    }
    
    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os=std::cout);

    // === Parameter access - TBB time-series ===================================

    //! Get the LOFAR common attributes for this dataset
    CommonAttributes commonAttributes ();

    //! Get the container for system-wide logs
    SysLog sysLog ();
    
    /*!
      \brief Get the number of station groups collected into this file
      
      \return nofStationGroups -- The number of station groups collected into
              this file
    */
    inline uint nofStationGroups () const {
      return stationGroups_p.size();
    }

    //! Get the names of the embedded station group objects
    std::set<std::string> stationGroupNames ();
    
    //! Get one of the embedded station group objects
    TBB_StationGroup stationGroup (uint const &station);
    
    //! Get one of the embedded station group objects
    TBB_StationGroup stationGroup (std::string const &name);
    
    /*!
      \brief Get the number of dipole datasets collected into this file
      
      \return nofDipoleDatasets -- The number of dipole datasets collected into
              this file.
    */
    uint nofDipoleDatasets ();
    
    // === Parameter access - station group =====================================
    
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
    
    // === Methods ==============================================================
    
    //! Open the file containing the TBB time-series data.
    bool open (hid_t const &location,
	       std::string const &name,
	       bool const &create=true);
    //! Retrieve the list of dipole number contained within this data file
    std::vector<int> dipoleNumbers ();
    //! Retrieve the list of dipole names contained within this data file
    std::vector<std::string> dipoleNames ();
    //! Get the set of selected dipoles
    std::set<std::string> selectedDipoles ();
    //! Set the set of selected dipoles
    bool selectDipoles (std::set<std::string> const &selection);
    //! Get the map constaining the actual dipole dataset selection
    inline std::map<std::string,iterDipoleDataset> dipoleSelection () const {
      return selectedDatasets_p;
    }
    //! Select all dipoles within the dataset
    bool selectAllDipoles ();
    
    //  High-level access to data and attributes ___________
    
    //! Get the values of TIME for all present datasets
    std::vector<uint> time ();
    //! Get the number of samples elapsed since the last full second
    std::vector<uint> sample_number ();
    //! Get the values of DATA_LENGTH for all present datasets
    std::vector<uint> data_length ();
    //! Get the values of the ADC sample frequency
    std::vector<double> sample_frequency_value ();
    //! Get the unit of the ADC sample frequency
    std::vector<std::string> sample_frequency_unit ();
    //! Time offset between the individual antennas in units of samples
    std::vector<int> sample_offset (uint const &refAntenna);
    //! Retrieve the list of channel IDs
    std::vector<int> channelID ();
    //! Get the Nyquist zone for the A/D conversion
    std::vector<uint> nyquist_zone ();

#ifdef HAVE_CASA
    //! Retrieve a block of ADC values per dipole
    bool readData (casa::Matrix<double> &data,
		   int const &start=0,
		   int const &nofSamples=1);
    //! Retrieve a block of ADC values per dipole
    bool readData (casa::Matrix<double> &data,
		   casa::Vector<int> const &start,
		   int const &nofSamples=1);
    
    //  Parameter access - dipole dataset __________________
    
    //! Get the sample frequency as vector of casa::MFrequency
    casa::Vector<casa::MFrequency> sample_frequency ();
#endif
    
  protected:
    
    //! Set up the list of attributes attached to the structure
    void setAttributes ();
    //! Open the structures embedded within the current one
    bool openEmbedded (bool const &create);

  private:

    //! Open the group acting as a container for system-wide logs
    bool openSysLog (bool const &create);
    //! Locate and register the station groups contained within the file
    bool openStationGroups ();
    //! Set local map used for book-keeping on selected dipole datasets
    bool setSelectedDatasets ();
    //! Unconditional copying
    void copy (TBB_Timeseries const &other);
    //! Unconditional deletion
    void destroy(void);
    
  };  // end -- class TBB_Timeseries
  
} // end -- namespace DAL

#endif /* TBB_TIMESERIES_H */
