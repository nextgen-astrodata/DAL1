/***************************************************************************
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
  //                                                                       julday
  
  long double julday (time_t seconds,
                      long &intmjd,
                      long double &fracmjd)
  {
    struct tm *ptr = gmtime(&seconds);
    assert (ptr);

    long double dayfrac = 0;
    long double jd      = 0;
    int year            = ptr->tm_year;
    int yday            = ptr->tm_yday + 1;
    int hour            = ptr->tm_hour;
    int min             = ptr->tm_min;
    long double sec     = (long double)ptr->tm_sec;
    unsigned int nd     = 0;

    // fraction of day
    dayfrac = ( (sec/60.0L + (long double) min)/60.0L + (long double)hour)/24.0L;
    nd      = year * 365;
    nd     += (year - 1)/4;
    nd     += yday + 2415020;
    
    intmjd  = nd - 2400001;
    fracmjd = dayfrac;
    
    jd = (long double)nd + dayfrac - 0.5L;

    // release allocated memory
    if (ptr!=NULL) {
      delete ptr;
    }
    
    return jd;
  }
  
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
