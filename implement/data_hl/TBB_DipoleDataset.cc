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

#include <TBB_DipoleDataset.h>

using std::cerr;
using std::cout;
using std::endl;

namespace DAL {  // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                            TBB_DipoleDataset
  
  TBB_DipoleDataset::TBB_DipoleDataset ()
    : CommonInterface ()
  {
    location_p = 0;
  }
  
  //_____________________________________________________________________________
  //                                                            TBB_DipoleDataset
  
  /*!
    \param filename -- Name of the HDF5 file within which the dataset is
           contained.
    \param dataset  -- Name of the dataset, in this case the full path from
           the base of the hierarchical structure within the HDF5 file.
  */
  TBB_DipoleDataset::TBB_DipoleDataset (std::string const &filename,
                                        std::string const &dataset)
    : CommonInterface ()
  {
    init (filename,
          dataset);
  }
  
  //_____________________________________________________________________________
  //                                                            TBB_DipoleDataset
  
  /*!
    \param location -- Identifier for the location within the HDF5 file, below
           which the dataset is placed.
    \param dataset  -- Name of the dataset.
  */
  TBB_DipoleDataset::TBB_DipoleDataset (hid_t const &location,
                                        std::string const &dataset)
    : CommonInterface ()
  {
    // Initialize internal variables
    location_p = 0;
    
    init (location,
          dataset);
  }
  
  //_____________________________________________________________________________
  //                                                            TBB_DipoleDataset
  
