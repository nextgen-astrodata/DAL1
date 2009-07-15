/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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
#define BEAMGROUP_H

#ifndef BEAMSUBBAND_H
#include <BeamSubband.h>
#endif

namespace DAL
  {

  /*!
    \class BeamGroup

    \ingroup DAL

    \brief High-level interface between beam-formed data and the DAL

    \author Joseph Masters

    \test tBeamGroup.cpp

    <h3>Synopsis</h3>

    Besides the the possibility to retrieve attributes attached to the HDF5 group
    there are also a few methods available,
    <ul>
      <li>center_frequencies()
      <li>tableIDs()
      <li>nofTableFields()
      <li>nofTableRows()
    </ul>
    which wrap the access to the embedded BeamSubband objects; by this an
    application programmer will not be required to individually retrieve the
    needed information from the embedded sub-band tables.

    <h3>Usage</h3>

    <ol>
      <li>Construction of a new BeamGroup object can be done via the default
      constructor (which will not be connected to any dataset)
      \code
      DAL::BeamGroup group;
      \endcode
      or from an existing dalDataset
      \code
      // Name of the dataset to open
      std::string nameFile ("mydata.h5");
      // Name of the beam group within the dataset
      std::string nameBeam ("beam000");
      // Open dataset into a dalDataset
      DAL::dalDataset dataset;
      dataset.open(nameFile.c_str());
      // Extract beam group from the dataset
      DAL::BeamGroup beam (dataset,nameBeam);
      \endcode
      Another method to get the same result is by extracting the object for a
      beam-group from a previously created BeamFormed object:
      \code
      std::string filename ("<the name of the dataset you want to access>");
      int numBeam (<number of the beam>);

      // Open beamformed dataset
      DAL::BeamFormed bf (filename);
      // Extract contents for the selected beam
      DAL::BeamGroup * group = bf.getBeam (numBeam);
      \endcode
      <li>Get the center frequencies of the sub-bands contained within this beam:
      \code
      std::vector<int> freq = beam.center_frequencies();
      \endcode
      <li>Retrieve values from a selected sub-band:
      \code
      int subband (0);
      int start (0)
      int length (20);
      // Vector into which the requested data are returned
      std::vector<std::complex<short> > data (length);
      // Extract data values from the table
      beam.getSubbandData_X (subband,start,length,data);
      \endcode
      This will retrieve the first 20 X-component data-points [0 .. 19] from the
      first sub-band. Stepping through the data volume along the time axis then
      is simply done by incrementing the value of the starting point:
      \code
      dataX = beam.getSubbandData_X (0,20,length);
      \endcode
      which will return values [20 .. 39] from the same table column accessed above.
      </ol>
  */

  class BeamGroup
    {

    private:

      //! HDF5 file handle ID
      hid_t H5fileID_p;
      //! HDF5 file handle ID
      hid_t H5groupID_p;
      //! Group object of the Data Access Library
      dalGroup *group_p;
      //! Dataset object of the Data Access Library
      dalDataset dataset_p;
      //! Vector of subband tables within the dataset
      std::vector<BeamSubband*> subbands_p;

    public:

      // -------------------------------------- Construction/Destruction

      //! Default constructor
      BeamGroup();
      //! Argumented constructor
      BeamGroup ( dalDataset &dataset,
                  std::string const &name );
      //! Destructor
      ~BeamGroup();

      // -------------------------------------------- Parameter handling

      //! Initialize the object's internal parameters
      void init();
      /*!
        \brief Get the HDF5 file handle ID
        \return H5fileID -- The HDF5 file handle ID for this dataset
      */
      inline hid_t fileID () const
        {
          return H5fileID_p;
        }
      /*!
        \brief Get the HDF5 group handle ID
        \return H5groupID -- The HDF5 group handle ID for this dataset
      */
      inline hid_t groupID () const
        {
          return H5groupID_p;
        }
      /*!
        \brief Get the name of the underlying HDF5 group
        \return name -- The name of the underlying HDF5 group.
      */
      inline std::string groupName () const
        {
          return group_p->getName();
        }

      // ------------------------------------------------------- Methods

      //! Initialize the beam group values.
      bool setBeamGroup ( dalDataset &dataset,
                          std::string const &name);
      //! Get the sub-band table
      dalTable * getSubbandTable (int subband);
      //! Get the X column data for a given subband.
      void getSubbandData_X (int &subband,
                             std::vector< std::complex<short> > &values);
      //! Get the Y column data for a given subband.
      void getSubbandData_Y (int &subband,
                             std::vector< std::complex<short> > &values);
      /*     //! Get X column data for a given subband. */
      /*     std::complex<short>  * getSubbandData_X (int &subband, */
      /* 					     int &start, */
      /* 					     int &length); */
      /*     //! Get Y column data for a given subband. */
      /*     std::complex<short>  * getSubbandData_Y (int &subband, */
      /* 					     int &start, */
      /* 					     int &length); */
      //! Get X column data for a given subband.
      void getSubbandData_X( int &subband,
                             int &start,
                             int &length,
                             std::vector< std::complex<short> > &values );
      //! Get Y column data for a given subband.
      void getSubbandData_Y( int &subband,
                             int &start,
                             int &length,
                             std::vector< std::complex<short> > &values );
      //! Get X and Y columns data for a given subband
      void getSubbandData_XY( int &subband,
                              int &start,
                              int &length,
                              std::vector< std::complex<short> > &values_x,
                              std::vector< std::complex<short> > &values_y );
      //! Get total instensity data for a given subband
      float *  getIntensity( int &subband,
                             int &start,
                             int &length );
      //! Get a subband from the beam
      BeamSubband * getSubband( int subband );
      //! Get a subband from the beam
      void getSubband( BeamSubband &subband,
                       int sb );
      //! Provide a summary of the object's properties
      inline void summary()
      {
        summary(std::cout);
      }
      //! Provide a summary of the object's properties
      void summary(std::ostream &os);
      //! Get the RA of the beam direction
      float ra();
      //! Get the declination of the beam direction
      float dec();
      //! Get the beam direction
      std::vector<float> beam_direction ();
      //! Get the number of sub-bands for this beam
      int nofSubbands ();

      //! Get the center frequencies of the sub-bands
      std::vector<int> center_frequencies ();
      //! Get the table IDs of the sub-bands
      std::vector<hid_t> tableIDs ();
      //! Get the nof. fields within the sub-band tables
      std::vector<hsize_t> nofTableFields ();
      //! Get the number of table rows
      std::vector<long> nofTableRows ();

      /************************************************************************
       *
       * The following functions are boost wrappers to allow some previously
       *   defined functions to be easily called from a python prompt.
       *
       ************************************************************************/
#ifdef PYTHON

      void summary_boost();
      bpl::numeric::array getIntensity_boost( int subband,
                                              int start,
                                              int length );
      bpl::numeric::array getSubbandData_X_boost( int subband,
          int start,
          int length );

      bpl::numeric::array getSubbandData_Y_boost( int subband,
          int start,
          int length );

      bpl::numeric::array getSubbandData_XY_boost( int subband,
          int start,
          int length );
#endif // end #ifdef PYTHON

    }; // end BeamGroup class


} // end DAL namespace

#endif
