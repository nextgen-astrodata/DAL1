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

#include <core/AccessMode.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  AccessMode::AccessMode ()
  {;}
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  AccessMode::AccessMode (AccessMode const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  AccessMode::~AccessMode ()
  {
    destroy();
  }
  
  void AccessMode::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  AccessMode& AccessMode::operator= (AccessMode const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void AccessMode::copy (AccessMode const &other)
  {
    itsMode = other.itsMode;
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
  void AccessMode::summary (std::ostream &os)
  {
    os << "[AccessMode] Summary of internal parameters." << std::endl;
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
  
  //_____________________________________________________________________________
  //                                                                     modesMap

  std::map<DAL::IO::Mode,std::string> AccessMode::modesMap ()
  {
    std::map<DAL::IO::Mode,std::string> modes;

    modes[DAL::IO::Create]       = "Create";
    modes[DAL::IO::CreateNew]    = "CreateNew";
    modes[DAL::IO::Open]         = "Open";
    modes[DAL::IO::OpenOrCreate] = "OpenOrCreate";
    modes[DAL::IO::Truncate]     = "Truncate";

    modes[DAL::IO::ReadOnly]     = "ReadOnly";
    modes[DAL::IO::WriteOnly]    = "WriteOnly";
    modes[DAL::IO::ReadWrite]    = "ReadWrite";

    return modes;
  }

  //_____________________________________________________________________________
  //                                                                    modesType

  std::vector<DAL::IO::Mode> AccessMode::modesType ()
  {
    std::map<DAL::IO::Mode,std::string> modes = modesMap();
    std::map<DAL::IO::Mode,std::string>::iterator it;
    std::vector<DAL::IO::Mode> types;

    for (it=modes.begin(); it!=modes.end(); ++it) {
      types.push_back(it->first);
    } 

    return types;
  }

  //_____________________________________________________________________________
  //                                                                    modesName

  std::vector<std::string> AccessMode::modesName ()
  {
    std::map<DAL::IO::Mode,std::string> modes = modesMap();
    std::map<DAL::IO::Mode,std::string>::iterator it;
    std::vector<std::string> names;

    for (it=modes.begin(); it!=modes.end(); ++it) {
      names.push_back(it->second);
    } 

    return names;
  }

} // Namespace DAL -- end
