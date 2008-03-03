/*-------------------------------------------------------------------------*
 | $Id:: BeamGroup.h 1126 2007-12-10 17:14:20Z masters                   $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2007 by Joseph Masters                                  *
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

#ifndef BEAMGROUP_H
#include "BeamGroup.h"
#endif

namespace DAL {

  BeamGroup::BeamGroup()
  {
    init();
  }

  void BeamGroup::init()
  {
    H5fileID_p        = -1;
    group_p           = new dalGroup();
    group_p->setName ("UNDEFINED");
  }

  BeamGroup::BeamGroup (dalDataset &dataset, std::string const &name)
  {
    bool status (true);

    init ();

    status = setBeamGroup (dataset, name);
  }

  void BeamGroup::summary(std::ostream &os)
  {

    os << "\n[" << group_p->getName() << " Group] Summary of object properties"  << endl;

    os << "-- RA ................... : " << ra()              << endl;
    os << "-- DEC .................. : " << dec()             << endl;
    os << "-- Number of Sub-bands .. : " << n_subbands()      << endl;

    os << endl;
    os << "-- Sub-bands : " << endl;
    vector<string> memnames = group_p->getMemberNames();
    for (unsigned int jj=0; jj<memnames.size(); jj++)
    {
       os << memnames[jj] << endl;
       dalTable * table = dataset_p.openTable(memnames[jj],group_p->getName());

       dalColumn * col;
       dalData * data;

       col = table->getColumn("X");
       os << "X: ";
       data = col->data( 0, 5);
       complex<char> * x;
       for (int jj=0; jj<5; jj++)
       {
         x = (complex<char>*)data->get(jj);
         printf("(%d,%d), ",x->real(),x->imag());
       }
       printf("\b\b\n");
       col->close();

       col = table->getColumn("Y");
       os << "Y: ";
       data = col->data( 0, 5);
       complex<char> * y;
       for (int jj=0; jj<5; jj++)
       {
         y = (complex<char>*)data->get(jj);
         printf("(%d,%d), ",y->real(),y->imag());
       }
       printf("\b\b\n");
       col->close();

       delete col;
    }
    os << endl;

  }

  BeamSubband * BeamGroup::getSubband( int sb )
  {
    return &beamSubbands_p[ sb ];
  }

  bool BeamGroup::setBeamGroup (dalDataset &dataset, std::string const &name)
  {
    bool status (true);

    try {
      dataset_p = dataset;
      H5fileID_p = dataset.getFileHandle();
      group_p    = dataset.openGroup(name);
    } catch (std::string message) {
      std::cerr << "[BeamGroup::setBeamGroup] " << message << endl;
      status = false;
    }

    return status;
  }

  std::string BeamGroup::ra ()
  {
    std::string attribute_ra ("");

    if (group_p->getName() != "UNDEFINED") {
      try {
	char * ra = reinterpret_cast<char*>(group_p->getAttribute("RA"));
	attribute_ra = string(ra);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute RA" << endl;
	attribute_ra = "";
      }
    }
   return attribute_ra;
  }

  std::string BeamGroup::dec ()
  {
    std::string attribute_dec ("");

    if (group_p->getName() != "UNDEFINED") {
      try {
	char * dec = reinterpret_cast<char*>(group_p->getAttribute("DEC"));
	attribute_dec = string(dec);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute DEC" << endl;
	attribute_dec = "";
      }
    }
   return attribute_dec;
  }

  int BeamGroup::n_subbands ()
  {
    int n_subbands;
    if (group_p->getName() != "UNDEFINED") {
      try {
	int * n_subbands_p = reinterpret_cast<int*>(group_p->getAttribute("NUMBER_OF_SUBBANDS"));
	n_subbands = *n_subbands_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute NUMBER_OF_SUBBANDS" << endl;
	n_subbands = -1;
      }
    }
    return n_subbands;
  }


  void BeamGroup::getSubbandData_X( int subband, int start, int length )
  {
    vector<string> memnames = group_p->getMemberNames();
    dalTable * table = dataset_p.openTable(memnames[ subband ],group_p->getName());
    dalColumn * col;
    dalData * data;

    col = table->getColumn("X");
    cerr << "X: ";
    data = col->data( start, length );
    complex<char> * x;
    for (int jj=0; jj < length; jj++)
    {
      x = (complex<char>*)data->get(jj);
      printf("(%d,%d), ",x->real(),x->imag());
    }
    printf("\b\b\n");
    col->close();

    delete col;
  }

  void BeamGroup::getSubbandData_Y( int subband, int start, int length )
  {
    vector<string> memnames = group_p->getMemberNames();
    dalTable * table = dataset_p.openTable(memnames[ subband ],group_p->getName());
    dalColumn * col;
    dalData * data;

    col = table->getColumn("Y");
    cerr << "Y: ";
    data = col->data( start, length );
    complex<char> * x;
    for (int jj=0; jj < length; jj++)
    {
      x = (complex<char>*)data->get(jj);
      printf("(%d,%d), ",x->real(),x->imag());
    }
    printf("\b\b\n");
    col->close();

    delete col;
  }

} // end namespace DAL
