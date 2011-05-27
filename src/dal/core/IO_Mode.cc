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

#include <core/IO_Mode.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  IO_Mode::IO_Mode ()
  {;}
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  IO_Mode::IO_Mode (IO_Mode const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  IO_Mode::~IO_Mode ()
  {
    destroy();
  }
  
  void IO_Mode::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  IO_Mode& IO_Mode::operator= (IO_Mode const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void IO_Mode::copy (IO_Mode const &other)
  {
    itsFlags = other.itsFlags;
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
  void IO_Mode::summary (std::ostream &os)
  {
    os << "[IO_Mode] Summary of internal parameters." << std::endl;
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
  //                                                                     flagsMap

  /*!
    \return flags -- 
   */
  std::map<IO_Mode::Flags,std::string> IO_Mode::flagsMap ()
  {
    std::map<IO_Mode::Flags,std::string> flags;

    flags[IO_Mode::Create]       = "Create";
    flags[IO_Mode::CreateNew]    = "CreateNew";
    flags[IO_Mode::Open]         = "Open";
    flags[IO_Mode::OpenOrCreate] = "OpenOrCreate";
    flags[IO_Mode::Truncate]     = "Truncate";

    flags[IO_Mode::ReadOnly]     = "ReadOnly";
    flags[IO_Mode::WriteOnly]    = "WriteOnly";
    flags[IO_Mode::ReadWrite]    = "ReadWrite";

    return flags;
  }

  //_____________________________________________________________________________
  //                                                                    modesType

  std::vector<IO_Mode::Flags> IO_Mode::modesType ()
  {
    std::map<IO_Mode::Flags,std::string> flags = flagsMap();
    std::map<IO_Mode::Flags,std::string>::iterator it;
    std::vector<IO_Mode::Flags> types;

    for (it=flags.begin(); it!=flags.end(); ++it) {
      types.push_back(it->first);
    } 

    return types;
  }

  //_____________________________________________________________________________
  //                                                                    modesName

  std::vector<std::string> IO_Mode::modesName ()
  {
    std::map<IO_Mode::Flags,std::string> flags = flagsMap();
    std::map<IO_Mode::Flags,std::string>::iterator it;
    std::vector<std::string> names;

    for (it=flags.begin(); it!=flags.end(); ++it) {
      names.push_back(it->second);
    } 

    return names;
  }

} // Namespace DAL -- end
