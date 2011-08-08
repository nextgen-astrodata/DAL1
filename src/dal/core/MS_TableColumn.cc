/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars Baehren (lbaehren@gmail.com)                                     *
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

#include <core/MS_TableColumn.h>

namespace DAL { // Namespace DAL -- begin
  
#ifdef DAL_WITH_CASA

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  MS_TableColumn::MS_TableColumn ()
  {;}
  
  /*!
    \param other -- Another MS_TableColumn object from which to create this new
           one.
  */
  MS_TableColumn::MS_TableColumn (MS_TableColumn const &other)
    : dalObjectBase (other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  MS_TableColumn::~MS_TableColumn ()
  {
    destroy();
  }
  
  void MS_TableColumn::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another MS_TableColumn object from which to make a copy.
  */
  MS_TableColumn& MS_TableColumn::operator= (MS_TableColumn const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  /*!
    \param other -- Another MS_TableColumn object from which to make a copy.
  */
  void MS_TableColumn::copy (MS_TableColumn const &other)
  {;}

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
  void MS_TableColumn::summary (std::ostream &os)
  {
    os << "[MS_TableColumn] Summary of internal parameters." << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  

  // ============================================================================
  //
  //  Static methods
  //
  // ============================================================================
  
  
#else

  MS_TableColumn::MS_TableColumn ()
    : dalObjectBase ()
  {
  }

#endif  

} // Namespace DAL -- end
