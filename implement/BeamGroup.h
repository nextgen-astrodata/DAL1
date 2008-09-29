/*-------------------------------------------------------------------------*
 | $Id:: dal.h 1126 2007-12-10 17:14:20Z masters                         $ |
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
    there are also a few methods available, which wrap the access to the embedded 
    BeamSubband objects:
    - center_frequencies()
    - tableIDs()
    - nofTableFields()
    - nofTableRows()
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
    inline hid_t fileID () const { return H5fileID_p; }
    /*!
      \brief Get the HDF5 group handle ID
      \return H5groupID -- The HDF5 group handle ID for this dataset
    */
    inline hid_t groupID () const { return H5groupID_p; }
    /*!
      \brief Get the name of the underlying HDF5 group
      \return name -- The name of the underlying HDF5 group.
    */
    inline std::string groupName () const { return group_p->getName(); }

    // ------------------------------------------------------- Methods

    //! Initialize the beam group values.
    bool setBeamGroup ( dalDataset &dataset,
			std::string const &name);
    dalTable * getSubbandTable (int subband);
    float *  getIntensity( int subband, int start, int &length );
    std::complex<short>  * getSubbandData_X( int subband,
					     int start,
					     int &length );
    
    std::complex<short>  * getSubbandData_Y( int subband,
					     int start,
					     int &length );
    void getSubbandData_X( int subband,
			   int start,
			   int &length,
			   std::vector< std::complex<short> > &values );
    void getSubbandData_Y( int subband,
			   int start,
			   int &length,
			   std::vector< std::complex<short> > &values );
    void getSubbandData_XY( int subband,
			    int start,
			    int &length,
			    std::vector< std::complex<short> > &x_values,
			    std::vector< std::complex<short> > &y_values );
    //! Get a subband from the beam
    BeamSubband * getSubband( int subband );
    //! Get a subband from the beam
    void getSubband( BeamSubband &subband,
		     int sb );
    //! Provide a summary of the object's properties
    inline void summary() { summary(std::cout); }
    //! Provide a summary of the object's properties
    void summary(std::ostream &os);
    //! Get the RA of the beam direction
    std::string ra();
    //! Get the declination of the beam direction
    std::string dec();
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
