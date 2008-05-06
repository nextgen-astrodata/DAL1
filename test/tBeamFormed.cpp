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

#include <dalDataset.h>
#include <dalFilter.h>

int main(int argc,char *argv[])
{

  DAL::BeamFormed * file;

  if (argc > 1)
    file = new DAL::BeamFormed(argv[1]);
  else
  {
    cerr << "Please provide a HDF5 filename." << endl;
    exit(1);
  }

  file->summary();

  DAL::BeamGroup * beam;
  beam = file->getBeam( 0 );

  std::vector<std::string> sources = file->sources();
  cerr << "Sources:" << endl;
  for (unsigned int idx=0; idx<sources.size(); idx++)
  {
    cerr << sources[ idx ] << endl;
  }
  cerr << endl;

  int subband = 0;
  int start = 0;
  int length = 144;//441344;
  std::vector< std::complex<short> > xvals;
  std::vector< std::complex<short> > yvals;
  xvals.clear();
  yvals.clear();

std::vector< std::complex<short> > xx;
xx.clear();
//xx = new std::complex<short>[length];
beam->getSubbandData_X( subband, start, length, xx );
// printf( "(%d,%d)" , xx[10000000].real(), xx[10000000].imag() );

for (int ii=0; ii < length; ii++ )
{
  printf( "(%d,%d)" , xx[ii].real(), xx[ii].imag() );
}

  // step through the data for subband 0, 10 samples at a time, 10 times
  for (unsigned int count=0; count < 10; count++ )
  {
    beam->getSubbandData_XY( subband, start, length, xvals, yvals );

    printf( "Values %d through %d\n", start, start + length );
    for (unsigned int ii=0; ii < xvals.size(); ii++ )
    {
      printf( "(%d,%d),(%d,%d)\n" , xvals[ii].real(),
                                    xvals[ii].imag(),
                                    yvals[ii].real(),
                                    yvals[ii].imag() );
    }
    printf("\n");
    xvals.clear();
    yvals.clear();
    start += length;
  }

  delete beam;
  delete file;

}
