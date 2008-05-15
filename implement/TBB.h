/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
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

#ifndef TBB_H
#define TBB_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

// socket headers
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace DAL {

/*!
   \class TBB

   \ingroup DAL

   \brief High-level interface between TBB data and the DAL

   \author Joseph Masters
 */

  class TBB {

  string name;
  dalDataset * dataset;
  std::vector<dalGroup> station;

  public:

  TBB( string const& name );  // constructor

  int connectsocket( char* ipaddress, char* portnumber );

  }; // class TBB

// check if an object exists in a vector
template <class T>
bool it_exists( vector<T> vec, T item )
{
  for( unsigned int ss = 0; ss < vec.size(); ss++ )
  {
    if ( item == vec[ss] )
      return true;
  }
  return false;
}

template <class T>
void list_vector_members( vector<T> vec )
{
  for( unsigned int ss = 0; ss < vec.size(); ss++ )
  {
     cerr << vec[ss] << endl;
  }
}

/*
long double julday(time_t seconds,long *intmjd, long double *fracmjd)
{
   long double dayfrac, jd, sec;
   int year, yday;
   int hour, min;
   struct tm *ptr = NULL;

   unsigned int nd;

   ptr = gmtime(&seconds);

   hour = ptr->tm_hour;
   min = ptr->tm_min;
   sec = (long double)ptr->tm_sec;

   year = ptr->tm_year;
   yday = ptr->tm_yday + 1;

   dayfrac = ( (sec/60.0L + (long double) min)/60.0L + \
          (long double)hour)/24.0L;
   nd = year * 365;
   nd += (year - 1)/4;
   nd += yday + 2415020;

   *intmjd = nd - 2400001;
   *fracmjd = dayfrac;

   jd = (long double)nd + dayfrac - 0.5L;

   return jd;
}
*/

const Int32 ETHEREAL_HEADER_LENGTH = 46;
const Int32 FIRST_EXTRA_HDR_LENGTH = 40;
const Int32 EXTRA_HDR_LENGTH = 16;

typedef struct AntennaStruct {
unsigned int frameno;
	unsigned int rsp_id;
	unsigned int rcu_id;
	unsigned int time;
	unsigned int sample_nr;
	unsigned int samples_per_frame;
	char feed[16];
	double ant_position[ 3 ];
	double ant_orientation[ 3 ];
	hvl_t data[1];
} AntennaStruct;

typedef struct writebuffer {
	AntennaStruct antenna;
} writebuffer;

typedef struct TBB_Header {
	unsigned char stationid;
	unsigned char rspid;
	unsigned char rcuid;
	unsigned char sample_freq;
	UInt32 seqnr;
	Int32 time;
	UInt32 sample_nr;
	UInt16 n_samples_per_frame;
	UInt16 n_freq_bands;
	char bandsel[64];
	Int16 spare;
	UInt16 crc;
};

typedef struct TransientSample {
	Int16 value;
};

typedef struct SpectralSample {
	complex<Int16> value;
};

typedef struct CosmicRayStruct {
	//int nofDatapoints;
	Int16 data;
} CosmicRayStruct;

} // DAL namespace


#endif // TBB_H
