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
  
  BeamGroup::BeamGroup()
  {
    init();
  }
  
  // ------------------------------------------------------------------ BeamGroup
  
  /*!
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
  
  void BeamGroup::init()
  {
    H5fileID_p        = -1;
    H5groupID_p       = -1;
    group_p           = NULL;
    group_p           = new dalGroup;
    group_p->setName ("UNDEFINED");
  }
  
  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================
  
  // ----------------------------------------------------------------- ~BeamGroup
  
  BeamGroup::~BeamGroup()
  {
    if ( NULL != group_p ) {
      group_p->close();
      delete group_p;
      group_p = NULL;
    }

    for (uint n(0); n<subbands_p.size(); n++) {
      delete subbands_p[n];
    }
  }
  
  // ============================================================================
  //
  //  Parameters & Attributes
  //
  // ============================================================================
    
  // -------------------------------------------------------------------- summary
  
  /*!
    \param os -- Output stream to which the summary will be written.
  */
  void BeamGroup::summary(std::ostream &os)
  {
    os << "[BeamGroup] Summary of object properties"  << endl;
    
    /* We need to guard against trying to get a summary for an object
     * created via the default constructor; since in that case no connection
     * to a file and/or group is made, no attribute values can be retrieved.
     */
    if (fileID() < 0) {
      os << "-- No summary available." << endl;
    } else {
      os << "-- HDF5 file ID ............. : " << fileID()              << endl;
      os << "-- HDF5 group ID ............ : " << groupID()             << endl;
      os << "-- HDF5 group name .......... : " << groupName()           << endl;
      os << "-- RA ....................... : " << ra()                  << endl;
      os << "-- DEC ...................... : " << dec()                 << endl;
      os << "-- Number of sub-bands ...... : " << nofSubbands()         << endl;
      os << "-- Number of sub-band tables  : " << subbands_p.size() << endl;
      
      /* If the BeamGroup contains subband tables, we can list them - otherwise
       * there is nothing else to do here.
       */
      if (nofSubbands() > 0) {
	vector<string> memberNames   = group_p->getMemberNames();	
	std::vector<hid_t> ids       = tableIDs();
	std::vector<hsize_t> fields  = nofTableFields();
	std::vector<long> rows       = nofTableRows();
	std::vector<int> frequencies = center_frequencies();
	/* Name of the subband tables */
	os << "-- Subband table names ...... : [";
	for (unsigned int n(0); n<memberNames.size(); n++) {
	  os << " " << memberNames[n];
	}
	os << " ]" << std::endl;
	/* HDF5 table IDs */
	os << "-- Subband table HDF5 IDs     : [";
	for (unsigned int n(0); n<ids.size(); n++) {
	  os << " " << ids[n];
	}
	os << " ]" << std::endl;
	/* Fields within the tables */
	os << "-- nof. table fields          : [";
	for (unsigned int n(0); n<fields.size(); n++) {
	  os << " " << fields[n];
	}
	os << " ]" << std::endl;
	/* Number of table rows */
	os << "-- nof. table rows            : [";
	for (unsigned int n(0); n<rows.size(); n++) {
	  os << " " << rows[n];
	}
	os << " ]" << std::endl;
	/* Center frequencies of the subbands */
	os << "-- Subband center frequencies : [";
	for (unsigned int n(0); n<frequencies.size(); n++) {
	  os << " " << frequencies[n];
	}
	os << " ]" << std::endl;
      }
      
    }
  }
    
  // --------------------------------------------------------------- setBeamGroup

  /*!
   \param dataset -- The dalDataset the the group belongs to.
   \param name    -- The name of the group.

   \return status Status of the operation; returns <tt>false</tt> in case an
           error was encountered.
   */
  bool BeamGroup::setBeamGroup (dalDataset &dataset,
				std::string const &name)
  {
    bool status (true);

    /* [1] Assign identifiers */

    try {
      dataset_p = dataset;
      delete group_p; // allocated during construction in init()
      group_p     = dataset.openGroup(name);
      /* Extract the values of the identifiers */
      H5fileID_p  = dataset.getFileHandle();
      H5groupID_p = group_p->getId();
    } catch (std::string message) {
      std::cerr << "[BeamGroup::setBeamGroup] " << message << endl;
      status = false;
    }

    /* [2] Set up list of objects handling subband data */
    
    try {
      vector<string> tableNames = group_p->getMemberNames();
      unsigned int nofSubbands  = tableNames.size();
      
      if (nofSubbands>0) {
	subbands_p.clear();
	for (unsigned int n(0); n<nofSubbands; n++) {
	  std::string tableName = "/" + groupName() + "/" + tableNames[n];
	  /* Create new dalTable object for sub-band data */
	  dalTable * table = dataset_p.openTable (tableNames[n],groupName());
	  /* Create new sub-band object from table */
	  BeamSubband * band = new BeamSubband (table);
	  subbands_p.push_back(band);
	}
      }
    } catch (std::string message) {
      std::cerr << "[BeamGroup::setBeamGroup] " << message << endl;
      status = false;
    }
    
    return status;
  }
  
  // ----------------------------------------------------------------- getSubband

  /*!
   \param sb -- Number of the subband you want to retrieve.

   \return subband -- Array pointer to the set of subbands contained within
           the BeamGroup.
  */
  BeamSubband * BeamGroup::getSubband( int sb )
  {
    return subbands_p[ sb ];
  }
  
  // ----------------------------------------------------------------- getSubband

  /*!
   \retval subband -- The requested subband from the BeamGroup
   \param sb       --  Number of the subband you want to retrieve.
  */
  void BeamGroup::getSubband( BeamSubband &subband,
			      int sb )
  {
    subband = (*subbands_p[sb]);
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
  
  // ------------------------------------------------------------------------- ra

  /*!
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

  // ---------------------------------------------------------------- nofSubbands

  /*!
    \return subbands -- The number of sub-bands.
  */
  int BeamGroup::nofSubbands ()
  {
    int nofSubbands = -1;
    if (group_p->getName() != "UNDEFINED")
      {
        if ( DAL::FAIL == h5get_attribute( nofSubbands, "NUMBER_OF_SUBBANDS",
                                           group_p->getId() ) )
          {
            std::cerr << "-- Error extracting attribute NUMBER_OF_SUBBANDS" << endl;
          }
      }
    return nofSubbands;
  }

  // ============================================================================
  //
  //  Methods for access of metadata/attributes
  //
  // ============================================================================
  
  /*!
    \return frequencies -- The center frequencies of the subbands
  */
  std::vector<int> BeamGroup::center_frequencies ()
  {
    std::vector<int> frequencies (1,0);
    
    if (group_p->getName() != "UNDEFINED") {
      unsigned int nofSubbands = subbands_p.size();
      frequencies.resize(nofSubbands);
      /* Retrieve the center frequencies from the individual subbands */
      for (unsigned int n(0); n<nofSubbands; n++) {
	frequencies[n] = subbands_p[n]->center_frequency();
      }
    }
    
    return frequencies;
  }

  // ------------------------------------------------------------------- tableIDs

  /*!
    \return ids -- Vector with the HDF5 table IDs of the embedded Subband
            objects
  */
  std::vector<hid_t> BeamGroup::tableIDs ()
  {
    std::vector<hid_t> ids (1,0);
    
    if (group_p->getName() != "UNDEFINED") {
      unsigned int nofSubbands = subbands_p.size();
      ids.resize(nofSubbands);
      /* Retrieve the center frequencies from the individual subbands */
      for (unsigned int n(0); n<nofSubbands; n++) {
	ids[n] = subbands_p[n]->tableID();
      }
    }
    
    return ids;
  }

  // ------------------------------------------------------------- nofTableFields

  /*!
    \return nofFields -- The number of fields within the sub-band tables
  */
  std::vector<hsize_t> BeamGroup::nofTableFields ()
  {
    std::vector<hsize_t> nofFields (1,0);
    
    if (group_p->getName() != "UNDEFINED") {
      unsigned int nofSubbands = subbands_p.size();
      nofFields.resize(nofSubbands);
      /* Retrieve the center frequencies from the individual subbands */
      for (unsigned int n(0); n<nofSubbands; n++) {
	nofFields[n] = subbands_p[n]->nofFields();
      }
    }
    
    return nofFields;
  }

  // --------------------------------------------------------------- nofTableRows

  /*!
    \return rows -- The number of rows within the sub-band tables
  */
  std::vector<long> BeamGroup::nofTableRows ()
  {
    std::vector<long> rows (1,0);
    
    if (group_p->getName() != "UNDEFINED") {
      unsigned int nofSubbands = subbands_p.size();
      rows.resize(nofSubbands);
      /* Retrieve the center frequencies from the individual subbands */
      for (unsigned int n(0); n<nofSubbands; n++) {
	rows[n] = subbands_p[n]->nofTableRows();
      }
    }
    
    return rows;
  }

  // ============================================================================
  //
  //  Methods for access of subband data
  //
  // ============================================================================

  /*!
    \param subband -- The number of the subband from which to retrieve the data
  */
  std::complex<short> * BeamGroup::getSubbandData_X (int const &subband)
  {
    int start  = 0;
    int length = subbands_p[subband]->nofTableRows();
    
    return getSubbandData_X (subband,
			     start,
			     length);
    
  }
  
  /*!
    \param subband -- The number of the subband from which to retrieve the data
  */
  std::complex<short> * BeamGroup::getSubbandData_Y (int const &subband)
  {
    int start  = 0;
    int length = subbands_p[subband]->nofTableRows();

    return getSubbandData_X (subband,
			     start,
			     length);
  }
  
  /*!
    \retval data   -- The data from the selected sub-band
    \param subband -- The number of the subband from which to retrieve the data
  */
  void BeamGroup::getSubbandData_X (std::vector<std::complex<short> > &data,
				    int const &subband)
  {
    int length = subbands_p[subband]->nofTableRows();
    std::complex<short> *tmp = getSubbandData_X (subband);

    data.clear();
    data.assign (tmp,&tmp[length]);
  }
  
  /*!
    \retval data   -- The data from the selected sub-band
    \param subband -- The number of the subband from which to retrieve the data
  */
  void BeamGroup::getSubbandData_Y (std::vector<std::complex<short> > &data,
				    int const &subband)
  {
    int length = subbands_p[subband]->nofTableRows();
    std::complex<short> *tmp;

    tmp = getSubbandData_X (subband);

    data.clear();
    data.assign (tmp,&tmp[length]);
  }
  
  // --------------------------------------------------------------- getIntensity
  
  /*!
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
