/***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
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
  \file tBeamFormed.cpp

  \ingroup DAL

  \brief Test program for beam-formed functionality.

  \author Joseph Masters

  \date 12-04-07
*/

#ifndef BEAMFORMED_H
#include <BeamFormed.h>
#endif

int main(int argc,char *argv[])
{

  DAL::BeamFormed * bf;

  if (argc > 1)
    bf = new DAL::BeamFormed(argv[1]);
  else
  {
    cerr << "Please provide a HDF5 filename." << endl;
    exit(1);
  }
  bf->summary();

  DAL::BeamGroup * beam;
  beam = bf->getBeam( 0 );

  std::vector<std::string> sources = bf->sources();
  cerr << "Sources:" << endl;
  for (unsigned int idx=0; idx<sources.size(); idx++)
  {
    cerr << sources[ idx ] << endl;
  }

  int subband = 0;
  int start = 0;
  int length = 10;
  std::vector< std::complex<short> > xvals;
  std::vector< std::complex<short> > yvals;
  xvals.clear();
  yvals.clear();

  // step through the data for subband 0, 10 samples at a time, 10 times
  for (unsigned int count=0; count < 10; count++ )
  {
    beam->getSubbandData_XY( subband, start, length, xvals, yvals );

    for (unsigned int ii=0; ii < xvals.size(); ii++ )
    {
      printf( "{ (%d,%d),(%d,%d) }," , xvals[ii].real(),
                                       xvals[ii].imag(),
                                       yvals[ii].real(),
                                       yvals[ii].imag() );
    }
    printf("\n\n");
    xvals.clear();
    yvals.clear();
    start += length;
  }

  delete bf;

}
