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
  \file pydal_bf.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings DAL C++ BF classes 

  \author Sven Duscha
*/


#include <num_util.h>
#include <data_hl/BF_BeamGroup.h>
#include <data_hl/BF_RootGroup.h>
#include <data_hl/SysLog.h>
#include <data_common/CommonAttributes.h>

using DAL::BF_BeamGroup;
using DAL::BF_RootGroup;

// ==============================================================================
//
//                                                                   BF_BeamGroup
//
// ==============================================================================

void export_BF_BeamGroup ()
{
  boost::python::class_<BF_BeamGroup>("BF_BeamGroup")
    // Construction
    .def( boost::python::init<>())
    .def( boost::python::init<hid_t,std::string>())
    // Access to internal parameters
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
  
  bool (BF_RootGroup::*openSubArrayPointing1)(unsigned int const &,
					      DAL::IO_Mode const &) 
    = &BF_RootGroup::openSubArrayPointing;


  bool (BF_RootGroup::*openStokesDataset1)(unsigned int const &,
					   unsigned int const &,
					   unsigned int const &,
					   DAL::IO_Mode const &)
    = &BF_RootGroup::openStokesDataset;
  bool (BF_RootGroup::*openStokesDataset2)(unsigned int const &,
					   unsigned int const &,
					   unsigned int const &,
					   unsigned int const &,
					   unsigned int const &,
					   unsigned int const &,
					   DAL::Stokes::Component const &,
					   hid_t const &,
					   DAL::IO_Mode const &) 
    = &BF_RootGroup::openStokesDataset;
  bool (BF_RootGroup::*openStokesDataset3)(unsigned int const &,
					   unsigned int const &,
					   unsigned int const &,
					   unsigned int const &,
					   std::vector<unsigned int> const &,
					   DAL::Stokes::Component const &,
					   hid_t const &,
					   DAL::IO_Mode const &) 
    = &BF_RootGroup::openStokesDataset;
  
  //________________________________________________________
  // Bindings for class and its methods
  
  boost::python::class_<BF_RootGroup>("BF_RootGroup", boost::python::init<const std::string>())
    // Construction
    .def( boost::python::init<DAL::Filename, bool>())
    .def( boost::python::init<std::string const &>())
    .def( boost::python::init<std::string &>())
    .def( boost::python::init<DAL::CommonAttributes, bool>())     
    // Parameter access
    // Public methods
    .def("summary",
	 summary1,
	 "Summary of the object's internal parameters and status.")
    .def("summary",
	 summary2,
	 "Summary of the object's internal parameters and status.")
    .def( "className",
	  &BF_RootGroup::className, 
	  "Get name of the class" )
    .def( "CommonAttributes",
	  &BF_RootGroup::commonAttributes, 
	  "Get the common attributes of the class." )      
    .def( "sysLog",
	  &BF_RootGroup::sysLog, 
	  "Get the SYS_LOG group object." )      
    .def( "nofSubArrayPointings",
	  &BF_RootGroup::nofSubArrayPointings,
	  "Get the number of SubArray pointings." )
    .def( "open",
	  &BF_RootGroup::open, 
	  "Open the file containing the beamformed data." )
    .def( "openBeam",
	  &BF_RootGroup::openBeam, 
	  "Open a beam group." )
    .def( "openSubArrayPointing",
	  openSubArrayPointing1, 
	  "Open a SubArrayPointing direction group." )      
    .def("openStokesDataset",
	 openStokesDataset1,
	 "Open a Stokes dataset.")
    .def("openStokesDataset",
	 openStokesDataset2,
	 "Create a new Stokes dataset.")
    .def("openStokesDataset",
	 openStokesDataset3,
	 "Create a new Stokes dataset.")
    ;
  
   BF_BeamGroup ( BF_RootGroup::*getBeamGroup)(unsigned int const &pointingID,
			       unsigned int const &beamID) = &BF_RootGroup::getBeamGroup;
}


// ==============================================================================
//
//                                                                   BF_RootGroup
//
// ==============================================================================
/*
boost::python::numeric::array BF_BeamGroup::getIntensity_boost( int subband,
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

