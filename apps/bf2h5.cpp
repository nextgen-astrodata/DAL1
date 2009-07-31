/*-------------------------------------------------------------------------*
| $Id::                                                                 $ |
*-------------------------------------------------------------------------*
***************************************************************************
*   Copyright (C) 2007 by Joseph Masters & Alwin de Jong                  *
*   jmasters@science.uva.nl                                               *
*   jong@astron.nl                                                        *
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
  \file bf2h5.cpp

  \ingroup DAL

  \brief Write Beam-Formed data into an HDF5 file.

  \author Joseph Masters, Lars B&auml;hren, Alwin de Jong

  This file represents a LOFAR Tied-Array (aka Beam-Formed, aka Pulsar) data
  reader.

  It reads raw data and dumps samples to the screen in blocks of
  155,648 per ~1 second.  There are no time stamps provided.  The output
  columns (as described by the first two lines) are:

  - sample number (1 to 155648 for each frame)
  - X coordinate as a complex number (Xr,Xi)
  - Y coordinate as a complex number (Yr,Yi)

  The very last line of output gives the cumulative observation time for the file.

  The program was written for and tested on little-endian machines

  <h3>Usage</h3>

  Command line interface:
  \verbatim
  -D [ --factor ] arg   Downsample with this integer factor
  -H [ --help ]         Show help messages
  -I [ --infile ] arg   Name of the input file
  -O [ --outfile ] arg  Name of the output dataset
	--version							Show version information
  --intensity           Compute total intensity
  \endverbatim

  If \e --intensity or \e --factor is set, the output file will contain
  subband tables with a TOTAL_INTENSITY^2 column. If neither of these parameters
  is set, the subband tables will contain real and imaginary polarization data
  at full resolution (XrXi,YrYi).

  Examples:
  <ol>
    <li>Simple conversion of \e reprocessed (!) correlator output to HDF5 file:
    \verbatim
    bf2h5 -I /path/B0329.cor -O B0329.h5
    \endverbatim
    This will cause an averaging of the total intensities over every 128
    samples.
    <li>Compute total intensities and downsample the dataset:
    \verbatim
    bf2h5 -I /path/B0329.cor -O B0329.h5 -D 128 
    \endverbatim
    This will downsample the data by calculating the average of the intensities over every 128
    samples.
  </ol>
*/

const static char *version = "$Id";

#ifndef BFRAW_H
#include "BFRaw.h"
#endif

// reading a complete binary file
#include <iostream> // for cout
#include <iomanip>  // for cout field width
#include <fstream>  // for file handle
#include <complex>  // for complex datatypes
#include <assert.h>
#ifdef HAVE_GSL
#include <gsl/gsl_fft_complex.h> // for fft's during channelization
#endif
#include <new>

#include <boost/program_options.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/detail/cmdline.hpp>
namespace bpo = boost::program_options;

using std::cerr;
using std::cout;
using std::endl;

//_______________________________________________________________________________
//                                                                 print_examples

