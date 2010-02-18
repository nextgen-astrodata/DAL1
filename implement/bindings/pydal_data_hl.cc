/*-------------------------------------------------------------------------*
 | $Id:: pywrapper.cc 4184 2010-02-18 14:14:48Z baehren                  $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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
  \file pydal_data_hl.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings DAL C++ classes in the data_hl directory

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <BeamFormed.h>
#include <BeamGroup.h>
#include <TBB_Timeseries.h>
#include <TBB_StationGroup.h>
#include <TBB_DipoleDataset.h>

//_______________________________________________________________________________
//                                                                 TBB_Timeseries

void export_TBB_Timeseries ()
{  
  bpl::class_<TBB_Timeseries>("TBB_Timeseries")
    /* Construction */
    .def( bpl::init<>())
    .def( bpl::init<string>())
    /* Access to internal parameters */
    .def( "filename", &TBB_Timeseries::filename,
	  "Get the name of the data file." )
    .def( "locationID", &TBB_Timeseries::locationID,
	  "Get the object identifier for the data file." )
    .def( "nofStationGroups", &TBB_Timeseries::nofStationGroups,
	  "Get the number of station groups collected into this file." )
    .def( "nofDipoleDatasets", &TBB_Timeseries::nofDipoleDatasets,
	  "Get the number of dipole datasets collected into this file." )
    ;
}

  //_____________________________________________________________________________
  //                                                             TBB_StationGroup
  
void export_TBB_StationGroup () 
{
  bpl::class_<TBB_StationGroup>("TBB_StationGroup")
    /* Construction */
    .def( bpl::init<>())
    .def( bpl::init<uint,string>())
    .def( bpl::init<uint>())
    /* Access to internal parameters */
    .def( "group_name", &TBB_StationGroup::group_name,
	  "Get the name for this group within the HDF5 file." )
    ;
}

//_______________________________________________________________________________
//                                                              TBB_DipoleDataset

void export_TBB_DipoleDataset()
{
  bpl::class_<TBB_DipoleDataset>("TBB_DipoleDataset")
    /* Construction */
    .def( bpl::init<>())
    .def( bpl::init<uint,string>())
    /* Access to internal parameters */
    .def( "nofAttributes", &TBB_DipoleDataset::nofAttributes,
	  "Get the number of attributes attached to the dataset." )
    .def( "julianDay", &TBB_DipoleDataset::julianDay,
	  "Get the time as Julian Day." )
    ;
}

//_______________________________________________________________________________
//                                                                      BeamGroup

void export_BeamGroup ()
{
  bpl::class_<BeamGroup>("BeamGroup")
    .def( bpl::init<>())
    .def( "summary", &BeamGroup::summary_boost)
    .def( "fileID", &BeamGroup::fileID,
	  "Get the HDF5 file handle ID")
    .def( "groupID", &BeamGroup::groupID,
	  "Get the HDF5 group handle ID")
    .def( "ra", &BeamGroup::ra,
	  "Return ra value for the beam")
    .def( "dec", &BeamGroup::dec,
	  "Return dec value for the beam")
    .def( "nofSubbands", &BeamGroup::nofSubbands,
	  "Return number of subbands")
    .def( "getIntensity", &BeamGroup::getIntensity_boost,
	  "Get a numpy array of intensities for a given subband")
    .def( "getIntensitySquared", &BeamGroup::getIntensitySquared_boost,
	  "Get a numpy array of squared intensities for a given subband")
    .def( "getSubbandData_X", &BeamGroup::getSubbandData_X_boost,
	  "Get a numpy array of values for a given subband")
    .def( "getSubbandData_Y", &BeamGroup::getSubbandData_Y_boost,
	  "Get a numpy array of values for a given subband")
    .def( "getSubbandData_XY", &BeamGroup::getSubbandData_XY_boost,
	  "Get a numpy array of values for a given subband")
    ;
}

//_______________________________________________________________________________
//                                                                     BeamFormed

void export_BeamFormed ()
{
  bpl::class_<BeamFormed>("BeamFormed")
    .def( bpl::init<>())
    .def( bpl::init<string>())
    .def( "summary", &BeamFormed::summary_boost,
	  "Print a summary of the file contents" )
    .def( "beams", &BeamFormed::beams_boost,
	  "Return a list of the beams" )
    .def( "source", &BeamFormed::source_boost,
	  "Return a list of the sources" )
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
	  bpl::return_value_policy<bpl::manage_new_object>(),
	  "Return a beam object from the file." )
    ;
}
