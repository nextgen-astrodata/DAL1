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

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================
  
  // ------------------------------------------------------------------ BeamGroup
  
  /*!
    \brief Default constructor
  */
  BeamGroup::BeamGroup()
  {
    init();
  }
  
  // ------------------------------------------------------------------ BeamGroup
  
  /*!
    \brief Argumented constructor
    
    \param dataset The dalDataset the the group belongs to.
    \param name The name of the group.
  */
  BeamGroup::BeamGroup (dalDataset &dataset,
			std::string const &name)
  {
    bool status (true);
    
    init ();
    
    status = setBeamGroup (dataset, name);
  }
  
  
  // ----------------------------------------------------------------------- init
  
  
  /*!
    \brief Initialize the object's internal parameters
    
    Initialize the object's internal parameters
  */
  void BeamGroup::init()
  {
    H5fileID_p        = -1;
    H5groupID_p       = -1;
    group_p           = NULL;
    group_p           = new dalGroup;
    group_p->setName ("UNDEFINED");
    beamSubbands_p.clear();
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  // ----------------------------------------------------------------- ~BeamGroup
  
  /*!
    \brief Default destructor
  */
  BeamGroup::~BeamGroup()
  {
    delete group_p;
  }
  
  // ============================================================================
  //
  //  Parameters & Attributes
  //
  // ============================================================================
    
  // -------------------------------------------------------------------- summary
  
  /*!
    \brief Provide a summary of the object's interal parameters

    \param os -- Output stream to which the summary will be written.
  */
  void BeamGroup::summary(std::ostream &os)
  {
    os << "\n[BeamGroup] Summary of object properties"  << endl;
    
    /* We need to guard against trying to get a summary for an object
     * created via the default constructor; since in that case no connection
     * to a file and/or group is made, no attribute values can be retrieved.
     */
    if (fileID() < 0) {
      os << "-- No summary available." << endl;
    } else {
      os << "-- HDF5 file ID ............. : " << fileID()        << endl;
      os << "-- HDF5 group ID ............ : " << groupID()       << endl;
      os << "-- HDF5 group name .......... : " << groupName()     << endl;
      os << "-- RA ....................... : " << ra()            << endl;
      os << "-- DEC ...................... : " << dec()           << endl;
      os << "-- Number of Sub-bands ...... : " << n_subbands()    << endl;

      /* If the BeamGroup contains subband tables, we can list them - otherwise
       * there is nothing else to do here.
       */
      
      if (n_subbands() > 0) {
	/* Name of the subband tables */
	vector<string> memberNames = group_p->getMemberNames();	
	os << "-- Subband table names ...... : [";
	for (unsigned int n(0); n<memberNames.size(); n++) {
	  os << " " << memberNames[n];
	}
	os << " ]" << std::endl;
	/* Center frequencies of the subbands */
	std::vector<int> frequencies = center_frequencies();
	os << "-- Subband center frequencies : [";
	for (unsigned int n(0); n<frequencies.size(); n++) {
	  os << " " << frequencies[n];
	}
	os << " ]" << std::endl;
      }
      
    }
  }
    
    
  // ---------------------------------------------------------- getSubband


  /*!
   \brief Get a subband object from the beam.

   \param subband Number of the subband you want to retrieve.

   \return subbands Array pointer to the set of subbands contained within
           the BeamGroup.
  */
  BeamSubband * BeamGroup::getSubband( int sb )
  {
    return &beamSubbands_p[ sb ];
  }

  // ------------------------------------------------------------ getSubbandTable

  /*!
    \brief Get a dalTable object of a subband from the beam

    \param subband -- Number of the subband for which to retrieve the table
    
    \return table -- Array pointer to the table containing the data for the
            selected subband.
  */
  dalTable * BeamGroup::getSubbandTable (int subband)
  {
    dalTable * table;
    /* Get the list of Subband-tables contained in this group */
    vector<string> memberNames = group_p->getMemberNames();
    
    table = dataset_p.openTable(memberNames[ subband ],group_p->getName());

    return table;
  }
  
  // ---------------------------------------------------------- setBeamGroup


  /*!
   \brief Initialize the beam group values.

   \param dataset The dalDataset the the group belongs to.
   \param name The name of the group.

   \return status Status of the operation; returns <tt>false</tt> in case an
           error was encountered.
   */
  bool BeamGroup::setBeamGroup (dalDataset &dataset,
				std::string const &name)
  {
    bool status (true);

    try
      {
        dataset_p = dataset;
        delete group_p; // allocated during construction in init()
        group_p     = dataset.openGroup(name);
	/* Extract the values of the identifiers */
        H5fileID_p  = dataset.getFileHandle();
	H5groupID_p = group_p->getId();
      }
    catch (std::string message)
      {
        std::cerr << "[BeamGroup::setBeamGroup] " << message << endl;
        status = false;
      }

    return status;
  }

  // ------------------------------------------------------------------------- ra

  /*!
    \brief Get the ra of the beam

    \return ra -- The ra of the beam pointing direction
  */
  std::string BeamGroup::ra ()
  {
    std::string ra ("");

    if (group_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == h5get_attribute( ra, "RA", group_p->getId() ) )
          {
            std::cerr << "-- Error extracting attribute RA" << endl;
          }
      }
    return ra;
  }

  // ------------------------------------------------------------------------ dec

  /*!
    \brief Get the declination of the beam

    \return dec -- The declination of the beam pointing direction
  */
  std::string BeamGroup::dec ()
  {
    std::string dec ("");

    if (group_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == h5get_attribute( dec, "DEC", group_p->getId() ) )
          {
            std::cerr << "-- Error extracting attribute DEC" << endl;
          }
      }
    return dec;
  }


  // ---------------------------------------------------------- n_subbands


  /*!
    \brief Get the number of sub-bands

    \return subbands -- The number of sub-bands.
  */
  int BeamGroup::n_subbands ()
  {
    int n_subbands = -1;
    if (group_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == h5get_attribute( n_subbands, "NUMBER_OF_SUBBANDS",
                                           group_p->getId() ) )
          {
            std::cerr << "-- Error extracting attribute NUMBER_OF_SUBBANDS" << endl;
          }
      }
    return n_subbands;
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  /*!
    \brief Get the center frequencies of the subbands

    \return frequencies -- The center frequencies of the subbands
  */
  std::vector<int> BeamGroup::center_frequencies ()
  {
    std::vector<int> frequencies (1,0);
    
    if (group_p->getName() != "UNDEFINED") {
      unsigned int nofSubbands = beamSubbands_p.size();
      frequencies.resize(nofSubbands);
      /* Retrieve the center frequencies from the individual subbands */
      for (unsigned int n(0); n<nofSubbands; n++) {
	frequencies[n] = beamSubbands_p[n].center_frequency();
      }
    }
    
    return frequencies;
  }
  
  // --------------------------------------------------------------- getIntensity

  /*!
   \brief Get total instensity data for a given subband

   Get X and Y columns data for a given subband

   \param subband Subband to get the data from.
   \param start Start number of the cell in the column.
   \param length The number of cells to retrieve.
   \return intensities Array of intensities

  */
  float *
  BeamGroup::getIntensity( int subband,
                           int start,
                           int &length )
  {
    dalTable * table;
    dalColumn * col;
    dalData * data;

    vector<string> memberNames = group_p->getMemberNames();

    table = dataset_p.openTable(memberNames[ subband ],group_p->getName());
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
    dalColumn * col  = NULL;
    dalData * data   = NULL;

    vector<string> memberNames = group_p->getMemberNames();

    table = dataset_p.openTable( memberNames[subband], group_p->getName() );
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

    vector<string> memberNames = group_p->getMemberNames();

    table = dataset_p.openTable(memberNames[ subband ],group_p->getName());
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
    vector<string> memberNames = group_p->getMemberNames();

    table = dataset_p.openTable(memberNames[ subband ],group_p->getName());
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

    vector<string> memberNames = group_p->getMemberNames();

    table = dataset_p.openTable(memberNames[ subband ],group_p->getName());
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
