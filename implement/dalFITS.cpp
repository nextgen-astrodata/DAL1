/*-------------------------------------------------------------------------*
 | $Id:: dalFITS.h 2126 2008-11-07 13:31:59Z baehren              $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Sven Duscha (sduscha@mpa-garching.mpg.de)                                                        *
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

#include "dalFITS.h"

using namespace std;

namespace DAL
{
  // ============================================================================
  //
  //  Construction / Destruction
  //
  // ============================================================================
  
  //! Default constructor
  dalFITS::dalFITS()
  {
  
  
  }


  //! Constructor with associated filename
  dalFITS::dalFITS(const string &filename) 
  {

  
  
  
  }


  
  dalFITS::~dalFITS() 
  {
    // close the FITS file
  
    // deallocate memory
  
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  /*!
    Header access functions (specific to RM)
  */
  
  


 
  /*!
    Functions to set FITS object parameters that were not declared inline
  */

  



  /*!
      \param os -- Output stream to which the summary is going to be written
  */

  void dalFITS::summary (std::ostream &os)
  {
    os << "[dalFITS] Summary of object properties" << std::endl;
    os << "-- Filename : " << << std::endl;
  }
  
}
