/***************************************************************************
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

#include <vector>

#include <core/dalObjectBase.h>

namespace DAL {
  
  /*!
    \class dalFilter

    \ingroup DAL
    \ingroup core

    \brief Class representing a filter that can be applied to a table.

    \author Joseph Masters
    \author Lars B&auml;hren
  */
  
  class dalFilter : public dalObjectBase {

    //! Table filter std::string
    std::string itsFilterString;
    //! Book-keeping whether a filter is set or not.
    bool itsFilterIsSet;
    
  public:

    // === Construction =========================================================

    //! Default constructor    
    dalFilter();

    //! Argumented constructor
    dalFilter (dalFileType const &type,
	       std::string columns);
    //! Argumented constructor
    dalFilter (dalFileType const &type,
	       std::string columns,
	       std::string conditions);
    
    // === Methods ==============================================================

    //! Restrict the opening of a table to particular columns.
    bool setFilter (std::string const &columns);
    
    //! Restrict the opening of a table to particular columns.
    bool setFilter (std::vector<std::string> const &columns);
    
    //! Restrict the opening of a table to particular columns and conditions.
    bool setFilter (std::string const &columns,
		    std::string const &conditions);
    
    //! Check to see if the filter is defined.
    inline bool isSet () const {
      return itsFilterIsSet;
    }

    //! Retrieve the filter string.
    inline std::string filterString () {
      return itsFilterString;
    }
    
    //! Retrieve the filter string.
    inline std::string get () {
      return itsFilterString;
    }

    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }
    //! Provide a summary of the internal status
    void summary (std::ostream &os);    
    
  private:

    //! Initialize internal parameters
    void init (dalFileType const &type=dalFileType());
    
  };
  
} // DAL namespace

#endif  // DALFILTER_H
