/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
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

  BeamFormed::BeamFormed(){};

  BeamFormed::BeamFormed(std::string const &filename)
  {
    filename_p = filename;
    init();
  }

  BeamFormed::~BeamFormed()
  {
    dataset_p->close();
  }

  void BeamFormed::summary (std::ostream &os, bool const &listBeams)
  {
    os << "\n[BeamFormed Data] Summary of object properties"     << endl;

    os << "-- Filename ............. : " << filename()              << endl;
    os << "-- Telesope ............. : " << telescope()             << endl;
    os << "-- Number of Stations ... : " << nstations()             << endl;
    os << "-- Datatype ............. : " << datatype()              << endl;
    os << "-- Emband   ............. : " << emband()                << endl;
    os << "-- Source(s) ............ : " /*<< sources()*/               << endl;
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
    os << "-- Center Frequency ..... : " << center_freq()           << endl;
    os << "-- Bandwidth ............ : " << bandwidth()             << endl;
    os << "-- Total Integration Time : " << integration_time()      << endl;

    os << "-- Breaks in the data ... : " << breaks()                << endl;
    os << "-- Dispersion measure ... : " << dispersion_measure()    << endl;
    os << "-- Number of time samples : " << number_of_samples()     << endl;
    os << "-- Sampling time ........ : " << sampling_time()         << endl;
    os << "-- Notes ................ : " /*<< notes()*/                 << endl;
    os << "-- Number of beams ...... : " << number_of_beams()       << endl;
    os << "-- FWHM of the sub-beams  : " << sub_beam_diameter()     << endl;
    os << "-- Weather temperature .. : " << weather_temperature()   << endl;
    os << "-- Weather humidity ..... : " << weather_humidity()      << endl;
    os << "-- Station temperature(s) : " /*<< station_temperatures()*/  << endl;

    if (listBeams) {
      for (uint beam(0); beam<beamGroups_p.size(); beam++) {
	beamGroups_p[beam].summary();
      }
    }

  }

  BeamGroup * BeamFormed::getBeam( int beam )
  {
     return &beamGroups_p[ beam ];
  }

  std::vector<std::string> BeamFormed::beams()
  {
    unsigned int nofBeams (100);
    char stationstr[10];
    std::vector<std::string> beamGroups;

    for (unsigned int station(0); station<nofBeams; station++) {
      /* create ID string for the group */
      sprintf( stationstr, "beam%03d", station );
      /* Check if group of given name exists in the file; according to the
	 documentation of the DAL, the openGroup() function returns a NULL
	 pointer in case no group of the given name exists. */
      if (dataset_p->openGroup(stationstr) != NULL) {
	beamGroups.push_back(stationstr);
      }
    }

    return beamGroups;
  }
