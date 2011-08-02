/***************************************************************************
 *   Copyright (C) 2009                                                    *
 *   Andreas Horneffer (A.Horneffer@astro.ru.nl)                           *
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sstream>

#include <dal_config.h>
#include <data_hl/TBBraw.h>

//includes for networking
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
//includes for threading
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
//includes for the commandline options
#include <boost/program_options.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/detail/cmdline.hpp>
//other includes
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
namespace bpo = boost::program_options;
//includes for setting the IO-priority
#include <sys/types.h>
#include <sys/syscall.h> /* For SYS_xxx definitions */
//#include <sys/capability.h>
#if defined linux
#include <linux/version.h>
#endif


/*!
  \file TBBraw2h5.cpp

  \ingroup DAL
  \ingroup dal_apps

  \brief Read TBB time-series data from a socket or file and generate an HDF5 file.

  \author Andreas Horneffer

  \date 2009/07/02

  <h3>Prerequisite</h3>

  - DAL::TBBraw -- Class to generate a TBB time-series hdf5 file from raw TBB data-frames.

  <h3>History</h3>

  This application and the corresponding \t TBBraw class is a rewrite of the \t tbb2h5
  application and its \t TBB class, reusing much of the original source code.

  <h3>Usage</h3>

  \t TBBraw2h5 supports reading in the raw TBB time-series data from files or
  an udp socket. It does not add data to an existing hdf5 file.

  <table border="0">
  <tr>
  <td class="indexkey">Command line</td>
  <td class="indexkey">Decription</td>
  </tr>
  <tr>
  <td>-H [--help]</td>
  <td>Show help messages</td>
  </tr>
  <tr>
  <td>-O [--outfile] arg</td>
  <td>Name of the output dataset. If the file already exists the programm terminates.</td>
  </tr>
  <tr>
  <td>-I [--infile] arg</td>
  <td>Name of the input file. Mutually exclusive to the -P option.</td>
  </tr>
  <tr>
  <td>-P [--port] arg</td>
  <td>Port number to accept data from. Mutually exclusive to the -I option.</td>
  </tr>
  <tr>
  <td>--ip arg</td>
  <td>Hostname/IP address on which to accept the data (currently not used)</td>
  </tr>
  <tr>
  <td>-S [--timeoutStart] arg</td>
  <td>Time-out, [sec], when opening socket connection, before dropping the
  conection. If the provided value is smaller but zero (which is the default)
  the connection to the port is kept open indefinitely.</td>
  </tr>
  <tr>
  <td>-R [--timeoutRead] arg</td>
  <td>Time-out, [sec], when opening socket connection, before dropping the
  conection. If the provided value is smaller but zero (which is the default)
  the connection to the port is kept open indefinitely.</td>
  </tr>
  <tr>
  <td>-F [--fixTimes] arg</td>
  <td>Fix broken time stamps (generated by the RSPs and passed through by the TBBs).
  (0): do not change time stamps
  (1): change old style time stamps: substract 1 from the second-counter of the last
  frame of a second and add 512 to the sample_nr of (true) odd-numbered second frames
  in 200MHz mode
  (2): change new style time stamps: add 512 to the sample_nr of odd-numbered second
frames in 200MHz mode (default)
       </td>
       </tr>
       <tr>
       <td>-C [--doCheckCRC] arg</td>
       <td> Check the CRCs of the frames:
  (0): do not check the CRCs
       (1): check the header CRCs and discard broken frames (default)
            </td>
            <tr>
            <td>-B [--bufferSize] arg</td>
            <td> Size of the input buffer (in frames) when reading from a socket. The default is 
            50000, which is about 100MByte. </td>
            </tr>
            <tr>
            <td>-K [--keepRunning]</td>
            <td>Keep running, i.e. process more than one event by restarting the procedure.</td>
            </tr>
            <tr>
            <td>-V [--verbose]</td>
            <td>Enable verbose mode, showing status messages during processing.</td>
            </tr>
            </table>


            */




            //Global variables
            //! (pointer to) the TBBraw object we are writing to.
            DAL::TBBraw *tbb;

            //!size of the buffer for the UDP-datagram
            // 1 byte larger than the frame size
#define UDP_PACKET_BUFFER_SIZE 2141
            //!number of frames in the input buffer (50000 is ca. 100MB)
            // (the vBuf of the system on the storage nodes can store ca. 800 frames)
            //#define INPUT_BUFFER_SIZE 50000
            int input_buffer_size;

            //!pointers (array indices) for the last buffer processed and the last buffer written
            int inBufProcessID,inBufStorID;
            //!the Input Buffer
            char * inputBuffer_p;
            //!end all running reader threads
            bool terminateThreads;
            //!maximum number of frames waiting in the vBuf while reading
            int maxWaitingFrames;
            //!maximum number of frames in the buffer
            int maxCachedFrames;
            //!number of frames dropped due to buffer overflow
            int noFramesDropped;
            //!number of running reader-threads
            int noRunning;
            //!mutex for writing into the buffer
            boost::mutex writeMutex;

            //_______________________________________________________________________________
            // Handling of IO-Priority settings

