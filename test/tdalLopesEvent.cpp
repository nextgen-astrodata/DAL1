/***************************************************************************
 *   Copyright (C) 2006                                                    *
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <fstream>
#include <dalLopesEvent.h>

/*!
  \file tdalLopesEvent.cpp

  \ingroup DAL
  
  \brief A collection of tests for the dalLopesEvent class
						
  \author Lars B&auml;hren

  \date 2007/04/12


  <h3>Usage</h3>

  In order to really test the reading of data, the test program needs to be
  provided with the path to a LOPES data file, e.g.:
  \verbatim
  ./tdalLopesEvent 2007.01.31.23\:59\:33.960.event
  \endverbatim

*/

// ------------------------------------------------------------------------------

/*!
  \brief Export the antenna-/channeldata to an ASCII-table

  The data will be exported to a simple ASCII table which then in turn can be
  plotted e.g. <a href="http://www.gnuplot.info">Gnuplot</a>:
  \verbatim
  set grid
  set xlabel 'Sample number'
  set ylabel 'ADC counts'
  plot 'lopesevent.data' u 1 t 'Antenna 1' w l, 'lopesevent.data' u 2 t 'Antenna 2' w l
  \endverbatim

  \param filename -- Name of the output file to which the data will be written
  \param data     -- [sample,antenna] Array containing the actual data stored in
                     original LOPES-Event file
 */
void export_data (std::string const &filename,
		  blitz::Array<short,2> const &data)
{
  std::ofstream outfile;
  int nofAntennas (data.columns());
  int nofSamples (data.rows());
  int antenna (0);
  int sample (0);

  std::cout << "-- Name of output file      = " << filename     << std::endl;
  std::cout << "-- Shape of the data array  = " << data.shape() << std::endl;
  std::cout << "-- nof. antennas            = " << nofAntennas  << std::endl;
  std::cout << "-- nof. samples per antenna = " << nofSamples   << std::endl;

  std::cout << "-- writing data to file ... " << std::flush;
  outfile.open (filename.c_str());
  for (sample=0; sample<nofSamples; sample++) {
    for (antenna=0; antenna<nofAntennas; antenna++) {
      outfile << data(sample,antenna) << "\t";
    }
    outfile << std::endl;
  }
  outfile.close();
  std::cout << "done" << std::endl;

}

// ------------------------------------------------------------------------------

/*!
  \brief Export the antenna-/channeldata to an ASCII-table

  The data will be exported to a simple ASCII table which then in turn can be
  plotted e.g. <a href="http://www.gnuplot.info">Gnuplot</a>:
  \verbatim
  set grid
  set xlabel 'Sample number'
  set ylabel 'ADC counts'
  plot 'lopesevent_cpp.data' u 1 t 'Antenna 1' w l, 'lopesevent_cpp.data' u 2 t 'Antenna 2' w l
  \endverbatim
  In order to compare this to the output from the Blitz++ array, also run the
  following:
  \code
  plot 'lopesevent.data' u 1 t 'Antenna 1 (Blitz++)' w l, 'lopesevent_cpp.data' u 1 t 'Antenna 1 (C++)' w l
  \endcode
  
  \param filename    -- Name of the output file to which the data will be written
  \param data        -- Array containing the actual data stored in original
                        LOPES-Event file
  \param nofAntennas -- Number of antennas in the data set
  \param blocksize   -- Number of samples per channel/antenna
*/
void export_data (std::string const &filename,
		  short *data,
		  unsigned int const &nofAntennas,
		  unsigned int const &blocksize)
{
  std::ofstream outfile;
  unsigned int antenna (0);
  unsigned int sample (0);
  unsigned int datapoint (0);
  
  std::cout << "-- Name of output file      = " << filename    << std::endl;
  std::cout << "-- nof. antennas            = " << nofAntennas << std::endl;
  std::cout << "-- nof. samples per antenna = " << blocksize   << std::endl;

  std::cout << "-- writing data to file ... " << std::flush;
  outfile.open (filename.c_str());
  for (sample=0; sample<blocksize; sample++) {
    for (antenna=0; antenna<nofAntennas; antenna++) {
      outfile << data[datapoint] << "\t";
      datapoint++;
    }
    outfile << std::endl;
  }
  outfile.close();
  std::cout << "done" << std::endl;

}

