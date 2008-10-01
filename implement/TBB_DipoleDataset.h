/*-------------------------------------------------------------------------*
 | $Id:: NewClass.h 1159 2007-12-21 15:40:14Z baehren                    $ |
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

#ifndef HDF5COMMON_H
#include <HDF5Common.h>
#endif

#ifndef ENUMERATIONS_H
#include <Enumerations.h>
#endif

namespace DAL { // Namespace DAL -- begin
  
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
      |   |   |-- ANT_POSITION_UNIT       ... Attribute      ... string
      |   |   |-- ANT_POSITION_FRAME      ... Attribute      ... string
      |   |   |-- ANT_ORIENTATION_VALUE   ... Attribute      ... array<double,1>
      |   |   |-- ANT_ORIENTATION_UNIT    ... Attribute      ... string
      |   |   `-- ANT_ORIENTATION_FRAME   ... Attribute      ... string
      \endverbatim
    </ol>
    
    <h3>Example(s)</h3>
    
  */  
  class TBB_DipoleDataset {
    
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
    
    TBB_DipoleDataset (hid_t const &dataset_id);
    
    /*!
      \brief Copy constructor
      
      \param other -- Another TBB_DipoleDataset object from which to create
             this new one.
    */
    TBB_DipoleDataset (TBB_DipoleDataset const &other);
    
    // -------------------------------------------------------------- Destruction

    /*!
      \brief Destructor
    */
    ~TBB_DipoleDataset ();
    
    // ---------------------------------------------------------------- Operators
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another TBB_DipoleDataset object from which to make a
             copy.
    */
    TBB_DipoleDataset& operator= (TBB_DipoleDataset const &other); 
    
    // --------------------------------------------------------------- Parameters
    
    /*!
      \brief Get the identifier for this dataset within the HDF5 file

      \return H5datasetID -- The identifier for this dataset within the HDF5 file
    */
    inline hid_t dataset_id () const { return datasetID_p; }

    /*!
      \brief Get the number of attributes attached to the dataset

      \return nofAttributes -- The number of attributes attached to the dataset;
              if the dataset ID is properly connected to a dataset within the 
	      HDF5 file, the returned value will be zero or greater. If a
	      negative value is returned, most likely te connection with the
	      file is broken.
    */
    int nofAttributes ();

    /*!
      \brief Get the ID of the LOFAR station this dipole belongs to

      \return STATION_ID -- ID of the LOFAR station this dipole belongs to
    */
    uint station_id ();

    /*!
      \brief Get the ID of the RSP board this dipole is connected with

      \return RSP_ID -- ID of the Remote Station Processing board (RSP) this
              dipole is connected with
    */
    uint rsp_id ();

    /*!
      \brief Get the ID of the receiver unit (RCU) this dipole is connected with

      \return RCU_ID -- ID of the receiver unit (RCU) this dipole is connected with
    */
    uint rcu_id ();

    /*!
      \brief Get the numerical value of the ADC sample frequency

      \param unit -- Units in which to return the ADC sample frequency; by
             default this is "Hz".

      \return value -- The numerical value of the ADC sample frequency, in units
              as specified.
    */
    double sample_frequency_value (std::string const &unit="Hz");

    /*!
      \brief Get the physical unit associated with the ADC sample frequency

      \return unit -- The physical unit associated with the numerical value of
              the ADC sample frequency.
    */
    std::string sample_frequency_unit ();

    /*!
      \brief Get the ADC sample frequency

      \return freq -- The ADC sample frequency, as CASA Quantity
    */
    casa::MFrequency sample_frequency ();

    /*!
      \brief Get the Nyquist zone in which the ADC is performed

      \return NYQUIST_ZONE -- The Nyquist zone in which the analog-to-digital
              conversion (ADC) is performed
    */
    uint nyquist_zone ();

    /*!
      \brief Get the (UNIX) time at which the data were recorded
      
      \return time -- The (UNIX) time at which the data were recorded.
    */
    uint time ();

    /*!
      \brief Get the time as Julian Day

      \param onlySeconds -- Fully quallified timestamp for the first sample? If
             set to <tt>true</tt> only the UNIX time -- qualifying the full
	     seconds -- will be returned.
    */
    double julianDay (bool const &onlySeconds=false);
    
    /*!
      \brief Get the timespan in samples since the last full second

      \return sample_number -- The timespan in samples since the last full
              second, as stored in <i>time</i>; the absolute time for this 
	      dataset thus is obtained by adding
	      <i>sample_number</i>/<i>sample_frequency</i> to the value of 
	      <i>time</i>.
    */
    uint sample_number ();

    /*!
      \brief The number of samples per original TBB-RSP frame

      \return samplesPerFrame -- The number of samples originally transmitted 
              per frame of data sent from TBB to RSP.
    */
    uint samples_per_frame ();
    
    /*!
      \brief Get the number of samples stored in this dataset

      \return dataLength -- The number of samples stored in this dataset; this
              corresponds to the maximum blocksize, which can be set for this
	      dataset.
    */
    uint data_length ();

    /*!
      \brief Get the type of feed for this dipole

      \return feed -- The type of antenna feed of this dipole
    */
    std::string feed ();

    /*!
      \brief Get the numerical value of the antenna position

      \return value -- Numerical value of the antenna position coordinates, e.g.
              <tt>value=[10,12,0]</tt>
    */
    casa::Vector<double> antenna_position_value ();

    /*!
      \brief Get the physical unit within which the antenna position is given

      \return unit -- Physical unit associated with the numerical values for the
              antenna position, e.g. <tt>unit="m"</tt>
    */
    std::string antenna_position_unit ();

    /*!
      \brief Get the identifier for the reference frame of the antenna position

      \return Identifier for the reference frame within which the antenna position
              is provided, e.g. <tt>frame="ITRF"</tt>
    */
    std::string antenna_position_frame ();

    /*!
      \brief Get the antenna position as a measure
    */
    casa::MPosition antenna_position ();

    /*!
      \brief Get the numerical values describing the antenna orientation

      \return value -- The numerical values describing the antenna position; this
              can be either a set of Euler angles or a normal vector.
    */
    casa::Vector<double> antenna_orientation_value ();

    /*!
      \brief Get the physical unit within which the antenna orientation is given

      \return unit -- Physical unit associated with the numerical values for the
              antenna orientation; depending on the parametrization this can be
	      <tt>unit="rad"</tt>, <tt>unit="deg"</tt> or <tt>unit="m"</tt>.
    */
    std::string antenna_orientation_unit ();

    /*!
      \brief Get the identifier for the reference frame of the antenna orientation

      \return Identifier for the reference frame within which the antenna
              orientation is provided, e.g. <tt>frame="ITRF"</tt>
    */
    std::string antenna_orientation_frame ();

    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, TBB_DipoleDataset.
    */
    std::string className () const {
      return "TBB_DipoleDataset";
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

    // ------------------------------------------------------------------ Methods

    /*!
      \brief Get the unique channel/dipole identifier
      
      \return channelID -- The unique identifier for a signal channel/dipole
              within the whole LOFAR array; this ID is created from a combination
	      of station ID, RSP ID and RCU ID:
	      \f$ N_{\rm RCU} + 10^{3} \times N_{\rm RSP} + 10^{6} \times
	      N_{\rm Station} \f$
    */
    int channelID ();

    /*!
      \brief Get the unique channel/dipole identifier
      
      \return channel_id -- The unique identifier for a signal channel/dipole
              within the whole LOFAR array; this ID is created from a combination
	      of station ID, RSP ID and RCU ID.
    */
    std::string channelName ();

    /*!
      \brief Get a number of data values as recorded for this dipole
      
      \param start      -- Number of the sample at which to start reading
      \param nofSamples -- Number of samples to read, starting from the position
             given by <tt>start</tt>.
      \retval data       -- [nofSamples] Array with the raw ADC samples
              representing the electric field strength as function of time.
	      
      \return status -- Status of the operation; returns <tt>false</tt> in case
              an error was encountered.
    */
    bool fx (int const &start,
	     int const &nofSamples,
	     short *data);
    
    /*!
      \brief Get a number of data values as recorded for this dipole

      \param start      -- Number of the sample at which to start reading
      \param nofSamples -- Number of samples to read, starting from the position
             given by <tt>start</tt>.

      \return fx -- [nofSamples] Vector of raw ADC samples representing the 
              electric field strength as function of time.
     */
    casa::Vector<double> fx (int const &start=0,
			     int const &nofSamples=1);

    /*!
      \brief Get a casa::RecordDesc object describing the structure of the record

      \return recDesc -- Record descriptor containing the information on how to
              structure the record as which the attributes attached to the dataset
	      can be retrieved.
    */
    casa::RecordDesc recordDescription ();
    
    /*!
      \brief Get a casa::Record containing the values of the attributes

      \return record -- A casa::Record container holding the values of the 
              attributes attached to the dataset for this dipole
    */
    casa::Record attributes2record ();
    
  private:
    
    /*!
      \brief Initialize the internal dataspace
    */
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
  
