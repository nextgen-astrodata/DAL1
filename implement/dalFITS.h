/*-------------------------------------------------------------------------*
 | $Id:: TBB_Timeseries.h 2126 2008-11-07 13:31:59Z baehren              $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Sven Duscha ()                                                        *
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

#ifndef DALFITS_H
#define DALFITS_H

#include <iostream>
#include <string>

namespace DAL {
  
  /*!
    \class dalFITS

    \ingroup DAL

    \brief DAL class to provide access to FITS

    \author Sven Duscha

    \test tdalFITS.cpp
  */
  class dalFITS {

  public:

    //________________________________________________________________
    // Construction/Destruction
    
    //! Default constructor
    dalFITS ();
    
    //! Destructor
    ~dalFITS ();
    
    //________________________________________________________________
    // Methods

    //! Get the name of the class
    std::string className () const {
      return "dalFITS";
    }

    //! Provide a summary of the internal status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the internal status
    void summary (std::ostream &os);    

  };
  
}

#endif
