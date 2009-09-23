/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Andreas Horneffer (A.Horneffer@astro.ru.nl)                           *
 *   Lars B"ahren (bahren@astron.nl)                                       *
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

#include <TBBraw.h>

namespace DAL {  // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Construction, destruction, etc
  //
  // ============================================================================
  
  //_____________________________________________________________________________
  //                                                                       TBBraw
  
  TBBraw::TBBraw ()
  {
    init();
  }

  //_____________________________________________________________________________
  //                                                                       TBBraw
  
  TBBraw::TBBraw (string const &filename,
		  string const &observer,
		  string const &project,
		  string const &observation_id,
		  string const &observationMode,
		  string const &telescope)
  {
    init();
    open_file (filename,
	       observer,
	       project,
	       observation_id,
	       observationMode,
	       telescope);
  }
  
  //_____________________________________________________________________________
  //                                                                      ~TBBraw
  
  TBBraw::~TBBraw()
  {
    destroy();
  }

  //_____________________________________________________________________________
  //                                                                         init
  
  void TBBraw::init ()
  {
    bigendian_p    = BigEndian();
    dataset_p      = NULL;
    do_headerCRC_p = true;
    do_dataCRC_p   = false;

    fixTimes_p           = 2;
    nofProcessed_p       = 0;
    nofDiscardedHeader_p = 0;

    //initialize the buffers
    int i;
    stationBuf = new stationBufElem [MAX_NO_STATIONS];
    for (i=0; i<MAX_NO_STATIONS; i++)
      {
        stationBuf[i].ID =0;
        stationBuf[i].group = NULL;
      };
    dipoleBuf = new dipoleBufElem [MAX_NO_DIPOLES];
    for (i=0; i<MAX_NO_DIPOLES; i++)
      {
        dipoleBuf[i].ID = 0;
        dipoleBuf[i].array = NULL;
      };
    
  }

  //_____________________________________________________________________________
  //                                                                    open_file
  
  bool TBBraw::open_file (string const &filename,
			  string const &observer,
			  string const &project,
			  string const &observation_id,
			  string const &observationMode,
			  string const &telescope)
  {
    struct stat filestat;
    
    if ((stat(filename.c_str(), &filestat) != 0) && (errno == ENOENT))
      {
        cout << "TBBraw::open_file: Creating new dataset " << filename << endl;
        if (dataset_p != NULL)
          {
#ifdef DEBUGGING_MESSAGES
            cout << "TBBraw::open_file: Opening a file for the second time. This is deprecated." << endl;
#endif
            // We are already connected to a file, so delete everything, and initialize everything.
            destroy();
            init();
          };
        dataset_p = new dalDataset( filename.c_str(), "HDF5" );
        if (dataset_p != NULL)
          {
            dataset_p->setAttribute( attribute_name(TELESCOPE), telescope);
            dataset_p->setAttribute( attribute_name(OBSERVER), observer);
            dataset_p->setAttribute( attribute_name(PROJECT), project );
            dataset_p->setAttribute( attribute_name(OBSERVATION_ID), observation_id);
            dataset_p->setAttribute( attribute_name(OBSERVATION_MODE), observationMode );
            return true;
          }
        else
          {
            cerr << "TBBraw::open_file: Error during creation of dataset." << endl;
          };
      }
    else
      {
        cerr << "TBBraw::open_file: Error \"stat-ing\" filename. File already exists?" << endl;
      };
    return false;
  };
  
  //_____________________________________________________________________________
  //                                                                      destroy
  
