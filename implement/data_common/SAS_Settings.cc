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

#include <SAS_Settings.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  SAS_Settings::SAS_Settings ()
  {
    setValues();
  }
  
  SAS_Settings::SAS_Settings (SAS_Settings const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  SAS_Settings::~SAS_Settings ()
  {
    destroy();
  }
  
  void SAS_Settings::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  SAS_Settings& SAS_Settings::operator= (SAS_Settings const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }

  //_____________________________________________________________________________
  //                                                                         copy
  
  void SAS_Settings::copy (SAS_Settings const &other)
  {
    antennaSet_p.clear();
    antennaSet_p = other.antennaSet_p;

    filterSelection_p.clear();
    filterSelection_p = other.filterSelection_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  void SAS_Settings::summary (std::ostream &os)
  {
    os << "[SAS_Settings] Summary of internal parameters." << std::endl;
    os << "-- AntennaSet      = " << antennaSet_p          << std::endl;
    os << "-- FilterSelection = " << filterSelection_p     << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  void SAS_Settings::setValues ()
  {
    /* AntennaSet  (2009/11/18, Ruud Overeem) */
    antennaSet_p.clear();
    antennaSet_p.insert("LBA_INNER");
    antennaSet_p.insert("LBA_OUTER");
    antennaSet_p.insert("LBA_SPARSE");
    antennaSet_p.insert("LBA_X");
    antennaSet_p.insert("LBA_Y");
    antennaSet_p.insert("HBA_ONE");
    antennaSet_p.insert("HBA_TWO");
    antennaSet_p.insert("HBA_BOTH");

    /* FilterSelection */
    filterSelection_p.clear();
    filterSelection_p.insert("LBA_30_80");
    filterSelection_p.insert("LBA_10_90");
    filterSelection_p.insert("HBA_110_190");
    filterSelection_p.insert("HBA_170_230");
    filterSelection_p.insert("HBA_210_250");
  }
  
} // Namespace DAL -- end
