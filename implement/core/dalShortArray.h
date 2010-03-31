/*-------------------------------------------------------------------------*
 | $Id:: dalArray.h 4393 2010-03-11 20:35:09Z baehren                    $ |
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

#ifndef DALSHORTARRAY_H
#define DALSHORTARRAY_H

#include "dalArray.h"

namespace DAL {

  /*!
    \class dalShortArray

    \ingroup DAL
    \ingroup core

    \brief Represents an n-dimensional array of type \e short

    \author Joseph Masters
  */
  class dalShortArray: public dalArray {
    
  public:
    
    //! Constructor for extendible \e short array.
    dalShortArray (hid_t obj_id,
		   std::string arrayname,
		   std::vector<int> dims,
		   short data[],
		   std::vector<int>chnkdims);

    //! Read data  from the array
    short * readShortArray (hid_t obj_id,
			    std::string arrayname );
  };
  
  
} //  END  --  namespace DAL

#endif //  END  --  #ifndef DALSHORTARRAY_H
