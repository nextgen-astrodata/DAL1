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
  \file bf2h5.cpp

  \ingroup DAL

  \brief Write Beam-Formed data into an HDF5 file.

  \author Joseph Masters, Lars B&auml;hren
  
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
  -H [ --help ]         Show help messages
  -O [ --outfile ] arg  Name of the output dataset
  -I [ --infile ] arg   Name of the input file
  --downsample          Downsampling of the original data
  --intensity           Compute total intensity
  -F [ --factor ] arg   Downsampling factor
  \endverbatim

  If \e --intensity or \e --downsample is set, the output file will contain
  subband tables with a TOTAL_INTENSITY column. If neither of these parameters
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
    bf2h5 -I /path/B0329.cor -O B0329.h5 --intensity --downsample --factor 128
    \endverbatim
    This will cause an averaging of the total intensities over every 128
    samples.
  </ol>
*/

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

int main (int argc, char *argv[])
{
  std::string infile;
  std::string outfile;
  bool doIntensity      = false;
  bool doDownsample     = false;
  bool doChannelization = false;
  int factor            = 1;
  
  // -----------------------------------------------------------------
  // Processing of command line options

  bpo::options_description desc ("[bf2h5] Available command line options");
  
  desc.add_options ()
    ("help,H", "Show help messages")
    ("outfile,O",bpo::value<std::string>(), "Name of the output dataset")
    ("infile,I", bpo::value<std::string>(), "Name of the input file")
    ("downsample", "Downsampling of the original data")
    ("intensity", "Compute total intensity")
    ("factor,F", bpo::value<int>(), "Downsampling factor")
    ;

  bpo::variables_map vm;
  bpo::store (bpo::parse_command_line(argc,argv,desc), vm);
  
  if (vm.count("help") || argc == 1) {
    std::cout << "\n" << desc << endl;
    return 0;
  }
  
  if (vm.count("infile")) {
    infile     = vm["infile"].as<std::string>();
  } else {
    std::cerr << "[bf2h5] Missing name of input file!" << endl;
    std::cout << "\n" << desc << endl;
    return 0;
  }
  
  if (vm.count("outfile")) {
    outfile = vm["outfile"].as<std::string>();
  } else {
    std::cerr << "[bf2h5] Missing name of output file!" << endl;
    std::cout << "\n" << desc << endl;
    return 0;
  }

  if (vm.count("downsample")) {
    doDownsample = true;
    // can't downsample w/o total intensities
    doIntensity = true;
  }

  if (vm.count("intensity")) {
    doIntensity = true;
  }
  
  if (vm.count("factor")) {
    factor = vm["factor"].as<int>();
    // check parameter value
    if (factor <= 0) {
      factor = 1;
    }
  }
  
  // -----------------------------------------------------------------
  // Summary of the parameters provided from the command line

  std::cout << "[bf2h5] Summary of input parameters."          << endl;
  std::cout << "-- Input file ............ : " << infile       << endl;
  std::cout << "-- Output file ........... : " << outfile      << endl;
  std::cout << "-- Compute total intensity : " << doIntensity  << endl;
  std::cout << "-- Downsampling of data .. : " << doDownsample << endl;
  std::cout << "-- Downsampling factor ... : " << factor       << endl;

  // -----------------------------------------------------------------
  // Start processing of the input data
  
  DAL::BFRaw bf = DAL::BFRaw( outfile,
			      doIntensity,
			      doDownsample,
                              doChannelization,
			      factor );

  bf.openRawFile( infile.c_str() );
  bf.readRawFileHeader();
  bf.makeH5OutputFile();
  bf.processBlocks();
  return 0;
}