  /*!
    \param other -- Another TBB_DipoleDataset object from which to create
           this new one.
  */
  TBB_DipoleDataset::TBB_DipoleDataset (TBB_DipoleDataset const &other)
    : CommonInterface ()
  {
    // Initialize internal variables
    location_p = 0;

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

  //_____________________________________________________________________________
  //                                                                      destroy
  
  void TBB_DipoleDataset::destroy ()
  {
    /*
      If an identifier to the dataset was assigned, we need to release it; if
      no assignment was done, there is nothing else to be done here.
    */
    if (location_p > 0) {
      herr_t h5error (0);
      
      h5error = H5Dclose (location_p);
      h5error = H5Eclear1 ();
    }
  }
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  /*!
    \param other -- Another TBB_DipoleDataset object from which to make a copy.
  */
  TBB_DipoleDataset& TBB_DipoleDataset::operator= (TBB_DipoleDataset const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void TBB_DipoleDataset::copy (TBB_DipoleDataset const &other)
  {
    if (other.location_p > 0) {
      init (other.location_p);
    }
    else {
      location_p = 0;
    }
  }
  
  // ============================================================================
  //
  //  Access to attributes
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                       sample_frequency_value
  
  /*!
    \return value -- The numerical value of the ADC sample frequency, [Hz].
  */
  double TBB_DipoleDataset::sample_frequency_value ()
  {
    if (location_p > 0) {
      bool status (true);
      double val (0);
      std::string unit;
      
      status = DAL::h5get_attribute(location_p,
				    attribute_name(DAL::SAMPLE_FREQUENCY_VALUE),
				    val);
      unit = sample_frequency_unit ();
      
      if (status) {
	/* Do some checking on the value to be returned. */
	if (val < 1e3) {
	  if (unit == "Hz") {
	    val *= 1e6;
	  }
	  else if (unit == "kHz") {
	    val *= 1e3;
	  }
	}
	/* Return the value of the sampling frequency. */
	return val;
      }
      else {
	return 0;
      }
    }
    else {
      cerr << "[TBB_DipoleDataset::sample_frequency_value] Dataset undefined!"
	   << endl;
      return 0;
    }
  }
  
  //_____________________________________________________________________________
  //                                                        sample_frequency_unit
  
  /*!
    \return unit -- The physical unit associated with the numerical value of
            the ADC sample frequency.
  */
  std::string TBB_DipoleDataset::sample_frequency_unit ()
  {
    std::string unit;

    if (!getAttribute("SAMPLE_FREQUENCY_UNIT",unit)) {
      unit = "UNDEFINED";
    }

    return unit;
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
    uint val;

    if (!getAttribute("SAMPLE_NUMBER",val)) {
      val = 0;
    }

    return val;
  }
  
  //_____________________________________________________________________________
  //                                                            samples_per_frame

  /*!
    \return samplesPerFrame -- The number of samples originally transmitted
            per frame of data sent from TBB to RSP.
  */
  uint TBB_DipoleDataset::samples_per_frame ()
  {
    if (location_p > 0) {
      uint val (0);
      if (DAL::h5get_attribute(location_p,
			       attribute_name(DAL::SAMPLES_PER_FRAME),
			       val)) {
	return val;
      }
      else {
	return 0;
      }
    }
    else {
      return 0;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                  data_length
  
  /*!
    \return dataLength -- The number of samples stored in this dataset; this
            corresponds to the maximum blocksize, which can be set for this
	    dataset.
  */
  uint TBB_DipoleDataset::data_length ()
  {
    uint val (0);
    std::vector<uint> shape;

    if (DAL::h5get_dataset_shape(location_p,shape)) {
      val = shape[0];
      return val;
    }
    else {
      return 0;
    }
  }
  
  //_____________________________________________________________________________
  //                                                                         feed
  
  /*!
    \return feed -- The type of antenna feed of this dipole
  */
  std::string TBB_DipoleDataset::feed ()
  {
    std::string val;

    if (!getAttribute("FEED",val)) {
      val = std::string ("");
    }

    return val;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  // ----------------------------------------------------------------------- init

  void TBB_DipoleDataset::init (hid_t const &dataset_id)
  {
    bool status (true);
    std::string filename;
    std::string dataset;

    //________________________________________________________________
    // Get filename and dataset name from the dataset ID

    status = DAL::h5get_filename (filename,
                                  dataset_id);

    if (status) {
      status = DAL::h5get_name (dataset,
				dataset_id);
    }
    
    //________________________________________________________________
    // Debugging messages

#ifdef DEBUGGING_MESSAGES
    std::cout << "[TBB_DipoleDataset::init]" << std::endl;
    std::cout << "-- Filename     = " << filename << std::endl;
    std::cout << "-- Dataset name = " << dataset    << std::endl;
    std::cout << "-- Dataset ID   = " << dataset_id << std::endl;
#endif

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
    location_p = 0;

    // open the file
    file_id = H5Fopen (filename.c_str(),
                       H5F_ACC_RDONLY,
                       H5P_DEFAULT);

    // if opening of file was successfull, try to open dataset
    if (file_id > 0) {
      init (file_id,
	    dataset);
    }
    else {
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
    open (location,dataset);
  }

  //_____________________________________________________________________________
  //                                                                setAttributes

  void TBB_DipoleDataset::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("STATION_ID");
    attributes_p.insert("RSP_ID");
    attributes_p.insert("RCU_ID");
    attributes_p.insert("SAMPLE_FREQUENCY_VALUE");
    attributes_p.insert("SAMPLE_FREQUENCY_UNIT");
    attributes_p.insert("TIME");
    attributes_p.insert("SAMPLE_NR");
    attributes_p.insert("SAMPLES_PER_FRAME");
    attributes_p.insert("DATA_LENGTH");
    attributes_p.insert("NYQUIST_ZONE");
    attributes_p.insert("FEED");
    attributes_p.insert("ANTENNA_POSITION_VALUE");
    attributes_p.insert("ANTENNA_POSITION_UNIT");
    attributes_p.insert("ANTENNA_POSITION_FRAME");
    attributes_p.insert("ANTENNA_ORIENTATION_VALUE");
    attributes_p.insert("ANTENNA_ORIENTATION_UNIT");
    attributes_p.insert("ANTENNA_ORIENTATION_FRAME");
  }

  //_____________________________________________________________________________
  //                                                                 openEmbedded

  bool TBB_DipoleDataset::open (hid_t const &location,
				std::string const &name,
				bool const &create)
  {
    bool status (true);

    /* Set up the list of attributes attached to the root group */
    setAttributes();

    /* Get the list of groups attached to this group */
    std::set<std::string> groups;
    h5get_names (groups,location,H5G_DATASET);

    if (static_cast<bool>(groups.count(name))) {
      location_p = H5Gopen (location,
			    name.c_str(),
			    H5P_DEFAULT);
    } else {
      location_p = 0;
    }

    if (location_p > 0) {
      status = true;
    } else {
      /* If failed to open the group, check if we are supposed to create one */
      if (create) {
	location_p = H5Gcreate (location,
				name.c_str(),
				H5P_DEFAULT,
				H5P_DEFAULT,
				H5P_DEFAULT);
	/* If creation was sucessful, add attributes with default values */
	if (location_p > 0) {
	  // write the attributes
	} else {
	  std::cerr << "[TBB_DipoleDataset::open] Failed to create group "
		    << name
		    << std::endl;
	  status = false;
	}
      } else {
	std::cerr << "[TBB_DipoleDataset::open] Failed to open group "
		  << name
		  << std::endl;
	status = false;
      }
    }
    
    // Open embedded groups
    if (status) {
      status = openEmbedded (create);
    } else {
      std::cerr << "[TBB_DipoleDataset::open] Skip opening embedded groups!"
		<< std::endl;
    }
 
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool TBB_DipoleDataset::openEmbedded (bool const &create)
  {
    bool status = create;

    return status;
  }

  //_____________________________________________________________________________
  //                                                                      summary

  void TBB_DipoleDataset::summary (std::ostream &os)
  {
    os << "[TBB_DipoleDataset::summary]"   << endl;
    os << "-- Dataset ID .............. = " << location_p  << endl;
    
    if (location_p) {
      /*
       * Additional check in the HDF5 dataset object; if it is valid, we should
       * be able to retrieve the number of attributes attached to it.
       */
      int nofAttributes = H5Aget_num_attrs (location_p);
      
      os << "-- nof. attributes ......... = " << nofAttributes << endl;
      
      if (nofAttributes < 0) {
	os << "--> Illegal number of attached attributes!" << endl;
      }
      else {
	uint stationID;
	uint rspID;
	uint rcuID;
	uint nyquist_zone;
	uint time;
	std::vector<double> antennaPositionValue;
	std::vector<std::string> antennaPositionUnit;
	std::vector<double> antennaOrientationValue;
	std::vector<std::string> antennaOrientationUnit;
	// Retrieve attributes
	getAttribute ("STATION_ID",   stationID);
	getAttribute ("RSP_ID",       rspID);
	getAttribute ("RCU_ID",       rcuID);
	getAttribute ("NYQUIST_ZONE", nyquist_zone);
	getAttribute ("TIME",         time);
	antenna_position_value(antennaPositionValue);
	antenna_position_unit(antennaPositionUnit);
	antenna_orientation_value(antennaOrientationValue);
	antenna_orientation_unit(antennaOrientationUnit);
	/* Display attributes */
	os << "-- STATION_ID .............. = " << stationID             << endl;
	os << "-- RSP_ID .................. = " << rspID                 << endl;
	os << "-- RCU_ID .................. = " << rcuID                 << endl;
	os << "-- CHANNEL_ID .............. = " << channelName()               << endl;
	os << "-- ANTENNA_POSITION_VALUE .. = " << antennaPositionValue    << endl;
	os << "-- ANTENNA_POSITION_UNIT ... = " << antennaPositionUnit     << endl;
	os << "-- ANTENNA_POSITION_FRAME .. = " << antenna_position_frame()    << endl;
	os << "-- ANTENNA_ORIENTATION_VALUE = " << antennaOrientationValue << endl;
	os << "-- ANTENNA_ORIENTATION_UNIT  = " << antennaOrientationUnit  << endl;
	os << "-- ANTENNA_ORIENTATION_FRAME = " << antenna_orientation_frame() << endl;
	os << "-- SAMPLE_FREQUENCY_VALUE .. = " << sample_frequency_value()    << endl;
	os << "-- SAMPLE_FREQUENCY_UNIT ... = " << sample_frequency_unit()     << endl;
	os << "-- NYQUIST_ZONE ............ = " << nyquist_zone << endl;
	os << "-- TIME [Unix seconds]...... = " << time << endl;
	os << "-- TIME [  Julian Day] ..... = " << julianDay()                 << endl;
	os << "-- SAMPLE_NUMBER ........... = " << sample_number()             << endl;
	os << "-- SAMPLES_PER_FRAME ....... = " << samples_per_frame()         << endl;
	os << "-- FEED .................... = " << feed()                      << endl;
	os << "-- DATA_LENGTH ............. = " << data_length()               << endl;
      }
    }
  }
  
  //_____________________________________________________________________________
  //                                                       antenna_position_value

  /*!
    \return value -- Numerical value of the antenna position coordinates, e.g.
            <tt>value=[10,12,0]</tt>
  */
  bool TBB_DipoleDataset::antenna_position_value (std::vector<double> &value)
  {
    return DAL::h5get_attribute(location_p,
				attribute_name(DAL::ANTENNA_POSITION_VALUE),
				value);
  }
  
  //_____________________________________________________________________________
  //                                                        antenna_position_unit

  bool TBB_DipoleDataset::antenna_position_unit (std::vector<std::string> &unit)
  {
    return DAL::h5get_attribute(location_p,
				attribute_name(DAL::ANTENNA_POSITION_UNIT),
				unit);
  }
  
  // ----------------------------------------------------- antenna_position_frame

  /*!
    \return frame -- Identifier for the reference frame within which the antenna
            position is provided, e.g. <tt>frame="ITRF"</tt>
  */
  std::string TBB_DipoleDataset::antenna_position_frame ()
  {
    std::string val;

    if (DAL::h5get_attribute(location_p,
                             attribute_name(DAL::ANTENNA_POSITION_FRAME),
                             val)) {
      return val;
    }
    else {
      return std::string ("UNDEFINED");
    }
  }
  
  // -------------------------------------------------- antenna_orientation_value

  /*!
    \return value -- The numerical values describing the antenna position; this
            can be either a set of Euler angles or a normal vector.
  */
  bool TBB_DipoleDataset::antenna_orientation_value (std::vector<double> &value)
  {
    return DAL::h5get_attribute(location_p,
				attribute_name(DAL::ANTENNA_ORIENTATION_VALUE),
				value);
  }

  // --------------------------------------------------- antenna_orientation_unit

  /*!
    \return unit -- Physical unit associated with the numerical values for the
            antenna orientation; depending on the parametrization this can be
      <tt>unit="rad"</tt>, <tt>unit="deg"</tt> or <tt>unit="m"</tt>.
  */
  bool TBB_DipoleDataset::antenna_orientation_unit (std::vector<std::string> &unit)
  {
    return DAL::h5get_attribute(location_p,
				attribute_name(DAL::ANTENNA_ORIENTATION_UNIT),
				unit);
  }
  
  // -------------------------------------------------- antenna_orientation_frame

  /*!
    \return frame -- Identifier for the reference frame within which the antenna
            orientation is provided, e.g. <tt>frame="ITRF"</tt>
  */
  std::string TBB_DipoleDataset::antenna_orientation_frame ()
  {
    std::string val;

    if (DAL::h5get_attribute(location_p,
                             attribute_name(DAL::ANTENNA_ORIENTATION_FRAME),
                             val)) {
      return val;
    }
    else {
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
    uint station_id;
    uint rsp_id;
    uint rcu_id;
    //
    getAttribute ("STATION_ID",station_id);
    getAttribute ("RSP_ID",    rsp_id);
    getAttribute ("RCU_ID",    rcu_id);
    //
    return rcu_id + 1000*rsp_id + 1000000*station_id;
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
    uint station_id;
    uint rsp_id;
    uint rcu_id;
    //
    getAttribute ("STATION_ID",station_id);
    getAttribute ("RSP_ID",    rsp_id);
    getAttribute ("RCU_ID",    rcu_id);
    //
    sprintf(uid,
            "%03d%03d%03d",
            station_id,
            rsp_id,
            rcu_id);
    std::string channelID (uid);

    return channelID;
  }

  // ------------------------------------------------------------------ julianDay
  
  /*!
    \param onlySeconds -- Fully quallified timestamp for the first sample? If
           set to <tt>true</tt> only the UNIX time -- qualifying the full
	   seconds -- will be returned.
  */
  double TBB_DipoleDataset::julianDay (bool const &onlySeconds)
  {
    uint seconds;
    double jd;

    getAttribute ("TIME",seconds);

    if (onlySeconds) {
      jd = seconds/86400+2440587.5;
    }
    else {
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

  //_____________________________________________________________________________
  //                                                                           fx

  /*!
    Basically we need to be able to deal with three different cases:
      a) all of the requested data are outside the valid range
      b) parts of the requested data are outside the valid range
      c) all of the requested data are within the valid range
    If parts of the requested data are outside the valid range of points which 
    can be requested from the file we need to 1) shift the position of the first
    data point requested from the file and then 2) adjust the number of
    datapoints to be requested from the file.

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
    bool status (true);

    /*
     *  Set up the logic for secure access to the underlying data
     */
    int dataStart;
    int dataEnd (start+nofSamples-1);
    int dataLength;
    int dataOffset;
    short *dataBuffer;
    
    if (start<0) {
      if (dataEnd<0) {
	for (int n(0); n<nofSamples; ++n) {
	  data[n] = 0;
	}
	return false;
      } else {
	dataStart = 0;
	/* Shift the position of the first data point requested from the file */
	dataOffset = -start;
	/* Adjust the number of datapoints to be requested from the file */
	dataLength = nofSamples-dataOffset;
	/* Initialize the array elements which will not be used for actual data */
	for (int n(0); n<dataOffset; ++n) {
	  data[n] = 0;
	}
      }
    } else {
      dataStart  = start;
      dataOffset = 0;
      dataLength = nofSamples;
    }

    dataBuffer = new short[dataLength];
    dataBuffer = &data[dataOffset];

#ifdef DEBUGGING_MESSAGES
    std::cout << "[TBB_DipoleDataset::fx]" << std::endl;
    std::cout << "-- Data start  = " << dataStart  << std::endl;
    std::cout << "-- Data offset = " << dataOffset << std::endl;
    std::cout << "-- Data length = " << dataLength << std::endl;
#endif

    /* Start accessing the data within the HDF5 file */
    
    if (location_p > 0) {
      int rank          = 0;
      herr_t h5error    = 0;
      hid_t dataspaceID = H5Dget_space(location_p);
      
      if (dataspaceID < 0) {
	cerr << "[TBB_DipoleDataset::fx]"
	     << " Error retrieving filespace of dataset!" << endl;
	return false;
      }
      
      /* Retrieve the rank of the dataspace asssociated with the dataset */
      
      rank = H5Sget_simple_extent_ndims(dataspaceID);
      
      if (rank < 0) {
	cerr << "[TBB_DipoleDataset::fx]"
	     << " Error retrieving rank of dataspace!" << endl;
	return false;
      }
      
      /* Retrieve the dimension of the dataspace, i.e. the number of samples */
      
      hsize_t shape[1];
      h5error = H5Sget_simple_extent_dims (dataspaceID,
					   shape,
					   NULL);
      if (h5error < 0) {
	cerr << "[TBB_DipoleDataset::fx]"
	     << " Error retrieving dataspace dimension!" << endl;
	return false;
      }
      else {
	shape[0] = dataLength;
      }
      
      /* Set up memory space to retrieve the data read from the file */
      
      hid_t memspaceID = H5Screate_simple (rank,
					   shape,
					   shape);
      hsize_t offset[1];
      
      if (dataspaceID < 0) {
	cerr << "[TBB_DipoleDataset::fx]"
	     << " Error creating memory space for reading in data!"
	     << endl;
	return false;
      }
      else {
	offset[0] = dataStart;
      }
      
      /* Select the hyperslab through the data volume */
      h5error = H5Sselect_hyperslab (dataspaceID,
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
      
      // Retrieve the actual data from the file ...
      h5error = H5Dread (location_p,
			 H5T_NATIVE_SHORT,
			 memspaceID,
			 dataspaceID,
			 H5P_DEFAULT,
			 data);
      // ... and indicate if there was an error during that procedure
      if (h5error < 0) {
	cerr << "[TBB_DipoleDataset::fx]"
	     << " Error reading data from file into buffer!"
	     << endl;
	status = false;
      }
      /* release HDF5 handlers */
      h5error = H5Sclose (dataspaceID);
      h5error = H5Sclose (memspaceID);
    }
    else {
      cerr << "[TBB_DipoleDataset::fx]"
	   << " Unable to read with connection to dataset object!"
	   << endl;
      status = false;
    } // END  ::  if (location_p > 0)
    
    return status;
  }
  
  // ============================================================================
  //
  //  Methods using casacore
  //
  // ============================================================================
  
#ifdef HAVE_CASA

  //_____________________________________________________________________________
  //                                                       antenna_position_value
  
  bool TBB_DipoleDataset::antenna_position_value (casa::Vector<double> &value)
  {
    return DAL::h5get_attribute(location_p,
				attribute_name(DAL::ANTENNA_POSITION_VALUE),
				value);
  }
  
  //_____________________________________________________________________________
  //                                                        antenna_position_unit
  
  /*!
    \return unit -- Physical unit associated with the numerical values for the
            antenna position, e.g. <tt>unit="m"</tt>
  */
  bool TBB_DipoleDataset::antenna_position_unit (casa::Vector<casa::String> &unit)
  {
    return DAL::h5get_attribute(location_p,
				attribute_name(DAL::ANTENNA_POSITION_UNIT),
				unit);
  }

  //_____________________________________________________________________________
  //                                                             antenna_position

  /*!
    \return position -- The antenna position as casa::Measure, combining the
            information from ANTENNA_POSITION_VALUE, ANTENNA_POSITION_UNIT and
      ANTENNA_POSITION_FRAME.
  */
  casa::MPosition TBB_DipoleDataset::antenna_position ()
  {
    return DAL::h5get_position (location_p,
                                DAL::ANTENNA_POSITION_VALUE,
                                DAL::ANTENNA_POSITION_UNIT,
                                DAL::ANTENNA_POSITION_FRAME);
  }

  //_____________________________________________________________________________
  //                                                    antenna_orientation_value
  
  bool TBB_DipoleDataset::antenna_orientation_value (casa::Vector<double> &value)
  {
    return DAL::h5get_attribute(location_p,
				attribute_name(DAL::ANTENNA_ORIENTATION_VALUE),
				value);
  }
  
  //_____________________________________________________________________________
  //                                                     antenna_orientation_unit
  
  bool TBB_DipoleDataset::antenna_orientation_unit (casa::Vector<casa::String> &unit)
  {
    return DAL::h5get_attribute(location_p,
				attribute_name(DAL::ANTENNA_ORIENTATION_UNIT),
				unit);
  }

  //_____________________________________________________________________________
  //                                                             sample_frequency
  
  bool TBB_DipoleDataset::sample_frequency (casa::Quantity &freq)
  { 
    if (location_p > 0) {
      freq = DAL::h5get_quantity (location_p,
				  DAL::SAMPLE_FREQUENCY_VALUE,
				  DAL::SAMPLE_FREQUENCY_UNIT);
      return true;
    }
    else {
      cerr << "[TBB_DipoleDataset::sample_frequency] Dataset undefined!"
	   << endl;
      return false;
    }
  }
  
  //_____________________________________________________________________________
  //                                                             sample_frequency
  
  bool TBB_DipoleDataset::sample_frequency (casa::MFrequency &freq)
  {
    if (location_p > 0) {
      casa::Quantity qFreq = DAL::h5get_quantity (location_p,
						  DAL::SAMPLE_FREQUENCY_VALUE,
						  DAL::SAMPLE_FREQUENCY_UNIT);
      freq = casa::MFrequency (qFreq,
			       casa::MFrequency::TOPO);
      return true;
    }
    else {
      cerr << "[TBB_DipoleDataset::sample_frequency] Dataset undefined!"
	   << endl;
      return false;
    }
  }

  //_____________________________________________________________________________
  //                                                         set_sample_frequency
  
  /*!
    \param freq -- The ADC sample frequency as casa::Quantity
  */
  bool TBB_DipoleDataset::set_sample_frequency (casa::Quantity const &freq)
  {
    bool status (true);
    double val       = freq.getValue();
    std::string unit = freq.getUnit();
    
    status *= setAttribute ("SAMPLE_FREQUENCY_VALUE",val);
    status *= setAttribute ("SAMPLE_FREQUENCY_UNIT",unit);
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                            recordDescription

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

  //_____________________________________________________________________________
  //                                                            attributes2record
  
  /*!
      \return record -- A casa::Record container holding the values of the
              attributes attached to the dataset for this dipole
  */
  casa::Record TBB_DipoleDataset::attributes2record ()
  {
    casa::Record rec;

    attributes2record (rec);

    return rec;
  }

  //_____________________________________________________________________________
  //                                                            attributes2record

  /*!
    \retval record -- A casa::Record container holding the values of the
            attributes attached to the dataset for this dipole
  */
  bool TBB_DipoleDataset::attributes2record (casa::Record &rec)
  {
    bool status (true);
    
    try {
      uint station_id;
      uint rsp_id;
      uint rcu_id;
      uint nyquist_zone;
      uint time;
      casa::Vector<double> antennaPositionValue;
      casa::Vector<casa::String> antennaPositionUnit;
      casa::Vector<double> antennaOrientationValue;
      casa::Vector<casa::String> antennaOrientationUnit;

      // Retrieve attribute values
      getAttribute ("STATION_ID",   station_id);
      getAttribute ("RSP_ID",       rsp_id);
      getAttribute ("RCU_ID",       rcu_id);
      getAttribute ("NYQUIST_ZONE", nyquist_zone);
      getAttribute ("TIME",         time);
      antenna_position_value (antennaPositionValue);
      antenna_position_unit  (antennaPositionUnit);
      antenna_orientation_value (antennaOrientationValue);
      antenna_orientation_unit (antennaOrientationUnit);

      // Fill record
      rec.define(casa::RecordFieldId(attribute_name(DAL::STATION_ID)),
		 station_id);
      rec.define(casa::RecordFieldId(attribute_name(DAL::RSP_ID)),
		 rsp_id);
      rec.define(casa::RecordFieldId(attribute_name(DAL::RCU_ID)),
		 rcu_id);
      rec.define(casa::RecordFieldId(attribute_name(DAL::TIME)),
		 time);
      rec.define(casa::RecordFieldId(attribute_name(DAL::NYQUIST_ZONE)),
		 nyquist_zone);
      rec.define(casa::RecordFieldId(attribute_name(DAL::SAMPLE_NUMBER)),
		 sample_number());
      rec.define(casa::RecordFieldId(attribute_name(DAL::SAMPLES_PER_FRAME)),
		 samples_per_frame());
      rec.define(casa::RecordFieldId(attribute_name(DAL::FEED)),
		 feed());
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_POSITION_VALUE)),
		 antennaPositionValue);
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_POSITION_UNIT)),
		 antennaPositionUnit);
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_POSITION_FRAME)),
		 antenna_position_frame());
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_ORIENTATION_VALUE)),
		 antennaOrientationValue);
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_ORIENTATION_UNIT)),
		 antennaOrientationUnit);
      rec.define(casa::RecordFieldId(attribute_name(DAL::ANTENNA_ORIENTATION_FRAME)),
		 antenna_orientation_frame());
      rec.define(casa::RecordFieldId(attribute_name(DAL::DATA_LENGTH)),
		 data_length());
    }
    catch (std::string message) {
      cerr << "[TBB_DipoleDataset::attributes2record] "
	   << "Error filling the record with attribute values!\n"
	   << message
	   << endl;
      status = false;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                           fx
  
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
    if (location_p > 0) {
      bool status   = true;
      short *buffer = new short [nofSamples];
      
      //______________________________________________________________
      // Retrieve the data from the file
      
      status = fx (start,
		   nofSamples,
		   buffer);
      
      //______________________________________________________________
      // Copy the data from the buffer into the vector returned by
      // this function
      
      if (status) {
	casa::Vector<double> data (nofSamples);
	// copy values to returned vector
	for (int sample(0); sample<nofSamples; sample++) {
	  data(sample) = double(buffer[sample]);
	}
	// release allocated memory
	delete [] buffer;
	// return data
	return data;
      }
      else {
	// release allocated memory
	delete [] buffer;
	// return data
	return casa::Vector<double> (1,0);
      }
      
    }
    else {
      return casa::Vector<double> (1,0);
    }
  }
  
#endif

} // Namespace DAL -- end