#define IOPRIO_BITS (16)
#define IOPRIO_CLASS_SHIFT (13)
#define IOPRIO_PRIO_MASK ((1UL << IOPRIO_CLASS_SHIFT) - 1)
#define IOPRIO_PRIO_CLASS(mask) ((mask) >> IOPRIO_CLASS_SHIFT)
#define IOPRIO_PRIO_DATA(mask) ((mask) & IOPRIO_PRIO_MASK)
#define IOPRIO_PRIO_VALUE(class, data) (((class) << IOPRIO_CLASS_SHIFT) | data)

            enum {
              IOPRIO_WHO_PROCESS = 1,
              IOPRIO_WHO_PGRP,
              IOPRIO_WHO_USER,
            };
enum {
  IOPRIO_CLASS_NONE,
  IOPRIO_CLASS_RT,
  IOPRIO_CLASS_BE,
  IOPRIO_CLASS_IDLE,
};

#if defined linux

int ioprio_set (int which, int who, int ioprio) {
  return syscall(SYS_ioprio_set, which, who, ioprio);
}
int ioprio_get(int which, int who) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,13))
  return syscall(SYS_ioprio_get, which, who);
#else
  return 0;
#endif
}

int increase_io_priority(bool verbose) {
  int ret;
  if (verbose) {
    std:: cout << "TBBraw2h5::increase_io_priority: PID: "<< getpid() 
      << " old IO Prio: " << ioprio_get(IOPRIO_WHO_PROCESS, getpid()) << std::endl;
  };
  // attempting to set RT priority
  ret = ioprio_set(IOPRIO_WHO_PROCESS, getpid(), IOPRIO_PRIO_VALUE(IOPRIO_CLASS_RT, 5));
  if (ret != 0) {
    perror("ioprio_set");
  };
  if (verbose) {
    std::cout << "TBBraw2h5::increase_io_priority: PID "<< getpid() 
      << " new IO Prio: " << ioprio_get(IOPRIO_WHO_PROCESS, getpid()) << std::endl;
  };
  return ret;
}

#endif

//_______________________________________________________________________________
//                                                             socketReaderThread

/*!
  \brief Thread that creates and then reads from a socket into the buffer

  \param port -- UDP port number to read data from
  \param ip -- Hostname (ip-address) to bind to (not used)
  \param startTimeout -- Timeout when opening socket connection [in sec]
  \param readTimeout -- Timeout while reading from the socket [in sec] 
  \param verbose -- Produce more output
  \param stayConnected -- stay connected even after readTimeout ran out.

  \return \t true if successful
 */