  void TBBraw::destroy()
  {
    int i;
    for (i=0; i<MAX_NO_DIPOLES; i++)
      {
        if ( dipoleBuf[i].array != NULL )
          {
            dipoleBuf[i].array->close();
            delete dipoleBuf[i].array;
          };
      };
    for (i=0; i<MAX_NO_STATIONS; i++)
      {
        if ( stationBuf[i].group != NULL )
          {
            stationBuf[i].group->close();
            delete stationBuf[i].group;
          };
      };
    if (dataset_p != NULL)
      {
        delete dataset_p;
        dataset_p=NULL;
      };
    delete [] stationBuf;
    delete [] dipoleBuf;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                           processTBBrawBlock
  
  bool TBBraw::processTBBrawBlock (char *inbuff,
				   int datalen,
				   bool bigEndian)
  {
    TBB_Header *headerp;

    if (bigEndian)
      {
        cout << "TBBraw::processTBBrawBlock: Big endian support is untested! "
             << "If you actually need it, test it first!!!" << endl;
      };
    if (datalen < TBB_FRAME_SIZE)
      {
        cerr << "TBBraw::processTBBrawBlock: Block too small! datalen: " << datalen << endl;
        return false;
      };
    nofProcessed_p++;
    headerp = (TBB_Header*)inbuff;

    if ( bigendian_p != bigEndian )
      {
        swapbytes( (char *)&(headerp->seqnr), 4 );
        swapbytes( (char *)&(headerp->time), 4 );
        swapbytes( (char *)&(headerp->sample_nr), 4 );
        swapbytes( (char *)&(headerp->n_samples_per_frame), 2);
        swapbytes( (char *)&(headerp->n_freq_bands), 2 );
        swapbytes( (char *)&(headerp->spare), 2 );
        swapbytes( (char *)&(headerp->crc), 2 );
      };

    if (do_headerCRC_p && !checkHeaderCRC(headerp))
      {
        nofDiscardedHeader_p++;
        return false;
      };

    if (headerp->n_freq_bands != 0)
      {
        cerr << "TBBraw::processTBBrawBlock: Can only process raw(=transient) data!" << endl;
        return false;
      };

    if (fixTimes_p==2)
      {
        fixDateNew(headerp);
      }
    else if (fixTimes_p==1)
      {
        fixDateOld(headerp);
      };

    int index = getDipoleIndex(headerp);
    if ((index<0) || (index>=MAX_NO_DIPOLES))
      {
        cerr << "TBBraw::processTBBrawBlock: Failed to get Dipole Index!" << endl;
        return false;
      };

    if (!addDataToDipole( index, inbuff, datalen, bigEndian))
      {
        return false;
      }

    return true;
  };

  //_____________________________________________________________________________
  //                                                                      summary
  
  void TBBraw::summary (std::ostream &os)
  {
    os << "[TBB_Timeseries] Summary of internal parameters"              << endl;
    // Basic properties and parameters
    os << "-- Is the system big-endian ..... : " << bigendian_p          << endl;
    os << "-- Check the header-CRC ......... : " << do_headerCRC_p       << endl;
    os << "-- Check the data-CRC ........... : " << do_dataCRC_p         << endl;
    os << "-- Fix broken time-stamps ....... : " << fixTimes_p           << endl;
    // Processing statistics
    os << "-- nof. processed data blocks ... : " << nofProcessed_p       << endl;
    os << "-- nof. blocks with broken header : " << nofDiscardedHeader_p << endl;
    os << "-- nof. blocks written to file .. : "
       << (nofProcessed_p-nofDiscardedHeader_p) << endl;
  }

  // ============================================================================
  //
  //  Private Methods
  //
  // ============================================================================

  //_____________________________________________________________________________
  //                                                               checkHeaderCRC
  
  /*!
    Check the CRC of a TBB frame header. Uses CRC16. Returns TRUE if OK, FALSE
    otherwise.
  */
  bool TBBraw::checkHeaderCRC(TBB_Header *headerp)
  {
    unsigned int seqnr = headerp->seqnr; // temporary; we need to zero this out for CRC check
    headerp->seqnr = 0;

    uint16_t * headerBuf = reinterpret_cast<uint16_t*> (headerp);
    uint16_t CRC = DAL::crc16(headerBuf, sizeof(TBB_Header) / sizeof(uint16_t));
    headerp->seqnr = seqnr; // and set it back again

    return (CRC == 0);
  }

  //_____________________________________________________________________________
  //                                                                   fixDateOld
  
  void TBBraw::fixDateOld(TBB_Header *headerp)
  {
    static bool oddSecond = false;
    if (headerp->sample_freq == 200)
      {
        if ((headerp->time%2)!=0)
          {
            if (headerp->sample_nr == 199999488)
              {
                headerp->time -= 1;
              }
            else
              {
                headerp->sample_nr += 512;
                oddSecond = true;
              };
          }
        else if (oddSecond && (headerp->sample_nr == 199998464))
          {
            headerp->time -= 1;
            headerp->sample_nr += 512;
          }
        else
          {
            oddSecond = false;
          };
      }
    else if (headerp->sample_freq == 160)
      {
        if (headerp->sample_nr == 159998976)
          {
            headerp->time -= 1;
          };
      }
    else
      {
        cerr << "TBBraw::fixDateOld: Unsupported samplerate!!!" << endl;
      };
  };

  //_____________________________________________________________________________
  //                                                                   fixDateNew
  
  void TBBraw::fixDateNew(TBB_Header *headerp)
  {
    if (headerp->sample_freq == 200)
      {
        if ((headerp->time%2)!=1)
          {
            headerp->sample_nr += 512;
          };
      }
    else if (headerp->sample_freq == 160)
      {
      }
    else
      {
        cerr << "TBBraw::fixDateNew: Unsupported samplerate!!!" << endl;
      };
  };

  //_____________________________________________________________________________
  //                                                               getDipoleIndex
  
  int TBBraw::getDipoleIndex(TBB_Header *headerp)
  {
    int i, dipoleIndex=-1;
    unsigned int dipoleID;

    dipoleID = headerp->stationid*1000000 + headerp->rspid*1000 + headerp->rcuid;

    for (i=0; i<MAX_NO_DIPOLES; i++)
      {
        if ( dipoleBuf[i].ID == dipoleID)
          {
            dipoleIndex=i;
            break;
          };
        if (dipoleBuf[i].array == NULL)
          {
            break;
          };
      }
    if (dipoleIndex != -1)
      {
        return dipoleIndex;
      }
    else
      {
        return createNewDipole(headerp);
      };
  };

  //_____________________________________________________________________________
  //                                                              createNewDipole
  
  int TBBraw::createNewDipole(TBB_Header *headerp)
  {
    int i, stationIndex=-1, dipoleIndex=-1;
    unsigned int stationID, dipoleID;

    // find the corresponding staion index
    stationID = headerp->stationid;
    for (i=0; i<MAX_NO_STATIONS; i++)
      {
        if ( stationBuf[i].ID == stationID)
          {
            stationIndex=i;
            break;
          };
        if (stationBuf[i].group == NULL)
          {
            break;
          };
      }
    if (stationIndex == -1)
      {
        stationIndex = createNewStation(headerp);
      };
    if (stationIndex == -1)
      {
        cerr << "TBBraw::createNewDipole: createNewStation() returned -1!" << endl;
        return -1;
      };
    // find an empty dipole index
    for (dipoleIndex=0; dipoleIndex<MAX_NO_DIPOLES; dipoleIndex++)
      {
        if (dipoleBuf[dipoleIndex].array == NULL)
          {
            break;
          };
      };
    if (dipoleIndex == MAX_NO_DIPOLES)
      {
        cerr << "TBBraw::createNewDipole: Buffer \"dipoleBuf\" is full, cannot create new dipole!" <<endl;
        return -1;
      };

    // Now we have the station and dipole index -> create the dipole

    vector<int> firstdims(1,0);
    vector<int> cdims(1,CHUNK_SIZE);
    short nodata[0];

    char newDipoleIDstr[10];
    sprintf(newDipoleIDstr, "%03d%03d%03d", headerp->stationid, headerp->rspid, headerp->rcuid);
    dipoleBuf[dipoleIndex].array =  //see next line
      stationBuf[stationIndex].group->createShortArray( newDipoleIDstr, firstdims, nodata, cdims );

    dipoleID = headerp->stationid*1000000 + headerp->rspid*1000 + headerp->rcuid;
    dipoleBuf[dipoleIndex].ID = dipoleID;
    dipoleBuf[dipoleIndex].dimensions.resize(1);
    dipoleBuf[dipoleIndex].dimensions[0] = 1;
    dipoleBuf[dipoleIndex].starttime = headerp->time;
    dipoleBuf[dipoleIndex].startsamplenum = headerp->sample_nr;

    unsigned int sid                 = headerp->stationid;
    unsigned int rsp                 = headerp->rspid;
    unsigned int rcu                 = headerp->rcuid;
    double sf                        = headerp->sample_freq;
    unsigned int samples_per_frame   = headerp->n_samples_per_frame;
    unsigned int nyquist_zone        = 1;
    double antenna_position_value[3] = { 0., 0., 0. };
    string antenna_position_unit[3]  = { "m", "m", "m" };

    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(STATION_ID), &sid );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(RSP_ID), &rsp );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(RCU_ID), &rcu );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(TIME),
        &(dipoleBuf[dipoleIndex].starttime) );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(SAMPLE_NUMBER),
        &(dipoleBuf[dipoleIndex].startsamplenum) );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(SAMPLES_PER_FRAME), &samples_per_frame );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(ANTENNA_POSITION_VALUE),
        antenna_position_value, 3 );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(ANTENNA_POSITION_UNIT),
        antenna_position_unit, 3 );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(ANTENNA_POSITION_FRAME),
        std::string("ITRF") );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(ANTENNA_ORIENTATION_VALUE),
        antenna_position_value, 3 );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(ANTENNA_ORIENTATION_UNIT),
        antenna_position_unit, 3 );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(ANTENNA_ORIENTATION_FRAME),
        std::string("ITRF") );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(FEED), std::string("UNDEFINED") );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(NYQUIST_ZONE),
        &nyquist_zone );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(SAMPLE_FREQUENCY_VALUE), &sf, 1 );
    dipoleBuf[dipoleIndex].array->setAttribute( attribute_name(SAMPLE_FREQUENCY_UNIT),
        std::string("MHz") );
