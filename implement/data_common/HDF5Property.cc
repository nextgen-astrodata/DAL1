/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#include <HDF5Property.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  HDF5Property::HDF5Property ()
  {;}
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  HDF5Property::HDF5Property (HDF5Property const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  HDF5Property::~HDF5Property ()
  {
    destroy();
  }
  
  void HDF5Property::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  HDF5Property& HDF5Property::operator= (HDF5Property const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void HDF5Property::copy (HDF5Property const &other)
  {
    fileAccessFlag_p.clear();
    fileAccessFlag_p = other.fileAccessFlag_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void HDF5Property::summary (std::ostream &os)
  {
    os << "[HDF5Property] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  

} // Namespace DAL -- end
