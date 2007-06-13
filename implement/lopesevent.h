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

/*!
  \file lopesevent.h
  
  \ingroup DAL
  
  \brief Include-file with the descriptions of the lopesevent datastructure
*/

#ifndef LOPESEVENT_H
#define LOPESEVENT_H

#define lopesevent lopesevent_v1

//! Number of channels
#define NUMCHANNELS 30

//! About 80MB maximal eventsize
#define MAXDATASIZE 0x5001000
#define MAXSAMPLESIZE ((MAXDATASIZE-0x1000) / 2 / NUMCHANNELS)

//! The header of a lopesevent is 12 (insigend) intergers long
#define LOPESEV_HEADERSIZE (12*sizeof(int))

#define VERS1 001
#define VERS2 002
#define VERS3 003
#define VERS4 004
#define VERSION VERS3

/*!
  \brief Type of TIM-board data
*/
enum datatypes {
  //! TIM40 board specification
  TIM40 = 1,
  //! TIM80 board specification
  TIM80 = 2
};

/*!
  \brief Class of event data
*/
enum lopesclass {
  //! Cosmic ray data 
  cosmic = 1,
  //! Simulation data
  simulation= 2,
  //! Test data set
  test=3,
  //! Data taken from a solar burst
  solar=4
};

typedef struct {
  //! length of the dataset (in bytes)
  unsigned int length;
  //! LOPES-Event version
  unsigned int version;
  //! KASCADE-style timestamp   
  unsigned int JDR,TL;
  //! Type of data. (currently only TIM-40 supported)
  unsigned int type;
  //! cosmic / simulation / solar-flare etc.
  unsigned int evclass;
  //! Size of one channel (number of shorts)  (optional)
  unsigned int blocksize;
  //! First entry is presync before the sync signal (optional)
  int presync;
  // --- This is the change from version 1 to version 2 ---
  //! 40MHz timestamp from menable card
  unsigned int LTL;
  // --- This is the change from version 2 to version 3 ---
  //! ID of the observatory (0:LOPES; 1:LORUN)
  int observatory;
  unsigned int frei[2];
  //! unstructured raw data with variable length
  short int data[MAXDATASIZE/2];
} lopesevent_v1;

typedef struct {
  //! channel ID: pcnum*10000 + boardnum*100 + chnum
  unsigned int channid;
  //! length of the data (number of shorts)
  unsigned int length;
  //! the data itself.
  short int *data;
} channeldata;


/*!
  \brief definition of transev 

  This is the structure, that will be sent to te KASCADE xevb
*/
typedef struct {
  // Length of the dataset (in bytes)
  unsigned int length;
  unsigned int version;
  // KASCADE-style timestamp   
  unsigned int JDR,TL;
  // Type of data. (currently only TIM-40 supported)
  unsigned int type;
  // cosmic / simulation / solar-flare etc.
  unsigned int evclass;
} transev1;


// KASCADE-xevb stuff
#define LOPES_GRP   11 /* Wie in ?/kascadeinc/xevbdefines.h :  "GroupIndex" */
#define LOPES_EVENT  0 /* Wie in ?/kascadeinc/lopestags.h */
#define LOPES_PORT  14711 /* portnumber, for xevb from lopesdaq. */

#define PID_EVB_KERNEL            0x02
#define TID_EVB                   0x02
#define GID_EVB                   0x00

#endif /* LOPESEVENT_H */
