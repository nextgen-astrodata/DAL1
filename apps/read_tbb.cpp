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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

int main(int argc, char *argv[])
{
  // parameter check
  if ( argc < 3 )
    {
      std::cerr << std::endl << "Too few parameters..." << std::endl << std::endl;
      std::cerr << "The first parameter is the tbb dataset name." << std::endl;
      std::cerr << "The second parameter is the dipole itentifier. "
                << "(i.e. \"001002003\").\n" << "  The first 3 characters are the "
                << "station id.\n  The next 3 are the rsp id.\n  The last 3 are the "
                << "rcu id." << std::endl;
      std::cerr << "The third parameter is the station group itentifier." << std::endl;
      std::cerr << std::endl;
      return DAL::FAIL;
    }

  DAL::dalDataset ds;
  if ( DAL::FAIL == ds.open( argv[1] ) )
    std::cerr << "ERROR: couldn't open file: " << argv[1] << std::endl;

  string id = argv[2];
  int start = 25;
  int length = 20;
  short data[length];
  ds.read_tbb( id, start, length, data );

  printf("\n");
  printf("Dataset: \n");
  for (int jj = 0; jj < length; jj++)
    {
      printf("%d ", data[jj]);
      printf("\n");
    }

  printf("\nStation groups:\n");
  std::vector<std::string> groups = ds.getGroupNames();
  for (uint idx=0; idx<groups.size(); idx++)
    std::cerr << groups[idx] << std::endl;
  std::cerr << std::endl;

  DAL::dalArray * array = NULL;
  if ( DAL::FAIL == ( array = ds.openArray( argv[2], argv[3] ) ) )
    std::cerr << "ERROR: couldn't open dipole: " << argv[2] << std::endl;

  std::cerr << "\nGetting TELESCOPE attribute from array...";
  std::string telescope("");
  if ( DAL::SUCCESS == ds.getAttribute( "TELESCOPE", telescope ) )
    std::cerr << telescope << " ...done.";
  else   std::cerr << "FAILED.";

  std::cerr << "\nGetting TIME attribute from array...";
  uint time = 0;
  if ( DAL::SUCCESS == array->getAttribute( "TIME", time ) )
    std::cerr << time << " ...done.";
  else   std::cerr << "FAILED.";

  std::cerr << "\n\nClosing integer array... ";
  if ( DAL::SUCCESS == array->close() )
    std::cerr << "done." << std::endl;
  else  std::cerr << "FAILED.";

  if ( DAL::FAIL == ds.close() )
    std::cerr << "ERROR: closing dataset failed\n";

  std::cerr << "\nSUCCESS" << std::endl;
  return DAL::SUCCESS;
}
