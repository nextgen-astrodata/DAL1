/******************************************************************************
 *
 *  This file represents a Lofar Tied-Array (aka Beam-Formed, aka Pulsar) data
 *  reader.
 *
 *   It reads raw data and dumps samples to the screen in blocks of
 *   155,648 per ~1 second.  There are no time stamps provided.  The output
 *   columns (as described by the first two lines) are:
 * 
 *    - sample number (1 to 155648 for each frame)
 *    - X coordinate as a complex number (Xr,Xi)
 *    - Y coordinate as a complex number (Yr,Yi)
 * 
 * The very last line of output gives the cumulative observation time for the file.
 * 
 * Usage is:  lta_reader <filename>
 * Example:   lta_reader 
 * 
 * The program was written for and tested on little-endian machines
 *
 * Author:  Joseph Masters <jmasters at science dot uva dot nl>
 * Created:  February 2007
 *
 ******************************************************************************/

/*!
  \file bf2h5.cpp
  
  \ingroup DAL

  \brief Write Beam-Formed data into an HDF5 file.

  \author Joseph Masters

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
#ifdef WITH_GSL
#include <gsl/gsl_fft_complex.h> // for fft's during channelization
#endif
#include <new>

using namespace DAL;

int main (int argc, char *argv[])
{

  // If less than two arguments provided, print usage
  if ( 3 != argc ) {
    cout << "Usage:  bf2h5 <input filename> <output filename>" << endl << endl;
    exit(1);
  }

  BFRaw bf = BFRaw( argv[2] );
  bf.openRawFile( argv[1] );
  bf.readRawFileHeader();
  bf.makeH5OutputFile();
  bf.processBlocks();
  return 0;
}
