/***************************************************************************
 *   Copyright (C) 2006                                                    *
 *   Andreas Horneffer (<mail>)                                            *
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

/* $Id: dalLopesEvent.cc,v 1.2 2007/03/07 17:56:44 bahren Exp $*/

#include <dalLopesEvent.h>

// ============================================================================
//
//  Construction
//
// ============================================================================

dalLopesEvent::dalLopesEvent()
{
  init();
}

dalLopesEvent::dalLopesEvent (string filename)
{
  init();
  attachFile(filename);
}

void dalLopesEvent::init()
{
  nofAntennas_p = 0;
  filename_p    = "";
  attached_p    = false;
  headerpoint_p = (lopesevent_v1*)malloc(LOPESEV_HEADERSIZE);
}

// ============================================================================
//
//  Destruction
//
// ============================================================================

dalLopesEvent::~dalLopesEvent()
{
  destroy();
}

void dalLopesEvent::destroy()
{
  // release the memory allocated for the header structure
  free(headerpoint_p);
}

// ============================================================================
//
//  Operators
//
// ============================================================================



// ============================================================================
//
//  Methods
//
// ============================================================================

bool dalLopesEvent::attachFile (string filename)
{
  int i (0);
  unsigned int ui (0);
  unsigned int tmpchan,tmplen;
  short *tmppoint;
  
  FILE *fd = fopen(filename.c_str(),"r");
  if (fd == NULL) {
    cerr << "LOPESEventIn:attachFile: Can't open file: " << filename << endl;
    return false;
  };
  attached_p = false;
  ui = fread(headerpoint_p, 1, LOPESEV_HEADERSIZE, fd);
  if ( (ui != LOPESEV_HEADERSIZE ) || (headerpoint_p->type != TIM40) ) {
    cerr << "LOPESEventIn:attachFile: Inconsitent file: " << filename << endl;
    fclose(fd);
    return false;
  };
  nofAntennas_p =0;
  fread(&tmpchan, 1, sizeof(unsigned int), fd);
  while (!feof(fd)){
    fread(&tmplen, 1, sizeof(unsigned int), fd); 
    if (headerpoint_p->blocksize==0) {headerpoint_p->blocksize = tmplen;};
    if (headerpoint_p->blocksize != tmplen) {
      cerr << "LOPESEventIn:attachFile: Inconsitent file (different blocksizes): "
	   << filename
	   << endl;
      fclose(fd);
      return false;
    };
    i = fseek(fd,tmplen*sizeof(short),SEEK_CUR);
    if (i != 0) {
      cerr << "LOPESEventIn:attachFile: Inconsitent file (unexpected end): "
	   << filename 
	   << " in channel " << tmpchan
	   << " len:"        << tmplen
	   << endl;
      fclose(fd);
      return false;
    };
    nofAntennas_p++;
    
     //this will trigger eof condition after last channel;
    fread(&tmpchan, 1, sizeof(unsigned int), fd);
  };
  try {
    AntennaIDs_p.resize(nofAntennas_p);
    channeldata_p.resize(headerpoint_p->blocksize,nofAntennas_p);
    tmppoint = (short*)malloc(headerpoint_p->blocksize*sizeof(short));
    if (tmppoint == NULL) {
      cerr << "LOPESEventIn:attachFile: Error while allocating temporary memory " << endl;
      fclose(fd);
      return false;
      
    };
    //    clearerr(fd);
    
    // this should reset the eof condition
    fseek(fd,LOPESEV_HEADERSIZE,SEEK_SET);
    for (i=0;i<nofAntennas_p;i++){
      fread(&tmpchan, 1, sizeof(unsigned int), fd); 
      fread(&tmplen, 1, sizeof(unsigned int), fd); 
      AntennaIDs_p(i) = (int)tmpchan;
      fread(tmppoint, sizeof(short),headerpoint_p->blocksize , fd); 
      // channel data for antenna i go into column i
      // 	channeldata_p(Range(Range::all()),i) = Array<short,1>(IPosition(1,headerpoint_p->blocksize),tmppoint,SHARE);
      channeldata_p(Range(Range::all()),i) = Array<short,1>(tmppoint,shape(headerpoint_p->blocksize),neverDeleteData);
    };
    filename_p = filename;
    attached_p = true;
  } catch (string message) {
    cerr << "LOPESEventIn:attachFile: " << message << endl;
    return false;
  }; 
  return true;
}


// ============================================================================
//
//  Parameters
//
// ============================================================================

void dalLopesEvent::summary (std::ostream &os)
{
  // data available as individual data elements
  os << "-- Filename                        : " << filename_p            << endl;
  os << "-- Object attached to file?          " << attached_p            << endl;
  os << "-- nof. antennas in the file       : " << nofAntennas()         << endl;
  os << "-- Antenna IDs                     : " << AntennaIDs_p          << endl;
  os << "-- Shape of the channel data array : " << channeldata_p.shape() << endl;

  // data stored within the header-data structure
  os << "-- LOPES-Event version             : " << version()              << endl;
  os << "-- Length of the dataset [Bytes]   : " << length()               << endl;
  os << "-- Timestamp (KASCADE)         JDR : " << timestampJDR()         << endl;
  os << "                                TL : " << timestampTL()          << endl;
  os << "-- Type of data                    : " << dataType()             << endl;
  os << "-- Type of stored event            : " << eventClass()           << endl;
  os << "-- Size of one channel             : " << blocksize()            << endl;
  os << "-- Presync                         : " << presync()              << endl;
  os << "-- Timestamp from the manable card : " << timestampLTL()         << endl;
  os << "-- ID of the observatory           : " << observatory()          << endl;

}
