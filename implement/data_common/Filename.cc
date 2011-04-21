/***************************************************************************
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
  
  //_____________________________________________________________________________
  //                                                                     Filename

  Filename::Filename ()
  {
    init ();
  }
  
  //_____________________________________________________________________________
  //                                                                     Filename

  /*!
    \param observationID       -- Unique identifier for the observation
    \param optionalDescription -- Optional descriptors
    \param filetype            -- Marker for the contents of the file
    \param extension           -- Extension of the file
    \param path                -- Path to the location of the file
   */
  Filename::Filename (std::string const &observationID,
		      std::string const &optionalDescription,
		      FileType const &filetype,
		      FileExtension const &extension,
		      std::string const &path)
  {
    setObservationID (observationID);
    setOptionalDescription (optionalDescription);
    setFiletype (filetype);
    setExtension (extension);
    setPath (path);
  }

  //_____________________________________________________________________________
  //                                                                     Filename

  /*!
    \param other -- Another Filename object from which to make a copy.
  */
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
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another Filename object from which to make a copy.

    \return filename -- Pointer to a Filename reference object.
  */
  Filename& Filename::operator= (Filename const &other)
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
    \param other -- Another Filename object from which to make a copy.
  */
  void Filename::copy (Filename const &other)
  {
    observationID_p       = other.observationID_p;
    itsOptionalDescriptor = other.itsOptionalDescriptor;
    itsFiletype           = other.itsFiletype;
    itsExtension           = other.itsExtension;
    itsPath                = other.itsPath;
  }

  // ============================================================================
  //
  //  Parameters
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                       setOptionalDescription
  
  /*!
    \param optionalDescription -- Optional descriptor(s)
    
    Before accepting a new value, a few basic checks need to passed:
    <ul>
      <li>There is no leading underscore ("_").
      <li>Sub-strings are not separated by a hyphen ("-").
    </ul>
  */
  void Filename::setOptionalDescription (std::string const &optionalDescription)
  {
    itsOptionalDescriptor = optionalDescription;
  }
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void Filename::summary (std::ostream &os)
  {
    os << "[Filename] Summary of internal parameters."          << std::endl;
    os << "-- Observation ID       = " << observationID_p       << std::endl;
    os << "-- Optional description = " << itsOptionalDescriptor << std::endl;
    os << "-- File type            = " << getName(itsFiletype)   << std::endl;
    os << "-- File extension       = " << getName(itsExtension)  << std::endl;
    os << "-- Filename             = " << filename()            << std::endl;
    os << "-- File path            = " << path()                << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init

  void Filename::init ()
  {
    observationID_p       = "";
    itsOptionalDescriptor = "";
    itsFiletype           = Filename::uv;
    itsExtension           = Filename::h5;
    itsPath               = "";
  }

  //_____________________________________________________________________________
  //                                                                     filename

  /*!
    \param fullpath -- Provide the full path to the file.
  */
  std::string Filename::filename (bool const &fullpath)
  {
    std::string name ("");

    if (fullpath && itsPath != "") {
      name += itsPath;
      name += "/";
    }

    /* Construct the actual filename out of the individual components */
    name += "L";
    name += observationID_p;
    if (itsOptionalDescriptor != "") {
      name += "_";
      name += itsOptionalDescriptor;
    }
    name += "_";
    name += getName(itsFiletype);
    name += ".";
    name += getName (itsExtension);

    return name;
  }

  //_____________________________________________________________________________
  //                                                                      getName

  /*!
    \param extension -- Extension (suffix) of the file.
    \return name     -- Extension (suffix) of the file.
   */
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

  /*!
    \param filetype -- Marker for the contents of the file.
    \return name    -- The marker for the contents of the file as string.
   */
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
