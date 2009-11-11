/*-------------------------------------------------------------------------*
 | $Id:: NewClass.cc 1964 2008-09-06 17:52:38Z baehren                   $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
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

#include <BF_StationBeam.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  BF_StationBeam::BF_StationBeam ()
  {;}
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  BF_StationBeam::~BF_StationBeam ()
  {
    destroy();
  }
  
  void BF_StationBeam::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                      summary
  
  void BF_StationBeam::summary (std::ostream &os)
  {
    os << "[BF_StationBeam] Summary of internal parameters." << std::endl;
  }
  
  
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes

   void BF_StationBeam::setAttributes ()
   {
     attributes_p.clear();

     attributes_p.insert("GROUPTYPE");
     attributes_p.insert("NOF_STATIONS");
     attributes_p.insert("STATIONS_LIST");
     attributes_p.insert("POINT_RA");
     attributes_p.insert("POINT_DEC");
     attributes_p.insert("TRACKING");
     attributes_p.insert("POINT_ALTITUDE");
     attributes_p.insert("POINT_AZIMUTH");
     attributes_p.insert("NOF_PENCIL_BEAMS");
  }
  
  //_____________________________________________________________________________
  //                                                                         open
  
  /*!
    \param location -- Identifier of the location to which the to be opened
           structure is attached.
    \param name   -- Name of the structure (file, group, dataset, etc.) to be
           opened.
    \param create -- Create the corresponding data structure, if it does not 
           exist yet?
    
    \return status -- Status of the operation; returns <tt>false</tt> in case
            an error was encountered.
  */
  bool BF_StationBeam::open (hid_t const &location,
			     std::string const &name,
			     bool const &create)
  {
    bool status (true);

    location_p = H5Gopen2 (location,name.c_str(),H5P_DEFAULT);
    
    return status;
  }


} // Namespace DAL -- end