/*
  void BeamFormed::beams()
  {
     std::vector< std::string > lcl_beams = dataset_p->getGroupNames();
     if ( lcl_beams.size() > 0 )
     {
       cout << "\nBeams:" << endl;
       for(unsigned int bb=0; bb<lcl_beams.size(); bb++)
         cout << lcl_beams[bb] << endl;
       cout << endl;
     }
  }
*/
  std::string BeamFormed::filename ()
  {
    std::string attribute_filename ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * filename = reinterpret_cast<char*>(dataset_p->getAttribute("FILENAME"));
	attribute_filename = string(filename);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute FILENAME" << endl;
	attribute_filename = "";
      }
    }
   return attribute_filename;
  }

  std::string BeamFormed::telescope ()
  {
    std::string attribute_telescope ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * telescope = reinterpret_cast<char*>(dataset_p->getAttribute("TELESCOPE"));
	attribute_telescope = string(telescope);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute TELESCOPE" << endl;
	attribute_telescope = "";
      }
    }
   return attribute_telescope;
  }

  int BeamFormed::nstations ()
  {
    int nstations;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * nstations_p = reinterpret_cast<int*>(dataset_p->getAttribute("NUMBER_OF_STATIONS"));
	nstations = *nstations_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute NUMBER_OF_STATIONS" << endl;
	nstations = -1;
      }
    }
    return nstations;
  }

  std::string BeamFormed::observer ()
  {
    std::string attribute_observer ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * observer = reinterpret_cast<char*>(dataset_p->getAttribute("OBSERVER"));
	attribute_observer = string(observer);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute OBSERVER" << endl;
	attribute_observer = "";
      }
    }
   return attribute_observer;
  }

  std::string BeamFormed::datatype ()
  {
    std::string attribute_datatype ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * datatype = reinterpret_cast<char*>(dataset_p->getAttribute("DATATYPE"));
	attribute_datatype = string(datatype);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute DATATYPE" << endl;
	attribute_datatype = "";
      }
    }
   return attribute_datatype;
  }

  std::string BeamFormed::emband ()
  {
    std::string attribute_emband ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * emband = reinterpret_cast<char*>(dataset_p->getAttribute("EMBAND"));
	attribute_emband = string(emband);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute EMBAND" << endl;
	attribute_emband = "";
      }
    }
   return attribute_emband;
  }

  std::vector<std::string> BeamFormed::sources()
  {
    status = true;
    std::vector<std::string> sources;
/*    try {
      status = h5get_attribute ( sources,
				      attribute_name("SOURCE"),
				      H5fileID_p );
    } catch (std::string message) {
      cerr << message << endl;
    }*/
    return sources;
  }

  std::string BeamFormed::observation_id ()
  {
    std::string attribute_observation_id ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * observation_id = reinterpret_cast<char*>(dataset_p->getAttribute("OBSERVATION_ID"));
	attribute_observation_id = string(observation_id);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute OBSERVATION_ID" << endl;
	attribute_observation_id = "";
      }
    }
   return attribute_observation_id;
  }

  std::string BeamFormed::proj_id ()
  {
    std::string attribute_proj_id ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * proj_id = reinterpret_cast<char*>(dataset_p->getAttribute("PROJ_ID"));
	attribute_proj_id = string(proj_id);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute PROJ_ID" << endl;
	attribute_proj_id = "";
      }
    }
   return attribute_proj_id;
  }

  std::string BeamFormed::point_ra ()
  {
    std::string attribute_point_ra ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * point_ra = reinterpret_cast<char*>(dataset_p->getAttribute("POINT_RA"));
	attribute_point_ra = string(point_ra);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute POINT_RA" << endl;
	attribute_point_ra = "";
      }
    }
   return attribute_point_ra;
  }

  std::string BeamFormed::point_dec ()
  {
    std::string attribute_point_dec ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * point_dec = reinterpret_cast<char*>(dataset_p->getAttribute("POINT_DEC"));
	attribute_point_dec = string(point_dec);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute POINT_DEC" << endl;
	attribute_point_dec = "";
      }
    }
   return attribute_point_dec;
  }

  double BeamFormed::epoch_mjd ()
  {
    double epoch_mjd;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	double * epoch_mjd_p = reinterpret_cast<double*>(dataset_p->getAttribute("EPOCH_MJD"));
	epoch_mjd = *epoch_mjd_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute EPOCH_MJD" << endl;
	epoch_mjd = -1;
      }
    }
    return epoch_mjd;
  }

  std::string BeamFormed::epoch_date ()
  {
    std::string attribute_epoch_date ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * epoch_date = reinterpret_cast<char*>(dataset_p->getAttribute("EPOCH_DATE"));
	attribute_epoch_date = string(epoch_date);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute EPOCH_DATE" << endl;
	attribute_epoch_date = "";
      }
    }
   return attribute_epoch_date;
  }

  std::string BeamFormed::epoch_utc ()
  {
    std::string attribute_epoch_utc ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * epoch_utc = reinterpret_cast<char*>(dataset_p->getAttribute("EPOCH_UTC"));
	attribute_epoch_utc = string(epoch_utc);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute EPOCH_UTC" << endl;
	attribute_epoch_utc = "";
      }
    }
   return attribute_epoch_utc;
  }

  std::string BeamFormed::epoch_lst ()
  {
    std::string attribute_epoch_lst ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * epoch_lst = reinterpret_cast<char*>(dataset_p->getAttribute("EPOCH_LST"));
	attribute_epoch_lst = string(epoch_lst);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute EPOCH_LST" << endl;
	attribute_epoch_lst = "";
      }
    }
   return attribute_epoch_lst;
  }

  int BeamFormed::main_beam_diam ()
  {
    int main_beam_diam;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * main_beam_diam_p = reinterpret_cast<int*>(dataset_p->getAttribute("MAIN_BEAM_DIAM"));
	main_beam_diam = *main_beam_diam_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute MAIN_BEAM_DIAM" << endl;
	main_beam_diam = -1;
      }
    }
    return main_beam_diam;
  }

  int BeamFormed::center_freq ()
  {
    int center_freq;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * center_freq_p = reinterpret_cast<int*>(dataset_p->getAttribute("CENTER_FREQUENCY"));
	center_freq = *center_freq_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute CENTER_FREQUENCY" << endl;
	center_freq = -1;
      }
    }
    return center_freq;
  }

  int BeamFormed::bandwidth ()
  {
    int bandwidth;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * bandwidth_p = reinterpret_cast<int*>(dataset_p->getAttribute("BANDWIDTH"));
	bandwidth = *bandwidth_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute BANDWIDTH" << endl;
	bandwidth = -1;
      }
    }
    return bandwidth;
  }

  double BeamFormed::integration_time ()
  {
    double integration_time;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	double * integration_time_p = reinterpret_cast<double*>(dataset_p->getAttribute("TOTAL_INTEGRATION_TIME"));
	integration_time = *integration_time_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute TOTAL_INTEGRATION_TIME" << endl;
	integration_time = -1;
      }
    }
    return integration_time;
  }

  int BeamFormed::breaks ()
  {
    int breaks;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * breaks_p = reinterpret_cast<int*>(dataset_p->getAttribute("BREAKS_IN_DATA"));
	breaks = *breaks_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute BREAKS_IN_DATA" << endl;
	breaks = -1;
      }
    }
    return breaks;
  }

  int BeamFormed::dispersion_measure ()
  {
    int dispersion_measure;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * dispersion_measure_p = reinterpret_cast<int*>(dataset_p->getAttribute("DISPERSION_MEASURE"));
	dispersion_measure = *dispersion_measure_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute DISPERSION_MEASURE" << endl;
	dispersion_measure = -1;
      }
    }
    return dispersion_measure;
  }

  int BeamFormed::number_of_samples ()
  {
    int number_of_samples;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * number_of_samples_p = reinterpret_cast<int*>(dataset_p->getAttribute("NUMBER_OF_SAMPLES"));
	number_of_samples = *number_of_samples_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute NUMBER_OF_SAMPLES" << endl;
	number_of_samples = -1;
      }
    }
    return number_of_samples;
  }

  double BeamFormed::sampling_time ()
  {
    double sampling_time;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	double * sampling_time_p = reinterpret_cast<double*>(dataset_p->getAttribute("SAMPLING_TIME"));
	sampling_time = *sampling_time_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute SAMPLING_TIME" << endl;
	sampling_time = -1;
      }
    }
    return sampling_time;
  }

  std::string BeamFormed::notes ()
  {
    std::string attribute_notes ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * notes = reinterpret_cast<char*>(dataset_p->getAttribute("NOTES"));
	attribute_notes = string(notes);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute NOTES" << endl;
	attribute_notes = "";
      }
    }
   return attribute_notes;
  }

  // ------------------------------------------------------------ number_of_beams

  int BeamFormed::number_of_beams ()
  {
    int number_of_beams;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * number_of_beams_p = reinterpret_cast<int*>(dataset_p->getAttribute("NUMBER_OF_BEAMS"));
	number_of_beams = *number_of_beams_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute NUMBER_OF_BEAMS" << endl;
	number_of_beams = -1;
      }
    }
    return number_of_beams;
  }

  // ---------------------------------------------------------- sub_beam_diameter

  int BeamFormed::sub_beam_diameter ()
  {
    int sub_beam_diameter;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * sub_beam_diameter_p = reinterpret_cast<int*>(dataset_p->getAttribute("SUB_BEAM_DIAMETER"));
	sub_beam_diameter = *sub_beam_diameter_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute SUB_BEAM_DIAMETER" << endl;
	sub_beam_diameter = -1;
      }
    }
    return sub_beam_diameter;
  }

  // -------------------------------------------------------- weather_temperature

  int BeamFormed::weather_temperature ()
  {
    int weather_temperature;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * weather_temperature_p = reinterpret_cast<int*>(dataset_p->getAttribute("WEATHER_TEMPERATURE"));
	weather_temperature = *weather_temperature_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute WEATHER_TEMPERATURE" << endl;
	weather_temperature = -1;
      }
    }
    return weather_temperature;
  }

  // ----------------------------------------------------------- weather_humidity

  int BeamFormed::weather_humidity ()
  {
    int weather_humidity;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * weather_humidity_p = reinterpret_cast<int*>(dataset_p->getAttribute("WEATHER_HUMIDITY"));
	weather_humidity = *weather_humidity_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute WEATHER_HUMIDITY" << endl;
	weather_humidity = -1;
      }
    }
    return weather_humidity;
  }

  // ------------------------------------------------------- station_temperatures

  int BeamFormed::station_temperatures ()
  {
    int station_temperatures;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * station_temperatures_p = reinterpret_cast<int*>(dataset_p->getAttribute("TSYS"));
	station_temperatures = *station_temperatures_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute TSYS" << endl;
	station_temperatures = -1;
      }
    }
    return station_temperatures;
  }

  // ----------------------------------------------------------------------- init

  bool BeamFormed::init ()
  {
    bool status (true);

    // Connection to dataset via DAL layer

    dataset_p = new dalDataset();
    if (dataset_p->open(filename_p.c_str())) {
      std::cerr << "[BeamFormed::init] Error opening file into dalDataset!"
		<< std::endl;
      return false;
    }

    /*
      [1] If opening the data file was successful, we can continue. First we scan
      for the beam groups available in the file, since this is the basic unit
      within which the actual data for the individual sub-bands are stored.
    */
    std::vector<std::string> beamGroups = beams();

    /* Always check if actually a list of groups has been extracted */
    if (beamGroups.size() > 0) {

      for (uint beam(0); beam<beamGroups.size(); beam++) {
	// assemble internal list of beam groups
	BeamGroup group ((*dataset_p), beamGroups[beam]);
	beamGroups_p.push_back(group);
      }

    } else {
      std::cerr << "[BeamFormed::init] No beam group found in data set!"
		<< std::endl;
      status = false;
    }

    return status;
  }

#ifdef PYTHON
/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/

void BeamFormed::summary_boost()
{
  summary();
}

bpl::list BeamFormed::beams_boost()
{
   bpl::list beams_list;

   std::vector<std::string> beams_vec;
   
   beams_vec = beams();
   
   for( unsigned int ii; ii<beams_vec.size(); ii++ )
     beams_list.append( string(beams_vec[ii]) );
   
   return beams_list;
}

#endif

} // end namespace DAL
