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
  \file pydal_data_hl.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings DAL C++ classes in the data_hl directory

  \author Lars B&auml;hren
*/

// DAL headers
#include "pydal.h"
#include <data_hl/BeamFormed.h>
#include <data_hl/BeamGroup.h>
#include <data_hl/BF_BeamGroup.h>
#include <data_hl/BF_RootGroup.h>
#include <data_common/CommonAttributes.h>
#include <data_hl/TBB_Timeseries.h>
#include <data_hl/TBB_StationGroup.h>
#include <data_hl/TBB_DipoleDataset.h>
#include <data_hl/LOPES_EventFile.h>

using DAL::BeamFormed;
using DAL::BeamGroup;
using DAL::BF_BeamGroup;
using DAL::BF_RootGroup;
using DAL::LOPES_EventFile;
using DAL::TBB_Timeseries;
using DAL::TBB_DipoleDataset;
using DAL::TBB_StationGroup;

// ==============================================================================
//
//                                                                   BF_BeamGroup
//
// ==============================================================================

void export_BF_BeamGroup ()
{
  bpl::class_<BF_BeamGroup>("BF_BeamGroup")
    /* Construction */
    .def( bpl::init<>())
    .def( bpl::init<hid_t,string>())
    /* Access to internal parameters */
    .def( "locationID", &BF_BeamGroup::locationID,
	  "Get the object identifier for the data file." )
    .def( "className", &BF_BeamGroup::className,
	  "Get name of the class." )
    ;
}


// ==============================================================================
//
//                                                                   BF_RootGroup
//
// ==============================================================================

void export_BF_RootGroup ()
{
   bpl::class_<BF_RootGroup>("BF_RootGroup", bpl::init<const string>())
      // Argumented constructor 
      .def( bpl::init<DAL::Filename, bool>())
      .def( bpl::init<string const &>())
      .def( bpl::init<string &>())
      .def( bpl::init<DAL::CommonAttributes, bool>())     
      // Access to internal parameters 
      .def( "className", &BF_RootGroup::className, 
      "Get name of the class" )
      //void ( BF_RootGroup::*summary)() = &BF_RootGroup::summary)
      .def( "CommonAttributes", &BF_RootGroup::commonAttributes, 
      "Get the common attributes of the class." )      
      ;
      
      // Data access methods       
      bool ( BF_RootGroup::*open)(hid_t const &,
                string const &,
                DAL::IO_Mode const &) = &BF_RootGroup::open;

      bool ( BF_RootGroup::*openSubArrayPointing)( unsigned int const &,
                DAL::IO_Mode const &) = &BF_RootGroup::openSubArrayPointing;

      bool ( BF_RootGroup::*openBeam)(hid_t const &,
               const string &,
               DAL::IO_Mode const &) = &BF_RootGroup::open;
      
      bool ( BF_RootGroup::*openStokesDataset)(unsigned const &,
               unsigned int const &,
               unsigned int const &,
               DAL::IO_Mode const &) = &BF_RootGroup::openStokesDataset;

      ;
}


// ==============================================================================
//
//                                                                 TBB_Timeseries
//
// ==============================================================================

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

// ==============================================================================
//
//                                                               TBB_StationGroup
//
// ==============================================================================

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

// ==============================================================================
//
//                                                              TBB_DipoleDataset
//
// ==============================================================================

void export_TBB_DipoleDataset()
{
  void (TBB_DipoleDataset::*summary1)()                = &TBB_DipoleDataset::summary;
  void (TBB_DipoleDataset::*summary2)(std::ostream &)  = &TBB_DipoleDataset::summary;
  bool (TBB_DipoleDataset::*open1)(hid_t const &)      = &TBB_DipoleDataset::open;
  bool (TBB_DipoleDataset::*open2)(hid_t const &,
				   std::string const &,
				   DAL::IO_Mode const &) = &TBB_DipoleDataset::open;
  bool (TBB_DipoleDataset::*open3)(hid_t const &,
				   uint const &,
				   uint const &,
				   uint const &,
				   std::vector<hsize_t> const &,
				   hid_t const &)      = &TBB_DipoleDataset::open;
  int (TBB_DipoleDataset::*dipoleNumber1)()      = &TBB_DipoleDataset::dipoleNumber;
  std::string (TBB_DipoleDataset::*dipoleName1)()      = &TBB_DipoleDataset::dipoleName;
//   std::string (TBB_DipoleDataset::*dipoleName2)(unsigned int const &,
// 					     unsigned int const &,
// 					     unsigned int const &)
//     = &TBB_DipoleDataset::dipoleName;
  
  bpl::class_<TBB_DipoleDataset>("TBB_DipoleDataset")
    /* Construction */
    .def( bpl::init<>())
    .def( bpl::init<hid_t const &, std::string const &>())
    .def( bpl::init<hid_t const &, uint const &, uint const &, uint const &>())
    /* Access to internal parameters */
    .def( "nofAttributes", &TBB_DipoleDataset::nofAttributes,
	  "Get the number of attributes attached to the dataset." )
    .def( "julianDay", &TBB_DipoleDataset::julianDay,
	  "Get the time as Julian Day." )
    .def("summary", summary1,
	 "Provide a summary of the internal status.")
    .def("summary", summary2,
	 "Provide a summary of the internal status.")
    .def("open", open1,
	 "Open a dipole dataset.")
    .def("open", open2,
	 "Open a dipole dataset.")
    .def("open", open3,
	 "Open a dipole dataset.")
    .def( "dipoleNumber", dipoleNumber1,
	  "Get the unique channel/dipole identifier." )
    .def( "getName", dipoleName1,
	  "Get the unique channel/dipole identifier." )
//     .def( "getName", dipoleName2,
// 	  "Get the unique channel/dipole identifier." )
    ;
}

