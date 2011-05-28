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
  
  //_____________________________________________________________________________
  //                                                                      IO_Mode
  
  IO_Mode::IO_Mode ()
  {
    setFlag (IO_Mode::Open);
    addFlag (IO_Mode::ReadOnly);
  }

  //_____________________________________________________________________________
  //                                                                      IO_Mode
  
  IO_Mode::IO_Mode (IO_Mode::Flags const &flag)
  {
    itsFlags = flag;
    verifyFlags (itsFlags,true);
  }
  
  //_____________________________________________________________________________
  //                                                                      IO_Mode
  
  IO_Mode::IO_Mode (int const &flags)
  {
    itsFlags = flags;
    verifyFlags (itsFlags,true);
  }

  //_____________________________________________________________________________
  //                                                                      IO_Mode
  
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
    std::vector<std::string> names = flagDescriptions();
    unsigned int nofNames          = names.size();

    os << "[IO_Mode] Summary of internal parameters." << std::endl;
    os << "-- I/O mode value    = " << itsFlags << std::endl;

    os << "-- Flag descriptions = [";
    for (unsigned int n=0; n<nofNames; ++n) {
      os << " " << names[n];
    }
    os << " ]" << std::endl;
  }
  
  // ============================================================================
  //
  //  Public methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                             flagDescriptions

  std::vector<std::string> IO_Mode::flagDescriptions ()
  {
    return flagDescriptions (itsFlags);
  }
  
  //_____________________________________________________________________________
  //                                                                      addFlag

  /*!
    \param flag    -- 
    \return status -- 
  */
  bool IO_Mode::addFlag (IO_Mode::Flags const &flag)
  {
    itsFlags = itsFlags | flag;
    return verifyFlags (itsFlags);
  }

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
  //                                                                    flagsType

  std::vector<IO_Mode::Flags> IO_Mode::flagsType ()
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

  //_____________________________________________________________________________
  //                                                             flagDescriptions

  std::vector<std::string> IO_Mode::flagDescriptions (IO_Mode::Flags const &flag)
  {
    return flagDescriptions(flag);
  }

  //_____________________________________________________________________________
  //                                                             flagDescriptions

  /*!
    \param flags  -- 
    \return names -- 
  */
  std::vector<std::string> IO_Mode::flagDescriptions (int const &flags)
  {
    std::map<IO_Mode::Flags,std::string> m = flagsMap();
    std::map<IO_Mode::Flags,std::string>::iterator it;
    std::vector<std::string> names;

    for (it=m.begin(); it!=m.end(); ++it) {
      /* Check if the flags is set */
      if ( (flags & it->first) == it->first ) {
	names.push_back(it->second);
      }
    }

    return names;
  }
  
  //_____________________________________________________________________________
  //                                                                  verifyFlags

  /*!
    \param flags        -- 
    \param correctFlags -- Attempt to correct the flags.
    \return status      -- Status of the operation; returns \c false in case an
            eror was encountered, i.e. the provided combination of flags is 
	    considered to be incorrect.
  */
  bool IO_Mode::verifyFlags (int &flags,
			     bool const &correctFlags)
  {
    bool status = true;

    return status;
  }

} // Namespace DAL -- end
