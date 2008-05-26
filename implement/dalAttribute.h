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

#ifndef DALATTRIBUTE_H
#define DALATTRIBUTE_H

#ifndef DAL_H
#include "dal.h"
#endif


namespace DAL {

/*!
  \class dalAttribute

  \brief Represents a header keyword or attribute.

  \ingroup DAL
  
  \author Joseph Masters
*/
class dalAttribute {
  
  //! The name of the attribute
  string name_p;
  //! The units of the attribute
  string units_p;
  //! Description of the column
  string description_p;
  
 public:
  
  /*!
    \brief Default constructor.
    
    Default attribute constructor.
  */
  dalAttribute();

  /*!
    \brief Get the name of the attribute

    \return name -- The name of the attribute
  */
  inline string name () const {
    return name_p;
  }

  /*!
    \brief Set the name of the attribute

    \param name -- The name of the attribute

    \return status -- Status of the operation; returns <tt>false</tt> in case an
            error was encountered
  */
  inline bool setName (string const &name) {
    name_p = name;
    return true;
  }
  
  /*!
    \brief Get the units of the attribute

    \return units -- The units of the attribute
  */
  inline string units () const {
    return units_p;
  }

  /*!
    \brief Set the units of the attribute

    \param units -- The units of the attribute

    \return status -- Status of the operation; returns <tt>false</tt> in case an
            error was encountered
  */
  inline bool setUnits (string const &units) {
    units_p = units;
    return true;
  }
  
  /*!
    \brief Get the description to the attribute

    \return description -- The description to the attribute
  */
  inline string description () const {
    return description_p;
  }

  /*!
    \brief Set the description of the attribute

    \param description -- The description of the attribute

    \return status -- Status of the operation; returns <tt>false</tt> in case an
            error was encountered
  */
  inline bool setDescription (string const &description) {
    description_p = description;
    return true;
  }
  
}; // dalAttribute class


} // DAL namespace

#endif  // DALATTRIBUTE_H
