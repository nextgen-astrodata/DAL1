//#  BFRawFormat.h
//#
//#  Copyright (C) 2009
//#  ASTRON (Netherlands Foundation for Research in Astronomy)
//#  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, jong@astron.nl
//#
//#  This program is free software; you can redistribute it and/or modify
//#  it under the terms of the GNU General Public License as published by
//#  the Free Software Foundation; either version 2 of the License, or
//#  (at your option) any later version.
//#
//#  This program is distributed in the hope that it will be useful,
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#  GNU General Public License for more details.
//#
//#  You should have received a copy of the GNU General Public License
//#  along with this program; if not, write to the Free Software
//#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#
//#  $Id: BFRawFormat.h 12611 2009-03-11 14:04:07Z de jong $

#ifndef BFRAWFORMAT_H
#define BFRAWFORMAT_H

#include <complex>

namespace DAL {

  /*!
    \file BFRawFormat.h
    
    \ingroup DAL
    
    \brief Definition of the format of the BFRaw data type
    
    \author Alwin de Jong

    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>DAL::BFRaw
    </ul>
    <h3>Synopsis</h3>
    
    This class BFRawFormat defines the format of the BFRaw data type.
    
    This class is kept separate from the actual BFRaw class to seperate the data
    format from the functionality added to the BFRaw data type defined in the
    BFRaw class while making sure that there is only one location where the
    format is defined. The BFRaw class is derived from this class.
  */
  
  class BFRawFormat {
    
  public:
    
    static const short maxNrSubbands = 62;
    
    //! Components of the BFRaw header
    struct BFRaw_Header
    {
      //! 0x3F8304EC, also determines endianness
      uint32_t    magic;
      //! The number of bits per sample
      uint8_t     bitsPerSample;
      //! The number of polarizations
      uint8_t     nrPolarizations;
      //! Number of subbands
      uint16_t    nrSubbands;
      //! 155648 (160Mhz) or 196608 (200Mhz)
      uint32_t    nrSamplesPerSubband;
      //! Name of the station
      char      station[20];
      //! The sample rate: 156250.0 or 195312.5 .. double
      double   sampleRate;
      //! The frequencies within a subband
      double   subbandFrequencies[maxNrSubbands];
      //! The beam pointing directions
      double   beamDirections[8][2];
      //! mapping from subbands to beams
      int16_t     subbandToBeamMapping[maxNrSubbands];
      //! Padding to circumvent 8-byte alignment
      uint32_t    padding;
    } header;
    
    //! Components of the header of a single block of raw data
    struct BlockHeader
    {
      //! 0x2913D852
      uint32_t      magic;
      int32_t       coarseDelayApplied[8];
      //! Padding to circumvent 8-byte alignment
      uint8_t       padding[4];
      double     fineDelayRemainingAtBegin[8];
      double     fineDelayRemainingAfterEnd[8];
      //! Compatible with TimeStamp class.
      int64_t      time[8];
      
      //		uint32_t      nrFlagsRanges[8];
      /*    
	    struct range
	    {
	    uint32_t    begin; // inclusive
	    uint32_t    end;   // exclusive
	    } flagsRanges[8][16];
      */
      
      struct marshalledFlags {
	uint32_t      nrFlagsRanges;
	struct range
	{
	  uint32_t    begin; // inclusive
	  uint32_t    end;   // exclusive
	} flagsRanges[16];
      } flags[8];
      
    } block_header;
    
    
    // dataStruct is 8 bytes
    
    struct Sample
    {
      std::complex<int16_t> xx;
      std::complex<int16_t> yy;
    };
    
    
  public:
    //! Get the number of bits per sample
    inline uint8_t bitsPerSample () const { return this->header.bitsPerSample; }
    //! Get the number of polarizations
    inline uint8_t nofPolarizations () const { return this->header.nrPolarizations; }
    //! Get the number of beamlets
    inline uint16_t nofSubbands () const { return this->header.nrSubbands;}
    //! Get the number of samples per beamlet
    inline uint32_t nofSamplesPerSubband () const { return this->header.nrSamplesPerSubband;}
    //! Get the sample rate
    inline double sampleRate () const { return this->header.sampleRate;}
    
  };
  
}
#endif  // BFRAWFORMAT_H