void socketReaderThread (int port,
    string ip,
    double startTimeout,
    double readTimeout,
    bool verbose,
    bool stayConnected=false)
{
  // Create the main socket
  int main_socket = 0;
  fd_set readSet;
  struct timeval TimeoutWait, TimeoutRead, NullTimeout;

  TimeoutRead.tv_sec  = floor(readTimeout);
  TimeoutRead.tv_usec = (readTimeout-TimeoutRead.tv_sec)*1e6;
  NullTimeout.tv_sec  = NullTimeout.tv_usec =0;
  main_socket         = socket(PF_INET, SOCK_DGRAM, 0);

  if (main_socket<0) {
    cerr << "[TBBraw2h5::socketReaderThread] " << port
      << " : Failed to create the main socket."<<endl;
    boost::mutex::scoped_lock lock(writeMutex);
    noRunning--;
    return;
  };

  //Create a sockaddr_in to describe the local port
  sockaddr_in local_info;
  local_info.sin_family = AF_INET;
  local_info.sin_addr.s_addr = htonl(INADDR_ANY);
  local_info.sin_port = htons(port);
  //Bind the socket to the port
  int erg = bind(main_socket, (sockaddr *) &local_info, sizeof(local_info));
  if (erg<0)
  {
    cerr << "TBBraw2h5::socketReaderThread:"<<port<<": Failed to bind to port"
      << "(with ip: " << ip <<")"<< endl;
    boost::mutex::scoped_lock lock(writeMutex);
    noRunning--;
    return;
  };
  //Wait for the first data to arrive
  cout << "TBBraw2h5::socketReaderThread:"<<port<<": Waiting for data." << endl;
  FD_ZERO(&readSet);
  if (startTimeout > 0) {
    while (!terminateThreads && (startTimeout>0)){
      if (startTimeout>5) {
        TimeoutWait.tv_sec = 5;
        TimeoutWait.tv_usec = 0;
        startTimeout -= 5;
      } else {
        TimeoutWait.tv_sec = floor(startTimeout);
        TimeoutWait.tv_usec = (startTimeout-TimeoutWait.tv_sec)*1e6;
        startTimeout = 0;
      };
      FD_SET(main_socket, &readSet);
      if (select( main_socket + 1, &readSet, NULL, NULL, &TimeoutWait ) != 0){
        break;
      };
    }
  } else {
    while (!terminateThreads){
      TimeoutWait.tv_sec = 5;
      TimeoutWait.tv_usec = 0;
      FD_SET(main_socket, &readSet);
      if (select( main_socket + 1, &readSet, NULL, NULL, &TimeoutWait ) != 0){
        break;
      };
    };
  };
  bool ImRunning=true;
  int status, numWaiting=0, newBufID;
  struct sockaddr_in incoming_addr;
  socklen_t socklen = sizeof(incoming_addr);
  while (ImRunning && !terminateThreads)
  {
    if (verbose)
    {
      FD_ZERO(&readSet);
      FD_SET(main_socket, &readSet);
      if ((status = select(main_socket + 1, &readSet, NULL, NULL, &NullTimeout)) )
      {
        numWaiting++;
      }
      else
      {
        boost::mutex::scoped_lock lock(writeMutex);
        if (numWaiting > maxWaitingFrames)
        {
          maxWaitingFrames=numWaiting;
        };
        numWaiting=0;
      };
    };
    FD_ZERO(&readSet);
    FD_SET(main_socket, &readSet);
    TimeoutWait = TimeoutRead;
    if ((status = select(main_socket + 1, &readSet, NULL, NULL, &TimeoutWait)) )
    {
      //there is a frame waiting in the vBuffer
      {
        //  make sure that we are the only ones writing to the input buffer
        //  (concurrent reading is O.K.)
        boost::mutex::scoped_lock lock(writeMutex);
        newBufID = inBufStorID+1;
        if (newBufID >= input_buffer_size)
        {
          newBufID =0;
        }
        if (newBufID == inBufProcessID)
        {
          noFramesDropped++;
          newBufID = inBufStorID;
        };
        //perform the actual read
        erg = recvfrom( main_socket, (inputBuffer_p + (newBufID*UDP_PACKET_BUFFER_SIZE)),
            UDP_PACKET_BUFFER_SIZE, 0, (sockaddr *) &incoming_addr, &socklen);
        inBufStorID = newBufID;
      }
      ;// writeMutex lock is released here
      if (verbose)
      {
        if (erg != 2140)
        {
          cout << "TBBraw2h5::socketReaderThread:"<<port
            << ": Received strange packet size: " << erg <<endl;
        };
      };
    }
    else if (! stayConnected)
    {
      if (verbose)
      {
        cout << "TBBraw2h5::socketReaderThread:"<<port<<": Data stopped coming." << endl;
      };
      ImRunning=false;
    };
  };
  if (verbose && ImRunning && terminateThreads ) {
    cout << "TBBraw2h5::socketReaderThread:"<<port<<": stopped because terminateThreads was set!" << endl;
  };
  {
    boost::mutex::scoped_lock lock(writeMutex);
    noRunning--;
  };
  close(main_socket);
  return;
};

//_______________________________________________________________________________
//                                                                readFromSockets

/*!
  \brief Read the TBB-data from several udp sockets

  \param ports           -- Vector with UDP port numbers to read data from
  \param ip              -- Hostname (ip-address) to bind to (not used!)
  \param startTimeout    -- Timeout when opening socket connection [in sec]
  \param readTimeout     -- Timeout while reading from the socket [in sec]
  \param verbose         -- Produce more output
  \param waitForAllPorts -- Wait until all reader-threads have received some
  data (Or kill the threads that got no data)

  \return status -- Returns \t true if successful, \e false in case an error
  was encountered.
 */
