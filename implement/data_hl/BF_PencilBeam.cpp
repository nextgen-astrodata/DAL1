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

#include <BF_PencilBeam.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  BF_PencilBeam::BF_PencilBeam ()
  {;}
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  BF_PencilBeam::~BF_PencilBeam ()
  {
    destroy();
  }
  
  void BF_PencilBeam::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                      summary
  
  void BF_PencilBeam::summary (std::ostream &os)
  {
    os << "[BF_PencilBeam] Summary of internal parameters." << std::endl;
  }
  
  
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                setAttributes
  
  void BF_PencilBeam::setAttributes ()
  {
    attributes_p.clear();

    attributes_p.insert("GROUPTYPE");
    attributes_p.insert("TARGET");
    attributes_p.insert("POINT_RA");
    attributes_p.insert("POINT_DEC");
    attributes_p.insert("POSITION_OFFSET_RA");
    attributes_p.insert("POSITION_OFFSET_DEC");
    attributes_p.insert("PB_DIAMETER_RA");
    attributes_p.insert("PB_DIAMETER_DEC");
    attributes_p.insert("PB_CENTER_FREQUENCY");
    attributes_p.insert("PB_CENTER_FREQUENCY_UNIT");
    attributes_p.insert("FOLDED_DATA");
    attributes_p.insert("FOLD_PERIOD");
    attributes_p.insert("FOLD_PERIOD_UNIT");
    attributes_p.insert("DEDISPERSION");
    attributes_p.insert("DISPERSION_MEASURE");
    attributes_p.insert("DISPERSION_MEASURE_UNIT");
    attributes_p.insert("BARYCENTER");
    attributes_p.insert("STOKES_COMPONENTS");
    attributes_p.insert("COMPLEX_VOLTAGE");
    attributes_p.insert("SIGNAL_SUM");
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
  bool BF_PencilBeam::open (hid_t const &location,
			    std::string const &name,
			    bool const &create)
  {
    return false;
  }

  //_____________________________________________________________________________
  //                                                                 openEmbedded
  
  bool BF_PencilBeam::openEmbedded ()
  {
    return false;
  }
  
} // Namespace DAL -- end
