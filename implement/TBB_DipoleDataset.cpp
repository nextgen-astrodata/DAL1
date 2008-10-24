/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 1159 2007-12-21 15:40:14Z baehren                   $ |
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

#include <TBB_DipoleDataset.h>

using std::cerr;
using std::cout;
using std::endl;

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  // -------------------------------------------------------- TBB_DipoleDataset

  TBB_DipoleDataset::TBB_DipoleDataset ()
  {
    init ();
  }
  
  // -------------------------------------------------------- TBB_DipoleDataset
  
  /*!
      \param filename -- Name of the HDF5 file within which the dataset is
             contained.
      \param dataset  -- Name of the dataset, in this case the full path from
             the base of the hierarchical structure within the HDF5 file.
  */  
  TBB_DipoleDataset::TBB_DipoleDataset (std::string const &filename,
					    std::string const &dataset)
  {
    init (filename,
	  dataset);
  }

  // -------------------------------------------------------- TBB_DipoleDataset

  /*!
    \param location -- Identifier for the location within the HDF5 file, below
           which the dataset is placed.
    \param dataset  -- Name of the dataset.
  */
  TBB_DipoleDataset::TBB_DipoleDataset (hid_t const &location,
					    std::string const &dataset)
  {
    // Initialize internal variables
    datasetID_p = 0;
    
    init (location,
	  dataset);
  }

  // -------------------------------------------------------- TBB_DipoleDataset

  /*!
    \param dataset_id -- Identifier for the dataset contained within the HDF5
           file
  */
  TBB_DipoleDataset::TBB_DipoleDataset (hid_t const &dataset_id)
  {
    init (dataset_id);
  }
  
  // -------------------------------------------------------- TBB_DipoleDataset

  /*!
    \param other -- Another TBB_DipoleDataset object from which to create
           this new one.
  */
  TBB_DipoleDataset::TBB_DipoleDataset (TBB_DipoleDataset const &other)
  {
    // Initialize internal variables
    datasetID_p = 0;
    
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  TBB_DipoleDataset::~TBB_DipoleDataset ()
  {
    destroy();
  }

  // -------------------------------------------------------------------- destroy
  
  void TBB_DipoleDataset::destroy ()
  {
    /*
      If an identifier to the dataset was assigned, we need to release it; if
      no assignment was done, there is nothing else to be done here.
    */
    if (datasetID_p > 0) {
      herr_t h5error (0);

      h5error = H5Dclose (datasetID_p);
      h5error = H5Eclear1 ();
    }
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  /*!
    \param other -- Another TBB_DipoleDataset object from which to make a
           copy.
  */
  TBB_DipoleDataset& TBB_DipoleDataset::operator= (TBB_DipoleDataset const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  // ----------------------------------------------------------------------- copy

  void TBB_DipoleDataset::copy (TBB_DipoleDataset const &other)
  {
    if (other.datasetID_p > 0) {
      init (other.datasetID_p);
    } else {
      datasetID_p = 0;
    }
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  // -------------------------------------------------------------- nofAttributes
  
  /*!
    \return nofAttributes -- The number of attributes attached to the dataset;
            if the dataset ID is properly connected to a dataset within the 
	    HDF5 file, the returned value will be zero or greater. If a
	    negative value is returned, most likely te connection with the
	    file is broken.
  */
  int TBB_DipoleDataset::nofAttributes ()
  {
    if (datasetID_p > 0) {
      return H5Aget_num_attrs (datasetID_p);
    } else {
      return -1;
    }
  }
  
  // -------------------------------------------------------------------- summary
  
  void TBB_DipoleDataset::summary (std::ostream &os)
  {
    os << "[TBB_DipoleDataset::summary]"   << endl;
    os << "-- Dataset ID             = " << datasetID_p  << endl;
  
    if (datasetID_p) {
      /*
       * Additional check in the HDF5 dataset object; if it is valid, we should
       * be able to retrieve the number of attributes attached to it.
       */
      int nofAttributes = H5Aget_num_attrs (datasetID_p);
      
      os << "-- nof. attributes        = " << nofAttributes         << endl;

      if (nofAttributes < 0) {
	os << "--> Illegal number of attached attributes!" << endl;
      } else {
	os << "-- STATION_ID             = " << station_id()             << endl;
	os << "-- RSP_ID                 = " << rsp_id()                 << endl;
	os << "-- RCU_ID                 = " << rcu_id()                 << endl;
	os << "-- CHANNEL_ID             = " << channelName()            << endl;
	os << "-- SAMPLE_FREQUENCY_VALUE = " << sample_frequency_value() << endl;
	os << "-- SAMPLE_FREQUENCY_UNIT  = " << sample_frequency_unit()  << endl;
	os << "-- NYQUIST_ZONE           = " << nyquist_zone()           << endl;
	os << "-- TIME [Unix seconds]    = " << time()                   << endl;
	os << "-- TIME [  Julian Day]    = " << julianDay()              << endl;
	os << "-- SAMPLE_NUMBER          = " << sample_number()          << endl;
	os << "-- SAMPLES_PER_FRAME      = " << samples_per_frame()      << endl;
	os << "-- FEED                   = " << feed()                   << endl;
	os << "-- DATA_LENGTH            = " << data_length()            << endl;
      }
    }
  }
  
  // ----------------------------------------------------------------- station_id
  
  /*!
    \return STATION_ID -- ID of the LOFAR station this dipole belongs to
  */
  uint TBB_DipoleDataset::station_id ()
  {    
    if (datasetID_p > 0) {
      uint val (0);
      if (DAL::h5get_attribute(datasetID_p,
			       attribute_name(DAL::STATION_ID),
			       val)) {
	return val;
      } else {
	cerr << "[TBB_DipoleDataset::station_id]"
	     << " Error retrieving attribute value!" << endl;
	return 0;
      }
    } else {
      cerr << "[TBB_DipoleDataset::station_id] Dataset undefined!"
	   << endl;;
      return 0;
    }
  }
  
  // --------------------------------------------------------------------- rsp_id
  
  /*!
    \return RSP_ID -- ID of the Remote Station Processing board (RSP) this
            dipole is connected with
  */
  uint TBB_DipoleDataset::rsp_id ()
  {
    if (datasetID_p > 0) {
      uint val (0);
      if (DAL::h5get_attribute(datasetID_p,
			       attribute_name(DAL::RSP_ID),
			       val)) {
	return val;
      } else {
	cerr << "[TBB_DipoleDataset::rsp_id]"
	     << " Error retrieving attribute value!" << endl;
	return 0;
      }
    } else {
      cerr << "[TBB_DipoleDataset::rsp_id] Dataset undefined!"
	   << endl;
      return 0;
    }
  }
  
  // --------------------------------------------------------------------- rcu_id
  
  /*!
    \return RCU_ID -- ID of the receiver unit (RCU) this dipole is connected with
  */
  uint TBB_DipoleDataset::rcu_id ()
  {
    if (datasetID_p > 0) {
      uint val (0);
      if (DAL::h5get_attribute(datasetID_p,
			       attribute_name(DAL::RCU_ID),
			       val)) {
	return val;
      } else {
	cerr << "[TBB_DipoleDataset::rcu_id]"
	     << " Error retrieving attribute value!" << endl;
	return 0;
      }
    } else {
      cerr << "[TBB_DipoleDataset::rcu_id] Dataset undefined!"
	   << endl;
      return 0;
    }
  }
  
  // ----------------------------------------------------------------------- time
  
  /*!
    \return time -- The (UNIX) time at which the data were recorded.
  */
  uint TBB_DipoleDataset::time ()
  {
    if (datasetID_p > 0) {
      uint val (0);
      if (DAL::h5get_attribute(datasetID_p,
			       attribute_name(DAL::TIME),
			       val)) {
	return val;
      } else {
	cerr << "[TBB_DipoleDataset::time]"
	     << " Error retrieving attribute value!" << endl;
	return 0;
      }
    } else {
      cerr << "[TBB_DipoleDataset::time] Dataset undefined!"
	   << endl;
      return 0;
    }
  }
  
  // ------------------------------------------------------------------ julianDay
  
  /*!
    \param onlySeconds -- Fully quallified timestamp for the first sample? If
           set to <tt>true</tt> only the UNIX time -- qualifying the full
	   seconds -- will be returned.
  */
  double TBB_DipoleDataset::julianDay (bool const &onlySeconds)
  {
    uint seconds = time ();
    double jd    = 0.0;
    
    if (onlySeconds) {
      jd = seconds/86400+2440587.5;
    } else {
      // retrieve additional information
      uint samplesSinceSecond = sample_number();
      double sampleFrequency  = sample_frequency_value ();
      // conversion
      if (sampleFrequency > 0) {
	jd = (seconds+samplesSinceSecond/sampleFrequency)/86400+2440587.5;
      }
    }
    
    return jd;
  }
  
  // ----------------------------------------------------- sample_frequency_value
  
  /*!
    \return value -- The numerical value of the ADC sample frequency, [Hz].
  */
  double TBB_DipoleDataset::sample_frequency_value ()
  {
    if (datasetID_p > 0) {
      bool status (true);
      double val (0);
      std::string unit;
      
      status = DAL::h5get_attribute(datasetID_p,
				    attribute_name(DAL::SAMPLE_FREQUENCY_VALUE),
				    val);
      unit = sample_frequency_unit ();

      if (status) {
	if (unit == "Hz" && val < 1e3) {
#ifdef DEBUGGING_MESSAGES
	  std::cerr << "[TBB_DipoleDataset::sample_frequency_value] "
		    << "Encountered combination of value and unit for sample "
		    << "outside LOFAR range -- assuming value in MHz and "
		    << "correction for it."
		    << std::endl;
#endif
	  val *= 1e6;
	} else if (unit == "kHz" && val < 1e6) {
#ifdef DEBUGGING_MESSAGES
	  std::cerr << "[TBB_DipoleDataset::sample_frequency_value] "
		    << "Encountered combination of value and unit for sample "
		    << "outside LOFAR range -- assuming value in kHz and "
		    << "correction for it."
		    << std::endl;
#endif
	  val *= 1e3;
	}
	return val;
      } else {
	return 0;
      }
    } else {
      cerr << "[TBB_DipoleDataset::sample_frequency_value] Dataset undefined!"
	   << endl;
      return 0;
    }
  }
  
  // ------------------------------------------------------ sample_frequency_unit
  
  /*!
    \return unit -- The physical unit associated with the numerical value of
            the ADC sample frequency.
  */
  std::string TBB_DipoleDataset::sample_frequency_unit ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(datasetID_p,
			     attribute_name(DAL::SAMPLE_FREQUENCY_UNIT),
			     val)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // ----------------------------------------------------------- sample_frequency
  
  /*!
    \return freq -- The ADC sample frequency, as casa::Measure
  */
  casa::MFrequency TBB_DipoleDataset::sample_frequency ()
  {
    if (datasetID_p > 0) {
      casa::Quantity qFreq = DAL::h5get_quantity (datasetID_p,
						  DAL::SAMPLE_FREQUENCY_VALUE,
						  DAL::SAMPLE_FREQUENCY_UNIT);
      return casa::MFrequency (qFreq,
			       casa::MFrequency::TOPO);
    } else {
      cerr << "[TBB_DipoleDataset::sample_frequency] Dataset undefined!"
	   << endl;
      return casa::MFrequency();
    }
  }
  
  // --------------------------------------------------------------- nyquist_zone
  
  /*!
    \return zone -- The Nyquist zone in which the analog-to-digital
            conversion (ADC) is performed
  */
  uint TBB_DipoleDataset::nyquist_zone ()
  {
    if (datasetID_p > 0) {
      uint val (0);
      if (DAL::h5get_attribute(datasetID_p,
			       attribute_name(DAL::NYQUIST_ZONE),
			       val)) {
	return val;
      } else {
	return 0;
      }
    } else {
      cerr << "[TBB_DipoleDataset::nyquist_zone] Dataset undefined!"
	   << endl;
      return 0;
    }
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  // ----------------------------------------------------------------------- init
  
  void TBB_DipoleDataset::init ()
  {
    datasetID_p = 0;
  }
  
  // ----------------------------------------------------------------------- init
  
  void TBB_DipoleDataset::init (hid_t const &dataset_id)
  {
    bool status (true);
    std::string filename;
    std::string dataset;
    /*
     * In order to avoid using the same object identifier, we first need to
     * reconstruct the filename from the dataset identifier.
     */
    status = DAL::h5get_filename (filename,
				  dataset_id);
    /*
     * If reconstruction of the filename was ok, then we need the name of the
     * dataset next, to know which position to point to within the file
     * structure.
     */
    if (status) {
      status = DAL::h5get_name (dataset,
				dataset_id);
    }
    /*
     * Forward the reverse engineered information to the init() function to 
     * set up a new object identifier for the dataset in question.
     */
    if (status) {
      init (filename,
	    dataset);
    }
  }
  
  // ----------------------------------------------------------------------- init
  
  void TBB_DipoleDataset::init (std::string const &filename,
				std::string const &dataset)
  {
    hid_t file_id (0);
    herr_t h5error (0);
    
    // Initialize internal variables
    datasetID_p = 0;
    
    // open the file
    file_id = H5Fopen (filename.c_str(),
		       H5F_ACC_RDONLY,
		       H5P_DEFAULT);
    
    // if opening of file was successfull, try to open dataset
    if (file_id > 0) {
      init (file_id,
	    dataset);
    } else {
      cerr << "[TBB_DipoleDataset::init] Error opening HDF5 file "
	   << filename 
	   << " !"
	   << endl;
    }
    
    // release the global file handle and clear the error stack
    if (file_id > 0) {
      h5error = H5Fclose (file_id);
      h5error = H5Eclear1 ();
    }
  }
  
  // ----------------------------------------------------------------------- init
  
  void TBB_DipoleDataset::init (hid_t const &location,
				std::string const &dataset)
  {
    bool status (true);
    hid_t dataset_id (0);
    
    /*
      Try to open the dataset within the HDF5 file; the dataset is expected
      to reside below the object identified by "location".
    */
    try {
      dataset_id = H5Dopen1 (location,
			     dataset.c_str());
    } catch (std::string message) {
      cerr << "[TBB_DipoleDataset::init] " << message << endl;
      status = false;
    }

    if (dataset_id > 0) {
      datasetID_p = dataset_id;
    } else {
      datasetID_p = 0;
    }
    
  }

  // -------------------------------------------------------------- sample_number
  
  /*!
    \return sample_number -- The timespan in samples since the last full
            second, as stored in <i>time</i>; the absolute time for this 
	    dataset thus is obtained by adding
	    <i>sample_number</i>/<i>sample_frequency</i> to the value of 
	    <i>time</i>.
  */
  uint TBB_DipoleDataset::sample_number ()
  {
    if (datasetID_p > 0) {
      uint val (0);
      if (DAL::h5get_attribute(datasetID_p,
			       attribute_name(DAL::SAMPLE_NUMBER),
			       val)) {
	return val;
      } else {
	return 0;
      }
    } else {
      return 0;
    }
  }
  
  // ---------------------------------------------------------- samples_per_frame
  
  /*!
    \return samplesPerFrame -- The number of samples originally transmitted 
            per frame of data sent from TBB to RSP.
  */
  uint TBB_DipoleDataset::samples_per_frame ()
  {
    if (datasetID_p > 0) {
      uint val (0);
      if (DAL::h5get_attribute(datasetID_p,
			       attribute_name(DAL::SAMPLES_PER_FRAME),
			       val)) {
	return val;
      } else {
	return 0;
      }
    } else {
      return 0;
    }
  }
  
  // ---------------------------------------------------------------- data_length
  
  /*!
    \return dataLength -- The number of samples stored in this dataset; this
            corresponds to the maximum blocksize, which can be set for this
	    dataset.
  */
  uint TBB_DipoleDataset::data_length ()
  {
    uint val (0);
    std::vector<uint> shape;
    
    if (DAL::h5get_dataset_shape(shape,
				 datasetID_p)) {
      val = shape[0];
      return val;
    } else {
      return 0;
    }    
  }

  // ----------------------------------------------------------------------- feed
  
  /*!
    \return feed -- The type of antenna feed of this dipole
  */
  std::string TBB_DipoleDataset::feed ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(datasetID_p,
			     attribute_name(DAL::FEED),
			     val)) {
      return val;
    } else {
      return std::string ("");
    }
  }
  
  // ----------------------------------------------------- antenna_position_value
  
  /*!
    \return value -- Numerical value of the antenna position coordinates, e.g.
            <tt>value=[10,12,0]</tt>
  */
  casa::Vector<double> TBB_DipoleDataset::antenna_position_value ()
  {
    casa::Vector<double> val;
    
    if (DAL::h5get_attribute(datasetID_p,
			     attribute_name(DAL::ANTENNA_POSITION_VALUE),
			     val)) {
      return val;
    } else {
      return casa::Vector<double> (1);
    }
  }
  
  // ------------------------------------------------------ antenna_position_unit
  
  /*!
    \return unit -- Physical unit associated with the numerical values for the
            antenna position, e.g. <tt>unit="m"</tt>
  */
  casa::Vector<casa::String> TBB_DipoleDataset::antenna_position_unit ()
  {
    casa::Vector<casa::String> val;
    
    if (DAL::h5get_attribute(datasetID_p,
			     attribute_name(DAL::ANTENNA_POSITION_UNIT),
			     val)) {
      return val;
    } else {
      return casa::Vector<casa::String> (1);
    }
  }
  
  // ----------------------------------------------------- antenna_position_frame
  
  /*!
    \return frame -- Identifier for the reference frame within which the antenna
            position is provided, e.g. <tt>frame="ITRF"</tt>
  */
  std::string TBB_DipoleDataset::antenna_position_frame ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(datasetID_p,
			     attribute_name(DAL::ANTENNA_POSITION_FRAME),
			     val)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // ----------------------------------------------------------- antenna_position

  /*!
    \return position -- The antenna position as casa::Measure, combining the 
            information from ANTENNA_POSITION_VALUE, ANTENNA_POSITION_UNIT and
	    ANTENNA_POSITION_FRAME.
  */
  casa::MPosition TBB_DipoleDataset::antenna_position ()
  {
    return DAL::h5get_position (datasetID_p,
				DAL::ANTENNA_POSITION_VALUE,
				DAL::ANTENNA_POSITION_UNIT,
				DAL::ANTENNA_POSITION_FRAME);
  }
  
  // -------------------------------------------------- antenna_orientation_value
  
  /*!
    \return value -- The numerical values describing the antenna position; this
            can be either a set of Euler angles or a normal vector.
  */
  casa::Vector<double> TBB_DipoleDataset::antenna_orientation_value ()
  {
    casa::Vector<double> val;
    
    if (DAL::h5get_attribute(datasetID_p,
			     attribute_name(DAL::ANTENNA_ORIENTATION_VALUE),
			     val)) {
      return val;
    } else {
      return casa::Vector<double> (1);
    }
  }
  
  // --------------------------------------------------- antenna_orientation_unit
  
  /*!
    \return unit -- Physical unit associated with the numerical values for the
            antenna orientation; depending on the parametrization this can be
	    <tt>unit="rad"</tt>, <tt>unit="deg"</tt> or <tt>unit="m"</tt>.
  */
  casa::Vector<casa::String> TBB_DipoleDataset::antenna_orientation_unit ()
  {
    casa::Vector<casa::String> val;
    
    if (DAL::h5get_attribute(datasetID_p,
			     attribute_name(DAL::ANTENNA_ORIENTATION_UNIT),
			     val)) {
      return val;
    } else {
      return casa::Vector<casa::String> (1);
    }
  }
  
  // -------------------------------------------------- antenna_orientation_frame
  
  /*!
    \return frame -- Identifier for the reference frame within which the antenna
            orientation is provided, e.g. <tt>frame="ITRF"</tt>
  */
  std::string TBB_DipoleDataset::antenna_orientation_frame ()
  {
    std::string val;
    
    if (DAL::h5get_attribute(datasetID_p,
			     attribute_name(DAL::ANTENNA_ORIENTATION_FRAME),
			     val)) {
      return val;
    } else {
      return std::string ("UNDEFINED");
    }
  }
  
  // ----------------------------------------------------------------- channel_id
  
  /*!
    \return channelID -- The unique identifier for a signal channel/dipole
            within the whole LOFAR array; this ID is created from a combination
	    of station ID, RSP ID and RCU ID:
	    \f$ N_{\rm RCU} + 10^{3} \times N_{\rm RSP} + 10^{6} \times
	    N_{\rm Station} \f$
  */
  int TBB_DipoleDataset::channelID ()
  {
    return rcu_id() + 1000*rsp_id() + 1000000*station_id();
  }
  
  // ---------------------------------------------------------------- channelName
  
  /*!
    \return channel_id -- The unique identifier for a signal channel/dipole
            within the whole LOFAR array; this ID is created from a combination
	    of station ID, RSP ID and RCU ID.
  */
  std::string TBB_DipoleDataset::channelName ()
  {
    char uid[10];
    sprintf(uid,
	    "%03d%03d%03d",
	    station_id(),
	    rsp_id(),
	    rcu_id());
    std::string channelID (uid);

    return channelID;
  }

  // ------------------------------------------------------------------------- fx
  
  /*!
    \param start      -- Number of the sample at which to start reading
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.
    \retval data       -- [nofSamples] Array with the raw ADC samples
            representing the electric field strength as function of time.
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool TBB_DipoleDataset::fx (int const &start,
			      int const &nofSamples,
			      short *data)
  {
    if (datasetID_p > 0) {
      herr_t h5error (0);
      hid_t filespaceID (0);
      int rank (0);
      
      /* Retrieve the identifier of the dataspace associated with the dataset */
      
      filespaceID = H5Dget_space(datasetID_p);
      
      if (filespaceID < 0) {
	cerr << "[TBB_DipoleDataset::fx]"
	     << " Error retrieving filespace of dataset!" << endl;
	return false;
      }
      
      /* Retrieve the rank of the dataspace asssociated with the dataset */
      
      rank = H5Sget_simple_extent_ndims(filespaceID);
      
      if (rank < 0) {
	cerr << "[TBB_DipoleDataset::fx]"
	     << " Error retrieving rank of dataspace!" << endl;
	return false;
      }
      
      /* Retrieve the dimension of the dataspace, i.e. the number of samples */
      
      hsize_t shape[1];
      h5error = H5Sget_simple_extent_dims (filespaceID,
					   shape,
					   NULL);
      if (h5error < 0) {
	cerr << "[TBB_DipoleDataset::fx]"
	     << " Error retrieving dataspace dimension!" << endl;
	return false;
      } else {
	shape[0] = nofSamples;
      }
      
      /* Set up memory space to retrieve the data read from the file */
      
      hid_t memspace = H5Screate_simple (rank,
					 shape,
					 NULL);
      hsize_t offset[1];

      if (filespaceID < 0) {
	cerr << "[TBB_DipoleDataset::fx]"
	     << " Error creating memory space for reading in data!"
	     << endl;
	return false;
      } else {
	offset[0] = start;
      }
      
      /* Select the hyperslab through the data volume */
      
      h5error = H5Sselect_hyperslab (filespaceID,
				     H5S_SELECT_SET,
				     offset,
				     NULL, 
				     shape,
				     NULL);
      
      if (h5error < 0) {
	cerr << "[TBB_DipoleDataset::fx]"
	     << " Error selecting hyperslab through the data!"
	     << endl;
	return false;
      }
      
      /* Retrieve the actual data from the file */
      
      h5error = H5Dread (datasetID_p,
			 H5T_NATIVE_SHORT,
			 memspace,
			 filespaceID,
			 H5P_DEFAULT,
			 data);
      
      if (h5error < 0) {
	cerr << "[TBB_DipoleDataset::fx]"
	     << " Error reading data from file into buffer!"
	     << endl;
	return false;
      }
    } else {
      cerr << "[TBB_DipoleDataset::fx]"
	   << " Unable to read with connection to dataset object!"
	   << endl;
      return false;
    }
    
    return true;
  }
  
  // ------------------------------------------------------------------------- fx
  
  /*!
    \param start      -- Number of the sample at which to start reading
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.
    
    \return fx -- [nofSamples] Vector of raw ADC samples representing the 
            electric field strength as function of time.
  */
  casa::Vector<double> TBB_DipoleDataset::fx (int const &start,
					      int const &nofSamples)
  {
    if (datasetID_p > 0) {
      bool status (true);
      short *dataBuffer;
      
      dataBuffer = new short [nofSamples];
      
      /* Retrieve the data from the file */
      
      status = fx (start,
		   nofSamples,
		   dataBuffer);
      
      /* Copy the data from the buffer into the vector returned by this function */
      
      if (status) {
	casa::Vector<double> data (nofSamples);
	// copy values to returned vector
	for (int sample(0); sample<nofSamples; sample++) {
	  data(sample) = double(dataBuffer[sample]);
	}
	// clean up memory
	delete [] dataBuffer;
	// return data
	return data;
      } else {
	return casa::Vector<double> (1,0);
      }
      
    } else {
      return casa::Vector<double> (1,0);
    }
  }

  // ---------------------------------------------------------- recordDescription
  
  /*!
    \return recDesc -- Record descriptor containing the information on how to
            structure the record as which the attributes attached to the dataset
	    can be retrieved.
  */
  casa::RecordDesc TBB_DipoleDataset::recordDescription ()
  {
    casa::RecordDesc desc;

    desc.addField (DAL::attribute_name(DAL::STATION_ID),casa::TpUInt);
    desc.addField (DAL::attribute_name(DAL::RSP_ID),casa::TpUInt);
    desc.addField (DAL::attribute_name(DAL::RCU_ID),casa::TpUInt);
    desc.addField (DAL::attribute_name(DAL::TIME),casa::TpUInt);
    desc.addField (DAL::attribute_name(DAL::SAMPLE_FREQUENCY_VALUE),casa::TpDouble);
    desc.addField (DAL::attribute_name(DAL::SAMPLE_FREQUENCY_UNIT),casa::TpString);
    desc.addField (DAL::attribute_name(DAL::NYQUIST_ZONE),casa::TpUInt);
    desc.addField (DAL::attribute_name(DAL::SAMPLE_NUMBER),casa::TpUInt);
    desc.addField (DAL::attribute_name(DAL::SAMPLES_PER_FRAME),casa::TpUInt);
    desc.addField (DAL::attribute_name(DAL::ANTENNA_POSITION_VALUE),casa::TpDouble);
    desc.addField (DAL::attribute_name(DAL::ANTENNA_POSITION_UNIT),casa::TpString);
    desc.addField (DAL::attribute_name(DAL::ANTENNA_POSITION_FRAME),casa::TpString);
    desc.addField (DAL::attribute_name(DAL::ANTENNA_ORIENTATION_VALUE),casa::TpDouble);
    desc.addField (DAL::attribute_name(DAL::ANTENNA_ORIENTATION_UNIT),casa::TpString);
    desc.addField (DAL::attribute_name(DAL::ANTENNA_ORIENTATION_FRAME),casa::TpString);
    desc.addField (DAL::attribute_name(DAL::FEED),casa::TpString);

    return desc;
  }
  
  // ---------------------------------------------------------- attributes2record
  
  /*!
      \return record -- A casa::Record container holding the values of the 
              attributes attached to the dataset for this dipole
  */
  casa::Record TBB_DipoleDataset::attributes2record ()
  {
    casa::Record rec;
    
    try {
      rec.define(casa::RecordFieldId(attribute_name(DAL::STATION_ID)),
		 station_id());
      rec.define(casa::RecordFieldId(attribute_name(DAL::RSP_ID)),
		 rsp_id());
      rec.define(casa::RecordFieldId(attribute_name(DAL::RCU_ID)),
		 rcu_id());
      rec.define(casa::RecordFieldId(attribute_name(DAL::TIME)),
		 time());
      rec.define(casa::RecordFieldId(attribute_name(DAL::NYQUIST_ZONE)),
		 nyquist_zone());
      rec.define(casa::RecordFieldId(attribute_name(DAL::SAMPLE_NUMBER)),
		 sample_number());
      rec.define(casa::RecordFieldId(attribute_name(DAL::SAMPLES_PER_FRAME)),
		 samples_per_frame());
      rec.define(casa::RecordFieldId(attribute_name(DAL::FEED)),
		 feed());
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_POSITION_VALUE)),
		 antenna_position_value());
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_POSITION_UNIT)),
		 antenna_position_unit());
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_POSITION_FRAME)),
		 antenna_position_frame());
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_ORIENTATION_VALUE)),
		 antenna_orientation_value());
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_ORIENTATION_UNIT)),
		 antenna_orientation_unit());
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_ORIENTATION_FRAME)),
		 antenna_orientation_frame());
      rec.define(casa::RecordFieldId(attribute_name(DAL::DATA_LENGTH)),
		 data_length());
    } catch (std::string message) {
      cerr << "[TBB_DipoleDataset::attributes2record] "
	   << "Error filling the record with attribute values!\n"
	   << message
	   << endl;
    }
    
    return rec;
  }

} // Namespace DAL -- end