bool readFromSockets (std::vector<int> ports,
    string ip,
    float startTimeout,
    float readTimeout,
    bool verbose=false,
    bool waitForAllPorts=false,
    std::string outFileBase="",
    std::string observer="UNDEFINED",
    std::string project="UNDEFINED",
    std::string observationID="UNDEFINED",
    std::string filterSelection="UNDEFINED",
    std::string antennaSet="UNDEFINED",
    int doCheckCRC=1,
    int fixTransientTimes=2)
{
  unsigned int i = 0;
  char * bufferPointer;
  time_t timestamp;
  struct tm *timestamp_utc;
  double timestamp_fraction;
  char timestamp_buffer[20];

  //________________________________________________________
  // initialize the buffer

  terminateThreads = false;
  maxCachedFrames  = maxWaitingFrames = 0;
  inBufProcessID   = inBufStorID =0;
  noRunning        = 0;
  inputBuffer_p    = new char[(input_buffer_size*UDP_PACKET_BUFFER_SIZE)];

  if (inputBuffer_p == NULL) {
    cerr << "TBBraw2h5::readFromSockets: Failed to allocate input buffer!" <<endl;
    return false;
  } else if (verbose) {
    cout << "TBBraw2h5::readFromSockets: Allocated " << input_buffer_size*UDP_PACKET_BUFFER_SIZE
      << " bytes for the input buffer." << endl;
  };

  // start the reader-threads
  boost::thread **readerThreads;
  readerThreads = new boost::thread*[ports.size()];
  for (i=0; i < ports.size(); i++) {
    readerThreads[i] = new boost::thread(boost::bind(socketReaderThread,
          ports[i],
          ip,
          startTimeout,
          readTimeout,
          verbose,
          false));
    if (readerThreads[i]->boost::thread::joinable() ) {
      noRunning++;
    } else {
      cout << "TBBraw2h5::readFromSockets: Failed to start reader thread for port :" << ports[i] << endl;
      cout << "  Aborting!!! " << endl;
      terminateThreads=true;
      return false;
    };
  };
  int processingID, tmpint;
  int amWaiting=0;
  while ((noRunning>0) || (inBufStorID != inBufProcessID) )  {
    if (inBufStorID == inBufProcessID)  {
      if (verbose && ((amWaiting%100)==1) ) {
        cout << "TBBraw2h5::readFromSockets: Status report: Buffer is empty! waiting." << endl;
        cout << "  Status: inBufStorID:" << inBufStorID << " inBufProcessID: " << inBufProcessID
          << " noRunning: " << noRunning << " waiting for: " << amWaiting*0.10 << " sec."<< endl;
      };
      amWaiting++;
      usleep(100000);
      if (!waitForAllPorts && maxCachedFrames>0 && (amWaiting*0.10 > readTimeout)){
        if (verbose && ! terminateThreads) {
          cout << "TBBraw2h5::readFromSockets: Stopping all other reader-threads." << endl;
        };
        terminateThreads = true;
      };
      continue;
    };
    amWaiting=0;
    tmpint = (inBufStorID-inBufProcessID+input_buffer_size)%input_buffer_size;
    if (tmpint > maxCachedFrames) {
      maxCachedFrames = tmpint;
    };
    processingID = inBufProcessID+1;
    if (processingID >= input_buffer_size) {
      processingID -= input_buffer_size;
    };

    // Create new time stamped file if required
    bufferPointer = inputBuffer_p + (processingID*UDP_PACKET_BUFFER_SIZE);
    if (tbb == NULL)
    {
      // Get timestamp and convert to ISO 8601 format for filename
      timestamp = (time_t) DAL::TBBraw::getDataTime(bufferPointer);
      timestamp_utc = gmtime( &timestamp );
      timestamp_fraction = DAL::TBBraw::getDataTimeFraction(bufferPointer) + timestamp_utc->tm_sec;
      strftime (timestamp_buffer, 20, "%Y%m%dT%H%M", timestamp_utc);

      // Generate filename
      std::ostringstream outfile;
      outfile << outFileBase << "-" << timestamp_buffer << std::setw(6) << std::setfill('0') << std::setiosflags(std::ios::fixed) << std::setprecision(3) << timestamp_fraction << "Z";

      // Check if filename exists already and change it accordingly
      if (boost::filesystem::exists(outfile.str()+".h5"))
      {
        int n = 0;
        while (boost::filesystem::exists(outfile.str()+"-"+boost::lexical_cast<std::string>(n)+".h5"))
        {
          ++n;
        }
        outfile << "-" << n;
      }
      // Create file
      tbb = new DAL::TBBraw(outfile.str()+".h5", observer, project, observationID, filterSelection, "LOFAR", antennaSet);
      if ( !tbb->isConnected() )
      {
        cout << "[TBBraw2h5] Failed to open output file." << endl;
        return 1;
      };

      // Set the options in the TBBraw object
      if (doCheckCRC>0) {
        tbb->doHeaderCRC(true);
      }
      else {
        tbb->doHeaderCRC(false);
      };
      tbb->setFixTimes(fixTransientTimes);
    }
      
    tbb->processTBBrawBlock(bufferPointer,
        UDP_PACKET_BUFFER_SIZE);
    inBufProcessID = processingID;
  };
  terminateThreads = true;
  for (i=0;  i< ports.size(); i++){
    readerThreads[i]->join();
    delete readerThreads[i];
  };
  delete [] readerThreads;
  delete inputBuffer_p;
  if (verbose) {
    cout << "Socket and Buffer Stats: Maximum # of waiting frames:" << maxWaitingFrames << endl;
    cout << "                        Maximum # of frames in cache:" << maxCachedFrames << endl;
    cout << "     Number of frames dropped due to buffer overflow:" << noFramesDropped << endl;
  };
  return true;
};

