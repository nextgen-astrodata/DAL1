/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007 by Joseph Masters                                  *
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
#ifndef DALFILTER_H
#define DALFILTER_H

#include "dalBaseTypes.h"
#include "dalFileType.h"

namespace DAL {
  
  /*!
    \class dalFilter

    \ingroup DAL
    \ingroup core

    \brief Class representing a filter that can be applied to a table.

    \author Joseph Masters, Lars B&auml;hren
  */
  
  class dalFilter {

    //! Table filter std::string
    std::string filterstring_p;
    //! File type: MSCASA, HDF5, FITS, etc.
    dalFileType filetype_p;
    //! Book-keeping whether a filter is set or not.
    bool filterIsSet_p;
    
  public:

    // === Construction =========================================================

    //! Default constructor    
    dalFilter();
    //! Argumented constructor
    dalFilter (DAL::dalFileType const &type,
	       std::string columns);
    //! Argumented constructor
    dalFilter( std::string type,
	       std::string columns);
    //! Argumented constructor
    dalFilter (DAL::dalFileType const &type,
	       std::string columns,
	       std::string conditions);
    //! Argumented constructor
    dalFilter( std::string type,
	       std::string columns,
	       std::string conditions);
    
    // === Methods ==============================================================

    //! Restrict the opening of a table to particular columns.
    bool set (std::string columns);

    //! Restrict the opening of a table to particular columns.
    bool set (std::vector<std::string> const &columns);

    //! Restrict the opening of a table to particular columns and conditions.
    void set (std::string columns,
	      std::string conditions);

    //! Get the type of the file
    inline std::string filetype () {
      return filetype_p.name();
    }

    //! Declare the type of the file
    bool setFiletype (DAL::dalFileType const &type);

    //! Declare the type of the file (i.e. "MSCASA", "HDF5", etc.)
    bool setFiletype (std::string const &type);

    //! Check to see if the filter is defined.
    inline bool isSet () const {
      return filterIsSet_p;
    }

    //! Retrieve the filter string.
    inline std::string get () {
      return filterstring_p;
    }

    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the internal status
    void summary (std::ostream &os);    
    
  private:

    //! Initialize internal parameters
    void init ();
    
  };
  
} // DAL namespace

#endif  // DALFILTER_H