// ------------------------------------------------------------------------------

/*!
  \brief Element-by-element comparison of the data in the two arrays

  \param array1D -- Data contained in a 1-dim Blitz++ array
  \param data    -- Data contained in a C++ array
*/
void compare_data (blitz::Array<short,1> const &array1D,
		   short *data)
{
  unsigned int nelem (array1D.numElements());
  unsigned int nofDifferent (0);

  // Element-by-element comparison of the two arrays
  for (unsigned int sample(0); sample<nelem; sample++) {
    if (array1D(sample) != data[sample]) {
      nofDifferent++;
    }
  }

  // Summary of the check
  if (nofDifferent) {
    cout << "-- Found " << nofDifferent << "/" << nelem << " different values." << endl;
    cout << "   This corresponds to " << (100.0*nofDifferent)/nelem << "% of the data." << endl;
  }
}

// ------------------------------------------------------------------------------

/*!
  \brief Test constructors for a new dalLopesEvent object

  \return nofFailedTests -- The number of failed tests
*/
int test_constructors (std::string const &filename)
{
  std::cout << "\n[test_constructors]\n" << std::endl;

  int nofFailedTests (0);

  std::cout << "[1] Testing default constructor..." << std::endl;
  try {
    dalLopesEvent event;
    event.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  std::cout << "[2] Testing argumented constructor..." << std::endl;
  try {
    dalLopesEvent event (filename);
    event.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  return nofFailedTests;  
}

// ------------------------------------------------------------------------------

/*!
  \brief Test retrieving and working with the channeldata
  
  \return nofFailedTests -- The number of failed tests
*/
int test_channeldata (std::string const &filename)
{
  std::cout << "\n[test_channeldata]\n" << std::endl;

  int nofFailedTests (0);
  unsigned int nofAntennas (0);
  unsigned int blocksize (0);

  std::cout << "[1] Get all data as Blitz array..." << std::endl;
  try {
    std::cout << "-- Opening file " << filename << " ..." << std::endl;
    dalLopesEvent event (filename);
    std::cout << "-- Retrieving data ..." << std::endl;
    blitz::Array<short,2> data = event.channeldata();
    // export data to file
    export_data ("lopesevent.data",data);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[2] Get data per antenna as Blitz array..." << std::endl;
  try {
    std::cout << "-- Opening file " << filename << " ..." << std::endl;
    dalLopesEvent event (filename);
    nofAntennas = event.nofAntennas();
    blitz::Array<short,1> data (nofAntennas);
    std::cout << "-- Retrieving data for individual antennas ..." << std::endl;
    for (unsigned int ant(0); ant<nofAntennas; ant++) {
      data = event.channeldata (ant);
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  std::cout << "[3] Get all data as C++ array..." << std::endl;
  try {
    short *data;

    std::cout << "-- Opening file " << filename << " ..." << std::endl;
    dalLopesEvent event (filename);
    nofAntennas = event.nofAntennas();
    blocksize   = event.blocksize();
    std::cout << "-- Adjusting array to receive data ..." << std::endl;
    data = new short[event.nofDatapoints()];
    std::cout << "-- Retrieving data ..." << std::endl;
    data = event.data();
    // export data to file
    export_data ("lopesevent_cpp.data",
		 data,
		 nofAntennas,
		 blocksize);
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }
  
  std::cout << "[4] Get data per antenna as C++ array..." << std::endl;
  try {
    short *data;

    std::cout << "-- Opening file " << filename << " ..." << std::endl;
    dalLopesEvent event (filename);
    nofAntennas = event.nofAntennas();
    blocksize   = event.blocksize();
    std::cout << "-- Adjusting array to receive data ..." << std::endl;
    data = new short[blocksize];
    std::cout << "-- Retrieving data ..." << std::endl;
    for (unsigned int antenna(0); antenna<nofAntennas; antenna++) {
      event.data(data,antenna);
//       std::cout << "\t(" << antenna << ")\t"
// 		<< data[0] << " (" << &data[0] << ") .. "
// 		<< data[1] << " (" << &data[1] << ") .. "
// 		<< data[2] << " (" << &data[2] << ") "
// 		<< std::endl;
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  std::cout << "[5] Compare retrieval using C++ and Blitz++ arrays..." << std::endl;
  try {
    short *data;
    unsigned int nofAntennas (0);
    unsigned int blocksize (0);

    std::cout << "-- Opening file " << filename << " ..." << std::endl;
    dalLopesEvent event (filename);
    nofAntennas = event.nofAntennas();
    blocksize   = event.blocksize();
    std::cout << "-- Adjusting array to receive data ..." << std::endl;
    data = new short[blocksize];
    Array<short,1> channeldata (blocksize);
    std::cout << "-- Retrieving data ..." << std::endl;
    for (unsigned int antenna(0); antenna<nofAntennas; antenna++) {
      channeldata = event.channeldata (antenna);
      event.data(data,antenna);
      //
      cout << " ant=" << antenna
	   << "\t" << channeldata(0) << " .. " << data[0]
	   << "\t" << channeldata(1) << " .. " << data[1]
	   << "\t" << channeldata(2) << " .. " << data[2]
	   << endl;
      //
      compare_data (channeldata,data);
    }
  } catch (std::string message) {
    std::cerr << message << std::endl;
    nofFailedTests++;
  }

  return nofFailedTests;  
}
// ------------------------------------------------------------------------------

/*!
  \brief Generate some statistics for the input data

  In order to plot the generated output with Gnuplot use:
  \verbatim
  set grid
  set xlabel 'Antenna number'
  set ylabel 'ADC counts'
  plot 'lopesevent_statistics.data' u 1:2 t 'min(data)' w l,\
   'lopesevent_statistics.data' u 1:3 t 'mean(data)' w l,\
   'lopesevent_statistics.data' u 1:4 t 'max(data)' w l
  plot 'lopesevent_statistics.data' u 1:5 t 'sum(data)' w lp
  \endverbatim
 */
int test_statistics (std::string const &filename)
{
  std::cout << "\n[test_channeldata]\n" << std::endl;

  int nofFailedTests (0);
  std::ofstream outfile;
  
  std::cout << "-- Opening file " << filename << " ..." << std::endl;
  dalLopesEvent event (filename);
  // get the number of antenna in the data set
  unsigned int nofAntennas (event.nofAntennas());
  // Array for taking up the data
  blitz::Array<short,1> data (nofAntennas);
  
  std::cout << "-- Generating and exporting statistics ..." << std::endl;
  outfile.open("lopesevent_statistics.data");
  for (unsigned int ant(0); ant<nofAntennas; ant++) {
    data = event.channeldata (ant);
    // generate statistics and report them
    outfile << ant << "\t"
	    << min(data) << "\t"
	    << mean(data) << "\t"
	    << max(data) << "\t"
	    << sum(data) << "\t"
	    << count(data > 0.25*max(data)) << "\t"
	    << std::endl;
  }
  outfile.close();
  std::cout << "-- Done." << std::endl;
  
  return nofFailedTests;
}

// ------------------------------------------------------------------------------

/*!
  \brief Main routine of the test program
  
  \return nofFailedTests -- The number of failed tests
*/
int main (int argc, char *argv[])
{
  int nofFailedTests (0);
  std::string filename ("UNDEFINED");
  
  if (argc < 2) {
    cerr << "[tLopesEvent] Missing name of event file!" << endl;
  } else {
    filename = argv[1];
  }

  nofFailedTests += test_constructors (filename);

  nofFailedTests += test_channeldata (filename);
  
  nofFailedTests += test_statistics (filename);
  
  return nofFailedTests;
}