/*!
  \brief Print list of usage examples

  \param os -- Stream to which the output is written
*/
void print_examples (std::ostream &os)
{
  os << "[bf2h5] Usage examples." << endl;
  os << endl;
  os << "1) Convert file with dump of BF data to a HDF5 file:" << endl;
  os << "  bf2h5 --infile <raw data> --outfile <HDF5 output>" << endl;
  os << endl;
  os << "2) Read data from TCP stream to a HDF5 file:" << endl;
  os << "  bf2h5 --port <port number> --outfile <HDF5 output>" << endl;
  os << endl;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc, char *argv[])
{
  std::string infile;
  std::string outfile;
  std::string ip;
  std::string port;
  bool socketmode       = false;
  bool doIntensity      = false;
  bool doDownsample     = false;
  bool doChannelization = false;
  uint dsFactor            = 1;


  // -----------------------------------------------------------------
  // Processing of command line options

  bpo::options_description desc ("[bf2h5] Available command line options");

  desc.add_options ()
	("help,H", "Show help messages")
	("factor,D", bpo::value<uint>(), "Downsample with this factor")
	("infile,I", bpo::value<std::string>(), "Name of the input file")
	("outfile,O",bpo::value<std::string>(), "Name of the output dataset")
  //			("source,S", bpo::value<std::string>(), "the source IP address from which to accept the data")
  ("port,P", bpo::value<std::string>(), "Port number to accept beam formed raw data from")
  //("downsample", "Downsampling of the original data")
	("intensity", "Compute total intensity")
	("version", "Show bf2h5 version information")
			;

  bpo::variables_map vm;
  bpo::store (bpo::parse_command_line(argc,argv,desc), vm);

	if (vm.count("version")) {
		std::cout << "bf2h5 version: " << version << std::endl;
		return 0;
	}
	
  if (vm.count("help") || argc == 1)
    {
      std::cout << "\n" << desc << endl;
      return 0;
    }

  if (vm.count("infile"))
    {
      infile = vm["infile"].as<std::string>();
    }

  if (vm.count("outfile"))
    {
      outfile = vm["outfile"].as<std::string>();
    }
  else
    {
      std::cerr << "[bf2h5] Missing name of output file!" << endl;
      std::cout << "\n" << desc << endl;
      return 0;
    }

  if (vm.count("port"))
    {
      port     = vm["port"].as<std::string>();
      socketmode = true;
    }

/*  if (vm.count("downsample"))
    {
      doDownsample = true;
      // can't downsample w/o total intensities
      doIntensity = true;
} */

  if (vm.count("intensity"))
    {
      doIntensity = true;
    }

  if (vm.count("factor"))
    {
      dsFactor = vm["factor"].as<uint>();
      // check parameter value
      if (dsFactor <= 0)
        {
          dsFactor = 1;
        }
				else if (dsFactor > 1) {
					doIntensity = true;
					doDownsample = true;
				}
    }

  // -----------------------------------------------------------------
  // Checks if parameters complete

  if (socketmode)
    {
      /*
      if (!vm.count("ip")) {
      std::cerr << "IP address of source not specified!" << endl;
      std::cerr << desc << endl;
      print_examples(std::cerr);
      return DAL::FAIL;
      }

      if (!vm.count("port")) {
      std::cerr << "Port number not specified!" << endl;
      std::cerr << desc << endl;
      print_examples(std::cerr);
      return DAL::FAIL;
      }
      */
      if (!vm.count("outfile"))
        {
          std::cerr << "Output file name not specified!" << endl;
          std::cerr << desc << endl;
          print_examples(std::cerr);
          return DAL::FAIL;
        }
    }
  else   // file mode
    {
      if (!vm.count("infile") || !vm.count("outfile"))
        {
          std::cerr << "Both infile and outfile need to be specified for file mode!" << endl;
          std::cerr << desc << endl;
          print_examples(std::cerr);
          return DAL::FAIL;
        }
    }

  // -----------------------------------------------------------------
  // Summary of the parameters provided from the command line

  std::cout << "[bf2h5] Summary of input parameters." << endl;
  if (socketmode)
    {
      std::cout << "   Socket mode:" << endl;
      //		std::cout << "-- IP address .............. : " << ip << endl;
      std::cout << "-- Port number ............ : " << port << endl;
    }
  else
    {
      std::cout << "   File mode:" << endl;
      std::cout << "-- Input file ............ : " << infile << endl;
      std::cout << "-- Output file ........... : " << outfile << endl;
    }
  std::cout << "-- Compute total intensity : " << doIntensity  << endl;
  std::cout << "-- Downsampling of data .. : " << doDownsample << endl;
  std::cout << "-- Downsampling factor ... : " << dsFactor       << endl;

  // -----------------------------------------------------------------
  // Start processing of the input data

  DAL::BFRaw bf = DAL::BFRaw( outfile,
                              doIntensity,
                              doDownsample,
                              doChannelization,
                              dsFactor );

  if (socketmode)
    {
      if (bf.connectsocket( port.c_str() ))   // server
        {
          if (bf.readRawSocketHeader())
            {
              bf.processBFRawDataBlockFromSocket();
            }
        }
    }
  else   // File mode
    {
      bf.openRawFile( infile.c_str() );
      bf.readRawFileHeader();
      bf.makeH5OutputFile();
      bf.processBlocks();
    }
  return 0;
}
