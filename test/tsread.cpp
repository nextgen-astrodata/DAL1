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
/**
 * \file dal.cpp
 * \author Joseph Masters
 * \date 12-04-06
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#ifndef DALGROUP_H
#include <dalGroup.h>
#endif

//#include <mpi.h>       /* Include the MPI definitions */

const long BSIZE = 10000;
const int LOOPMAX = 10000;

/*! doxygen comment in dal.cpp */
int main(int argc, char *argv[])
{
  
  // parameter check
  if ( argc < 2 )
  {
     cout << endl << "Too few parameters..." << endl << endl;
     cout << "The first parameter is the dataset name." << endl;
     cout << "The second parameter is the filetype. (optional)" << endl;
     cout << endl;
     return FAIL;
  }


  dalDataset * dataset = new dalDataset();
/*
  if ( NULL == argv[2] )
	  dataset = new dalDataset( argv[1] );
  else
	  dataset = new dalDataset( argv[1], argv[2] );
  */
  
  if ( 0 != dataset->open( argv[1] ) )
  {
  	cout << "Problem opening dataset: " << argv[1] << '.' << " Quiting." << endl;
  	exit(FAIL);
  }
   
  // define the structure of a table
  // define the data to go in the table
  // create the table in the file or group
  dalGroup * stationGroup = dataset->openGroup("Station");

  cout << endl << "Station Group Attributes:" << endl;
  stationGroup->getAttribute_string("TELESCOPE");
  stationGroup->getAttribute_string("OBSERVER");
  stationGroup->getAttribute_string("PROJECT");
  stationGroup->getAttribute_string("OBSERVATION_ID");
  stationGroup->getAttribute_string("OBSERVATION_MODE");
  stationGroup->getAttribute_string("TRIGGER_TYPE");
  stationGroup->getAttribute_double("TRIGGER_OFFSET");
  stationGroup->getAttribute_int("TRIGGERED_ANTENNAS");
  //stationGroup->getAttribute_double("BEAM_DIRECTION", beam_direction, 2 );

  cout << endl;

  delete stationGroup;


  // define the structure of an image
  // define the data to go in the image
  // create the image in the file or group
//   dataset.createImage();
  

  delete dataset;
  
  cout << "SUCCESS" << endl;
  return SUCCESS;
}
