/***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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
  \file tDatabase.cc

  \ingroup DAL
  \ingroup core

  \brief A collection of test routines for the DAL::Database class

  \author Lars B&auml;hren

  \date 2010/03/21
*/

#include <iostream>
#include <core/Database.h>

using std::cout;
using std::endl;

//_______________________________________________________________________________
//                                                              test_constructors

/*!
  \brief Test the various constructors for an object of type Database

  \param server   -- 
  \param name     -- 
  \param passwd   -- 
  \param database -- 

  \return nofFailedTests -- The number of failed tests encountered within this
          function
*/
int test_constructors (std::string const &server="pc-swinbank",
		       std::string const &name="lofar",
		       std::string const &passwd="cs1",
		       std::string const &database="pipeline")
{
  cout << "\n[tDatabase::test_constructors]\n" << endl;

  int nofFailedTests (0);
  
  cout << "[1] Testing Database() ..." << endl;
  try {
    DAL::Database db;
    db.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  cout << "[2] Testing Database(string,string,string,string) ..." << endl;
  try {
    // Create object
    DAL::Database db (server, name, passwd, database);
    db.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  cout << "[3] Testing Database(string,string,string,string,uint) ..." << endl;
  try {
    unsigned int port = 100;
    // Create object
    DAL::Database db (server, name, passwd, database,port);
    db.summary();
  } catch (std::string message) {
    std::cerr << message << endl;
    ++nofFailedTests;
  }
  
  return nofFailedTests;
}

//_______________________________________________________________________________
//                                                                           main

int main (int argc,
          char *argv[])
{
  int nofFailedTests   = 0;
  bool haveDataset     = false;
  std::string filename = "tHDF5Dataset.h5";
  
  //________________________________________________________
  // Process parameters from the command line
  
  if (argc < 2) {
    haveDataset = false;
  } else {
    filename    = argv[1];
    haveDataset = true;
  }

  //________________________________________________________
  // Run the tests

  nofFailedTests += test_constructors();
  
  return nofFailedTests;
}
