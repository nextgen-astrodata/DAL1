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
  
  // ---------------------------------------------------------- BeamFormed
  
  BeamFormed::BeamFormed()
  {
    filename_p = "";
    H5fileID_p = 0;
    beamGroups_p.clear();
  };
  
  // ---------------------------------------------------------- BeamFormed
  
  /*!
    \param filename -- Name of the file from which to read in the data
  */
  BeamFormed::BeamFormed(std::string const &filename)
  {
    filename_p = "";
    H5fileID_p = 0;
    beamGroups_p.clear();
    
    filename_p = filename;
    
    init();
  }

  //_____________________________________________________________________________
  // BeamFormed

  BeamFormed::BeamFormed (CommonAttributes const &commonAttributes)
  {
    commonAttributes_p = commonAttributes;
    H5fileID_p = 0;
    beamGroups_p.clear();
    
    init();
  }
  
  // ----------------------------------------------------------------------- init
  
  /*!
    \brief Object initialization method
    
    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool BeamFormed::init ()
  {
    bool status (true);

    // Initialize interal variables

    dataset_p  = NULL;
    status     = true;
    
    // Connection to dataset via DAL layer
    
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
  
  // ---------------------------------------------------------------- ~BeamFormed
  
  BeamFormed::~BeamFormed()
  {
    for (uint beam(0); beam<beamGroups_p.size(); beam++) {
      delete beamGroups_p[beam];
    }
    
    if ( NULL != dataset_p ) {
      dataset_p->close();
      delete dataset_p;
      dataset_p = NULL;
    }
  }
  
  // ---------------------------------------------------------- h5get_str_array_attr
  
  /*!
    \brief Get a list of values for a string array attribute
    \param attrname       -- attribute name [I]
    \param obj_id         -- hdf5 object identifier [I]
  */
  std::vector< std::string >
  BeamFormed::h5get_str_array_attr( std::string attrname,
                                    hid_t obj_id )
  {
    status = true;
    hid_t attribute_id (0);
    std::vector<std::string> lcl_sources;
    attribute_id = H5Aopen_name( obj_id, attrname.c_str() );

    bool status (true);
    std::vector<uint> shape;

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
  
  // ---------------------------------------------------------- summary
  
  /*!
    \brief Provide a summary of the internal status
    \param os              -- output stream [I]
    \param listBeams       -- flag to determine whether or not to list beam
           summaries [I]
  */
  void BeamFormed::summary (std::ostream &os, bool const &listBeams)
  {
    os << "\n[BeamFormed Data] Summary of object properties"     << endl;

    os << "-- Status ............... : " << status     << endl;
    os << "-- Filename ............. : " << filename_p << endl;
    os << "-- HDF5 file ID ......... : " << H5fileID_p << endl;

    if (dataset_p != NULL)
      {
        std::vector< std::string > srcs = sources();
        std::vector< int > temps        = station_temperatures();

        os << "-- Telesope ............. : " << telescope()         << endl;
        os << "-- Number of Stations ... : " << nofStations()       << endl;
        os << "-- Datatype ............. : " << datatype()          << endl;
        os << "-- Emband   ............. : " << emband()            << endl;

        os << "-- Source(s) ............ : " << srcs                << endl;

        os << "-- Observation Id ....... : " << observation_id()        << endl;
        os << "-- Project Id ........... : " << proj_id()               << endl;

        os << "-- Point RA ............. : " << point_ra()              << endl;
        os << "-- Point DEC ............ : " << point_dec()             << endl;
        os << "-- Observer ............. : " << observer()              << endl;
        os << "-- Epoch MJD ............ : " << epoch_mjd()             << endl;
        os << "-- Epoch Date ........... : " << epoch_date()            << endl;
        os << "-- Epoch UTC ............ : " << epoch_utc()             << endl;
        os << "-- Epoch LST ............ : " << epoch_lst()             << endl;
        os << "-- FWHM of the main beam  : " << main_beam_diam()        << endl;
        os << "-- Bandwidth ............ : " << bandwidth()             << endl;

        os << "-- Breaks in the data ... : " << breaks()                << endl;
        os << "-- Dispersion measure ... : " << dispersion_measure()    << endl;
        os << "-- Number of time samples : " << number_of_samples()     << endl;
        os << "-- Sampling time (Hz).... : " << sampling_time()         << endl;
        os << "-- Notes ................ : " << notes()                 << endl;
        os << "-- Number of beams ...... : " << number_of_beams()       << endl;
        os << "-- FWHM of the sub-beams  : " << sub_beam_diameter()     << endl;
        os << "-- Weather temperature .. : " << weather_temperature()   << endl;
        os << "-- Weather humidity ..... : " << weather_humidity()      << endl;
        os << "-- Station temperature(s) : " << temps                   << endl;

        if (listBeams) {
	  for (uint beam(0); beam<beamGroups_p.size(); beam++) {
	    beamGroups_p[beam]->summary();
	  }
	}
      }
    
  }
  
  // ---------------------------------------------------------- getBeam
  
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

  // ---------------------------------------------------------- beams

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

  //_____________________________________________________________________________
  //                                                          setCommonAttributes

  bool BeamFormed::setCommonAttributes (CommonAttributes const &attributes)
  {
    bool status (true);

    commonAttributes_p = attributes;

    return status;
  }
  
  // ---------------------------------------------------------- filename

  /*!
    \brief Get the name of the data file
    \return filename - The name of the data file
  */
  std::string BeamFormed::filename ()
  {
    std::string filename ("");

    if ( dataset_p->getName() != "UNDEFINED" ) {
      if ( DAL::FAIL == dataset_p->getAttribute( "FILENAME", filename ) )
	{
	  std::cerr << "-- Error extracting attribute FILENAME\n";
	  filename = "";
	}
    }
    
    return filename;
  }
  
  // ---------------------------------------------------------- telescope
  
  /*!
    \return telescope -- The name of the telescope with which the data were
            recorded; returns an empty string in case no keyword value could
            be extracted.
  */
  std::string BeamFormed::telescope ()
  {
    std::string telescope ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL ==  dataset_p->getAttribute( "TELESCOPE", telescope ) )
          {
            std::cerr << "-- Error extracting attribute TELESCOPE\n";
            telescope = "";
          }
      }
    return telescope;
  }

  // ---------------------------------------------------------------- nofStations

  /*!
    \return nstations - the number of stations in the file
  */
  int BeamFormed::nofStations ()
  {
    int nstations = -1;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "NUMBER_OF_STATIONS",
             nstations ) )
          {
            std::cerr << "-- Error extracting attribute NUMBER_OF_STATIONS\n";
          }
      }
    return nstations;
  }

  // ---------------------------------------------------------- datatype

  /*!
    \return datatype - the datatype of the observation
  */
  std::string BeamFormed::datatype ()
  {
    std::string datatype ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL ==  dataset_p->getAttribute( "DATATYPE", datatype ) )
          {
            std::cerr << "-- Error extracting attribute DATATYPE\n";
          }
      }
    return datatype;
  }

  // ---------------------------------------------------------- emband

  /*!
    \brief Get the emband
    \return emband
  */
  std::string BeamFormed::emband ()
  {
    std::string emband ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "EMBAND", emband ) )
          {
            std::cerr << "-- Error extracting attribute EMBAND\n";
          }
      }
    return emband;
  }

  // ---------------------------------------------------------- sources

  /*!
    \return source list - a vector of source ids
  */
  std::vector<std::string> BeamFormed::sources()
  {
    return h5get_str_array_attr( "SOURCE", H5fileID_p );
  }

  // ---------------------------------------------------------- notes

  /*!
    \return notes - a string of notes from associated with the observation
  */
  std::string BeamFormed::notes()
  {
    std::string notes ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "NOTES", notes ) )
          {
            std::cerr << "-- Error extracting attribute NOTES\n";
          }
      }
    return notes;
  }

  // ---------------------------------------------------------- observation_id

  /*!
    \return obs_id - the observation identifier
  */
  std::string BeamFormed::observation_id ()
  {
    std::string observation_id ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "OBSERVATION_ID",
             observation_id ) )
          {
            std::cerr << "-- Error extracting attribute OBSERVATION_ID\n";
          }
      }
    return observation_id;
  }

  // ---------------------------------------------------------- proj_id

  /*!
    \brief Get the project identifier
    \return proj_id - the project identifier
  */
  std::string BeamFormed::proj_id ()
  {
    std::string proj_id ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "PROJ_ID", proj_id ) )
          {
            std::cerr << "-- Error extracting attribute PROJ_ID\n";
          }
      }
    return proj_id;
  }

  // ---------------------------------------------------------- pointing_ra

  /*!
    \return ra - the RA pointing direction
  */
  std::string BeamFormed::point_ra ()
  {
    std::string point_ra ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "POINT_RA", point_ra ) )
          {
            std::cerr << "-- Error extracting attribute POINT_RA\n";
          }
      }
    return point_ra;
  }


  // ---------------------------------------------------------- pointing_dec

  /*!
    \return dec - the dec pointing direction
  */
  std::string BeamFormed::point_dec ()
  {
    std::string point_dec ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "POINT_DEC", point_dec ) )
          {
            std::cerr << "-- Error extracting attribute POINT_DEC\n";
          }
      }
    return point_dec;
  }


  // ---------------------------------------------------------- observer

  /*!
    \return observer -- The name of the observer; returns an empty string in
            case no keyword value could be extracted.
  */
  std::string BeamFormed::observer ()
  {
    std::string observer ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "OBSERVER", observer ) )
          {
            std::cerr << "-- Error extracting attribute OBSERVER\n";
          }
      }
    return observer;
  }

  // ---------------------------------------------------------- epoch_mjd


  /*!
    \brief Get the epoch mjd
    \return epoch_mjd
  */
  std::string BeamFormed::epoch_mjd ()
  {
    std::string epoch_mjd ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "EPOCH_MJD", epoch_mjd ) )
          {
            std::cerr << "-- Error extracting attribute EPOCH_MJD\n";
          }
      }
    return epoch_mjd;
  }


  // ---------------------------------------------------------- epoch_date

  /*!
    \brief Get the epoch date
    \return epoch_date
  */
  std::string BeamFormed::epoch_date ()
  {
    std::string epoch_date ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "EPOCH_DATE", epoch_date ) )
          {
            std::cerr << "-- Error extracting attribute EPOCH_DATE\n";
          }
      }
    return epoch_date;
  }


  // ---------------------------------------------------------- epoch_utc

  /*!
    \brief Get the epoch utc
    \return epoch utc
  */
  std::string BeamFormed::epoch_utc ()
  {
    std::string epoch_utc ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "EPOCH_UTC", epoch_utc ) )
          {
            std::cerr << "-- Error extracting attribute EPOCH_UTC\n";
          }
      }
    return epoch_utc;
  }

  // ---------------------------------------------------------- epoch_lst

  /*!
    \brief Get the epoch lst
    \return epoch lst
  */
  std::string BeamFormed::epoch_lst ()
  {
    std::string epoch_lst ("");

    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "EPOCH_LST", epoch_lst ) )
          {
            std::cerr << "-- Error extracting attribute EPOCH_LST\n";
          }
      }
    return epoch_lst;
  }


  // ---------------------------------------------------------- main_beam_diam

  /*!
    \brief Get the main beam diameter
    \return main beam diameter
  */
  int BeamFormed::main_beam_diam ()
  {
    int main_beam_diam = -1;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "MAIN_BEAM_DIAM",
             main_beam_diam ) )
          {
            std::cerr << "-- Error extracting attribute MAIN_BEAM_DIAM\n";
          }
      }
    return main_beam_diam;
  }


  // ---------------------------------------------------------- center_freq

  /*!
    \return center frequency
  */
  int BeamFormed::center_freq ()
  {
    int center_freq = -1;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "CENTER_FREQUENCY",
             center_freq ) )
          {
            std::cerr << "-- Error extracting attribute CENTER_FREQUENCY\n";
          }
      }
    return center_freq;
  }


  // ---------------------------------------------------------- bandwidth

  /*!
    \brief Get the bandwidth
    \return bandwidth
  */
  int BeamFormed::bandwidth ()
  {
    int bandwidth = -1;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "BANDWIDTH", bandwidth ) )
          {
            std::cerr << "-- Error extracting attribute BANDWIDTH\n";
          }
      }
    return bandwidth;
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
    int breaks = -1;
    if (dataset_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == dataset_p->getAttribute( "BREAKS_IN_DATA", breaks ) )
          {
            std::cerr << "-- Error extracting attribute BREAKS_IN_DATA\n";
          }
      }
    return breaks;
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
    try
      {
        status = h5get_attribute( H5fileID_p, "TSYS", station_temperatures );
      }
    catch (std::string message)
      {
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
                                     std::vector<complex<short> > &data)
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
                                     std::vector<complex<short> > &data)
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
