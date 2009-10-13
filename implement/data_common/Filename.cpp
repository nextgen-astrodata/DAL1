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

#include <Filename.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  Filename::Filename ()
  {
    init ();
  }
  
  Filename::Filename (std::string const &observationID,
		      std::string const &optionalDescription,
		      FileType const &filetype,
		      FileExtension const &extension)
  {
    setObservationID (observationID);
    setOptionalDescription (optionalDescription);
    setFiletype (filetype);
    setExtension (extension);
  }

  Filename::Filename (Filename const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  Filename::~Filename ()
  {
    destroy();
  }
  
  void Filename::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  Filename& Filename::operator= (Filename const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  void Filename::copy (Filename const &other)
  {
    observationID_p       = other.observationID_p;
    optionalDescription_p = other.optionalDescription_p;
    filetype_p            = other.filetype_p;
    extension_p           = other.extension_p;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================
  
  void Filename::summary (std::ostream &os)
  {
    os << "[Filename] Summary of internal parameters." << std::endl;
    os << "-- Observation ID       = " << observationID_p       << std::endl;
    os << "-- Optional description = " << optionalDescription_p << std::endl;
    os << "-- File type            = " << getName(filetype_p)   << std::endl;
    os << "-- File extension       = " << getName(extension_p)  << std::endl;
    os << "-- Filename             = " << filename()            << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  void Filename::init ()
  {
    observationID_p       = "";
    optionalDescription_p = "";
    filetype_p            = Filename::uv;
    extension_p           = Filename::h5;
  }

  //_____________________________________________________________________________
  //                                                                     filename

  std::string Filename::filename ()
  {
    std::string name ("L");

    name += observationID_p;
    if (optionalDescription_p != "") {
      name += "_";
      name += optionalDescription_p;
    }
    name += "_";
    name += getName(filetype_p);
    name += ".";
    name += getName (extension_p);

    return name;
  }

  //_____________________________________________________________________________
  //                                                                      getName

  std::string Filename::getName (Filename::FileExtension const &extension)
  {
    std::string name ("UNDEFINED");
    
    switch (extension) {
    case MS:
      name = "MS";
      break;
    case h5:
      name = "h5";
      break;
    case fits:
      name = "fits";
      break;
    case log:
      name = "log";
      break;
    case parset:
      name = "parset";
      break;
    case lsm:
      name = "lsm";
      break;
    case IM:
      name = "IM";
      break;
    case PD:
      name = "PD";
      break;
    case vds:
      name = "vds";
      break;
    case gds:
      name = "gds";
      break;
    case conf:
      name = "conf";
      break;
    };
    
    return name;
  }

  //_____________________________________________________________________________
  //                                                                      getName

  std::string Filename::getName (Filename::FileType const &filetype)
  {
    std::string name ("UNDEFINED");
    
    switch (filetype) {
    case uv:
      name = "uv";
      break;
    case sky:
      name = "sky";
      break;
    case rm:
      name = "rm";
      break;
    case nfi:
      name = "nfi";
      break;
    case dynspec:
      name = "dynspec";
      break;
    case bf:
      name = "bf";
      break;
    case tbb:
      name = "tbb";
      break;
    };

    return name;
  }
  
} // Namespace DAL -- end
