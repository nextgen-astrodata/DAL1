/***************************************************************************
 *   Copyright (C) 2010                                                    *
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

/*!
  \file pydal_BeamFormed.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::BeamFormed class

  \author Lars B&auml;hren
*/

// DAL headers
#include <pydal.h>
#include <data_hl/BeamFormed.h>

using DAL::BeamFormed;

// ==============================================================================
//
//                                                                     BeamFormed
//
// ==============================================================================

void export_BeamFormed ()
{
  boost::python::class_<BeamFormed>("BeamFormed")
    .def( boost::python::init<>())
    .def( boost::python::init<std::string>())
    .def( "notes", &BeamFormed::notes,
	  "Return the notes from the header" )
    .def( "filename", &BeamFormed::filename,
	  "Return the filename from the header" )
    .def( "telescope", &BeamFormed::telescope,
	  "Return telescope the from the header" )
    .def( "nstations", &BeamFormed::nofStations,
	  "Return the nstations from the header" )
    .def( "datatype", &BeamFormed::datatype,
	  "Return the datatype from the header" )
    .def( "emband", &BeamFormed::emband,
	  "Return the emband from the header" )
    .def( "observation_id", &BeamFormed::observation_id,
	  "Return the observation_id from the header" )
    .def( "proj_id", &BeamFormed::proj_id,
	  "Return the proj_id from the header" )
    .def( "point_ra", &BeamFormed::point_ra,
	  "Return the point_ra from the header" )
    .def( "point_dec", &BeamFormed::point_dec,
	  "Return the point_dec from the header" )
    .def( "observer", &BeamFormed::observer,
	  "Return the observer from the header" )
    .def( "epoch_mjd", &BeamFormed::epoch_mjd,
	  "Return the epoch_mjd from the header" )
    .def( "epoch_date", &BeamFormed::epoch_date,
	  "Return the epoch_date from the header" )
    .def( "epoch_utc", &BeamFormed::epoch_utc,
	  "Return the epoch_utc from the header" )
    .def( "epoch_lst", &BeamFormed::epoch_lst,
	  "Return the epoch_lst from the header" )
    .def( "main_beam_diam", &BeamFormed::main_beam_diam,
	  "Return the main_beam_diam from the header" )
    .def( "center_freq", &BeamFormed::center_freq,
	  "Return the center_freq from the header" )
    .def( "bandwidth", &BeamFormed::bandwidth,
	  "Return the bandwidth from the header" )
    .def( "total_integration_time", &BeamFormed::total_integration_time,
	  "Return the total_integration_time from the header" )
    .def( "breaks", &BeamFormed::breaks,
	  "Return the breaks from the header" )
    .def( "dispersion_measure", &BeamFormed::dispersion_measure,
	  "Return the dispersion_measure from the header" )
    .def( "number_of_samples", &BeamFormed::number_of_samples,
	  "Return the number_of_samples from the header" )
    .def( "sampling_time", &BeamFormed::sampling_time,
	  "Return the sampling_time from the header" )
    .def( "number_of_beams", &BeamFormed::number_of_beams,
	  "Return the number_of_beams from the header" )
    .def( "sub_beam_diameter", &BeamFormed::sub_beam_diameter,
	  "Return the sub_beam_diameter from the header" )
    .def( "weather_temperature", &BeamFormed::weather_temperature,
	  "Return the weather_temperature from the header" )
    .def( "weather_humidity", &BeamFormed::weather_humidity,
	  "Return the weather_humidity from the header" )
    .def( "station_temperatures", &BeamFormed::station_temperatures,
	  "Return the station_temperatures from the header" )
    .def( "getBeam", &BeamFormed::getBeam,
	  boost::python::return_value_policy<boost::python::manage_new_object>(),
	  "Return a beam object from the file." )
    ;
}
