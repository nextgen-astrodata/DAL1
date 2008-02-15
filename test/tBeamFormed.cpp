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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef BEAMFORMED_H
#include <BeamFormed.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#ifndef DALGROUP_H
#include <dalGroup.h>
#endif

#define FILENAME "bf.h5"

int main()
{

  DAL::BeamFormed * bf = new DAL::BeamFormed(FILENAME);
  bf->summary();
  delete bf;

/*// create a dataset

// -------- open the dataset ----------

   dalDataset * ds;
   cout << "\nOpening an existing beam-formed HDF5 dataset called "
        << FILENAME << "... ";
   ds = new dalDataset;
   if ( DAL::SUCCESS != ds->open( FILENAME ) )
   {
      cout << "\nERROR: could not open " << FILENAME << endl;
	  return DAL::FAIL;
   }
   else
      cout << "opened" << endl;


// --------- print a file summary -------------

// --------- list the beam groups ---------

  cout << "\nGetting a list of the beam groups in the file...\n";
  vector<string> groupnames = ds->getGroupNames();
  if ( 0 == groupnames.size() )
    cout << "no groups in file." << endl;
  else
  {
    for (unsigned int jj=0; jj<groupnames.size(); jj++)
       cout << '[' << jj << "] " << groupnames[jj] << endl;
    cout << endl;
  }
  
// --------- open the first beam group -------

  dalGroup * mygroup;
  if ( groupnames.size() > 0 )
  {
    cout << "\nOpening the first group in the file... ";
    mygroup = ds->openGroup( groupnames[ 0 ] );
	if ( NULL == mygroup )
	 cout << "\nERROR: opening of group " << groupnames[0] << " failed\n";
	else
	 cout << "\'" << groupnames[0] << "\' opened\n";
	
  }
    
// --------- print a group summary -------------

// --------- grab a few keywords ----------
  if ( mygroup )
  {
    cout << "\nGetting some group attributes from " << groupnames[0] << "\'...\n";
    char * ra;
    ra = reinterpret_cast<char*>( mygroup->getAttribute("RA") );
    cout << "RA = " << ra << endl;
    char * dec;
    dec = reinterpret_cast<char*>( mygroup->getAttribute("DEC") );
    cout << "DEC = " << dec << endl;
  }

// --------- grab some data ------------

// --------- close the group ------------

  if ( mygroup )
  {
    cout << "\nClosing the group: \'" << groupnames[0] << "\'... ";
	if ( 0 != mygroup->close() )
	  cout << "\nERROR: problem closing group \'" << groupnames[0] << "\'.\n";
	else
	  cout << "closed.\n";
  }

// --------- close the dataset -----------

   cout << "\nClosing the dataset: \'" << FILENAME << "\'... ";
   if ( 0!= ds->close() )
	cout << "\nERROR: problem closing dataset \'" << FILENAME << "\'.\n";
   else
	cout << "closed.\n";
	
   delete ds;

   cout << "\nSUCCESS" << endl;
   return DAL::SUCCESS;
*/
}

