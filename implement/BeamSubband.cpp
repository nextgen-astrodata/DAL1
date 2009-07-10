/*-------------------------------------------------------------------------*
 | $Id:: BeamGroup.h 1126 2007-12-10 17:14:20Z masters                   $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008 by Joseph Masters                                  *
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

#ifndef BEAMSUBBAND_H
#include "BeamSubband.h"
#endif

using std::cerr;
using std::endl;

namespace DAL
  {

  // ============================================================================
  //
  //  Construction
  //
  // ============================================================================

  // ---------------------------------------------------------------- BeamSubband

  BeamSubband::BeamSubband ()
  {
    table_p     = NULL;
    H5tableID_p = -1;
  }

  // ---------------------------------------------------------------- BeamSubband

  /*!
    \param table -- Pointer to a dalTable object
  */
  BeamSubband::BeamSubband ( dalTable * table )
  {
    table_p     = table;
    H5tableID_p = table_p->tableID();
  }

  // ---------------------------------------------------------------- BeamSubband

  /*!
    \param dataset -- dalDataset within which the table is contained
    \param group   -- Name of the BeamGroup within which the sub-band in contained
    \param table   -- Name of the sub-band table
  */
  BeamSubband::BeamSubband (dalDataset &dataset,
                            std::string const &group,
                            std::string const &table)
  {
    table_p     = dataset.openTable (table,group);
    H5tableID_p = table_p->tableID();
  }

  // ============================================================================
  //
  //  Destruction
  //
  // ============================================================================

  BeamSubband::~BeamSubband()
  {
    if ( NULL != table_p )
      {
        delete table_p;
      }
  }

  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================

  // -------------------------------------------------------------------- summary

  /*!
    \param os -- Output stream to which the summary will be written.
  */
  void BeamSubband::summary (std::ostream &os)
  {
    os << "[BeamSubband] Summary of object properties." << endl;

    if (table_p != NULL)
      {
        os << "-- Table HDF5 object ID   = " << table_p->tableID()         << endl;
        os << "-- nof. table fields      = " << table_p->nofFields()       << endl;
        os << "-- nof. table records     = " << table_p->nofRecords()      << endl;
        os << "-- Number of table rows   = " << table_p->getNumberOfRows() << endl;
        /* List of table column names */
        std::vector<std::string> names = columnNames ();
//       os << "-- Names of table columns = [";
//       for (unsigned int n(0); n<names.size(); n++) {
// 	os << " " << names[n];
//       }
//       os << " ]" << endl;
      }
  }

  // ------------------------------------------------------------------ nofFields

  /*!
    \return nofFields -- The number of fields within the table
  */
  hsize_t BeamSubband::nofFields ()
  {
    hsize_t fields (0);

    if (table_p != NULL)
      {
        fields = table_p->nofFields();
      }

    return fields;
  }

  // ----------------------------------------------------------------- nofRecords

  /*!
    \return nofRecords -- The number of records within the table
  */
  hsize_t BeamSubband::nofRecords ()
  {
    hsize_t records (0);

    if (table_p != NULL)
      {
        records = table_p->nofRecords();
      }

    return records;
  }

  // ----------------------------------------------------------------- nofRecords

  /*!
    \return nofTableRows -- The number of rows within the table
  */
  long BeamSubband::nofTableRows ()
  {
    long rows (0);

    if (table_p != NULL)
      {
        rows = table_p->getNumberOfRows();
      }

    return rows;
  }

  // ---------------------------------------------------------------- columnNames

  std::vector<std::string> BeamSubband::columnNames ()
  {
    if (table_p != NULL)
      {
        return table_p->listColumns();
      }
    else
      {
        std::vector<std::string> names (1,"");
        return names;
      }
  }

  // ----------------------------------------------------------- center_frequency

  /*!
    \return frequency -- The frequency of the subband
  */
  int BeamSubband::center_frequency () const
    {
      int frequency (0);

      if (H5tableID_p > 0)
        {
          if ( DAL::FAIL == h5get_attribute( H5tableID_p,
                                             "CENTER_FREQUENCY",
                                             frequency) )
            {
              std::cerr << "-- Error extracting attribute CENTER_FREQUENCY" << endl;
            }
        }
      else
        {
          std::cerr << "[BeamSubband::center_frequency] Unable to retrieve attribute;"
                    << " HDF5 table ID not assigned" << endl;
        }

      return frequency;
    }


}

