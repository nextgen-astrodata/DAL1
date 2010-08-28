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

#include <dalConversions.h>

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Conversion between time formats
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                                     mjd2unix
  
  /*!
    \param mjd_time -- The time as Modified Julian Date
    
    \return unix -- The time as UNIX seconds
  */
  double mjd2unix (double mjd_time)
  {
    // The Unix base date is MJD 40587.
    // and 1 mjd Day = 24 hours or 1440 minutes or 86400 seconds
    // so (unix seconds) = (mjd seconds) - ( unix base date in seconds )
    return ( mjd_time - (40587.0 * 86400.0) );
  }

  //_____________________________________________________________________________
  //                                                               mjd2unix_boost
  
#ifdef PYTHON
  /*!
    - The Unix base date is MJD 40587.
    - 1 mjd Day = 24 hours or 1440 minutes or 86400 seconds
    - (unix seconds) = (mjd seconds) - ( unix base date in seconds )
    
    \param mjd_time The time as Modified Julian Date.
  */
  bpl::numeric::array mjd2unix_boost ( bpl::numeric::array mjd_time )
  {
    int array_size           = bpl::len( mjd_time );
    double unix_base_time    = 40587;
    double seconds_per_day   = 86400;
    double adjustment_factor = unix_base_time*seconds_per_day;

    for ( int idx=0; idx < array_size; idx++ ) {
      mjd_time[ idx ] = bpl::extract<double>( mjd_time[ idx ] ) - adjustment_factor;
    }
    
    return mjd_time;
  }
#endif
  
} // Namespace DAL -- end
