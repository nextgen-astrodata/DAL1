/*------------------------------------------------------------------------*
| $Id::                                                                 $ |
*-------------------------------------------------------------------------*
***************************************************************************
*   Copyright (C) 2009 by ASTRON                                          *
*    Author: Alwin de Jong ( jong@astron.nl )                             *
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

#define DEBUGGING_MESSAGES

const static char *version = "4.0";

#include <iostream> // for cout
#include <iomanip>  // for cout field width
#include <fstream>  // for file handle
#include <complex>  // for complex datatypes
#include <boost/program_options.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/detail/cmdline.hpp>

#include "bf2h5.h"

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
	std::string parsetfile;
	std::string ip;
	uint port;
//	bool useParset			  = false;
	bool socketmode       = false;
	bool non_interactive  = false;
	bool doIntensity      = false;
	bool doDownsample     = false;
//	bool doChannelization = false;
	uint dsFactor            = 1;


  // -----------------------------------------------------------------
  // Processing of command line options

	bpo::options_description desc ("[bf2h5] Available command line options");

	desc.add_options ()
			("help,H", "Show help messages")
			("parsetfile,F", bpo::value<std::string>(), "Use parset file for all settings and to get information about input data")
			("downsample,D", bpo::value<uint>(), "Downsample with this factor")
			("infile,I", bpo::value<std::string>(), "Name of the input file")
			("outfile,O",bpo::value<std::string>(), "Name of the output dataset")
  //			("source,S", bpo::value<std::string>(), "the source IP address from which to accept the data")
			("port,P", bpo::value<uint>(), "Port number to accept beam formed raw data from")
  //("downsample", "Downsampling of the original data")
			("intensity", "Compute total intensity")
			("version", "Show bf2h5 version information")
			("noninteractive", "non-interactive mode, automatically overwrites output file if it exists")
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
	/*
	if (vm.count("parsetfile"))
	{
	parsetfile = vm["parsetfile"].as<std::string>();
	useParset = true;
}
	*/
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
		port = vm["port"].as<uint>();
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

	if (vm.count("downsample"))
	{
		dsFactor = vm["downsample"].as<uint>();
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
	if (vm.count("noninteractive")) {
		non_interactive = true; 
	}

  // -----------------------------------------------------------------
  // Checks if parameters complete

	if (socketmode)
	{
		if (!vm.count("outfile"))
		{
			std::cerr << "Output file name not specified!" << endl;
			std::cerr << desc << endl;
			print_examples(std::cerr);
			return 1;
		}
	}
	else   // file mode
	{
		if (!vm.count("infile") || !vm.count("outfile"))
		{
			std::cerr << "Both infile and outfile need to be specified for file mode!" << endl;
			std::cerr << desc << endl;
			print_examples(std::cerr);
			return 1;
		}
	}

  // -----------------------------------------------------------------
  // Summary of the parameters provided from the command line

	std::cout << "[bf2h5] Summary of input parameters." << endl;
	/*
	if (useParset) {
	std::cout << "-- Using parset file ..... : " << parsetfile << endl;
} */
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

	//check if output file exists
	std::fstream filestr;
	filestr.open(outfile.data());
	if ( filestr.is_open() ) {
		if (!non_interactive) {
			filestr.close();
			std::string line;
			std::cout << "Warning: output file exist. Do you want to overwrite? (y/n) [n]" << std::endl;
			std::getline( std::cin, line );
			if (!line.empty()) {
				if (line == "n" | line == "N") return 0;
				else if (line == "y" | line == "Y") {
					if( remove( outfile.data() ) == 0 ) {
						std::cout << "overwriting file" << std::endl;
					} else {
						perror( "Error deleting file" );
						std::cerr << "now exiting" << std::endl;
						return 0;
					}
				}
			} 
			else return 0; // empty line, default answer is no
		}
		else { // non_interactive mode, delete file and continue
			if( remove( outfile.data() ) != 0 ) {
				perror( "Error deleting file" );
				std::cerr << "now exiting" << std::endl;
				return 0;
			}
		}
	}
	BF2H5 bf2h5(outfile, dsFactor, doIntensity);
	
	if (socketmode)
	{
		bf2h5.setSocketMode(port);

	}
	else   // File mode
	{
		bf2h5.setFileMode(infile);
	}
	
	bf2h5.start();	
	
	return 0;
}