//_______________________________________________________________________________
//                                                        readStationsFromSockets

/*!
  \brief Read the TBB-data of several stations and from several udp sockets

  \param ports -- Vector with UDP port numbers to read data from
  \param ip -- Hostname (ip-address) to bind to (not used!)
  \param startTimeout -- Timeout when opening socket connection [in sec]
  \param readTimeout -- Timeout while reading from the socket [in sec]
  \param verbose -- Produce more output

  \return \t false if something went wrong

  Compared to \t readFromSockets() this function generates less (usefull)
  debug output. So the other (old) version should stay around.
 */
bool readStationsFromSockets (std::vector<int> ports,
    std::string ip,
    float startTimeout,
    float readTimeout,
    std::string outFileBase,
    std::string observer,
    std::string project,
    std::string observationID,
    std::string filterSelection,
    std::string antennaSet,
    bool verbose=false)
{
  unsigned int i = 0;

  //________________________________________________________
  // Initialize the buffer

  terminateThreads = false;
  maxCachedFrames  = 0;
  maxWaitingFrames = 0;
  inBufProcessID   = 0;
  inBufStorID      = 0;
  noRunning        = 0;
  inputBuffer_p    = new char[(input_buffer_size*UDP_PACKET_BUFFER_SIZE)];

  if (inputBuffer_p == NULL) {
    std::cerr << "TBBraw2h5::readStationsFromSockets: Failed to allocate input buffer!"
      << std::endl;
    return false;
  } else if (verbose) {
    cout << "TBBraw2h5::readStationsFromSockets: Allocated "
      << input_buffer_size*UDP_PACKET_BUFFER_SIZE
      << " bytes for the input buffer." << endl;
  };

  //________________________________________________________
  // Get and initialize memory for the TBB pointers

  unsigned int nofTBBfiles = 256;
  int lasttimes[nofTBBfiles];  
  time_t timestamp;
  struct tm *timestamp_utc;
  double timestamp_fraction;
  char timestamp_buffer[20];
  
  DAL::TBBraw **TBBfiles = new DAL::TBBraw* [nofTBBfiles]; 

  for (i=0; i<nofTBBfiles; i++) {
    TBBfiles[i]   = NULL;
  }

  //________________________________________________________
  // Start the reader-threads

  boost::thread **readerThreads = new boost::thread*[ports.size()];

  for (i=0; i < ports.size(); i++) {
    readerThreads[i] = new boost::thread (boost::bind(socketReaderThread,
          ports[i],
          ip,
          startTimeout,
          readTimeout,
          verbose,
          true));
    if (readerThreads[i]->joinable() ) {
      noRunning++;
    } else {
      cout << "TBBraw2h5::readStationsFromSockets: Failed to start reader thread for port :" << ports[i] << endl;
      cout << "  Aborting!!! " << endl;
      terminateThreads=true;
      return false;
    };
  };

  //________________________________________________________
  // Look for and process incoming data

  int processingID = 0;
  int tmpint       = 0;
  int amWaiting    = 0;
  unsigned char stationId;
  char * bufferPointer;

  while ((noRunning>0) || (inBufStorID != inBufProcessID) )  {
    if (inBufStorID == inBufProcessID)  {
      if (verbose && ((amWaiting%100)==1) ) {
        std::cout << "[TBBraw2h5::readStationsFromSockets]"
          << " Status report: Buffer is empty! waiting." << std::endl;
        // Do not split this up into several lines, as it makes the logfile hard to read!
        std::cout << "  Status: inBufStorID:" << inBufStorID 
          << " inBufProcessID: " << inBufProcessID
          << " noRunning: " << noRunning 
          << " waiting for: " << amWaiting*0.10 << " sec." << std::endl;
      };
      amWaiting++;
      usleep(100000);
      if (amWaiting*0.10 > readTimeout){
        for (i=0; i<256; i++) {
          if (TBBfiles[i] != NULL) {
            if (verbose) {
              TBBfiles[i]->summary();
            };
            delete TBBfiles[i];
            TBBfiles[i] = NULL;
          };
        };	
      };
      continue;
    };
    amWaiting=0;
    tmpint = (inBufStorID-inBufProcessID+input_buffer_size)%input_buffer_size;
    if (tmpint > maxCachedFrames) {
      maxCachedFrames = tmpint;
    };
    processingID = inBufProcessID+1;
    if (processingID >= input_buffer_size) {
      processingID -= input_buffer_size;
    };
    bufferPointer = (inputBuffer_p + (processingID*UDP_PACKET_BUFFER_SIZE));
    stationId = DAL::TBBraw::getStationId(bufferPointer);
    if ( (TBBfiles[stationId] == NULL) || 
        (DAL::TBBraw::getDataTime(bufferPointer) > (lasttimes[stationId]+ceil(readTimeout)) ) ){
      if (TBBfiles[stationId] != NULL) {
        if (verbose) {
          TBBfiles[stationId]->summary();
        };
        delete TBBfiles[stationId];
        TBBfiles[stationId] = NULL;
      };

      // Get timestamp and convert to ISO 8601 format for filename
      timestamp = (time_t) DAL::TBBraw::getDataTime(bufferPointer);
      timestamp_utc = gmtime( &timestamp );
      timestamp_fraction = DAL::TBBraw::getDataTimeFraction(bufferPointer) + timestamp_utc->tm_sec;
      strftime (timestamp_buffer, 20, "%Y%m%dT%H%M", timestamp_utc);

      // Generate filename
      std::ostringstream outfile;
      outfile << outFileBase << "-" << timestamp_buffer << std::setw(6) << std::setfill('0') << std::setiosflags(std::ios::fixed) << std::setprecision(3) << timestamp_fraction << "Z" << "-" << std::setw(3) << std::setfill('0') << int(stationId);

      // Check if filename exists already and change it accordingly
      if (boost::filesystem::exists(outfile.str()+".h5"))
      {
        int n = 0;
        while (boost::filesystem::exists(outfile.str()+"-"+boost::lexical_cast<std::string>(n)+".h5"))
        {
          ++n;
        }
        outfile << "-" << n;
      }

      TBBfiles[stationId] = new DAL::TBBraw(outfile.str()+".h5", observer, project, observationID, filterSelection, "LOFAR", antennaSet);
      if ( !TBBfiles[stationId]->isConnected() ) {
        cout << "TBBraw2h5::readStationsFromSockets: Failed to open output file:" 
          << outfile.str() << endl;
        terminateThreads=true;
      };       
    };
    if ( TBBfiles[stationId]->processTBBrawBlock(bufferPointer, UDP_PACKET_BUFFER_SIZE) ){ 
      lasttimes[stationId] = DAL::TBBraw::getDataTime(bufferPointer);
    };
    inBufProcessID = processingID;    
  };

  // Release allocated memory
  delete [] TBBfiles;

  terminateThreads = true;

  return true;
}

