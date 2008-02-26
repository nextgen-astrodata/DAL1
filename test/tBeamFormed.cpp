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

#define FILENAME "bf.h5"

int main()
{

  DAL::BeamFormed * bf = new DAL::BeamFormed(FILENAME);
  bf->summary();
  std::vector<std::string> sources = bf->sources();
  delete bf;

}
