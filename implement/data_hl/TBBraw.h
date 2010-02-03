/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Andreas Horneffer (A.Hornffer@astro.ru.nl)                            *
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

#ifndef TBBRAW_H
#define TBBRAW_H

// Standard library header files
#include <iostream>
#include <string>
#include <vector>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// casacore header files
#ifdef HAVE_CASA
#include <casa/Arrays/Array.h>
#endif

// DAL header files
#include <dalCommon.h>
#include <dalDataset.h>
#include <CommonAttributes.h>

namespace DAL {  // Namespace DAL -- begin
  
  /*!
    \class TBBraw
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Class to generate HDF5 files from raw TBB data-frames
    
    \author Andreas Horneffer
    
    \date 2009/01/07
    
    \test tTBBraw.cpp
    
    <h3>Prerequisite</h3>
    
    <h3>Synopsis</h3>
    
    This class generates new LOFAR TBB-Timeseries hdf5 files from TBB data
    frames with raw-ADC (aka transient) data.

    The data frames need to be read in by an application (or derived class) from
    a file or an UDP-port.

    <i>Future enhancements:</i>
    - Suport for handling of TBB sub-band data needs to be added.
    - Support for big-endian systems is still untested.

    <h3>Example(s)</h3>

  */
  class TBBraw {
    
    //!some internal definitions
#define TBB_FRAME_SIZE 2140
#define MAX_NO_STATIONS 50
#define MAX_NO_DIPOLES 1000
    
  private:
    // ----------------------------------------------------------- Private Data
    
    //! pointer to the dataset (i.e. the file itself)
    dalDataset * dataset_p;
    //! Filename
    std::string filename_p;
    //! LOFAR common attributes attached to the root group of the file
    CommonAttributes commonAttributes_p;
    //! Check the header-CRC
    bool do_headerCRC_p;
    //! Check the data-CRC
    bool do_dataCRC_p;
    
    //! fix broken time-stamps?
    int fixTimes_p;
    
    //! number of processed data block
    int nofProcessed_p;
    
    //! number of discarded data blocks with broken crc
    int nofDiscardedHeader_p;
    
    //! am I big endian?
    bool bigendian_p;
    
    //! buffer for the stations
    struct stationBufElem
    {
      //! ID of the station
      unsigned int ID;
      //! pointer to the corresponding group
      dalGroup * group;
    };
    struct stationBufElem *stationBuf;
    
    //! buffer for the dipoles
    struct dipoleBufElem
    {
      //! ID of the dipole
      unsigned int ID;
      //! pointer to the corresponding array
      dalArray * array;
      //! dimension (size) of the array
      vector<int> dimensions;
      /*! time and samplenumer of the first element in the array
	(used to calculate array offsets).
      */
      unsigned int starttime, startsamplenum;
    };
    struct dipoleBufElem *dipoleBuf;
    
  protected:
    
    // ------------------------------------------------------- Type definitions
    
    //! Structure storing metadata stored in the TBB header block
    struct TBB_Header
    {
      //! The identifier to the station
      unsigned char stationid;
      //! The identifier for the RSP board
      unsigned char rspid;
      //! The identifier for the RCU board
      unsigned char rcuid;
      //! The sample frequency in the A/D conversion in MHz
      unsigned char sample_freq;
      //! Sequence-number, i.e. position in the TBB memory
      UInt32 seqnr;
      //! Unix-like time-stamp
      Int32 time;
      //! number of the sample since the start of the second
      UInt32 sample_nr;
      //! The number of samples per frame
      UInt16 n_samples_per_frame;
      //! number of frequency bands (n_freq_bands==0 means transient data)
      UInt16 n_freq_bands;
      char bandsel[64];
      Int16 spare;
      //! CRC of the header (with seqnr set to 0)
      UInt16 crc;
    };
    
    /*!
      \brief check the header CRC.
      
      \param headerp -- pointer to the frame header
      
      \return <tt>true</tt> if header-CRC is correct
    */
    bool checkHeaderCRC(TBB_Header *headerp);
    
  public:
    
    // ------------------------------------------------------------- Construction
    
    /*!
      \brief Default constructor
    */
    TBBraw();

    /*!
      \brief Argumented Constructor

      \param commonAttributes -- LOFAR common attribute attached to the root
             group of the file.
    */
    TBBraw (CommonAttributes const &commonAttributes);
    
    /*!
      \brief Argumented Constructor
      
      \param filename        -- Name of the output file to be created
      \param observer        -- Name of the observer
      \param project         -- Name of the project
      \param observation_id  -- Name of the observation ID
      \param filterSelection -- Filter (frequency-width) selection of the
             observation
      \param telescope -- name of the telescope (usually "LOFAR")
    */
    TBBraw (std::string const &filename,
	    string const &observer="John Doe",
	    string const &project="UNDEFINED",
	    string const &observation_id="UNDEFINED",
	    string const &filterSelection="UNDEFINED",
	    string const &telescope="LOFAR");
    
    // -------------------------------------------------------------- Destruction
    
    //! Destructor
    ~TBBraw();
    
    // --------------------------------------------------------------- Parameters
    
    /*!
      \brief Is this object connected to an output file?
      
      \return <tt>true</tt> if connected
    */
    inline bool isConnected()
    {
      return dataset_p != NULL;
    };
    
