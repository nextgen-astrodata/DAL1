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

#include <core/AccessProperties.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  AccessProperties::AccessProperties ()
  {;}
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  AccessProperties::AccessProperties (AccessProperties const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  AccessProperties::~AccessProperties ()
  {
    destroy();
  }
  
  void AccessProperties::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  AccessProperties& AccessProperties::operator= (AccessProperties const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void AccessProperties::copy (AccessProperties const &other)
  {
    itsAccessProperties = other.itsAccessProperties;
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
  void AccessProperties::summary (std::ostream &os)
  {
    os << "[AccessProperties] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  

} // Namespace DAL -- end
