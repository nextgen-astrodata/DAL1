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

#include <ITS_ExperimentMeta.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                           ITS_ExperimentMeta

  ITS_ExperimentMeta::ITS_ExperimentMeta ()
  {
    init ();
  }

  //_____________________________________________________________________________
  //                                                           ITS_ExperimentMeta

  /*!
    \param metafile -- Location of the "experiment.meta" file for an observation
  */
  ITS_ExperimentMeta::ITS_ExperimentMeta (std::string const &metafile)
  {
    init ();
    readMetafile (metafile);
  }
  
  //_____________________________________________________________________________
  //                                                           ITS_ExperimentMeta

  ITS_ExperimentMeta::ITS_ExperimentMeta (ITS_ExperimentMeta const &other)
  {
    copy (other);
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  ITS_ExperimentMeta::~ITS_ExperimentMeta ()
  {
    destroy();
  }
  
  void ITS_ExperimentMeta::destroy ()
  {;}
  
  // ============================================================================
  //
  //  Operators
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                    operator=
  
  ITS_ExperimentMeta& ITS_ExperimentMeta::operator= (ITS_ExperimentMeta const &other)
  {
    if (this != &other) {
      destroy ();
      copy (other);
    }
    return *this;
  }
  
  //_____________________________________________________________________________
  //                                                                         copy
  
  void ITS_ExperimentMeta::copy (ITS_ExperimentMeta const &other)
  {
    metafile_p       = other.metafile_p;
    description_p    = other.description_p;
    experimentType_p = other.experimentType_p;
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
  void ITS_ExperimentMeta::summary (std::ostream &os)
  {
    os << "[ITS_ExperimentMeta] Summary of internal parameters." << std::endl;
    os << "-- Experiment metafile    = " << metafile_p       << std::endl;
    os << "-- Experiment description = " << description_p    << std::endl;
    os << "-- Experiment type        = " << experimentType_p << std::endl;
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                         init
  
  void ITS_ExperimentMeta::init ()
  {
    metafile_p       = "UNDEFINED";
    description_p    = "UNDEFINED";
    experimentType_p = "UNDEFINED";
  }
  
  //_____________________________________________________________________________
  //                                                                 readMetafile
  
  /*!
    \param metafile -- Location of the "experiment.meta" file for an observation

    \return status -- Status of the operation; returns \e false in case an error
            was encountered.
  */
  bool ITS_ExperimentMeta::readMetafile (std::string const &metafile)
  {
    bool status (true);

    // if operations were successful, store the name of metafile
    if (status) {
      metafile_p = metafile;
    } else {
      init ();
    }
    
    return status;
  }
  
} // Namespace DAL -- end
