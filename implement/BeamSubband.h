/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Joseph Masters <jmasters@science.uva.nl>                              *
 *   Lars B"ahren <lbaehren@gmail.com>                                     *
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
#define BEAMSUBBAND_H

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#ifndef DALTABLE_H
#include <dalTable.h>
#endif

namespace DAL
  {

  /*!
    \class BeamSubband

    \ingroup DAL

    \brief High-level interface between beam-formed data and the DAL

    \author Joseph Masters, Lars B&auml;hren

    \test tBeamSubband.cpp

    <h3>Prerequisite</h3>

    <ul>
      <li>dalDataset -- Representation the file containing all sub-structures
      (tables, arrays, etc.).
      <li>dalTable -- Object representing a table within a dataset.
    </ul>
  */

  class BeamSubband
    {

    private:

      //! Group object of the Data Access Library
      dalTable * table_p;
      //! HDF5 table handle ID
      hid_t H5tableID_p;

    public:

      // ==========================================================================
      //  Construction

      //! Default constructor
      BeamSubband();
      //! Argumented constructor
      BeamSubband ( dalTable * table );
      //! Argumented constructor
      BeamSubband (dalDataset &dataset,
                   std::string const &group,
                   std::string const &table);
      //! Destructor
      ~BeamSubband();

      // ==========================================================================
      //  Parameters

      /*!
        \brief Get the HDF5 table handle ID

        \return tableID -- The HDF5 table handle ID
      */
      inline hid_t tableID () const
        {
          return H5tableID_p;
        }

      /*!
        \brief Provide a summary of the objects internal parameters

        The summary will be written to standard output
      */
      inline void summary()
      {
        summary(std::cout);
      }

      /*!
        \brief Provide a summary of the objects internal parameters

        \param os -- Output stream to which the summary is written.
      */
      void summary(std::ostream &os);

      // ==========================================================================
      //  Methods

      //! Get the number of fields within the table
      hsize_t nofFields ();
      //! Get the number of records within the table
      hsize_t nofRecords ();
      //! Get the number of table rows
      long nofTableRows ();
      //! Get a list with the names of the table columns
      std::vector<std::string> columnNames ();
      //! Get the center frequency of the sub-band
      int center_frequency () const;

    }; // class BeamSubband -- end

} // namespace DAL -- end

#endif
