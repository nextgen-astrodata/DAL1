/***************************************************************************
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

#include <data_hl/TBB_DipoleDataset.h>

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
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                            TBB_DipoleDataset
  
  /*!
    \param location -- Identifier for the location within the HDF5 file, below
           which the dataset is placed.
    \param dataset  -- Name of the dataset.
  */
  TBB_DipoleDataset::TBB_DipoleDataset (hid_t const &location,
                                        std::string const &name,
                                        IO_Mode const &flags)
    : HDF5GroupBase ()
  {
    init ();
    open (location,name,flags);
  }

  //_____________________________________________________________________________
  //                                                            TBB_DipoleDataset
  
  /*!
    \param location -- Identifier for the location within the HDF5 file, below
           which the dataset is placed.
    \param station  -- Station identifier.
    \param rsp      -- RSP identifier.
    \param rcu      -- RCU identifier.
  */
  TBB_DipoleDataset::TBB_DipoleDataset (hid_t const &location,
					uint const &stationID,
					uint const &rspID,
					uint const &rcuID)
  {
    std::string name = dipoleName (stationID, rspID, rcuID);

    init ();
    open (location,name,false);
  }
  

  //_____________________________________________________________________________
  //                                                            TBB_DipoleDataset
  
  /*!
    \param location -- Identifier for the location within the HDF5 file, below
           which the dataset is placed.
    \param station  -- Station identifier.
    \param rsp      -- RSP identifier.
    \param rcu      -- RCU identifier.
    \param shape    -- Shape of the dataset array.
    \param datatype -- Datatype of the array elements.
  */
  TBB_DipoleDataset::TBB_DipoleDataset (hid_t const &location,
					uint const &stationID,
					uint const &rspID,
					uint const &rcuID,
					std::vector<hsize_t> const &shape,
					hid_t const &datatype)
  {
    init ();
    open (location,
	  stationID,
	  rspID,
	  rcuID,
	  shape,
	  datatype);
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
    herr_t h5error;

    if (datatype_p>0 && H5Iis_valid(datatype_p)) {
      h5error    = H5Tclose (datatype_p);
      datatype_p = 0;
    }

    if (dataspace_p > 0 && H5Iis_valid(dataspace_p)) {
      h5error     = H5Sclose (dataspace_p);
      dataspace_p = 0;
    }

    if (location_p > 0 && H5Iis_valid(location_p)) {
      h5error    = H5Dclose (location_p);
      location_p = 0;
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
  
  /*!
    \param other -- Another TBB_DipoleDataset object from which to create
           this new one.
  */
  void TBB_DipoleDataset::copy (TBB_DipoleDataset const &other)
  {
    location_p  = other.location_p;
    datatype_p  = other.datatype_p;
    dataspace_p = other.dataspace_p;

    open (other.location_p);
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init
  
  void TBB_DipoleDataset::init ()
  {
    datatype_p  = -1;
    dataspace_p = -1;
    location_p  = -1;
    itsShape     = std::vector<hsize_t>();
  }

  //_____________________________________________________________________________
  //                                                                         init
  
  /*!
    \param filename -- HDF5 file within which the dataset in question is
           contained
    \param dataset  -- Name of the dataset which this object is to encapsulate.
  */
  void TBB_DipoleDataset::init (std::string const &filename,
                                std::string const &dataset)
  {
    hid_t file_id  = 0;
    herr_t h5error = 0;

    // Initialize internal variables
    location_p = 0;

    // open the file
    file_id = H5Fopen (filename.c_str(),
                       H5F_ACC_RDONLY,
                       H5P_DEFAULT);

    // if opening of file was successfull, try to open dataset
    if (file_id > 0) {
      open (file_id,
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
    attributes_p.insert("SAMPLE_NUMBER");
    attributes_p.insert("SAMPLES_PER_FRAME");
    attributes_p.insert("DATA_LENGTH");
    attributes_p.insert("NYQUIST_ZONE");
    attributes_p.insert("CABLE_DELAY");
    attributes_p.insert("CABLE_DELAY_UNIT");
    attributes_p.insert("FEED");
    attributes_p.insert("ANTENNA_POSITION_VALUE");
    attributes_p.insert("ANTENNA_POSITION_UNIT");
    attributes_p.insert("ANTENNA_POSITION_FRAME");
    attributes_p.insert("ANTENNA_ORIENTATION_VALUE");
    attributes_p.insert("ANTENNA_ORIENTATION_UNIT");
    attributes_p.insert("ANTENNA_ORIENTATION_FRAME");
  }

  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Identifier for the location within the HDF5 file, below
           which the dataset is placed.
  */
  bool TBB_DipoleDataset::open (hid_t const &location)
  {
    bool status (true);
    bool absolutePath (false);
    herr_t h5error;
    std::string filename;
    std::string dataset;
    hid_t fileID;
    IO_Mode flags = IO_Mode();
    
    // Get name of file and dataset ________________________
    
    status  = DAL::h5get_filename (filename, location);
    status *= DAL::h5get_name (dataset, location, absolutePath);

    if (status) {

      h5get_flags(flags, location);

      if (flags.flags() & IO_Mode::ReadOnly) {
        fileID = H5Fopen (filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
      }
      else {
        fileID = H5Fopen (filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);

        /* If opening the the file failed, this might have been due to wrong
           access permissions; check if the file can be opened as read-only. */
        if (fileID<0) { fileID = H5Fopen (filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
        }
      }
        // open the dataset
      status = open (fileID,dataset,flags);
      // release file handler
      h5error = H5Fclose (fileID);
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \param location -- Identifier for the location within the HDF5 file, below
           which the dataset is placed.
    \param name     -- Name of the dataset.
    \param create   -- I/O mode flags.
  */
  bool TBB_DipoleDataset::open (hid_t const &location,
				std::string const &name,
				IO_Mode const &flags)
  {
    bool status (true);
    bool ok (true);

    /* Set up the list of attributes */
    setAttributes();

    if (H5Lexists (location, name.c_str(), H5P_DEFAULT)) {
      location_p = H5Dopen (location,
          name.c_str(),
          H5P_DEFAULT);
    }
    else {
      location_p = 0;
    }

    if (location_p > 0) {
      datatype_p  = H5Dget_type (location_p);
      dataspace_p = H5Dget_space (location_p);
      int rank    = H5Sget_simple_extent_ndims (dataspace_p);
      if (rank>0) {
        itsShape.resize(rank);
        hsize_t * dims    = new hsize_t[rank];
        hsize_t * maxdims = new hsize_t[rank];
        rank              = H5Sget_simple_extent_dims (dataspace_p, dims, maxdims);
        for (int n(0); n<rank; ++n) {
          itsShape[n] = dims[n];
        }
        // release allocated memory
        delete [] dims;
        delete [] maxdims;
      }
      // update status
      status = true;
    } else {
      /* Check conditions to enable creation of a new dataset */
      if (datatype_p<0) {
        std::cerr << "[TBB_DipoleDataset::open] Skipping creation of dataset"
          << " - datatype undefined."
          << std::endl;
        ok = false;
      }
      /* If failed to open the group, check if we are supposed to create one */
      if ( (flags.flags() & IO_Mode::Create) ||
          (flags.flags() & IO_Mode::CreateNew) ) {
        // Create Dataspace
        int rank = itsShape.size();
        hsize_t dimensions [rank];
        for (int n(0); n<rank; ++n) {
          dimensions[n] = itsShape[n];
        }
        dataspace_p = H5Screate_simple (rank,dimensions,NULL);
        /* Create the dataset */
        location_p = H5Dcreate (location,
            name.c_str(),
            datatype_p,
            dataspace_p,
            H5P_DEFAULT,
            H5P_DEFAULT,
            H5P_DEFAULT);
        /* If creation was sucessful, add attributes with default values */
        if (location_p > 0) {
          std::string grouptype ("DipoleDataset");
          std::string undefined ("UNDEFINED");
          std::vector<double> vectDouble (3,0.0);
          std::vector<std::string> vectString (3,undefined);
          // write the attributes
          HDF5Attribute::write (location_p,"GROUPTYPE",                 grouptype   );
          HDF5Attribute::write (location_p,"STATION_ID",                uint (0)    );
          HDF5Attribute::write (location_p,"RSP_ID",                    uint (0)    );
          HDF5Attribute::write (location_p,"RCU_ID",                    uint (0)    );
          HDF5Attribute::write (location_p,"SAMPLE_FREQUENCY_VALUE",    double(0.0) );
          HDF5Attribute::write (location_p,"SAMPLE_FREQUENCY_UNIT",     undefined   );
          HDF5Attribute::write (location_p,"TIME",                      uint(0)     );
          HDF5Attribute::write (location_p,"SAMPLE_NUMBER",             uint(0)     );
          HDF5Attribute::write (location_p,"SAMPLES_PER_FRAME",         uint(0)     );
          HDF5Attribute::write (location_p,"DATA_LENGTH",               uint(0)     );
          HDF5Attribute::write (location_p,"NYQUIST_ZONE",              uint(0)     );
          HDF5Attribute::write (location_p,"FEED",                      undefined   );
          HDF5Attribute::write (location_p,"ANTENNA_POSITION_VALUE",    vectDouble  );
          HDF5Attribute::write (location_p,"ANTENNA_POSITION_UNIT",     vectString  );
          HDF5Attribute::write (location_p,"ANTENNA_POSITION_FRAME",    undefined   );
          HDF5Attribute::write (location_p,"ANTENNA_ORIENTATION_VALUE", vectDouble  );
          HDF5Attribute::write (location_p,"ANTENNA_ORIENTATION_UNIT",  vectString  );
          HDF5Attribute::write (location_p,"ANTENNA_ORIENTATION_FRAME", undefined   );
        } else {
#ifdef DAL_DEBUGGING_MESSAGES
          std::cerr << "[TBB_DipoleDataset::open] Failed to create group "
            << name
            << std::endl;
#endif
          status = false;
        }
      } else {
#ifdef DAL_DEBUGGING_MESSAGES
        std::cerr << "[TBB_DipoleDataset::open] Failed to open dataset "
          << name
          << std::endl;
#endif
        status = false;
      }
    }

    // Open embedded groups
    if (status) {
      status = openEmbedded (flags);
    }
#ifdef DAL_DEBUGGING_MESSAGES
    else {
      std::cerr << "[TBB_DipoleDataset::open] Skip opening embedded groups!"
        << std::endl;
    }
#endif

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                         open

  /*!
    \param location -- Identifier for the location within the HDF5 file, below
           which the dataset is placed.
    \param station  -- Station identifier.
    \param rsp      -- RSP identifier.
    \param rcu      -- RCU identifier.
    \param shape    -- Shape of the dataset array.
    \param datatype -- Datatype of the array elements.
  */
  bool TBB_DipoleDataset::open (hid_t const &location,
				uint const &stationID,
				uint const &rspID,
				uint const &rcuID,
				std::vector<hsize_t> const &shape,
				hid_t const &datatype)
  {
    bool status (true);

    // store variables describing the array
    if (datatype != H5I_BADID) {
      datatype_p = H5Tcopy(datatype);
      itsShape.resize(shape.size());
      itsShape = shape;
    } else {
      datatype_p = -1;
    }

    // convert IDs to name
    std::string name = dipoleName (stationID,rspID,rcuID);

    // open the dataset
    status = open (location,name,true);

    // store the IDs into the new
    if (status) {
      status *= setAttribute("STATION_ID",stationID);
      status *= setAttribute("RSP_ID",rspID);
      status *= setAttribute("RCU_ID",rcuID);
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  /*!
    \param flags   -- I/O mode flags.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool TBB_DipoleDataset::openEmbedded (IO_Mode const &flags)
  {
    bool status = flags.flags();
    std::vector<hsize_t> shape;
    
    status = HDF5Dataspace::shape (location_p,shape);

    if (status && shape.size() > 0 && !(flags.flags() & IO_Mode::ReadOnly)) {
      status *= setAttribute("DATA_LENGTH",shape[0]);
    }

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void TBB_DipoleDataset::summary (std::ostream &os)
  {
    os << "[TBB_DipoleDataset::summary]"                      << std::endl;
    os << "-- Dataset ID .............. = " << location_p     << std::endl;
    if (location_p > 0) {
      os << "-- Dataset name .......... = " << locationName() << std::endl;
      os << "-- Channel name (ID) ..... = " << dipoleName()   << std::endl;
    }
    os << "-- Dataspace ID ............ = " << dataspace_p    << std::endl;
    os << "-- Dataset datatype ........ = " << datatype_p     << std::endl;
    os << "-- Data array shape ........ = " << itsShape       << std::endl;
    
    if (location_p>0) {
      /*
       * Additional check in the HDF5 dataset object; if it is valid, we should
       * be able to retrieve the number of attributes attached to it.
       */
      int nofAttributes = H5Aget_num_attrs (location_p);
      
      os << "-- nof. attributes ......... = " << nofAttributes << endl;
      
      if (nofAttributes > 0) {
	uint stationID       = 0;
	uint rspID           = 0;
	uint rcuID           = 0;
	uint nyquist_zone    = 0;
	uint time            = 0;
	uint sampleNumber    = 0;
	uint samplesPerFrame = 0;
	uint dataLength      = 0;
	std::string feed;
	double sampleFreqValue;
	std::string sampleFreqUnit;
	std::vector<double> antennaPositionValue;
	std::vector<std::string> antennaPositionUnit;
	std::string antennaPositionFrame;
	std::vector<double> antennaOrientationValue;
	std::vector<std::string> antennaOrientationUnit;
	std::string antennaOrientationFrame;
	// Retrieve attributes
	getAttribute ("STATION_ID",                stationID);
	getAttribute ("RSP_ID",                    rspID);
	getAttribute ("RCU_ID",                    rcuID);
	getAttribute ("NYQUIST_ZONE",              nyquist_zone);
	getAttribute ("TIME",                      time);
	getAttribute ("SAMPLE_NUMBER",             sampleNumber);
	getAttribute ("SAMPLES_PER_FRAME",         samplesPerFrame);
	getAttribute ("SAMPLE_FREQUENCY_VALUE",    sampleFreqValue);
	getAttribute ("SAMPLE_FREQUENCY_UNIT",     sampleFreqUnit);
	getAttribute ("DATA_LENGTH",               dataLength);
	getAttribute ("FEED",                      feed);
	getAttribute ("ANTENNA_POSITION_VALUE",    antennaPositionValue);
	getAttribute ("ANTENNA_POSITION_UNIT",     antennaPositionUnit);
	getAttribute ("ANTENNA_POSITION_FRAME",    antennaPositionFrame);
	getAttribute ("ANTENNA_ORIENTATION_VALUE", antennaOrientationValue);
	getAttribute ("ANTENNA_ORIENTATION_UNIT",  antennaOrientationUnit);
	getAttribute ("ANTENNA_ORIENTATION_FRAME", antennaOrientationFrame);
	/* Display attributes */
	os << "-- STATION_ID .............. = " << stationID               << endl;
	os << "-- RSP_ID .................. = " << rspID                   << endl;
	os << "-- RCU_ID .................. = " << rcuID                   << endl;
	os << "-- DIPOLE_ID .,............. = " << dipoleName()            << endl;
	os << "-- ANTENNA_POSITION_VALUE .. = " << antennaPositionValue    << endl;
	os << "-- ANTENNA_POSITION_UNIT ... = " << antennaPositionUnit     << endl;
	os << "-- ANTENNA_POSITION_FRAME .. = " << antennaPositionFrame    << endl;
	os << "-- ANTENNA_ORIENTATION_VALUE = " << antennaOrientationValue << endl;
	os << "-- ANTENNA_ORIENTATION_UNIT  = " << antennaOrientationUnit  << endl;
	os << "-- ANTENNA_ORIENTATION_FRAME = " << antennaOrientationFrame << endl;
	os << "-- SAMPLE_FREQUENCY_VALUE .. = " << sampleFreqValue << endl;
	os << "-- SAMPLE_FREQUENCY_UNIT ... = " << sampleFreqUnit << endl;
	os << "-- NYQUIST_ZONE ............ = " << nyquist_zone << endl;
	os << "-- TIME [Unix seconds]...... = " << time << endl;
	os << "-- TIME [  Julian Day] ..... = " << julianDay() << endl;
	os << "-- SAMPLE_NUMBER ........... = " << sampleNumber << endl;
	os << "-- SAMPLES_PER_FRAME ....... = " << samplesPerFrame << endl;
	os << "-- FEED .................... = " << feed       << endl;
	os << "-- DATA_LENGTH ............. = " << dataLength << endl;
      }
    }
  }
  
  //_____________________________________________________________________________
  //                                                                 dipoleNumber

  /*!
    \param station -- Identifier for the station.
    \param rsp     -- Identifier for the RSP board.
    \param rcu     -- Identifier for RCU.
    
    \return dipoleNumber -- The unique identifier for a signal channel/dipole
            within the whole LOFAR array; this ID is created from a combination
	    of station ID, RSP ID and RCU ID:
	    \f$ N_{\rm RCU} + 10^{3} \times N_{\rm RSP} + 10^{6} \times
	    N_{\rm Station} \f$
  */
  int TBB_DipoleDataset::dipoleNumber (unsigned int const &station,
				       unsigned int const &rsp,
				       unsigned int const &rcu)
  {
    return rcu + 1e3*rsp + 1e6*station;
  }
  
  //_____________________________________________________________________________
  //                                                                 dipoleNumber

  /*!
    \return dipoleNumber -- The unique identifier for a signal channel/dipole
            within the whole LOFAR array; this ID is created from a combination
	    of station ID, RSP ID and RCU ID:
	    \f$ N_{\rm RCU} + 10^{3} \times N_{\rm RSP} + 10^{6} \times
	    N_{\rm Station} \f$
  */
  int TBB_DipoleDataset::dipoleNumber ()
  {
    uint station;
    uint rsp;
    uint rcu;
    //
    getAttribute ("STATION_ID",station);
    getAttribute ("RSP_ID",    rsp);
    getAttribute ("RCU_ID",    rcu);
    //
    return dipoleNumber (station, rsp, rcu);
  }

  //_____________________________________________________________________________
  //                                                                   dipoleName

  /*!
    \return channel_id -- The unique identifier for a signal channel/dipole
            within the whole LOFAR array; this ID is created from a combination
	    of station ID, RSP ID and RCU ID.
  */
  std::string TBB_DipoleDataset::dipoleName ()
  {
    uint station;
    uint rsp;
    uint rcu;
    //
    getAttribute ("STATION_ID",station);
    getAttribute ("RSP_ID",    rsp);
    getAttribute ("RCU_ID",    rcu);

    return dipoleName (station,rsp,rcu);
  }

  //_____________________________________________________________________________
  //                                                                   dipoleName

  /*!
    \param station -- 
    \param rsp     -- 
    \param rcu     -- 
    \return channel_id -- The unique identifier for a signal channel/dipole
            within the whole LOFAR array; this ID is created from a combination
	    of station ID, RSP ID and RCU ID.
  */
  std::string TBB_DipoleDataset::dipoleName (unsigned int const &station,
					     unsigned int const &rsp,
					     unsigned int const &rcu)
  {
    char uid[10];
    
    sprintf(uid, "%03d%03d%03d", station, rsp, rcu);
    
    std::string id (uid);
    
    return id;
  }
  
  //_____________________________________________________________________________
  //                                                                    julianDay

  /*!
    \param onlySeconds -- Fully quallified timestamp for the first sample? If
           set to <tt>true</tt> only the UNIX time -- qualifying the full
	   seconds -- will be returned.
  */
  double TBB_DipoleDataset::julianDay (bool const &onlySeconds)
  {
    uint seconds = 0;
    double jd    = 0;

    getAttribute ("TIME",seconds);

    if (onlySeconds) {
      jd = seconds/86400+2440587.5;
    }
    else {
      // retrieve additional information
      uint samplesSinceSecond;
      double sampleFrequency;
      //
      getAttribute ("SAMPLE_NUMBER",          samplesSinceSecond);
      getAttribute ("SAMPLE_FREQUENCY_VALUE", sampleFrequency);
      // conversion
      if (sampleFrequency > 0) {
	jd = (seconds+samplesSinceSecond/sampleFrequency)/86400+2440587.5;
      }
    }
    
    return jd;
  }

  //_____________________________________________________________________________
  //                                                                     readData

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
  bool TBB_DipoleDataset::readData (int const &start,
				    int const &nofSamples,
				    short *data)
  {
    bool status (true);

    //______________________________________________________
    // Set up the logic for secure access to the underlying data

    int dataStart  = 0;
    int dataEnd    = start+nofSamples-1;
    int dataLength = 0;
    int dataOffset = 0;
//     short *dataBuffer;
    
    if (start<0) {
      if (dataEnd<0) {
	return false;
      } // end -- (dataEnd<0) 
      else {
	dataStart = 0;
	/* Shift the position of the first data point requested from the file */
	dataOffset = -start;
	/* Adjust the number of datapoints to be requested from the file */
	dataLength = nofSamples-dataOffset;
	/* Initialize the array elements which will not be used for actual data */
      }
    } else {
      dataStart  = start;
      dataOffset = 0;
      dataLength = nofSamples;
    }

//     dataBuffer = new short[dataLength];
//     dataBuffer = &data[dataOffset];

    /* Start accessing the data within the HDF5 file */
    
    if (location_p > 0) {
      int rank          = 0;
      herr_t h5error    = 0;
      hid_t dataspaceID = H5Dget_space(location_p);
      
      if (dataspaceID < 0) {
	cerr << "[TBB_DipoleDataset::readData]"
	     << " Error retrieving filespace of dataset!" << endl;
	return false;
      }
      
      /* Retrieve the rank of the dataspace asssociated with the dataset */
      
      rank = H5Sget_simple_extent_ndims(dataspaceID);
      
      if (rank < 0) {
	cerr << "[TBB_DipoleDataset::readData]"
	     << " Error retrieving rank of dataspace!" << endl;
	return false;
      }
      
      /* Retrieve the dimension of the dataspace, i.e. the number of samples */
      
      hsize_t shape[1];
      h5error = H5Sget_simple_extent_dims (dataspaceID,
					   shape,
					   NULL);
      if (h5error < 0) {
	cerr << "[TBB_DipoleDataset::readData]"
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
	cerr << "[TBB_DipoleDataset::readData]"
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
	cerr << "[TBB_DipoleDataset::readData]"
	     << " Error selecting hyperslab through the data!"
	     << endl;
	return false;
      }
      
      // Zero out the data array (note zeros can thus be either real or unread samples)
	    for (int n(0); n<nofSamples; ++n) {
	      data[n] = 0;
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
	cerr << "[TBB_DipoleDataset::readData]"
	     << " Error reading data from file into buffer!"
	     << endl;
	status = false;
      }
      /* release HDF5 handlers */
      h5error = H5Sclose (dataspaceID);
      h5error = H5Sclose (memspaceID);
    }
    else {
      cerr << "[TBB_DipoleDataset::readData]"
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
  
#ifdef DAL_WITH_CASA

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
                                "ANTENNA_POSITION_VALUE",
                                "ANTENNA_POSITION_UNIT",
                                "ANTENNA_POSITION_FRAME");
  }

  //_____________________________________________________________________________
  //                                                         set_antenna_position

  /*!
    \param pos -- The antenna position as a casa::Measure
  */
  bool TBB_DipoleDataset::set_antenna_position (casa::MPosition &pos)
  {
    if (location_p > 0) {
      std::vector<double> value;
      std::string unit = pos.getUnit().getName();
      std::string frame (pos.getRefString());

      // Use internal unit of meters if none given
      if (unit == "") unit = "m";

      // Append x,y,z positions to vector
      value.push_back(static_cast<double>(pos.getValue()(0)));
      value.push_back(static_cast<double>(pos.getValue()(1)));
      value.push_back(static_cast<double>(pos.getValue()(2)));

      HDF5Attribute::write (location_p,"ANTENNA_POSITION_VALUE", value);
      HDF5Attribute::write (location_p,"ANTENNA_POSITION_UNIT", std::vector<std::string>(3, unit));
      HDF5Attribute::write (location_p,"ANTENNA_POSITION_FRAME", frame);
    } else {
#ifdef DAL_DEBUGGING_MESSAGES
      std::cerr << "[TBB_DipoleDataset::set_antenna_position] Failed to write to group." << std::endl;
#endif

      return false;
    }

    return true;
  }

  //_____________________________________________________________________________
  //                                                             sample_frequency
  
  /*!
    \retval freq   -- casa::Quantity returning the value of the frequency.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool TBB_DipoleDataset::sample_frequency (casa::Quantity &freq)
  { 
    if (location_p > 0) {
      freq = DAL::h5get_quantity (location_p,
				  "SAMPLE_FREQUENCY_VALUE",
				  "SAMPLE_FREQUENCY_UNIT");
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
  
  /*!
    \retval freq   -- casa::MFrequency returning the value of the frequency.
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool TBB_DipoleDataset::sample_frequency (casa::MFrequency &freq)
  {
    if (location_p > 0) {
      casa::Quantity qFreq = DAL::h5get_quantity (location_p,
						  "SAMPLE_FREQUENCY_VALUE",
						  "SAMPLE_FREQUENCY_UNIT");
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
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
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
  //                                                                getAttributes

  /*!
    \retval record -- A casa::Record container holding the values of the
            attributes attached to the dataset for this dipole
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool TBB_DipoleDataset::getAttributes (casa::Record &rec)
  {
    bool status (true);
    
    try {
      uint station_id(0);
      uint rsp_id(0);
      uint rcu_id(0);
      uint nyquist_zone(0);
      uint time(0);
      uint sampleNumber(0);
      uint samplesPerFrame(0);
      uint dataLength(0);
      casa::String feed;
      casa::Vector<double>       antennaPositionValue;
      casa::Vector<casa::String> antennaPositionUnit;
      casa::Vector<casa::String> antennaPositionFrame;
      casa::Vector<double>       antennaOrientationValue;
      casa::Vector<casa::String> antennaOrientationUnit;
      casa::String               antennaOrientationFrame;

      // Retrieve attribute values
      getAttribute ("STATION_ID",                station_id);
      getAttribute ("RSP_ID",                    rsp_id);
      getAttribute ("RCU_ID",                    rcu_id);
      getAttribute ("NYQUIST_ZONE",              nyquist_zone);
      getAttribute ("TIME",                      time);
      getAttribute ("SAMPLE_NUMBER",             sampleNumber);
      getAttribute ("SAMPLES_PER_FRAME",         samplesPerFrame);
      getAttribute ("FEED",                      feed);
      getAttribute ("ANTENNA_POSITION_VALUE",    antennaPositionValue);
      // getAttribute ("ANTENNA_POSITION_UNIT",     antennaPositionUnit);
      getAttribute ("ANTENNA_POSITION_FRAME",    antennaPositionFrame);
      getAttribute ("ANTENNA_ORIENTATION_VALUE", antennaOrientationValue);
      // getAttribute ("ANTENNA_ORIENTATION_UNIT",  antennaOrientationUnit);
      getAttribute ("ANTENNA_ORIENTATION_FRAME", antennaOrientationFrame);
      
      // Fill record
      rec.define("STATION_ID",                station_id              );
      rec.define("RSP_ID",                    rsp_id                  );
      rec.define("RCU_ID",                    rcu_id                  );
      rec.define("TIME",                      time                    );
      rec.define("NYQUIST_ZONE",              nyquist_zone            );
      rec.define("SAMPLE_NUMBER",             sampleNumber            );
      rec.define("SAMPLES_PER_FRAME",         samplesPerFrame         );
      rec.define("DATA_LENGTH",               dataLength              );
      rec.define("FEED",                      feed                    );
      rec.define("ANTENNA_POSITION_VALUE",    antennaPositionValue    );
      rec.define("ANTENNA_POSITION_UNIT",     antennaPositionUnit     );
      rec.define("ANTENNA_POSITION_FRAME",    antennaPositionFrame(0) );
      rec.define("ANTENNA_ORIENTATION_VALUE", antennaOrientationValue );
      rec.define("ANTENNA_ORIENTATION_UNIT",  antennaOrientationUnit  );
      rec.define("ANTENNA_ORIENTATION_FRAME", antennaOrientationFrame );
    }
    catch (casa::AipsError x) {
      cerr << "[TBB_DipoleDataset::getAttributes] "
	   << "Error filling the record with attribute values!\n"
	   << x.getMesg() << endl;
      status = false;
    }
    
    return status;
  }

  //_____________________________________________________________________________
  //                                                                           readData
  
  /*!
    \param start      -- Number of the sample at which to start reading
    \param nofSamples -- Number of samples to read, starting from the position
           given by <tt>start</tt>.

    \return readData -- [nofSamples] Vector of raw ADC samples representing the
            electric field strength as function of time.
  */
  casa::Vector<double> TBB_DipoleDataset::readData (int const &start,
						    int const &nofSamples)
  {
    if (location_p > 0) {
      bool status   = true;
      short *buffer = new short [nofSamples];
      
      //______________________________________________________________
      // Retrieve the data from the file
      
      status = readData (start,
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