//_______________________________________________________________________________
//                                                                   readFromFile

/*!
  \brief Read the TBB-data from a file

  \param infile -- Path to the input file.
  \param verbose -- Produce more output

  \return status -- Returns \e true if successful
 */
bool readFromFile (string infile,
    bool verbose=false)
{
  int i         = 0;
  int numblocks = 0;
  int size      = 0;
  struct stat filestat;
  char buffer[TBB_FRAME_SIZE];

  stat(infile.c_str(), &filestat);
  numblocks = filestat.st_size / TBB_FRAME_SIZE;
  if (numblocks < 1)
  {
    cerr << "TBBraw2h5::readFromFile " << infile << " too small (smaller than one blocksize)." <<endl;
    return false;
  };
  if (verbose)
  {
    cout << "TBBraw2h5::readFromFile reading in " << numblocks << " blocks." << endl;
  };
  FILE *fd = fopen(infile.c_str(),"r");
  if (fd == NULL)
  {
    cerr << "TBBraw2h5::readFromFile: Can't open file: " << infile << endl;
    return false;
  };
  for (i=0; i<numblocks; i++)
  {
    size = fread(buffer, 1, TBB_FRAME_SIZE, fd);
    if ( (size != TBB_FRAME_SIZE )  )
    {
      if (verbose)
      {
        cout << "TBBraw2h5::readFromFile Cannot read in block: " << i << endl;
      };
      fclose(fd);
      return false;
    };
    tbb->processTBBrawBlock(buffer, size);
  };
  fclose(fd);
  return true;
};

//_______________________________________________________________________________
//                                                                   readFromFile

