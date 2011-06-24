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
  bpl::class_<BF_BeamGroup>("BF_BeamGroup")
    // Construction
    .def( bpl::init<>())
    .def( bpl::init<hid_t,string>())
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
      
      .def( "nofSubArrayPointings", &BF_RootGroup::nofSubArrayPointings,
      "Get the number of SubArray pointings." )
      ;
      
   // Data access methods       
   bool ( BF_RootGroup::*open)(hid_t const &,
            string const &,
            DAL::IO_Mode const &) = &BF_RootGroup::open;

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
            DAL::IO_Mode const &flags) = &BF_RootGroup::openStokesDataset;

   bool ( BF_RootGroup::*openStokesDataset)(unsigned int const &pointingID,
            unsigned int const &beamID,
            unsigned int const &stokesID,
            unsigned int const &nofSamples,
            std::vector<unsigned int> const &nofChannels,
            DAL::Stokes::Component const &component,
            hid_t const &datatype,
            DAL::IO_Mode const &flags) = &BF_RootGroup::openStokesDataset;

   // Get the syslog object
   DAL::SysLog (BF_RootGroup::*sysLog)(void) ;

   BF_BeamGroup ( BF_RootGroup::*getBeamGroup)(unsigned int const &pointingID,
			       unsigned int const &beamID) = &BF_RootGroup::getBeamGroup;
}

