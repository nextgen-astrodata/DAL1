/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
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

#include <AttributesProject.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                            AttributesProject
  
  AttributesProject::AttributesProject ()
  {
    itsProjectID      = "UNDEFINED";
    itsProjectTitle   = "UNDEFINED";
    itsProjectPI      = "UNDEFINED";
    itsProjectCoI     = "UNDEFINED";
    itsProjectContact = "UNDEFINED";
  }

  //_____________________________________________________________________________
  //                                                            AttributesProject
  
  /*!
    \param projectID      -- Unique identifier for the project.
    \param projectTitle   -- Name of the project.
    \param projectPI      -- Name of the project's principal investigator.
    \param projectCoI     -- Name(s) of the project's co-PI(s).
    \param projectContact -- Names/Email-addresses of the project's primary
           contact person(s).
   */
  AttributesProject::AttributesProject (std::string const &projectID,
					std::string const &projectTitle,
					std::string const &projectPI,
					std::string const &projectCoI,
					std::string const &projectContact)
  {
    itsProjectID      = projectID;
    itsProjectTitle   = projectTitle;
    itsProjectPI      = projectPI;
    itsProjectCoI     = projectCoI;
    itsProjectContact = projectContact;
  }
  
  //_____________________________________________________________________________
  //                                                            AttributesProject
  
  /*!
    \param other -- Another HDF5Property object from which to create this new
           one.
  */
  AttributesProject::AttributesProject (AttributesProject const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  AttributesProject::~AttributesProject ()
  {
    destroy();
  }
  
  void AttributesProject::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  /*!
    \param other -- Another AttributesProject object from which to make a copy.
  */
  AttributesProject& AttributesProject::operator= (AttributesProject const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void AttributesProject::copy (AttributesProject const &other)
  {
    itsProjectID      = other.itsProjectID;
    itsProjectTitle   = other.itsProjectTitle;
    itsProjectPI      = other.itsProjectPI;
    itsProjectCoI     = other.itsProjectCoI;
    itsProjectContact = other.itsProjectContact;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                      summary
  
  /*!
    \param os -- Output stream to which the summary is written.
  */
  void AttributesProject::summary (std::ostream &os)
  {
    os << "[AttributesProject] Summary of internal parameters." << std::endl;
    os << "-- Project ID      = " << itsProjectID      << std::end;
    os << "-- Project title   = " << itsProjectTitle   << std::end;
    os << "-- Project PI      = " << itsProjectPI      << std::end;
    os << "-- Project CoI     = " << itsProjectCoI     << std::end;
    os << "-- Project contact = " << itsProjectContact << std::end;
  }

  //_____________________________________________________________________________
  //                                                                      h5write
  
#ifdef HAVE_HDF5
  
  void AttributesProject::h5write (hid_t const &groupID)
  {
    DAL::h5set_attribute( groupID, "PROJECT_ID",       itsProjectID );
    DAL::h5set_attribute( groupID, "PROJECT_TITLE",    itsProjectTitle );
    DAL::h5set_attribute( groupID, "PROJECT_PI",       itsProjectPI );
    DAL::h5set_attribute( groupID, "PROJECT_CO_I",     itsProjectCoI );
    DAL::h5set_attribute( groupID, "PROJECT_CONTACT",  itsProjectContact );
  }
  
  void AttributesProject::h5read (hid_t const &groupID)
  {
    DAL::h5get_attribute( groupID, "PROJECT_ID",       itsProjectID );
    DAL::h5get_attribute( groupID, "PROJECT_TITLE",    itsProjectTitle );
    DAL::h5get_attribute( groupID, "PROJECT_PI",       itsProjectPI );
    DAL::h5get_attribute( groupID, "PROJECT_CO_I",     itsProjectCoI );
    DAL::h5get_attribute( groupID, "PROJECT_CONTACT",  itsProjectContact );
  }
  
#endif

} // Namespace DAL -- end
