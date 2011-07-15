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
#include <data_hl/BF_BeamGroup.h>
#include <data_hl/BF_RootGroup.h>
#include <data_common/CommonAttributes.h>
#include <data_hl/TBB_Timeseries.h>
#include <data_hl/TBB_StationGroup.h>
#include <data_hl/TBB_DipoleDataset.h>
#include <data_hl/LOPES_EventFile.h>

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
  boost::python::class_<BF_BeamGroup>("BF_BeamGroup")
    /* Construction */
    .def( boost::python::init<>())
    .def( boost::python::init<hid_t,string>())
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

  //________________________________________________________
  // Specialisation of overloaded methods

  void (BF_RootGroup::*summary1)(bool const &) 
    = &BF_RootGroup::summary;
  void (BF_RootGroup::*summary2)(std::ostream &,
				 bool const &) 
    = &BF_RootGroup::summary;
  
  //________________________________________________________
  // Bindings for class and its methods

  boost::python::class_<BF_RootGroup>("BF_RootGroup", boost::python::init<const string>())
    // Argumented constructor 
    .def( boost::python::init<DAL::Filename, bool>())
    .def( boost::python::init<string const &>())
    .def( boost::python::init<string &>())
    .def( boost::python::init<DAL::CommonAttributes, bool>())     
    // Access to internal parameters 
    .def( "className", &BF_RootGroup::className, 
	  "Get name of the class" )
    .def( "CommonAttributes", &BF_RootGroup::commonAttributes, 
	  "Get the common attributes of the class." )      
    
    .def( "nofSubArrayPointings", &BF_RootGroup::nofSubArrayPointings,
	  "Get the number of SubArray pointings." )
    // Public methods
    .def( "open", &BF_RootGroup::open, 
	  "Open the file containing the beamformed data." )
    .def("summary",
	 summary1,
	 "Summary of the object's internal parameters and status.")
    .def("summary",
	 summary2,
	 "Summary of the object's internal parameters and status.")
    ;
  
  // Data access methods       
   bool ( BF_RootGroup::*openSubArrayPointing)( unsigned int const &,
						DAL::IO_Mode const &) = &BF_RootGroup::openSubArrayPointing;
   
   bool ( BF_RootGroup::*openBeam1)(hid_t const &,
				    const string &,
				    DAL::IO_Mode const &) = &BF_RootGroup::open;
   
   bool ( BF_RootGroup::*openStokesDataset1)(unsigned int const &,
					     unsigned int const &,
					     unsigned int const &,
					     DAL::IO_Mode const &) = &BF_RootGroup::openStokesDataset;
   
   bool ( BF_RootGroup::*openBeam2)(unsigned int const &,
				    unsigned const int &,
				    DAL::IO_Mode const &) = &BF_RootGroup::openBeam;
   
   bool ( BF_RootGroup::*openStokesDataset2)(unsigned int const &,
					     unsigned int const &,
					     unsigned int const &,
					     unsigned int const &,
					     unsigned int const &,
					     unsigned int const &,
					     DAL::Stokes::Component const &,
					     hid_t const &,
					     DAL::IO_Mode const &) = &BF_RootGroup::openStokesDataset;
   
   bool ( BF_RootGroup::*openStokesDataset3)(unsigned int const &pointingID,
					     unsigned int const &beamID,
					     unsigned int const &stokesID,
					     unsigned int const &nofSamples,
					     unsigned int const &nofSubbands,
					     unsigned int const &nofChannels,
					     DAL::Stokes::Component const &component,
					     hid_t const &datatype,
					     DAL::IO_Mode const &flags);
   
   bool ( BF_RootGroup::*openStokesDataset)(unsigned int const &pointingID,
					    unsigned int const &beamID,
					    unsigned int const &stokesID,
					    unsigned int const &nofSamples,
					    std::vector<unsigned int> const &nofChannels,
					    DAL::Stokes::Component const &component,
					    hid_t const &datatype,
					    DAL::IO_Mode const &flags);
   
   BF_BeamGroup ( BF_RootGroup::*getBeamGroup)(unsigned int const &pointingID,
					       unsigned int const &beamID);
}


// ==============================================================================
//
//                                                                 TBB_Timeseries
//
// ==============================================================================

void export_TBB_Timeseries ()
{  
  boost::python::class_<TBB_Timeseries>("TBB_Timeseries")
    /* Construction */
    .def( boost::python::init<>())
    .def( boost::python::init<string>())
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
  boost::python::class_<TBB_StationGroup>("TBB_StationGroup")
    /* Construction */
    .def( boost::python::init<>())
    .def( boost::python::init<uint,string>())
    .def( boost::python::init<uint>())
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
  
  boost::python::class_<TBB_DipoleDataset>("TBB_DipoleDataset")
    /* Construction */
    .def( boost::python::init<>())
    .def( boost::python::init<hid_t const &, std::string const &>())
    .def( boost::python::init<hid_t const &, uint const &, uint const &, uint const &>())
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
//                                                                LOPES_EventFile
//
// ==============================================================================

void export_LOPES_EventFile ()
{
  /* Enumeration: Event type */
  boost::python::enum_<LOPES_EventFile::EvType>("EvType")
    .value("Unspecified",LOPES_EventFile::Unspecified)
    .value("Cosmic",LOPES_EventFile::Cosmic)
    .value("Simulation",LOPES_EventFile::Simulation)
    .value("Test",LOPES_EventFile::Test)
    .value("SolarFlare",LOPES_EventFile::SolarFlare)
    .value("Other",LOPES_EventFile::Other)
    ;
  
  /* Enumeration: Observatory */
  boost::python::enum_<LOPES_EventFile::Observatory>("Observatory")
    .value("LOPES",LOPES_EventFile::LOPES)
    .value("LORUN",LOPES_EventFile::LORUN)
    ;
  
  boost::python::class_<LOPES_EventFile>("LOPES_EventFile")
    .def( boost::python::init<>())
    .def( boost::python::init<string>())
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


// ==============================================================================
//
//                                                                   BF_RootGroup
//
// ==============================================================================
            
 /*           
            boost::python::numeric::array BeamGroup::getIntensity_boost( int subband,
						   int start,
						   int length )
{
  float * values = NULL;
  values = getIntensity( subband, start, length );
  std::vector<int> mydims;
  mydims.push_back( length );
  boost::python::numeric::array narray = num_util::makeNum( values, mydims );
  delete [] values;
  values = NULL;
  return narray;
}
*/
