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

#include <dalLopesEvent.h>

/*!
  \file tdalLopesEvent.cpp

  \ingroup DAL
  
  \brief A collection of tests for the dalLopesEvent class
						
  \author Lars B&auml;hren

  \date 2007/04/12
*/

// ------------------------------------------------------------------------------

/*!
  \brief Test constructors for a new dalLopesEvent object

  \return nofFailedTests -- The number of failed tests
*/
int test_constructors (std::string const &filename)
{
  int nofFailedTests (0);

  std::cout << "[1] Testing default constructor..." << std::endl;
  try {
    dalLopesEvent event;
    event.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
  }

  std::cout << "[2] Testing argumented constructor..." << std::endl;
  try {
    dalLopesEvent event (filename);
    event.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
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
  int nofFailedTests (0);

  std::cout << "[1] Testing access to data..." << std::endl;
  try {
    std::cout << "-- Opening file << " << filename << " ..." << std::endl;
    dalLopesEvent event (filename);
    std::cout << "-- Retrieving data ..." << std::endl;
    blitz::Array<short,2> data = event.channeldata();
    std::cout << "-- Shape of the data array = " << data.shape() << std::endl;
  } catch (std::string message) {
    std::cerr << message << std::endl;
  }
  
  return nofFailedTests;  
}

// ------------------------------------------------------------------------------

/*!
  \brief main routines of the test program
  
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
  
  return nofFailedTests;
}
