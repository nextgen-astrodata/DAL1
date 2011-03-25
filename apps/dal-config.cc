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


#include <dal_config.h>
#include <iostream>
#include <boost/program_options.hpp>

namespace bpo = boost::program_options;

int main (int argc, char *argv[])
{
  int status = 0;

  bpo::options_description desc ("[dal-config] Available command line options");

  desc.add_options ()
  ("help,H",  "Show help messages")
  ("version", "Print version of DAL.")
  ("version-major", "Print major version of DAL.")
  ("version-minor", "Print minor version of DAL.")
  ("version-patch", "Print path version of DAL.")
  ;

  bpo::variables_map vm;
  bpo::store (bpo::parse_command_line(argc,argv,desc), vm);

  /*________________________________________________________
    Process command line options
  */
  
  if (vm.count("help") || argc == 1) {
    std::cout << "\n" << desc << std::endl;
    return 0;
  }
  
  return status;
}
