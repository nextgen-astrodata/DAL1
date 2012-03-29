/***************************************************************************
 *   Copyright (C) 2011                                                    *
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


#include <dal1_config.h>
#include <iostream>
#include <boost/program_options.hpp>

namespace bpo = boost::program_options;

int main (int argc, char *argv[])
{
  int status = 0;

  bpo::options_description desc ("[dal1-config] Available command line options");

  desc.add_options ()
  ("help,H",         "Show help messages")
  ("all,A",          "Print all available information")
  ("version",        "Print version of DAL1.")
  ("version-major",  "Print major version of DAL1.")
  ("version-minor",  "Print minor version of DAL1.")
  ("version-patch",  "Print patch version of DAL1.")
  ("version-cmake",  "Print full version of CMake used for build.")
  ("version-system", "Print name of the system the DAL1 was built for.")
  ;

  bpo::variables_map vm;
  bpo::store (bpo::parse_command_line(argc,argv,desc), vm);

  /*________________________________________________________
    Process command line options
  */
  
  if (vm.count("help") || argc == 1) {
    std::cout << "\n" << desc << std::endl;
    return 0;
  } else if (vm.count("all")) {
    std::cout << "DAL1 version         : " << DAL1_VERSION   << std::endl;
    std::cout << "CMake version       : " << CMAKE_VERSION << std::endl;
    std::cout << "System name/version : " << CMAKE_SYSTEM  << std::endl;
  } else if (vm.count("version")) {
    std::cout << "DAL1 version : " << DAL1_VERSION << std::endl;
  } else if (vm.count("version-major")) {
    std::cout << "DAL1 major version : " << DAL1_VERSION_MAJOR << std::endl;
  } else if (vm.count("version-minor")) {
    std::cout << "DAL1 minor version : " << DAL1_VERSION_MINOR << std::endl;
  } else if (vm.count("version-patch")) {
    std::cout << "DAL1 patch version : " << DAL1_VERSION_PATCH << std::endl;
  } else if (vm.count("version-cmake")) {
    std::cout << "CMake version : " << CMAKE_VERSION << std::endl;
  } else if (vm.count("version-patch")) {
    std::cout << "System name/version : " << CMAKE_SYSTEM << std::endl;
  }
  
  return status;
}