#ifdef DEBUGGING_MESSAGES
    /* Feedback */
    cout << "CREATED New dipole group: " << newDipoleIDstr << endl;
#endif

    return dipoleIndex;
  };

  // ----------------------------------------------------------- createNewStation
  int TBBraw::createNewStation(TBB_Header *headerp)
  {
    int stationIndex;

    if (dataset_p == NULL)
      {
        cerr << "TBBraw::createNewStation: \"dataset_p == NULL\" Not attached to a file!" <<endl;
        return -1;
      };

    // find an empty station index
    for (stationIndex=0; stationIndex<MAX_NO_STATIONS; stationIndex++)
      {
        if (stationBuf[stationIndex].group == NULL)
          {
            break;
          };
      };
    if (stationIndex == MAX_NO_STATIONS)
      {
        cerr << "TBBraw::createNewStation: Buffer \"dipoleBuf\" is full, cannot create new dipole!" <<endl;
        return -1;
      };

    // We got our stationIndex -> create the station group
    char newStationIDstr[10];
    sprintf( newStationIDstr, "Station%03d", headerp->stationid );
    stationBuf[stationIndex].group = dataset_p->createGroup( newStationIDstr );

    stationBuf[stationIndex].ID = headerp->stationid;

    double trigger_offset[1]             = { 0 };
    int triggered_antennas[1]            = { 0 };
    double station_position_value[3]     = { 0, 0, 0 };
    string station_position_unit[3]      = { "m", "m", "m" };
    string station_position_frame        = "ITRF";
    double beam_direction_value[2]       = { 0, 90 };
    string beam_direction_unit[2]        = { "deg", "deg"};
    string beam_direction_frame          = "AZEL";

    // Add attributes to "Station" group
    stationBuf[stationIndex].group->setAttribute( attribute_name(STATION_POSITION_VALUE),
        station_position_value, 3 );
    stationBuf[stationIndex].group->setAttribute( attribute_name(STATION_POSITION_UNIT),
        station_position_unit, 3 );
    stationBuf[stationIndex].group->setAttribute( attribute_name(STATION_POSITION_FRAME),
        station_position_frame );
    stationBuf[stationIndex].group->setAttribute( attribute_name(BEAM_DIRECTION_VALUE),
        beam_direction_value, 2 );
    stationBuf[stationIndex].group->setAttribute( attribute_name(BEAM_DIRECTION_UNIT),
        beam_direction_unit, 2 );
    stationBuf[stationIndex].group->setAttribute( attribute_name(BEAM_DIRECTION_FRAME),
        beam_direction_frame );
    stationBuf[stationIndex].group->setAttribute( attribute_name(TRIGGER_TYPE),
        string("UNDEFINED") );
    stationBuf[stationIndex].group->setAttribute( attribute_name(TRIGGER_OFFSET),
        trigger_offset );
    stationBuf[stationIndex].group->setAttribute( attribute_name(TRIGGERED_ANTENNAS),
        triggered_antennas);
    stationBuf[stationIndex].group->setAttribute( attribute_name(OBSERVATION_MODE),
        string("Transient") );
    return stationIndex;
  };

  //_____________________________________________________________________________
  //                                                              addDataToDipole
  
  bool TBBraw::addDataToDipole(int index, char *buffer, int bufflen, bool bigEndian)
  {
    int i;
    TBB_Header *headerp = (TBB_Header*)buffer;
    if (bufflen < (int)(headerp->n_samples_per_frame*sizeof(short)+sizeof(TBB_Header)))
      {
        cerr << "TBBraw::addDataToDipole: Too few data read in! Aborting." << endl;
        cerr << "  block size: " << bufflen << " bytes, estimated size: "
             << (headerp->n_samples_per_frame*sizeof(short)+sizeof(TBB_Header))
             << " bytes" << endl;
        return false;
      };

    //set sdata to the (hopefully correct) position in the udp-buffer
    char *tmpptr = buffer+sizeof(TBB_Header);
    short *sdata = (short *)(tmpptr);

    if ( bigendian_p != bigEndian )
      {
        for ( i=0; i < headerp->n_samples_per_frame; i++ )
          {
            swapbytes( (char *)&(sdata[i]), 2 );
          };
      };

    //calculate the writeOffset from time of first block and this block
    int writeOffset= (headerp->sample_nr-dipoleBuf[index].startsamplenum)+
                     ((headerp->time-dipoleBuf[index].starttime)*headerp->sample_freq*1000000);
#ifdef DEBUGGING_MESSAGES
    uint sid, rsp, rcu;
    dipoleBuf[index].array->getAttribute( attribute_name(STATION_ID), sid );
    dipoleBuf[index].array->getAttribute( attribute_name(RSP_ID), rsp );
    dipoleBuf[index].array->getAttribute( attribute_name(RCU_ID), rcu );
    std::cout << "Station: " << sid << " RSP: " << rsp << " RCU: " << rcu
              << " Sequence-Nr: " << headerp->seqnr << endl;
    std::cout << " starttime:"<< headerp->time-dipoleBuf[index].starttime << " startsamplenum:" << headerp->sample_nr-dipoleBuf[index].startsamplenum
              << " writeOffset:" << writeOffset << endl;
#endif
    //only write data if this block comes after the first block
    // (don't extend the array to the front)
    if (writeOffset >= 0)
      {
        //extend array if neccessary.
        if ((writeOffset+ headerp->n_samples_per_frame)> dipoleBuf[index].dimensions[0])
          {
#ifdef DEBUGGING_MESSAGES
            cout << "extending array to:" << writeOffset+ headerp->n_samples_per_frame
                 << " from:" << dipoleBuf[index].dimensions[0] << endl;
#endif
            dipoleBuf[index].dimensions[0] = writeOffset+ headerp->n_samples_per_frame;
            dipoleBuf[index].array->extend(dipoleBuf[index].dimensions);
          };
        dipoleBuf[index].array->write(writeOffset, sdata, headerp->n_samples_per_frame );
#ifdef DEBUGGING_MESSAGES
      }
    else
      {
        std::cout << "Block seq-nr: " << headerp->seqnr << " has negative write offset."
                  << " Block discarded!" << endl;
#endif
      };

    // That is not needed/used right now!
    /*
      tmpptr += headerp->n_samples_per_frame*sizeof(short);
      UInt32 *payloadp = (UInt32 *)tmpptr;
      payload_crc = *payloadp;
    */
    return true;
  };


  // ============================================================================
  //
  //
  //
  // ============================================================================




} // Namespace DAL -- end
