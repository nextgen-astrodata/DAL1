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

namespace DAL
  {

  // ---------------------------------------------------------- BeamGroup

  /*!
    \brief Default constructor

    Default constructor

  */
  BeamGroup::BeamGroup()
  {
    init();
  }



  // ---------------------------------------------------------- BeamGroup

  /*!
    \brief Argumented constructor

    Argumented constructor

    \param dataset The dalDataset the the group belongs to.
    \param name The name of the group.
  */
  BeamGroup::BeamGroup (dalDataset &dataset, std::string const &name)
  {
    bool status (true);

    init ();

    status = setBeamGroup (dataset, name);
  }


  // ---------------------------------------------------------- init


  /*!
    \brief Initialize the object's internal parameters

    Initialize the object's internal parameters
  */
  void BeamGroup::init()
  {
    H5groupID_p       = -1;
    group_p           = NULL;
    H5fileID_p        = -1;
    group_p           = new dalGroup;
    group_p->setName ("UNDEFINED");
    beamSubbands_p.clear();
  }



  // ---------------------------------------------------------- ~BeamGroup

  /*!
    \brief Default destructor

    Default destructor
  */
  BeamGroup::~BeamGroup()
  {
    delete group_p;
  }


  // ---------------------------------------------------------- summary



  /*!
    \brief Provide a summary of the object's interal parameters

    Provide a summary of the object's interal parameters
  */
  void BeamGroup::summary(std::ostream &os)
  {

    os << "\n[" << group_p->getName() << " Group] Summary of object properties"  << endl;

    os << "-- RA ................... : " << ra()              << endl;
    os << "-- DEC .................. : " << dec()             << endl;
    os << "-- Number of Sub-bands .. : " << n_subbands()      << endl;

    os << endl;
    os << "-- Sub-bands : " << endl;
    vector<string> memnames = group_p->getMemberNames();
    std::vector< std::complex<short> > xvals;
    std::vector< std::complex<short> > yvals;
    xvals.clear();
    yvals.clear();


    /*    int subband = 0;
        int start = 0;
        int length = 10;
        for (unsigned int jj=0; jj<memnames.size(); jj++)
        {
           os << memnames[jj] << endl;

           xvals.clear();
           yvals.clear();

           getSubbandData_XY( start, subband, length, xvals, yvals );

           for (unsigned int ii=0; ii < xvals.size(); ii++ ) {

               printf( "{ (%d,%d),(%d,%d) }," , xvals[ii].real(),
                                                xvals[ii].imag(),
                                                yvals[ii].real(),
                                                yvals[ii].imag() );
           }
           os << endl << endl;
        }*/
  }


  // ---------------------------------------------------------- getSubband


  /*!
   \brief Get a subband object from the beam.

   Get a subband object from the beam.

   \param subband Number of the subband you want to retrieve.
  */
  BeamSubband * BeamGroup::getSubband( int sb )
  {
    return &beamSubbands_p[ sb ];
  }


  // ---------------------------------------------------------- setBeamGroup


  /*!
   \brief Initialize the beam group values.

   Initialize the beam group with some real values.

   \param dataset The dalDataset the the group belongs to.
   \param name The name of the group.
   */
  bool BeamGroup::setBeamGroup (dalDataset &dataset, std::string const &name)
  {
    bool status (true);

    try
      {
        dataset_p = dataset;
        H5fileID_p = dataset.getFileHandle();
        delete group_p; // allocated during construction in init()
        group_p    = dataset.openGroup(name);
      }
    catch (std::string message)
      {
        std::cerr << "[BeamGroup::setBeamGroup] " << message << endl;
        status = false;
      }

    return status;
  }


  // ---------------------------------------------------------- ra


  /*!
    \brief Get the ra of the beam

    \return ra -- The ra of the beam pointing direction
  */
  std::string BeamGroup::ra ()
  {
    std::string attribute_ra ("");

    if (group_p->getName() != "UNDEFINED")
      {
        try
          {
            char * ra = reinterpret_cast<char*>(group_p->getAttribute("RA"));
            attribute_ra = string(ra);
            delete [] ra;
            ra = NULL;

          }
        catch (std::string message)
          {
            std::cerr << "-- Error extracting attribute RA" << endl;
            attribute_ra = "";
          }
      }
    return attribute_ra;
  }


  // ---------------------------------------------------------- dec


  /*!
    \brief Get the declination of the beam

    \return dec -- The declination of the beam pointing direction
  */
  std::string BeamGroup::dec ()
  {
    std::string attribute_dec ("");

    if (group_p->getName() != "UNDEFINED")
      {
        try
          {
            char * dec = reinterpret_cast<char*>(group_p->getAttribute("DEC"));
            attribute_dec = string(dec);
            delete [] dec;
            dec = NULL;

          }
        catch (std::string message)
          {
            std::cerr << "-- Error extracting attribute DEC" << endl;
            attribute_dec = "";
          }
      }
    return attribute_dec;
  }


  // ---------------------------------------------------------- n_subbands


  /*!
    \brief Get the number of sub-bands

    \return subbands -- The number of sub-bands.
  */
  int BeamGroup::n_subbands ()
  {
    int n_subbands = 0;
    if (group_p->getName() != "UNDEFINED")
      {
        try
          {
            int * n_subbands_p = reinterpret_cast<int*>(group_p->getAttribute("NUMBER_OF_SUBBANDS"));
            n_subbands = *n_subbands_p;
            delete [] n_subbands_p;
            n_subbands_p = NULL;

          }
        catch (std::string message)
          {
            std::cerr << "-- Error extracting attribute NUMBER_OF_SUBBANDS" << endl;
            n_subbands = -1;
          }
      }
    return n_subbands;
  }


  // ---------------------------------------------------------- getIntensity

  /*!
   \brief Get total instensity data for a given subband

   Get X and Y columns data for a given subband

   \param subband Subband to get the data from.
   \param start Start number of the cell in the column.
   \param length The number of cells to retrieve.
   \return array of intensities

  */
  float *
  BeamGroup::getIntensity( int subband,
                           int start,
                           int &length )
  {
    dalTable * table;
    dalColumn * col;
    dalData * data;

    vector<string> memnames = group_p->getMemberNames();

    table = dataset_p.openTable(memnames[ subband ],group_p->getName());
    if ( !table )
      {
        printf("ERROR: Subband %d does not exist for this beam\n", subband);
        return NULL;
      }

    col = table->getColumn_Float32("TOTAL_INTENSITY");
    data = col->data( start, length );
    if ( !data )
      {
        printf("ERROR: Column TOTAL_INTENSITY does not exist for this subband\n");
        return NULL;
      }

    float * values;
    values = (float*)data->data;

    return values;
  }

  // ---------------------------------------------------------- getSubbandData_XY

  /*!
   \brief Get X and Y columns data for a given subband

   Get X and Y columns data for a given subband

   \param subband Subband to get the data from.
   \param start Start number of the cell in the column.
   \param length The number of cells to retrieve.
   \param Output character vector of X values.
   \param Output character vector of Y values.

  */
  void BeamGroup::getSubbandData_XY( int subband,
                                     int start,
                                     int &length,
                                     std::vector< std::complex<short> > &x_values,
                                     std::vector< std::complex<short> > &y_values )
  {
    getSubbandData_X( subband, start, length, x_values );
    getSubbandData_Y( subband, start, length, y_values );
  }


  // ---------------------------------------------------------- getSubbandData_X


  /*!
   \brief Get X column data for a given subband

    Get X column data for a given subband

   \param subband Subband to get the data from.
   \param start Start number of the cell in the column.
   \param length The number of cells to retrieve.
   \param Output character vector of values.

  */

  std::complex<short> *
  BeamGroup::getSubbandData_X( int subband,
                               int start,
                               int &length )
  {
    dalTable * table = NULL;
    dalColumn * col = NULL;
    dalData * data = NULL;

    vector<string> memnames = group_p->getMemberNames();

    table = dataset_p.openTable( memnames[subband], group_p->getName() );
    if ( !table )
      {
        printf("ERROR: Subband %d does not exist for this beam\n", subband);
        return NULL;
      }

    col = table->getColumn_complexInt16("X");
    data = col->data( start, length );
    if ( !data )
      {
        printf("ERROR: Column X does not exist for this subband\n");
        return NULL;
      }

    std::complex<short> * values = NULL;
    values = (complex<short>*)data->data;

    delete data;
    delete col;
    delete table;

    return values;
  }


  // ---------------------------------------------------------- getSubbandData_Y

  /*!
   \brief Get Y column data for a given subband

    Get Y column data for a given subband

   \param subband Subband to get the data from.
   \param start Start number of the cell in the column.
   \param length The number of cells to retrieve.
   \return array of values

  */
  std::complex<short> *
  BeamGroup::getSubbandData_Y( int subband,
                               int start,
                               int &length )
  {
    dalTable * table = NULL;
    dalColumn * col = NULL;
    dalData * data = NULL;

    vector<string> memnames = group_p->getMemberNames();

    table = dataset_p.openTable(memnames[ subband ],group_p->getName());
    if ( !table )
      {
        printf("ERROR: Subband %d does not exist for this beam\n", subband);
        return NULL;
      }

    col = table->getColumn_complexInt16("Y");
    data = col->data( start, length );
    if ( !data )
      {
        printf("ERROR: Column Y does not exist for this subband\n");
        return NULL;
      }


    std::complex<short> * values = NULL;
    values = (complex<short>*)data->data;

    delete data;
    delete col;
    delete table;

    return values;
  }


  // ---------------------------------------------------------- getSubbandData_X

  /*!
   \brief Get X column data for a given subband

    Get X column data for a given subband

   \param subband Subband to get the data from.
   \param start Start number of the cell in the column.
   \param length The number of cells to retrieve.
   \return array of values

  */
  void BeamGroup::getSubbandData_X( int subband,
                                    int start,
                                    int &length,
                                    std::vector< std::complex<short> > &values )
  {
    dalTable * table = NULL;
    dalColumn * col = NULL;
    dalData * data = NULL;

    complex<short> * xx = NULL;
    vector<string> memnames = group_p->getMemberNames();

    table = dataset_p.openTable(memnames[ subband ],group_p->getName());
    if ( !table )
      {
        printf("ERROR: Subband %d does not exist for this beam\n", subband);
      }

    col = table->getColumn_complexInt16("X");
    data = col->data( start, length );
    if ( !data )
      {
        printf("ERROR: Column X does not exist for this subband\n");
      }

    for (int jj=0; jj < length; jj++)
      {
        xx = (complex<short>*)data->get(jj);
        values.push_back(*xx);
      }

    delete data;
    delete col;
    delete table;
  }



  // ---------------------------------------------------------- getSubbandData_Y

  /*!
   \brief Get Y column data for a given subband

   Get Y column data for a given subband

   \param subband Subband to get the data from.
   \param start Start number of the cell in the column.
   \param length The number of cells to retrieve.
   \param Output character vector of values.

  */
  void BeamGroup::getSubbandData_Y( int subband,
                                    int start,
                                    int &length,
                                    std::vector< std::complex<short> > &values )
  {
    complex<short> * yy = NULL;
    dalTable * table = NULL;
    dalColumn * col = NULL;
    dalData * data = NULL;

    vector<string> memnames = group_p->getMemberNames();

    table = dataset_p.openTable(memnames[ subband ],group_p->getName());
    if ( !table )
      {
        printf("ERROR: Subband %d does not exist for this beam\n", subband);
      }

    col = table->getColumn_complexInt16("Y");
    data = col->data( start, length );
    if ( !data )
      {
        printf("ERROR: Column Y does not exist for this subband\n");
      }

    for (int jj=0; jj < length; jj++)
      {
        yy = (complex<short>*)data->get(jj);
        values.push_back(*yy);
      }

    delete data;
    delete col;
    delete table;
  }

#ifdef PYTHON
  /************************************************************************
   *
   * The following functions are boost wrappers to allow some previously
   *   defined functions to be easily called from a python prompt.
   *
   ************************************************************************/


  // ---------------------------------------------------------- summary_boost

  void BeamGroup::summary_boost()
  {
    summary();
  }


  // ---------------------------------------------------------- getIntensity_boost

  bpl::numeric::array BeamGroup::getIntensity_boost( int subband,
      int start,
      int length )
  {
    float * values = NULL;
    values = getIntensity( subband, start, length );
    std::vector<int> mydims;
    mydims.push_back( length );
    bpl::numeric::array narray = num_util::makeNum( values, mydims );
    delete [] values;
    values = NULL;
    return narray;
  }


  // ---------------------------------------------------------- getSubbandData_X_boost

  bpl::numeric::array BeamGroup::getSubbandData_X_boost( int subband,
      int start,
      int length )
  {
    std::complex<short> * values = NULL;

    values = getSubbandData_X( subband, start, length );
    if ( !values )
      {
        bpl::list foo;
        foo.append( 0 );
        bpl::numeric::array narray = num_util::makeNum( foo );
        return narray;
      }

    std::complex<float> * value_list;
    value_list = new std::complex<float>[length];

    for (int ii=0; ii<length; ii++)
      {
        value_list[ii] = complex<float>(values[ii].real(),values[ii].imag());
      }
    delete [] values;
    std::vector<int> mydims;
    mydims.push_back( length );
    bpl::numeric::array narray = num_util::makeNum( value_list, mydims );
    delete [] value_list;
    value_list = NULL;
    return narray;
  }


  // ---------------------------------------------------------- getSubbandData_Y_boost

  bpl::numeric::array BeamGroup::getSubbandData_Y_boost( int subband,
      int start,
      int length )
  {
    std::complex<short> * values = NULL;
    values = getSubbandData_Y( subband, start, length );
    std::complex<float> * value_list;
    value_list = new std::complex<float>[length];

    for (int ii=0; ii<length; ii++)
      {
        value_list[ii] = complex<float>(values[ii].real(),values[ii].imag());
      }
    delete [] values;
    std::vector<int> mydims;
    mydims.push_back( length );
    bpl::numeric::array narray = num_util::makeNum( value_list, mydims );
    delete [] value_list;
    value_list = NULL;
    return narray;
  }


  // ---------------------------------------------------------- getSubbandData_XY_boost

  bpl::numeric::array BeamGroup::getSubbandData_XY_boost( int subband,
      int start,
      int length )
  {
    std::vector< std::complex<short> > x_values;
    std::vector< std::complex<short> >::iterator xvalit;
    std::vector< std::complex<short> > y_values;
    std::vector< std::complex<short> >::iterator yvalit;
    x_values.clear();
    y_values.clear();
    getSubbandData_X( subband, start, length, x_values );
    getSubbandData_Y( subband, start, length, y_values );

    bpl::list x_value_list;
    bpl::list y_value_list;
    bpl::list xy_value_list;

    for (xvalit=x_values.begin(); xvalit < x_values.end(); xvalit++)
      {
        std::complex<float> foo((*xvalit).real(),(*xvalit).imag());
        x_value_list.append( foo );
      }
    for (yvalit=y_values.begin(); yvalit < y_values.end(); yvalit++)
      {
        std::complex<float> foo((*yvalit).real(),(*yvalit).imag());
        y_value_list.append( foo );
      }

    xy_value_list.append(x_value_list);
    xy_value_list.append(y_value_list);

    bpl::numeric::array narray = num_util::makeNum( xy_value_list );

    return narray;
  }
#endif // end #ifdef PYTHON

} // end namespace DAL
