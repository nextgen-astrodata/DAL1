/*-------------------------------------------------------------------------*
 | $Id:: BeamGroup.h 1126 2007-12-10 17:14:20Z masters                   $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters                                  *
 *   jmasters@science.uva.nl                                               *
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

#ifndef BEAMSUBBAND_H
#include "BeamSubband.h"
#endif

namespace DAL
{

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  // ---------------------------------------------------------------- BeamSubband
  
  BeamSubband::BeamSubband ()
  {
    init();
  }
  
  // ---------------------------------------------------------------- BeamSubband
  /*!
    \param groupID -- HDF5 identifier of the group within which the table is 
           contained
    \param table -- Name of the table to be created/opened.
  */
  BeamSubband::BeamSubband ( hid_t const &groupID,
			     std::string const &table )
  {
    init (groupID,
	  table);
  }
  
  // ---------------------------------------------------------------- BeamSubband

  /*!
    \param dataset -- Dataset within which the table is embedded
    \param group   -- Groug within the dataset within which the table is
           embedded
    \param table -- Name of the table to be opened
  */
  BeamSubband::BeamSubband ( dalDataset &dataset,
			     std::string const &group,
			     std::string const &table )
  {
    init ( dataset,
	   group,
	   table );
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  BeamSubband::~BeamSubband()
  {
    if ( NULL != table_p ) {
      delete table_p;
    }
  }
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  // ----------------------------------------------------------------------- init

  void BeamSubband::init ()
  {
    table_p     = NULL;
    H5groupID_p = -1;
    H5tableID_p = -1;
  }

  // ----------------------------------------------------------------------- init
  
  /*!
    \param groupID -- HDF5 identifier of the group within which the table is 
           contained
    \param table -- Name of the table to be created/opened.
  */
  void BeamSubband::init ( hid_t const &groupID,
			   std::string const &table )
  {
    /* Basic initialization */
    init ();
    
    try {
      dalTable tmp (groupID,table);
      H5tableID_p = tmp.tableID();
    } catch (std::string message) {
      std::cerr << "[BeamSubband::init] ERROR : " << message << std::endl;
    }
  }
  
  // ----------------------------------------------------------------------- init

  /*!
    \param dataset -- Dataset within which the table is embedded
    \param group   -- Groug within the dataset within which the table is
           embedded
    \param table -- Name of the table to be opened
  */
  void BeamSubband::init ( dalDataset &dataset,
			   std::string const &group,
			   std::string const &table )
  {
    /* Basic initialization */
    init ();
    
    try {
      dalTable * newTable = dataset.openTable (group,table);
      table_p = newTable;
    } catch (std::string message) {
      std::cerr << "[BeamSubband::init] ERROR : " << message << std::endl;
    }
  }
  
  // -------------------------------------------------------------------- summary
  
  void BeamSubband::summary (std::ostream &os)
  {
    os << "[BeamSubband] Summary of object properties." << std::endl;
    
    os << "-- HDF5 group handle ID = " << H5groupID_p << std::endl;
    os << "-- HDF5 table handle ID = " << H5tableID_p << std::endl;
    
    if (H5tableID_p > 0) {
      os << "-- Number of table rows = " << table_p->getNumberOfRows()
	 << std::endl;
    }
  }

  // ----------------------------------------------------------- center_frequency

  /*!
    \brief Get the center frequency of the subband

    \return frequency -- The frequency of the subband
  */
  int BeamSubband::center_frequency () const
  {
    int frequency (0);

    if (H5tableID_p > 0) {
      if ( DAL::FAIL == h5get_attribute( frequency,
					 "CENTER_FREQUENCY",
					 H5tableID_p ) ) {
	std::cerr << "-- Error extracting attribute CENTER_FREQUENCY" << endl;
      }
    } else {
      std::cerr << "[BeamSubband::center_frequency] Unable to retrieve attribute;"
		<< " HDF5 table ID not assigned" << std::endl;
    }
    
    return frequency;
  }

  
}

