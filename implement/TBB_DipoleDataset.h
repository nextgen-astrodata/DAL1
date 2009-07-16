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

#ifndef TBB_DIPOLEDATASET_H
#define TBB_DIPOLEDATASET_H

#include <string>

#include <casa/aips.h>
#include <casa/Arrays/ArrayIO.h>
#include <casa/Arrays/Vector.h>
#include <casa/Quanta/Quantum.h>
#include <casa/Containers/Record.h>
#include <measures/Measures/MFrequency.h>
#include <measures/Measures/MPosition.h>

#ifndef DALCOMMON_H
#include <dalCommon.h>
#endif

#ifndef ENUMERATIONS_H
#include <Enumerations.h>
#endif

namespace DAL   // Namespace DAL -- begin
  {

  /*!
    \class TBB_DipoleDataset

    \ingroup DAL

    \brief Container for dipole-based data in a LOFAR TBB time-series dataset

    \author Lars B&auml;hren

    \date 2008/01/10

    \test tTBB_DipoleDataset.cpp

    <h3>Prerequisite</h3>

    <ul type="square">
      <li><a href="http://www.atnf.csiro.au/computing/software/casacore/classcasa_1_1Record.html">casa::Record</a>
      -- A hierarchical collection of named fields of various types.
      <li>DAL::TBB_StationGroup
      <li>DAL::TBB_Timeseries
    </ul>

    <h3>Synopsis</h3>

    <ol>
      <li>Structure of the HDF5 dataset inside the time-series data format:
      \verbatim
      /                             ... Group
      |-- Station001                ... Group
      |   |--
      |   |-- 001000000                   ... Dataset        ... array<uint,1>
      |   |   |-- STATION_ID              ... Attribute      ... uint
      |   |   |-- RSP_ID                  ... Attribute      ... uint
      |   |   |-- RCU_ID                  ... Attribute      ... uint
      |   |   |-- SAMPLE_FREQUENCY_VALUE  ... Attribute      ... double
      |   |   |-- SAMPLE_FREQUENCY_UNIT   ... Attribute      ... double
      |   |   |-- TIME                    ... Attribute      ... uint
      |   |   |-- SAMPLE_NR               ... Attribute      ... uint
      |   |   |-- SAMPLES_PER_FRAME       ... Attribute      ... uint
      |   |   |-- NYQUIST_ZONE            ... Attribute      ... uint
      |   |   |-- FEED                    ... Attribute      ... string
      |   |   |-- ANT_POSITION_VALUE      ... Attribute      ... array<double,1>
      |   |   |-- ANT_POSITION_UNIT       ... Attribute      ... array<string,1>
      |   |   |-- ANT_POSITION_FRAME      ... Attribute      ... string
      |   |   |-- ANT_ORIENTATION_VALUE   ... Attribute      ... array<double,1>
      |   |   |-- ANT_ORIENTATION_UNIT    ... Attribute      ... array<string,1>
      |   |   `-- ANT_ORIENTATION_FRAME   ... Attribute      ... string
      \endverbatim
    </ol>

    <h3>Example(s)</h3>

    <ul>
    <li>Use the default constructor for a new object and display its internal
    settings:
    \code
    // Create new object
    DAL::TBB_DipoleDataset dataset;
    // Summaru of the internal properties
    dataset.summary();
    \endcode
    <li>Create a new object for a specific dipole dataset embedded within a TBB
    time-series dataset:
    \code
    // The name of the file to open
    std::string fileName ("<the name of the file>");
    // The path to the dataset contained within the HDF5 file
    std::string datasetName ("<path to the dataset>");

    DAL::TBB_DipoleDataset dataset (fileName,
                                    datasetName);
    \endcode
  </ul>

  */
  class TBB_DipoleDataset
    {

      //! Identifier for this dataset within the HDF5 file
      hid_t datasetID_p;

    public:

      // ------------------------------------------------------------- Construction

      //! Default constructor
      TBB_DipoleDataset ();
      //! Argumented constructor
      TBB_DipoleDataset (std::string const &filename,
                         std::string const &dataset);
      //! Argumented constructor
      TBB_DipoleDataset (hid_t const &location,
                         std::string const &dataset);
      //! Argumented constructor
      TBB_DipoleDataset (hid_t const &dataset_id);
      //! Copy constructor
      TBB_DipoleDataset (TBB_DipoleDataset const &other);

      // -------------------------------------------------------------- Destruction

      //! Destructor
      ~TBB_DipoleDataset ();

      // ---------------------------------------------------------------- Operators

      //! Overloading of the copy operator
      TBB_DipoleDataset& operator= (TBB_DipoleDataset const &other);

      // --------------------------------------------------------------- Parameters

      /*!
        \brief Get the identifier for this dataset within the HDF5 file

        \return H5datasetID -- The identifier for this dataset within the HDF5 file
      */
      inline hid_t dataset_id () const {
	return datasetID_p;
      }
      
      //! Get the number of attributes attached to the dataset
      int nofAttributes ();

      //! Get the ID of the LOFAR station this dipole belongs to
      uint station_id ();

      //! Set the ID of the LOFAR station this dipole belongs to
      bool set_station_id (uint const &id);

      //! Get the ID of the RSP board this dipole is connected with
      uint rsp_id ();

      //! Set the ID of the RSP board this dipole is connected with
      bool set_rsp_id (uint const &id);

      //! Get the ID of the receiver unit (RCU) this dipole is connected with
      uint rcu_id ();

      //! Set the ID of the receiver unit (RCU) this dipole is connected with
      bool set_rcu_id (uint const &id);

      //! Get the numerical value of the ADC sample frequency
      double sample_frequency_value ();

      //! Set the numerical value of the ADC sample frequency
      bool set_sample_frequency_value (double const &value);

      //! Get the physical unit associated with the ADC sample frequency
      std::string sample_frequency_unit ();

      //! Set the physical unit associated with the ADC sample frequency
      bool set_sample_frequency_unit (std::string const &unit);

      //! Get the ADC sample frequency as casa::Measure
      casa::MFrequency sample_frequency ();

      //! Get the Nyquist zone in which the ADC is performed
      uint nyquist_zone ();

      //! Set the Nyquist zone in which the ADC is performed
      bool set_nyquist_zone (uint const &zone);

      //! Get the (UNIX) time at which the data were recorded
      uint time ();

      //! Set the (UNIX) time at which the data were recorded
      bool set_time (uint const &time);
      
      //! Get the time as Julian Day
      double julianDay (bool const &onlySeconds=false);

      //! Get the timespan in samples since the last full second
      uint sample_number ();

      //! Set the timespan in samples since the last full second
      bool set_sample_number (uint const &number);

      //! The number of samples per original TBB-RSP frame
      uint samples_per_frame ();

      //! Set the number of samples per original TBB-RSP frame
      bool set_samples_per_frame (uint const &samples);

      //! Get the number of samples stored in this dataset
      uint data_length ();

      //! Set the number of samples stored in this dataset
      bool set_data_length (uint const &length);

      //! Get the type of feed for this dipole
      std::string feed ();

      //! Set the type of feed for this dipole
      bool set_feed (std::string const &feed);

#ifdef HAVE_CASA
      //! Get the numerical value of the antenna position
      casa::Vector<double> antenna_position_value ();
      //! Get the physical unit within which the antenna position is given
      casa::Vector<casa::String> antenna_position_unit ();
      //! Get the antenna position as a measure
      casa::MPosition antenna_position ();
      //! Get the numerical values describing the antenna orientation
      casa::Vector<double> antenna_orientation_value ();
      //! Get the physical unit within which the antenna orientation is given
      casa::Vector<casa::String> antenna_orientation_unit ();
#else
      //! Get the numerical value of the antenna position
      std::vector<double> antenna_position_value ();
      //! Get the physical unit within which the antenna position is given
      std::vector<std::string> antenna_position_unit ();
      //! Get the numerical values describing the antenna orientation
      std::vector<double> antenna_orientation_value ();
      //! Get the physical unit within which the antenna orientation is given
      casa::Vector<casa::String> antenna_orientation_unit ();
#endif

      //! Get the identifier for the reference frame of the antenna position
      std::string antenna_position_frame ();

      //! Get the identifier for the reference frame of the antenna orientation
      std::string antenna_orientation_frame ();

      /*!
        \brief Get the name of the class

        \return className -- The name of the class, TBB_DipoleDataset.
      */
      inline std::string className () const {
	return "TBB_DipoleDataset";
      }
      
      //! Provide a summary of the internal status
      inline void summary () {
        summary (std::cout);
      }
      
      //! Provide a summary of the internal status
      void summary (std::ostream &os);

      // ------------------------------------------------------------------ Methods

      //! Get the unique channel/dipole identifier
      int channelID ();

      //! Get the unique channel/dipole identifier
      std::string channelName ();

      //! Get a number of data values as recorded for this dipole
      bool fx (int const &start,
               int const &nofSamples,
               short *data);

      //! Get a number of data values as recorded for this dipole
      casa::Vector<double> fx (int const &start=0,
                               int const &nofSamples=1);

      //! Get a casa::RecordDesc object describing the structure of the record
      casa::RecordDesc recordDescription ();

      //! Get a casa::Record containing the values of the attributes
      casa::Record attributes2record ();

      //! Get a casa::Record containing the values of the attributes
      bool attributes2record (casa::Record &rec);

    private:

      //! Initialize the internal dataspace
      void init ();

      /*!
        \brief Initialize the internal dataspace

        \param dataset_id -- Identifier for the dataset within the HDF5 file
       */
      void init (hid_t const &dataset_id);

      /*!
        \brief Initialize the internal dataspace

        \param filename -- HDF5 file within which the dataset in question is
               contained
        \param dataset  -- Name of the dataset which this object is to encapsulate.
       */
      void init (std::string const &filename,
                 std::string const &dataset);

      /*!
        \brief Initialize the internal dataspace

        \param location -- Location below which the dataset is found within the
               file.
        \param dataset  -- Name of the dataset which this object if to encapsulate.
       */
      void init (hid_t const &location,
                 std::string const &dataset);

      /*!
        \brief Unconditional copying

        \param other -- Another TBB_DipoleDataset object from which to create
               this new one.
      */
      void copy (TBB_DipoleDataset const &other);

      /*!
        \brief Unconditional deletion
      */
      void destroy(void);

    };

} // Namespace DAL -- end

#endif /* TBB_DIPOLEDATASET_H */

