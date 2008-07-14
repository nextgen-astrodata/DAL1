/***************************************************************************
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

  \author Joseph Masters

  <h4>Usage</h4>

  In order to write the data distributed over a set of dumps from raw TBB
  data into a HDF5 time-series dataset:
  \verbatim
  tbb2h5 <outfile> <infile1 infile2 ...>
  \endverbatim
  where 
  <ul>
    <li><tt>outfile</tt> is the name of the created HDF5 output file
    <li><tt>infile1 infile2 ...</tt> is a list of input files containing the
    dump of the raw TBB data
  </ul>
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef TBB_H
#include "TBB.h"
#endif

using namespace DAL;

int main(int argc, char *argv[])
{

  // parameter check
  if ( argc < 3 )
  {
     cerr << endl << "Too few parameters..." << endl << endl;
     cerr << "First parameter is the output dataset name.\n";
     cerr << "Second parameter indicates file mode (0) or socket mode (1).\n";
     cerr << "Third parameter is either the IP address to accept data from,";
     cerr << " or the input file name.\n";
     cerr << "Fourth parameter is the port number to accept data from.\n";
     cerr << endl;
     return DAL::FAIL;
  }

  string outfilename( argv[1] );
  int socketmode( atoi(&argv[2][0]) );

  TBB tbb = TBB( outfilename );

  if ( socketmode )  // socket mode?
  {
    if ( argc < 4 )
    {
       cerr << "ERROR: missing parameters." << endl;
       return DAL::FAIL;
    }
    tbb.connectsocket( argv[3], argv[4] );
  }
  else  // reading from a file
  {
    if ( !tbb.openRawFile( argv[3] ) )
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