// ==============================================================================
//
//                                                                      BeamGroup
//
// ==============================================================================

bpl::numeric::array BeamGroup::getIntensity_boost( int subband,
						   int start,
						   int length )
{
  float * values = NULL;
  values = getIntensity( subband, start, length );
  std::vector<int> mydims;
  mydims.push_back( length );
  bpl::numeric::array narray = num_util::makeNum( values, mydims );
  delete [] values;
  values = NULL;
  return narray;
}

bpl::numeric::array BeamGroup::getIntensitySquared_boost( int subband,
							  int start,
							  int length )
{
  float * values = NULL;
  values = getIntensitySquared( subband, start, length );
  std::vector<int> mydims;
  mydims.push_back( length );
  bpl::numeric::array narray = num_util::makeNum( values, mydims );
  delete [] values;
  values = NULL;
  return narray;
}

bpl::numeric::array BeamGroup::getSubbandData_X_boost( int subband,
						       int start,
						       int length )
{
  std::vector<std::complex<short> > values;
  getSubbandData_X( subband, start, length, values );
  
  std::complex<float> * value_list;
  value_list = new std::complex<float>[length];
  
  for (int ii=0; ii<length; ii++)
    {
      value_list[ii] = std::complex<float>(values[ii].real(),values[ii].imag());
    }
  std::vector<int> mydims;
  mydims.push_back( length );
  bpl::numeric::array narray = num_util::makeNum( value_list, mydims );
  delete [] value_list;
  value_list = NULL;
  return narray;
}

bpl::numeric::array BeamGroup::getSubbandData_Y_boost( int subband,
						       int start,
						       int length )
{
  std::vector<std::complex<short> > values;
  getSubbandData_Y( subband, start, length, values );
  
  std::complex<float> * value_list;
  value_list = new std::complex<float>[length];
  
  for (int ii=0; ii<length; ii++)
    {
      value_list[ii] = std::complex<float>(values[ii].real(),values[ii].imag());
    }
  std::vector<int> mydims;
  mydims.push_back( length );
  bpl::numeric::array narray = num_util::makeNum( value_list, mydims );
  delete [] value_list;
  value_list = NULL;
  return narray;
}


bpl::numeric::array BeamGroup::getSubbandData_XY_boost( int subband,
							int start,
							int length )
{
  std::vector< std::complex<short> > values_x;
  std::vector< std::complex<short> >::iterator xvalit;
  std::vector< std::complex<short> > values_y;
  std::vector< std::complex<short> >::iterator yvalit;
  values_x.clear();
  values_y.clear();
  getSubbandData_X( subband, start, length, values_x );
  getSubbandData_Y( subband, start, length, values_y );
  
  bpl::list x_value_list;
  bpl::list y_value_list;
  bpl::list xy_value_list;
  
  for (xvalit=values_x.begin(); xvalit < values_x.end(); xvalit++)
    {
      std::complex<float> foo((*xvalit).real(),(*xvalit).imag());
      x_value_list.append( foo );
    }
  for (yvalit=values_y.begin(); yvalit < values_y.end(); yvalit++)
    {
      std::complex<float> foo((*yvalit).real(),(*yvalit).imag());
      y_value_list.append( foo );
    }
  
  xy_value_list.append(x_value_list);
  xy_value_list.append(y_value_list);
  
  bpl::numeric::array narray = num_util::makeNum( xy_value_list );
  
  return narray;
}

void export_BeamGroup ()
{
  bpl::class_<BeamGroup>("BeamGroup")
    .def( bpl::init<>())
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

// ==============================================================================
//
//                                                                     BeamFormed
//
// ==============================================================================

void export_BeamFormed ()
{
  bpl::class_<BeamFormed>("BeamFormed")
    .def( bpl::init<>())
    .def( bpl::init<string>())
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

// ==============================================================================
//
//                                                                LOPES_EventFile
//
// ==============================================================================

void export_LOPES_EventFile ()
{
  /* Enumeration: Event type */
  bpl::enum_<LOPES_EventFile::EvType>("EvType")
    .value("Unspecified",LOPES_EventFile::Unspecified)
    .value("Cosmic",LOPES_EventFile::Cosmic)
    .value("Simulation",LOPES_EventFile::Simulation)
    .value("Test",LOPES_EventFile::Test)
    .value("SolarFlare",LOPES_EventFile::SolarFlare)
    .value("Other",LOPES_EventFile::Other)
    ;
  
  /* Enumeration: Observatory */
  bpl::enum_<LOPES_EventFile::Observatory>("Observatory")
    .value("LOPES",LOPES_EventFile::LOPES)
    .value("LORUN",LOPES_EventFile::LORUN)
    ;
  
  bpl::class_<LOPES_EventFile>("LOPES_EventFile")
    .def( bpl::init<>())
    .def( bpl::init<string>())
    .def( "filename", &LOPES_EventFile::filename,
	  "Get the name of the data file." )
    .def( "samplerate", &LOPES_EventFile::samplerate,
	  "Get the samplerate of the A/D conversion." )
    .def( "nyquistZone", &LOPES_EventFile::nyquistZone,
	  "Get the Nyquist zone in which the data are sampled." )
    .def( "nofAntennas", &LOPES_EventFile::nofAntennas,
	  "Get the number of antennas in the data set." )
    .def( "nofDatapoints", &LOPES_EventFile::nofDatapoints,
	  "Get the number of data points stored." )
    ;
}
