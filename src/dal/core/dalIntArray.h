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

#ifndef DALINTARRAY_H
#define DALINTARRAY_H

#include <core/dalArray.h>

namespace DAL {

  /*!
    \class dalIntArray
    \ingroup DAL
    \ingroup core
    \brief Represents an n-dimensional array of type \e int
  */
  class dalIntArray: public dalArray {
    
  public:

    //! Argumented constructor
    dalIntArray (hid_t obj_id,
		 std::string arrayname,
		 std::vector<int> dims,
		 int data[],
		 std::vector<int>chnkdims );
    //! Read array data from object \e obj_id
    int * readIntArray (hid_t obj_id,
			std::string arrayname);
  };
  
}   // END : namespace DAL

#endif