    /*!
      \brief Perform CRC checking of the header, yes or no (default yes).
      
      \param doit -- set to <tt>true</tt> to switch CRC checking on
    */
    inline void doHeaderCRC(const bool doit=true)
    {
      do_headerCRC_p=doit;
    };
    /*!
      \brief Perform CRC checking of the data (payload), yes or no (default no).
      
      \param doit -- set to <tt>true</tt> to switch CRC checking on
    */
    inline void doDataCRC(const bool doit=true)
    {
      if (doit) {
	cout << "TBBraw::doDataCRC: Data CRC-checking not (yet) implemented!" << endl;
	//      do_dataCRC_p=doit;
      };
    };
    
    /*!
      \brief Fix broken time-stamps (new style fixing is default)
      
      \param fixlevel -- set to: 0: don't fix, 1: fix old style, 2: fix new style
    */
    inline void setFixTimes(const int fixlevel=2)
    {
      fixTimes_p=fixlevel;
    };
    
    //! Get the LOFAR common attributes attached to the root level of the file
    inline CommonAttributes commonAttributes () const {
      return commonAttributes_p;
    }
    
    
    // ------------------------------------------------------------------ Methods
    
    /*!
      \brief Open (create) new file
      
      \return <tt>true</tt> if file is created successfully
      
      Creating more than one file with one instance of TBBraw is deprecated!
      Destroy the TBBraw object and create a new one instead!
      Currently only creates new files, and fails if a file with \t filename
      already exists.
    */
    bool open_file ();
    
    /*!
      \brief Open (create) new file
      
      \param filename        -- Name of the output file to be created
      \param observer        -- Name of the observer
      \param project         -- Name of the project
      \param observation_id  -- Name of the observation ID
      \param filterSelection -- Filter (frequency-width) selection of the
             observation
      \param telescope -- name of the telescope (usually "LOFAR")
      
      \return <tt>true</tt> if file is created successfully
      
      Creating more than one file with one instance of TBBraw is deprecated!
      Destroy the TBBraw object and create a new one instead!
      Currently only creates new files, and fails if a file with \t filename
      already exists.
    */
    bool open_file(std::string const &filename,
		   string const &observer="John Doe",
		   string const &project="UNDEFINED",
		   string const &observation_id="UNDEFINED",
		   string const &filterSelection="UNDEFINED",
		   string const &telescope="LOFAR");
    
    /*!
      \brief Process one block of data and add it's contents to the output file
      
      \param inbuff  -- pointer to one TBB data-frame (incl. header etc.)
      \param datalen -- length (number of bytes) of the data in inbuff
      \param bigEndian -- set to true if the data is in big endian byte order
      
      \return <tt>true</tt> if successful
      
      The data in inbuff must be readable and valid until the method returns. If
      datalen is larger than one TBB-frame, then the additional data is ignored.
      The TBBs send data in little endian. Currently I cannot think of a situation
      in which the input data is not in little endian, so the big endian support
      is untested. (Actually: Only processing little endian data on little endian
      machines is tested.)
    */
    bool processTBBrawBlock (char *inbuff,
			     int datalen,
			     bool bigEndian=false);
    
    //! Provide a summary of the internal status and processing statistics
    inline void summary () {
      summary (std::cout);
    }
    
    //! Provide a summary of the internal status and processing statistics
    void summary (std::ostream &os);
    
  private:
    // ----------------------------------------------------------- Private Methods

    //! Initialize the internal dataspace of the object
    void init();
    
    //! Release all temporary structures
    void destroy();
    
    /*!
      \brief fix old-style broken time-stamps
      
      \param headerp -- pointer to the frame header
    */
    void fixDateOld(TBB_Header *headerp);
    
    /*!
      \brief fix new-style broken time-stamps
      
      \param headerp -- pointer to the frame header
    */
    void fixDateNew(TBB_Header *headerp);
    
    /*!
      \brief get the index of the dipole in the dipole buffer
      
      \param headerp -- pointer to the frame header
      
      \return index of the dipole, or -1 if an error occured
    */
    int getDipoleIndex(TBB_Header *headerp);
    
    /*!
      \brief Create a new dipole array and return the its index
      
      \param headerp -- pointer to the frame header
      
      \return index of the new dipole, or -1 if an error occured
    */
    int createNewDipole(TBB_Header *headerp);
    
    /*!
      \brief Create a new station group and return the its index
      
      \param headerp -- pointer to the frame header
      
      \return index of the new station, or -1 if an error occured
    */
    int createNewStation(TBB_Header *headerp);
    
    /*!
      \brief Process one block of data and add it's contents to the output file
      
      \param index  -- index of the entry in dipoleBuf to add the data to
      \param buffer  -- pointer to the TBB data-frame (incl. header etc.)
      \param datalen -- length (number of bytes) of the data in inbuff
      \param bigEndian -- set to true if the data is in big endian byte order
      
      \return <tt>true</tt> if successful
    */
    bool addDataToDipole (int index,
			  char *buffer,
			  int bufflen,
			  bool bigEndian=false);
    
  }; // class TBBraw -- end
  
} // Namespace DAL -- end

#endif /* TBBRAW_H */

