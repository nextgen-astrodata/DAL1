/*-------------------------------------------------------------------------*
 | $Id:: templates.h 391 2007-06-13 09:25:11Z baehren                    $ |
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
  \file tbb2h5.cpp

  \ingroup DAL

  \brief Write TBB time-series data into an HDF5 file.

  \author Joseph Masters, Lars B&auml;hren

  <h4>Usage</h4>

  \t tbb2h5 supports reading in the raw TBB time-series data from two different
  types of sources: in \e file-mode data are read in from a dump file, whereas in
  \e socket-mode input is accepted from a socket.

  <table border="0">
    <tr>
    <td class="indexkey">Command line</td>
    <td class="indexkey">Decription</td>
    </tr>
    <tr>
      <td>--help</td>
      <td>Show help messages</td>
    </tr>
    <tr>
      <td>--outfile arg</td>
      <td>Name of the output dataset</td>
    </tr>
    <tr>
      <td>--infile arg</td>
      <td>Name of the input file</td>
    </tr>
    <tr>
      <td>--mode arg</td>
      <td>Operation mode: file (0), socket (1)</td>
    </tr>
    <tr>
      <td>--ip arg</td>
      <td>IP address from which to accept the data</td>
    </tr>
    <tr>
      <td>--port arg</td>
      <td>Port number to accept data from</td>
    </tr>
  </table>

  Even though \e mode serves as main switch for deciding whether \b tbb2h5
  attempts to read from a file or from a socket, the secondary parameters --
  \e infile for file mode, \e ip and \e port for socket mode -- can be used to 
  determine the basic behaviour.

  <ul>
    <li>Concert file with dump of raw TBB data to a HDF5 file:
    \verbatim
    tbb2h5 --infile <raw data> --outfile <HDF5 output>
    \endverbatim
    <li>Read data from socket to a HDF5 file:
    \verbatim
    tbb2h5 --ip <IP address> --port <port number> --outfile <HDF5 output>
    \endverbatim
  </ul>
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef TBB_H
#include "TBB.h"
#endif

#include <boost/program_options.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/detail/cmdline.hpp>
namespace bpo = boost::program_options;

using namespace DAL;

int main(int argc, char *argv[])
{

  std::string infile;
  std::string outfile;
  std::string ip;
  std::string port;
  int socketmode (0);

  // -----------------------------------------------------------------
  // Processing of command line options

  bpo::options_description desc ("[tbb2h5] Available command line options");
  
  desc.add_options ()
    ("help,H", "Show help messages")
    ("outfile,O",bpo::value<std::string>(), "Name of the output dataset")
    ("infile,I", bpo::value<std::string>(), "Name of the input file")
    ("mode,M", bpo::value<int>(), "Operation mode: file (0), socket (1)")
    ("ip", bpo::value<std::string>(), "IP address from which to accept the data")
    ("port,P", bpo::value<std::string>(), "Port number to accept data from")
    ;
  
  bpo::variables_map vm;
  bpo::store (bpo::parse_command_line(argc,argv,desc), vm);

  if (vm.count("help") || argc == 1) {
    std::cout << desc << std::endl;
  }

  if (vm.count("infile")) {
    infile     = vm["infile"].as<std::string>();
    socketmode = 0;
  }

  if (vm.count("outfile")) {
    outfile = vm["outfile"].as<std::string>();
  }

  if (vm.count("ip")) {
    ip = vm["ip"].as<std::string>();
    socketmode = 1;
  }

  if (vm.count("port")) {
    port = vm["port"].as<std::string>();
    socketmode = 1;
  }

  if (vm.count("mode")) {
    socketmode = vm["mode"].as<int>();
  }

  // -----------------------------------------------------------------
  // Check the provided input

  switch (socketmode) {
  case 0:
    if (!vm.count("infile") || !vm.count("outfile")) {
      std::cerr << "Incomplete list of parameters for file mode!" << std::endl;
      std::cerr << desc << std::endl;
      return DAL::FAIL;
    }
    break;
  case 1:
    if (!vm.count("ip") || !vm.count("socket")) {
      std::cerr << "Incomplete list of parameters for socket mode!" << std::endl;
      std::cerr << desc << std::endl;
      return DAL::FAIL;
    }
    break;
  }

  // -----------------------------------------------------------------
  // Start processing of the input data
  
  TBB tbb = TBB( outfile );

  if ( socketmode )  // socket mode?
    {
      tbb.connectsocket( ip.c_str(), port.c_str() );
    }
  else  // reading from a file
    {
      if ( !tbb.openRawFile( infile.c_str() ) )
        return DAL::FAIL;
    }
  
  int counter = 0;

  if (socketmode)  // reading from a socket
    {
      while ( true )
        {
          counter++;

          if ( !tbb.readRawSocketBlockHeader() )
            break;

          tbb.stationCheck();

          // if this is the first sample for this station, set header attributes
          if ( tbb.first_sample )
            {
              tbb.makeOutputHeader();
              tbb.first_sample = false;
            }

          if ( tbb.transientMode() )
            {
#ifdef DEBUGGING_MESSAGES
              cerr << "block " << counter << endl;
#endif
              if ( !tbb.processTransientSocketDataBlock() )
                break;
            }

        } // while (true)

    }
  else  // reading from a file
    {
      while ( !tbb.eof() )
        {
          counter++;

          tbb.readRawFileBlockHeader();

          tbb.stationCheck();

          // if this is the first sample for this station set header attributes
          if ( tbb.first_sample )
            {
              tbb.makeOutputHeader();
              tbb.first_sample = false;
            }

#ifdef DEBUGGING_MESSAGES
          cerr << "block " << counter << endl;
#endif

          if ( tbb.transientMode() )
            {
              tbb.processTransientFileDataBlock();
            }
          else  // spectral mode
            {
              tbb.processSpectralFileDataBlock();
            }

        } // !eof

    } // socket or file

  cerr << "SUCCESS" << endl;
  return DAL::SUCCESS;

} // main
