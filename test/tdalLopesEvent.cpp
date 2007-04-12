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
  
  \brief A collection of tests for the dalLopesEvent class
						
  \author Lars B&auml;hren

  \date 2007/04/12
*/

int test_constructors ()
{
  int nofFailedTests (0);

  std::cout << "[1] Testing default constructor..." << std::endl;
  try {
    dalLopesEvent event;
    event.summary();
  } catch (std::string message) {
    std::cerr << message << std::endl;
  }

  return nofFailedTests;
  
}

int main ()
{
  int nofFailedTests (0);

  nofFailedTests += test_constructors ();

  return nofFailedTests;
}
