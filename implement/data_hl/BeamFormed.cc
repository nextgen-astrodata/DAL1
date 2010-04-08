/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
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
#ifndef BEAMFORMED_H
#include "BeamFormed.h"
#endif

namespace DAL {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                   BeamFormed
  
  BeamFormed::BeamFormed()
  {
    init ();
  };
  
  //_____________________________________________________________________________
  //                                                                   BeamFormed
  
  /*!
    \param filename -- Name of the file from which to read in the data
  */
  BeamFormed::BeamFormed(std::string const &filename)
  {
    init ();
    init (filename);
  }

  //_____________________________________________________________________________
  //                                                                   BeamFormed
  
  BeamFormed::BeamFormed (CommonAttributes const &commonAttributes)
  {
    init ();
    init (commonAttributes);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                  ~BeamFormed
  
  BeamFormed::~BeamFormed()
  {
    // Clear beam groups _________________________

    std::cout << "[BeamFormed] Clearing beam groups ..." << std::endl << std::flush;

    if (!beamGroups_p.empty()) {
      for (uint beam(0); beam<beamGroups_p.size(); beam++) {
	delete beamGroups_p[beam];
      }
    }

    // Clear dalDataset __________________________
    
    std::cout << "[BeamFormed] Clearing dalDataset ..." << std::endl << std::flush;

    if ( dataset_p != NULL && H5Iis_valid(dataset_p->getId()) ) {
      std::cout << "-- closing dataset ..." << std::endl << std::flush;
      dataset_p->close();
      std::cout << "-- deleting pointer ..." << std::endl << std::flush;
      delete dataset_p;
      std::cout << "-- zero pointer ..." << std::endl << std::flush;
      dataset_p = NULL;
    }

    std::cout << "[BeamFormed] Done." << std::endl << std::flush;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                         init
  
  bool BeamFormed::init ()
  {
    status             = false;
    commonAttributes_p = CommonAttributes();
    filename_p         = "";
    H5fileID_p         = 0;
    dataset_p          = NULL;
    beamGroups_p.clear();

    return true;
  }
  
  //_____________________________________________________________________________
  //                                                                         init

  bool BeamFormed::init (CommonAttributes const &commonAttributes)
  {
    commonAttributes_p = CommonAttributes();

    return init (commonAttributes.filename());
  }
  
  //_____________________________________________________________________________
  //                                                                         init
  
  /*!
    \brief Object initialization method
    
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool BeamFormed::init (std::string const &filename)
  {
    // Store filename ____________________________

    filename_p = filename;

    // Open file through dalDataset ______________
    
    dataset_p = new dalDataset;
    if ( DAL::FAIL == dataset_p->open( filename_p.c_str() ) ) {
      std::cerr << "[BeamFormed::init] Error opening file into dalDataset!"
		<< std::endl;
      return false;
    }
    
    H5fileID_p = dataset_p->getFileHandle();
    
    /*
      [1] If opening the data file was successful, we can continue. First we scan
      for the beam groups available in the file, since this is the basic unit
      within which the actual data for the individual sub-bands are stored.
    */
    std::vector<std::string> beamGroups = beams();
    
    /* Always check if actually a list of groups has been extracted */
    if ( beamGroups.size() > 0 ) {
      for (uint beam(0); beam<beamGroups.size(); beam++) {
	// assemble internal list of beam groups
	BeamGroup * group = new BeamGroup((*dataset_p), beamGroups[beam]);
	beamGroups_p.push_back(group);
      }
    }
    else {
      std::cerr << "[BeamFormed::init] No beam group found in data set!"
		<< std::endl;
      status = false;
    }
    
    return status;
  }
  
  //_____________________________________________________________________________
  //                                                         h5get_str_array_attr
  
  /*!
    \brief Get a list of values for a string array attribute
    \param attrname       -- attribute name [I]
    \param obj_id         -- hdf5 object identifier [I]
  */
  std::vector< std::string >
  BeamFormed::h5get_str_array_attr (std::string attrname,
                                    hid_t obj_id )
  {
    status = true;
    hid_t attribute_id (0);
    std::vector<std::string> lcl_sources;
    attribute_id = H5Aopen_name( obj_id, attrname.c_str() );

    bool status (true);
    std::vector<hsize_t> shape;

    // get the shape of the dataspace
    status = h5get_dataspace_shape (attribute_id,shape);

    if (shape.size() == 1) {
      // additional local variables
      hid_t datatype_id  = H5Aget_type (attribute_id);
      hsize_t dims[1] = { shape[0] };
      
      char **data_in;
      
      /* How many strings are in the string array? */
      if (!(data_in = (char**)malloc(dims[0] * sizeof(char *))))
	cerr << "ERROR! malloc " << attrname << endl;
      
      /* Now read the array of strings. The HDF5 library will allocate
       * space for each string. */
      if ( H5Aread( attribute_id, datatype_id, data_in ) < 0)
	cerr << "ERROR! h5aread "  << attrname << endl;
      
      for (uint ii=0; ii<shape[0]; ii++)
	lcl_sources.push_back( data_in[ii] );
      
      for (uint ii=0; ii<shape[0]; ii++)
	free( data_in[ii] );
      
      free( data_in );
      
      /* Close HDF5 stuff. */
      if (H5Aclose(attribute_id) < 0)
	cerr << "ERROR! h5aclose " << attrname << endl;
      if (H5Tclose(datatype_id) < 0)
	cerr << "ERROR! h5tclose " << attrname << endl;
      
    }
    else {
      cerr << "[dataset_p->getAttribute] Wrong shape of attribute dataspace!"
	   << std::endl;
      status = false;
    }
    
    return lcl_sources;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \brief Provide a summary of the internal status
    \param os              -- output stream [I]
    \param listBeams       -- flag to determine whether or not to list beam
           summaries [I]
  */
  void BeamFormed::summary (std::ostream &os,
			    bool const &listBeams)
  {
    os << "[BeamFormed] Summary of object properties"     << endl;

    os << "-- Status ............... : " << status     << endl;
    os << "-- Filename ............. : " << filename_p << endl;
    os << "-- HDF5 file ID ......... : " << H5fileID_p << endl;

    if (H5Iis_valid(H5fileID_p) && H5Iis_valid(dataset_p->getId())) {
      std::vector< std::string > srcs = sources();
      std::vector< int > temps        = station_temperatures();
      
      os << "-- Telesope ............. : " << telescope()            << endl;
      os << "-- Number of Stations ... : " << nofStations()          << endl;
      os << "-- Datatype ............. : " << datatype()             << endl;
      os << "-- EM-band  ............. : " << emband()               << endl;
      os << "-- Source(s) ............ : " << srcs                   << endl;
      os << "-- Observation Id ....... : " << observation_id()       << endl;
      os << "-- Project Id ........... : " << proj_id()              << endl;
      os << "-- Point RA ............. : " << point_ra()             << endl;
      os << "-- Point DEC ............ : " << point_dec()            << endl;
      os << "-- Observer ............. : " << observer()             << endl;
      os << "-- Epoch MJD ............ : " << epoch_mjd()            << endl;
      os << "-- Epoch Date ........... : " << epoch_date()           << endl;
      os << "-- Epoch UTC ............ : " << epoch_utc()            << endl;
      os << "-- Epoch LST ............ : " << epoch_lst()            << endl;
      os << "-- FWHM of the main beam  : " << main_beam_diam()       << endl;
      os << "-- Bandwidth ............ : " << bandwidth()            << endl;
      os << "-- Breaks in the data ... : " << breaks()               << endl;
      os << "-- Dispersion measure ... : " << dispersion_measure()   << endl;
      os << "-- Number of time samples : " << number_of_samples()    << endl;
      os << "-- Sampling time (Hz).... : " << sampling_time()        << endl;
      os << "-- Notes ................ : " << notes()                << endl;
      os << "-- Number of beams ...... : " << number_of_beams()      << endl;
      os << "-- FWHM of the sub-beams  : " << sub_beam_diameter()    << endl;
      os << "-- Weather temperature .. : " << weather_temperature()  << endl;
      os << "-- Weather humidity ..... : " << weather_humidity()     << endl;
      os << "-- Station temperature(s) : " << temps                  << endl;
      
      if (listBeams) {
	for (uint beam(0); beam<beamGroups_p.size(); beam++) {
	  beamGroups_p[beam]->summary();
	}
      }
    } else {
      os << "-- Telesope ............. : UNDEFINED" << endl;
      os << "-- nof stations ......... : UNDEFINED" << endl;
      os << "-- Datatype ............. : UNDEFINED" << endl;
      os << "-- EM-band .............. : UNDEFINED" << endl;
    }
    
  }
  
  //_____________________________________________________________________________
  //                                                                      getBeam
  
  /*!
    \brief Get a beam group object
    \param beam       -- beam number [I]
    \return beam object - a BeamGroup object representing the requested beam
  */
  BeamGroup * BeamFormed::getBeam( int beam )
  {
    std::vector<std::string> beamGroups = beams();
    BeamGroup * group = new BeamGroup((*dataset_p), beamGroups[beam]);
    return group;
  }

  //_____________________________________________________________________________
  //                                                                        beams
  
  /*!
    \brief Print beam groups embedded within the dataset
    \return beams - a vector of strings representing the names of the beams
  */
  std::vector<std::string> BeamFormed::beams()
  {
    char * beamstr = new char[10];
    std::vector<std::string> beamGroups;
    dalGroup * beamgroup = NULL;
    std::vector< std::string > lcl_beams = dataset_p->getGroupNames();

    for (uint beam=0; beam<lcl_beams.size(); beam++) {
      
      /* create ID string for the group */
      sprintf( beamstr, "beam%03d", beam );
      
      beamgroup = dataset_p->openGroup(beamstr);
      
      if ( beamgroup ) {
	beamGroups.push_back(beamstr);
	delete beamgroup;
      }
    }
    
    delete [] beamstr;
    beamstr = NULL;
    
    return beamGroups;
  }

  // ============================================================================
  //
  //  Access to attributes
  //
  // ============================================================================

  std::string BeamFormed::getAttribute (std::string const &name)
  {
    std::string val = "";
    hid_t location  = dataset_p->getId();
    
    if (H5Iis_valid(location)) {
      if (h5get_attribute (location,name,val)) {
	return val;
      } else {
	std::cerr << "[BeamFormed] Error extracting attribute " << name
		  << std::endl;
      }
    }
    
    return val;
  }
  
  //_____________________________________________________________________________
  //                                                          setCommonAttributes

  bool BeamFormed::setCommonAttributes (CommonAttributes const &attributes)
  {
    bool status (true);

    commonAttributes_p = attributes;

    return status;
  }
  
  //_____________________________________________________________________________
  //                                                                     filename

  /*!
    \return filename - The name of the data file
  */
  std::string BeamFormed::filename ()
  {
    return getAttribute ("FILENAME");
  }
  
  //_____________________________________________________________________________
  //                                                                    telescope

  /*!
    \return telescope -- The name of the telescope with which the data were
            recorded; returns an empty string in case no keyword value could
            be extracted.
  */
  std::string BeamFormed::telescope ()
  {
    return getAttribute ("TELESCOPE");
  }

  //_____________________________________________________________________________
  //                                                                  nofStations

  /*!
    \return nstations - The number of stations in the file
  */
  int BeamFormed::nofStations ()
  {
    std::string name ("NUMBER_OF_STATIONS");
    int val (-1);
    hid_t location = dataset_p->getId();
    bool status    = h5get_attribute (location,name,val);
    
    if (!status) {
      std::cerr << "[BeamFormed] Error extracting attribute " << name
		<< std::endl;
      val = -1;
    }
    
    return val;
  }
  
  //_____________________________________________________________________________
  //                                                                     datatype

  /*!
    \return datatype - the datatype of the observation
  */
  std::string BeamFormed::datatype ()
  {
    return getAttribute ("DATATYPE");
  }

  //_____________________________________________________________________________
  //                                                                       emband

  /*!
    \return emband -- Selected EM-band
  */
  std::string BeamFormed::emband ()
  {
    return getAttribute ("EMBAND");
  }

  //_____________________________________________________________________________
  //                                                                      sources

  /*!
    \return source list - a vector of source ids
  */
  std::vector<std::string> BeamFormed::sources()
  {
    return h5get_str_array_attr( "SOURCE", H5fileID_p );
  }

  //_____________________________________________________________________________
  //                                                                        notes

  /*!
    \return notes - a string of notes from associated with the observation
  */
  std::string BeamFormed::notes()
  {
    return getAttribute ("NOTES");
  }

  //_____________________________________________________________________________
  //                                                               observation_id
  
  /*!
    \return obs_id - the observation identifier
  */
  std::string BeamFormed::observation_id ()
  {
    return getAttribute ("OBSERVATION_ID");
  }

  //_____________________________________________________________________________
  //                                                                      proj_id
  
  /*!
    \brief Get the project identifier
    \return proj_id - the project identifier
  */
  std::string BeamFormed::proj_id ()
  {
    return getAttribute ("PROJ_ID");
  }

  //_____________________________________________________________________________
  //                                                                     point_ra
  
  /*!
    \return ra - the RA pointing direction
  */
  std::string BeamFormed::point_ra ()
  {
    return getAttribute ("POINT_RA");
  }

  //_____________________________________________________________________________
  //                                                                    point_dec
  
  /*!
    \return dec - the dec pointing direction
  */
  std::string BeamFormed::point_dec ()
  {
    return getAttribute ("POINT_DEC");
  }

  //_____________________________________________________________________________
  //                                                                     observer
  
  /*!
    \return observer -- The name of the observer; returns an empty string in
            case no keyword value could be extracted.
  */
  std::string BeamFormed::observer ()
  {
    return getAttribute ("OBSERVER");
  }

  //_____________________________________________________________________________
  //                                                                    epoch_mjd
  
  /*!
    \brief Get the epoch mjd
    \return epoch_mjd
  */
  std::string BeamFormed::epoch_mjd ()
  {
    return getAttribute ("EPOCH_MJD");
  }

  //_____________________________________________________________________________
  //                                                                   epoch_date
  
  /*!
    \brief Get the epoch date
    \return epoch_date
  */
  std::string BeamFormed::epoch_date ()
  {
    return getAttribute ("EPOCH_DATE");
  }

  //_____________________________________________________________________________
  //                                                                    epoch_utc
  
  /*!
    \brief Get the epoch utc
    \return epoch utc
  */
  std::string BeamFormed::epoch_utc ()
  {
    return getAttribute ("EPOCH_UTC");
  }

  //_____________________________________________________________________________
  //                                                                    epoch_lst
  
  /*!
    \brief Get the epoch lst
    \return epoch lst
  */
  std::string BeamFormed::epoch_lst ()
  {
    return getAttribute ("EPOCH_LST");
  }

  //_____________________________________________________________________________
  //                                                               main_beam_diam
  
  /*!
    \brief Get the main beam diameter
    \return main beam diameter
  */
  int BeamFormed::main_beam_diam ()
  {
    std::string name ("MAIN_BEAM_DIAM");
    int val (-1);
    hid_t location = dataset_p->getId();
    bool status    = h5get_attribute (location,name,val);
    
    if (!status) {
      std::cerr << "[BeamFormed] Error extracting attribute " << name
		<< std::endl;
      val = -1;
    }
    
    return val;
  }

  //_____________________________________________________________________________
  //                                                                  center_freq
  
  /*!
    \return center frequency
  */
  int BeamFormed::center_freq ()
  {
    std::string name ("CENTER_FREQUENCY");
    int val (-1);
    hid_t location = dataset_p->getId();
    bool status    = h5get_attribute (location,name,val);
    
    if (!status) {
      std::cerr << "[BeamFormed] Error extracting attribute " << name
		<< std::endl;
      val = -1;
    }
    
    return val;
  }

  //_____________________________________________________________________________
  //                                                                    bandwidth
  
  /*!
    \brief Get the bandwidth
    \return bandwidth
  */
  int BeamFormed::bandwidth ()
  {
    std::string name ("BANDWIDTH");
    int val (-1);
    hid_t location = dataset_p->getId();
    bool status    = h5get_attribute (location,name,val);
    
    if (!status) {
      std::cerr << "[BeamFormed] Error extracting attribute " << name
		<< std::endl;
      val = -1;
    }
    
    return val;
  }


  // ------------------------------------------------- total_integration_time

  /*!
    \brief Get the integration time
    \return total integration time
  */
  double BeamFormed::total_integration_time ()
  {
    double integration_time = 0.0;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "TOTAL_INTEGRATION_TIME",
             integration_time ) )
          {
            std::cerr << "-- Error extracting attribute TOTAL_INTEGRATION_TIME\n";
          }
      }
    return integration_time;
  }


  // ---------------------------------------------------------- breaks

  /*!
    \brief Get the number of breaks
    \return number of breaks
  */
  int BeamFormed::breaks ()
  {
    std::string name ("BREAKS_IN_DATA");
    int val (-1);
    hid_t location = dataset_p->getId();
    bool status    = h5get_attribute (location,name,val);
    
    if (!status) {
      std::cerr << "[BeamFormed] Error extracting attribute " << name
		<< std::endl;
      val = -1;
    }
    
    return val;
  }


  // ---------------------------------------------------------- dispersion_measure

  /*!
    \return dispersion measure
  */
  int BeamFormed::dispersion_measure ()
  {
    int dispersion_measure = -1;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "DISPERSION_MEASURE",
             dispersion_measure ) )
          {
            std::cerr << "-- Error extracting attribute DISPERSION_MEASURE\n";
          }
      }
    return dispersion_measure;
  }


  // ---------------------------------------------------------- number_of_samples

  /*!
    \brief Get the number of samples
    \return number of samples
  */
  int BeamFormed::number_of_samples ()
  {
    int number_of_samples = -1;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "NUMBER_OF_SAMPLES",
             number_of_samples ) )
          {
            std::cerr << "-- Error extracting attribute NUMBER_OF_SAMPLES\n";
          }
      }
    return number_of_samples;
  }


  // ---------------------------------------------------------- sampling_time

  /*!
    \brief Get the sampling time
    \return sampling time
  */
  double BeamFormed::sampling_time ()
  {
    double sampling_time = -1.0;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "SAMPLING_TIME",
             sampling_time ) )
          {
            std::cerr << "-- Error extracting attribute SAMPLING_TIME\n";
          }
      }
    return sampling_time;
  }

  // ------------------------------------------------------------ number_of_beams


  /*!
    \brief Get the number of beams
    \return nofBeams -- The number of beams contained within this dataset
  */
  int BeamFormed::number_of_beams ()
  {
    int number_of_beams = -1;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "NUMBER_OF_BEAMS",
             number_of_beams ) )
          {
            std::cerr << "-- Error extracting attribute NUMBER_OF_BEAMS\n";
          }
      }
    return number_of_beams;
  }

  // ---------------------------------------------------------- sub_beam_diameter


  /*!
    \brief Get the sub beam diameter
    \return sub beam diameter
  */
  int BeamFormed::sub_beam_diameter ()
  {
    int sub_beam_diameter = -1;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "SUB_BEAM_DIAMETER",
             sub_beam_diameter ) )
          {
            std::cerr << "-- Error extracting attribute SUB_BEAM_DIAMETER\n";
          }
      }
    return sub_beam_diameter;
  }

  // -------------------------------------------------------- weather_temperature


  /*!
    \brief Get the weather temperature
    \return weather temperature
  */
  int BeamFormed::weather_temperature ()
  {
    int weather_temperature = -1;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "WEATHER_TEMPERATURE",
             weather_temperature ) )
          {
            std::cerr << "-- Error extracting attribute WEATHER_TEMPERATURE\n";
          }
      }
    return weather_temperature;
  }

  // ----------------------------------------------------------- weather_humidity


  /*!
    \brief Get the weather humidity
    \brief weather humidity
  */
  int BeamFormed::weather_humidity ()
  {
    int weather_humidity = -1;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "WEATHER_HUMIDITY",
             weather_humidity ) )
          {
            std::cerr << "-- Error extracting attribute WEATHER_HUMIDITY\n";
          }
      }
    return weather_humidity;
  }

  // ------------------------------------------------------- station_temperatures


  /*!
    \brief Get the station temperatures
    \return temperatures -- Vector with the temperatures for measured at the
            different stations.
  */
  std::vector< int > BeamFormed::station_temperatures ()
  {
    std::vector<int> station_temperatures;
    try {
      status = h5get_attribute( H5fileID_p, "TSYS", station_temperatures );
    }
    catch (std::string message) {
      cerr << message << endl;
    }
    return station_temperatures;
  }
  
  // ============================================================================
  //
  //  Access to the data stored in the sub-bands
  //
  // ============================================================================

  /*!
    \param beam    -- The number of the beam from which to retrieve the data
    \param subband -- The number of the sub-band from which to retrieve the data
    \param start   -- Start number of the cell in the column.
    \param length  -- The number of cells to retrieve.
    \retval values -- Vector with the extracted values
  */
  void BeamFormed::getSubbandData_X (int &beam,
                                     int &subband,
                                     int &start,
                                     int &length,
                                     std::vector<std::complex<short> > &data)
  {
    beamGroups_p[beam]->getSubbandData_X(subband,
                                         start,
                                         length,
                                         data);
  }

  /*!
    \param beam    -- The number of the beam from which to retrieve the data
    \param subband -- The number of the sub-band from which to retrieve the data
    \param start   -- Start number of the cell in the column.
    \param length  -- The number of cells to retrieve.
    \retval values -- Vector with the extracted values
  */
  void BeamFormed::getSubbandData_Y (int &beam,
                                     int &subband,
                                     int &start,
                                     int &length,
                                     std::vector<std::complex<short> > &data)
  {
    beamGroups_p[beam]->getSubbandData_Y(subband,
                                         start,
                                         length,
                                         data);
  }

  // ============================================================================
  //
  //  Boost wrappers to allow some previously defined functions to be easily
  //  called from a python prompt.
  //
  // ============================================================================

#ifdef PYTHON

  // ---------------------------------------------------------- summary_boost

  void BeamFormed::summary_boost()
  {
    summary();
  }

  // ---------------------------------------------------------- beams_boost

  bpl::list BeamFormed::beams_boost()
  {
    std::vector<std::string> beams_vec = beams();
    return vector2list( beams_vec );
  }

  // ---------------------------------------------------------- source_boost

  bpl::list BeamFormed::source_boost()
  {
    std::vector<std::string> source_vec = sources();
    return vector2list( source_vec );
  }
#endif

} // end namespace DAL
