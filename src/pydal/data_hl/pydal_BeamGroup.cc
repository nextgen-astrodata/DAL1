/***************************************************************************
 *   Copyright (C) 2011                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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
  \file pydal_BeamGroup.cc

  \ingroup DAL
  \ingroup pydal

  \brief Python bindings for the DAL::BeamGroup class

  \author Lars B&auml;hren
*/

// DAL headers
#include <pydal.h>
#include <data_hl/BeamGroup.h>

using DAL::BeamGroup;

// ==============================================================================
//
//                                                                      BeamGroup
//
// ==============================================================================

boost::python::numeric::array BeamGroup::getIntensity_boost( int subband,
						   int start,
						   int length )
{
  float * values = NULL;
  values = getIntensity( subband, start, length );
  std::vector<int> mydims;
  mydims.push_back( length );
  boost::python::numeric::array narray = num_util::makeNum( values, mydims );
  delete [] values;
  values = NULL;
  return narray;
}

boost::python::numeric::array BeamGroup::getIntensitySquared_boost( int subband,
							  int start,
							  int length )
{
  float * values = NULL;
  values = getIntensitySquared( subband, start, length );
  std::vector<int> mydims;
  mydims.push_back( length );
  boost::python::numeric::array narray = num_util::makeNum( values, mydims );
  delete [] values;
  values = NULL;
  return narray;
}

boost::python::numeric::array BeamGroup::getSubbandData_X_boost( int subband,
						       int start,
						       int length )
{
  std::vector<std::complex<short> > values;
  getSubbandData_X( subband, start, length, values );
  
  std::complex<float> * value_list;
  value_list = new std::complex<float>[length];
  
  for (int ii=0; ii<length; ii++)
    {
      value_list[ii] = std::complex<float>(values[ii].real(),values[ii].imag());
    }
  std::vector<int> mydims;
  mydims.push_back( length );
  boost::python::numeric::array narray = num_util::makeNum( value_list, mydims );
  delete [] value_list;
  value_list = NULL;
  return narray;
}

boost::python::numeric::array BeamGroup::getSubbandData_Y_boost( int subband,
						       int start,
						       int length )
{
  std::vector<std::complex<short> > values;
  getSubbandData_Y( subband, start, length, values );
  
  std::complex<float> * value_list;
  value_list = new std::complex<float>[length];
  
  for (int ii=0; ii<length; ii++)
    {
      value_list[ii] = std::complex<float>(values[ii].real(),values[ii].imag());
    }
  std::vector<int> mydims;
  mydims.push_back( length );
  boost::python::numeric::array narray = num_util::makeNum( value_list, mydims );
  delete [] value_list;
  value_list = NULL;
  return narray;
}


boost::python::numeric::array BeamGroup::getSubbandData_XY_boost( int subband,
							int start,
							int length )
{
  std::vector< std::complex<short> > values_x;
  std::vector< std::complex<short> >::iterator xvalit;
  std::vector< std::complex<short> > values_y;
  std::vector< std::complex<short> >::iterator yvalit;
  values_x.clear();
  values_y.clear();
  getSubbandData_X( subband, start, length, values_x );
  getSubbandData_Y( subband, start, length, values_y );
  
  boost::python::list x_value_list;
  boost::python::list y_value_list;
  boost::python::list xy_value_list;
  
  for (xvalit=values_x.begin(); xvalit < values_x.end(); xvalit++)
    {
      std::complex<float> foo((*xvalit).real(),(*xvalit).imag());
      x_value_list.append( foo );
    }
  for (yvalit=values_y.begin(); yvalit < values_y.end(); yvalit++)
    {
      std::complex<float> foo((*yvalit).real(),(*yvalit).imag());
      y_value_list.append( foo );
    }
  
  xy_value_list.append(x_value_list);
  xy_value_list.append(y_value_list);
  
  boost::python::numeric::array narray = num_util::makeNum( xy_value_list );
  
  return narray;
}

void export_BeamGroup ()
{
  boost::python::class_<BeamGroup>("BeamGroup")
    .def( boost::python::init<>())
    .def( "fileID", &BeamGroup::fileID,
	  "Get the HDF5 file handle ID")
    .def( "groupID", &BeamGroup::groupID,
	  "Get the HDF5 group handle ID")
    .def( "ra", &BeamGroup::ra,
	  "Return ra value for the beam")
    .def( "dec", &BeamGroup::dec,
	  "Return dec value for the beam")
    .def( "nofSubbands", &BeamGroup::nofSubbands,
	  "Return number of subbands")
    .def( "getIntensity", &BeamGroup::getIntensity_boost,
	  "Get a numpy array of intensities for a given subband")
    .def( "getIntensitySquared", &BeamGroup::getIntensitySquared_boost,
	  "Get a numpy array of squared intensities for a given subband")
    .def( "getSubbandData_X", &BeamGroup::getSubbandData_X_boost,
	  "Get a numpy array of values for a given subband")
    .def( "getSubbandData_Y", &BeamGroup::getSubbandData_Y_boost,
	  "Get a numpy array of values for a given subband")
    .def( "getSubbandData_XY", &BeamGroup::getSubbandData_XY_boost,
	  "Get a numpy array of values for a given subband")
    ;
}
