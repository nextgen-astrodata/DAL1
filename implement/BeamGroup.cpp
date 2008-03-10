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
    std::vector< std::complex<char> > xvals;
    std::vector< std::complex<char> > yvals;


    for (unsigned int jj=0; jj<memnames.size(); jj++)
    {
       os << memnames[jj] << endl;

       xvals.clear();
       yvals.clear();

       getSubbandData_XY( 0, 0, 10, xvals, yvals );

       for (unsigned int ii=0; ii < xvals.size(); ii++ ) {

           printf( "{ (%d,%d),(%d,%d) }," , xvals[ii].real(),
                                            xvals[ii].imag(),
                                            yvals[ii].real(),
                                            yvals[ii].imag() );
       }
       os << endl << endl;
    }
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


  void BeamGroup::getSubbandData_XY( int subband,
                                     int start,
                                     int length,
                                     std::vector< std::complex<char> > &x_values,
                                     std::vector< std::complex<char> > &y_values )
  {
     getSubbandData_X( subband, start, length, x_values );
     getSubbandData_Y( subband, start, length, y_values );
  }


  void BeamGroup::getSubbandData_X( int subband,
                                    int start,
                                    int length,
                                    std::vector< std::complex<char> > &values )
  {
    dalTable * table;
    dalColumn * col;
    dalData * data;

    complex<char> * xx;
    vector<string> memnames = group_p->getMemberNames();

    table = dataset_p.openTable(memnames[ subband ],group_p->getName());
    col = table->getColumn("X");
    data = col->data( start, length );

    for (int jj=0; jj < length; jj++)
    {
      xx = (complex<char>*)data->get(jj);
      values.push_back(*xx);
    }

    delete col;
  }

  void BeamGroup::getSubbandData_Y( int subband,
                                    int start,
                                    int length,
                                    std::vector< std::complex<char> > &values )
  {
    complex<char> * yy;
    dalTable * table;
    dalColumn * col;
    dalData * data;

    vector<string> memnames = group_p->getMemberNames();

    table = dataset_p.openTable(memnames[ subband ],group_p->getName());
    col = table->getColumn("Y");
    data = col->data( start, length );

    for (int jj=0; jj < length; jj++)
    {
      yy = (complex<char>*)data->get(jj);
      values.push_back(*yy);
    }

    delete col;
  }

#ifdef PYTHON
/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/

void BeamGroup::summary_boost()
{
  summary();
}

bpl::numeric::array BeamGroup::getSubbandData_X_boost( int subband,
                                                       int start,
                                                       int length )
{
  std::vector< std::complex<char> > values;
  getSubbandData_X( subband, start, length, values );
  bpl::list value_list;

  for (unsigned int ii=0; ii<values.size(); ii++)
  {
     std::complex<float> foo(values[ii].real(),values[ii].imag());
     value_list.append( foo );
  }

  bpl::numeric::array narray = num_util::makeNum( value_list );

  return narray;
}

bpl::numeric::array BeamGroup::getSubbandData_Y_boost( int subband,
                                                       int start,
                                                       int length )
{
  std::vector< std::complex<char> > values;
  getSubbandData_Y( subband, start, length, values );
  bpl::list value_list;

  for (unsigned int ii=0; ii<values.size(); ii++)
  {
     std::complex<float> foo(values[ii].real(),values[ii].imag());
     value_list.append( foo );
  }

  bpl::numeric::array narray = num_util::makeNum( value_list );

  return narray;
}

bpl::numeric::array BeamGroup::getSubbandData_XY_boost( int subband,
                                                        int start,
                                                        int length )
{
  std::vector< std::complex<char> > x_values;
  std::vector< std::complex<char> > y_values;
  getSubbandData_X( subband, start, length, x_values );
  getSubbandData_Y( subband, start, length, y_values );

  bpl::list x_value_list;
  bpl::list y_value_list;
  bpl::list xy_value_list;

  for (unsigned int ii=0; ii<x_values.size(); ii++)
  {
     std::complex<float> foo(x_values[ii].real(),x_values[ii].imag());
     x_value_list.append( foo );
  }
  for (unsigned int ii=0; ii<y_values.size(); ii++)
  {
     std::complex<float> foo(y_values[ii].real(),y_values[ii].imag());
     y_value_list.append( foo );
  }

  xy_value_list.append(x_value_list);
  xy_value_list.append(y_value_list);

  bpl::numeric::array narray = num_util::makeNum( xy_value_list );

  return narray;
}
#endif

} // end namespace DAL