int main(int argc, char *argv[])
{
  std::string infile;
  std::string outfileOrig;
  vector<int> ports;
  std::string outfile   = "TBB";
  std::string observer  = "UNDEFINED";
  std::string project   = "UNDEFINED";
  std::string observationID = "UNDEFINED";
  std::string filterSelection = "UNDEFINED";
  std::string antennaSet = "UNDEFINED";
  std::string ip        = "All Hosts";
  bool verboseMode      = false;
  float timeoutStart    = 0.0;
  float timeoutRead     = 0.5;
  int fixTransientTimes = 2;
  int doCheckCRC        = 1;
  int socketmode        = -1;
  bool keepRunning      = false;
  bool waitForAll       = false;
  bool multipeStations  = false;
  bool raiseIOprio      = false;
  int runNumber         = 0;

  input_buffer_size = 50000;

  bpo::options_description desc ("[TBBraw2h5] Available command line options");

  //________________________________________________________
  // Define command line options

  desc.add_options ()
    ("help,H", "Show help messages")
    ("observer", bpo::value<std::string>(), "Observer")
    ("project", bpo::value<std::string>(), "Project")
    ("observationID", bpo::value<std::string>(), "Observation ID")
    ("filterSelection", bpo::value<std::string>(), "Filter selection")
    ("antennaSet", bpo::value<std::string>(), "Antenna set")
    ("outfile,O", bpo::value<std::string>(), "Name of the output dataset")
    ("infile,I", bpo::value<std::string>(), "Name of the input file, Mutually exclusive to -P")
    ("port,P", bpo::value< vector<int> >(), "Port numbers to accept data from; Can be specified multiple times; Mutually exclusive to -I")
    ("ip", bpo::value<std::string>(), "Hostname/IP address on which to accept the data (not implemented)")
    ("timeoutStart,S", bpo::value<float>(), "Time-out when opening socket connection, [sec].")
    ("timeoutRead,R", bpo::value<float>(), "Time-out when while reading from socket, [sec].")
    ("fixTimes,F", bpo::value<int>(), "Fix broken time-stamps old style (1), new style (2, default), or not (0)")
    ("doCheckCRC,C", bpo::value<int>(), "Check the CRCs: (0) no check, (1,default) check header.")
    ("bufferSize,B", bpo::value<int>(), "Size of the input buffer, [frames] (default=50000, about 100MB).")
    ("keepRunning,K", "Keep running, i.e. process more than one event by restarting the procedure.")
    ("waitForAll,W", "Wait until (some) data was received on all ports.")
    ("multipeStations,M", "Process data from multiple stations into seperate files. (implies -K)")
    ("raiseIOprio", "Raise IO priority to \"real time\" (if possible).")
    ("verbose,V", "Verbose mode on")
    ;

  bpo::variables_map vm;
  bpo::store (bpo::parse_command_line(argc,argv,desc), vm);

  if (vm.count("help") || argc == 1)
  {
    cout << "\n" << desc << endl;
    return 0;
  }

  if (vm.count("verbose"))
  {
    verboseMode=true;
  }

  if (vm.count("observer"))
  {
    observer = vm["observer"].as<std::string>();
  }

  if (vm.count("project"))
  {
    project = vm["project"].as<std::string>();
  }

  if (vm.count("observationID"))
  {
    observationID = vm["observationID"].as<std::string>();
  }

  if (vm.count("filterSelection"))
  {
    filterSelection = vm["filterSelection"].as<std::string>();
  }

  if (vm.count("antennaSet"))
  {
    antennaSet = vm["antennaSet"].as<std::string>();
  }

  if (vm.count("waitForAll"))
  {
    waitForAll=true;
  }

  if (vm.count("keepRunning"))
  {
    keepRunning=true;
  }

  if (vm.count("multipeStations"))
  {
    multipeStations=true;
    keepRunning=true;
  }

  if (vm.count("raiseIOprio"))
  {
    raiseIOprio=true;
  }

  if (vm.count("infile"))
  {
    infile     = vm["infile"].as<std::string>();
    socketmode = 0;
  };

  if (vm.count("outfile"))
  {
    outfile = vm["outfile"].as<std::string>();
  }

  if (vm.count("ip"))
  {
    ip = vm["ip"].as<std::string>();
  }

  if (vm.count("port"))
  {
    ports = vm["port"].as< vector<int> >();
    socketmode = 1;
    //      port = ports[0];
    cout << " specified ports: " << ports << endl;
  }

  if (vm.count("timeoutStart"))
  {
    timeoutStart = vm["timeoutStart"].as<float>();
  }

  if (vm.count("timeoutRead"))
  {
    timeoutRead = vm["timeoutRead"].as<float>();
  }

  if (vm.count("fixTimes"))
  {
    fixTransientTimes = vm["fixTimes"].as<int>();
  }

  if (vm.count("doCheckCRC"))
  {
    doCheckCRC = vm["doCheckCRC"].as<int>();
  }

  if (vm.count("bufferSize"))
  {
    input_buffer_size = vm["bufferSize"].as<int>();
  }


  // -----------------------------------------------------------------
  // Check the provided input

  if (vm.count("infile") && vm.count("port"))
  {
    cout << "[TBBraw2h5] Both input file and port number given, chose one of the two!" << endl;
    cout << endl << desc << endl;
    return 1;
  };

  if (socketmode == -1)
  {
    cout << "[TBBraw2h5] Neither input file nor port number given, chose one of the two!" << endl;
    cout << endl << desc << endl;
    return 1;
  };

  if (input_buffer_size < 100) 
  {
    cout << "[TBBraw2h5] Buffer Size too small ("<< input_buffer_size << "<100), setting to default value" << endl;
    input_buffer_size = 50000;
  };

  if (keepRunning && !socketmode)
  {
    cout << "[TBBraw2h5] KeepRunning only usefull in socketmode, option disabled!" << endl;
    keepRunning = false;
  };

  // -----------------------------------------------------------------
  // Feedback on the settings

  if (verboseMode)
  {
    std::cout << "[TBBraw2h5] Summary of parameters."        << std::endl;
    std::cout << "-- Socket mode    = " << socketmode          << std::endl;
    std::cout << "-- Output file    = " << outfile             << std::endl;
    std::cout << "-- CRC checking   = " << doCheckCRC          << std::endl;
    std::cout << "-- Fix Times      = " << fixTransientTimes   << std::endl;
    std::cout << "-- Raise Priority = " << raiseIOprio         << std::endl;
    if (socketmode) {
      std::cout << "-- IP address      = " << ip             << std::endl;
      //	std::cout << "-- Port number     = " << port           << std::endl;
      std::cout << "-- Port numbers    = " << ports          << std::endl;
      std::cout << "-- Timeout (start) = " << timeoutStart   << std::endl;
      std::cout << "-- Timeout (read)  = " << timeoutRead    << std::endl;
      std::cout << "-- Wait for ports  = " << waitForAll     << std::endl;
      std::cout << "-- Keep Running    = " << keepRunning    << std::endl;
      std::cout << "-- Multipe Stations= " << multipeStations    << std::endl;
    }
    else {
      std::cout << "-- Input file   = " << infile  << std::endl;
    }
  }


  // -----------------------------------------------------------------
  // try to raise the IO priority if requested
  if (raiseIOprio) {
#if defined linux
    increase_io_priority(verboseMode);
#else
    std::cerr << "[TBBraw2h5] Warning: option 'raiseIOprio' only supported on Linux!"
      << std::endl;
#endif
  };

  // -----------------------------------------------------------------
  // Process data from multiple stations, returns only in case of an error

  if (multipeStations) {
    readStationsFromSockets(ports, ip, timeoutStart, timeoutRead, outfile, observer, project, observationID, filterSelection, antennaSet, verboseMode);
    return 1;
  };

  // -----------------------------------------------------------------
  // Process data in socket mode

  if (socketmode) {
    // -----------------------------------------------------------------
    // Begin of "keepRunning" loop
    do 
    {
      tbb = NULL;

      readFromSockets(ports, ip, timeoutStart, timeoutRead, verboseMode, waitForAll, outfile, observer, project, observationID, filterSelection, antennaSet, doCheckCRC, fixTransientTimes);

      // -----------------------------------------------------------------
      // Finish up, print some statistics.
      tbb->summary();

      delete tbb;
    } while (keepRunning);
    return 0;
  }

  // -----------------------------------------------------------------
  // Process data in file mode

  if (keepRunning) { 
    outfileOrig = outfile ; 
  };
  // -----------------------------------------------------------------
  // Begin of "keepRunning" loop
  do 
  {
    if (keepRunning) 
    {
      std::ostringstream temp;
      temp << outfileOrig << "-" << runNumber << ".h5";
      outfile = temp.str();
      runNumber++;
    };

    // -----------------------------------------------------------------
    // Generate TBBraw object and open output file

    tbb = new DAL::TBBraw(outfile);
    if ( !tbb->isConnected() )
    {
      cout << "[TBBraw2h5] Failed to open output file." << endl;
      return 1;
    };

    // -----------------------------------------------------------------
    // Set the options in the TBBraw object

    if (doCheckCRC>0) {
      tbb->doHeaderCRC(true);
    }
    else {
      tbb->doHeaderCRC(false);
    };
    tbb->setFixTimes(fixTransientTimes);

    // -----------------------------------------------------------------
    // call the conversion routines

    readFromFile(infile, verboseMode);

    // -----------------------------------------------------------------
    //finish up, print some statistics.
    tbb->summary();

    // and free the memory:
    delete tbb;


    // -----------------------------------------------------------------
    // End of "keepRunning" loop
  } 
  while (keepRunning);

  return 0;
};
