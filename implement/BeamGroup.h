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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif

#ifndef DALGROUP_H
#include <dalGroup.h>
#endif

#ifndef BEAMSUBBAND_H
#include <BeamSubband.h>
#endif

namespace DAL {

/*!
   \class BeamGroup
    
   \ingroup DAL
    
   \brief High-level interface between beam-formed data and the DAL
    
   \author Joseph Masters

 */
 
  class BeamGroup {

  private:

    //! HDF5 file handle ID
    hid_t H5groupID_p;

    //! Dataset object of the Data Access Library 
    dalDataset dataset_p;

    //! Group object of the Data Access Library 
    dalGroup *group_p;

    //! HDF5 file handle ID
    hid_t H5fileID_p;

    //! Vector of subband tables within the dataset
    std::vector<BeamSubband> beamSubbands_p;

  public:

    /*!
      \brief Default constructor
	  
	  Default constructor
    */
    BeamGroup();

    /*!
      \brief Argumented constructor
	  
	  Argumented constructor
	  
	  \param dataset The dalDataset the the group belongs to.
	  \param name The name of the group.
    */
    BeamGroup ( dalDataset &dataset,
	            std::string const &name );

    /*!
      \brief Initialize the object's internal parameters
	  
	  Initialize the object's internal parameters
    */
    void init();

    /*!
	  \brief Initialize the beam group values.
	  
	  Initialize the beam group with some real values.
	  
	  \param dataset The dalDataset the the group belongs to.
	  \param name The name of the group.
     */	  
    bool setBeamGroup ( dalDataset &dataset,
	                    std::string const &name);


    /*!
	  \brief Get X and Y columns data for a given subband
	  
	  Get X and Y columns data for a given subband
	  
	  \param subband Subband to get the data from.
	  \param start Start number of the cell in the column.
	  \param length The number of cells to retrieve.
	  \param Output character vector of X values.
	  \param Output character vector of Y values.
	  
	 */
    void getSubbandData_XY( int subband,
	                        int start,
							int length,
							std::vector< std::complex<char> > &x_values,
							std::vector< std::complex<char> > &y_values );

    /*!
	  \brief Get X column data for a given subband
	  
	  Get X column data for a given subband
	  
	  \param subband Subband to get the data from.
	  \param start Start number of the cell in the column.
	  \param length The number of cells to retrieve.
	  \param Output character vector of values.
	  
	 */
    void getSubbandData_X( int subband,
	                       int start,
				           int length,
					       std::vector< std::complex<char> > &values );


    /*!
	  \brief Get Y column data for a given subband
	  
	  Get Y column data for a given subband
	  
	  \param subband Subband to get the data from.
	  \param start Start number of the cell in the column.
	  \param length The number of cells to retrieve.
	  \param Output character vector of values.
	  
	 */
    void getSubbandData_Y( int subband,
	                       int start,
				           int length,
					       std::vector< std::complex<char> > &values );


    /*!
      \brief Provide a summary of the object's interal parameters
	  
	  Provide a summary of the object's interal parameters
    */
    inline void summary()
    {
       summary(cout);
    }

    /*!
	  \brief Get a subband object from the beam.
	  
	  Get a subband object from the beam.
	  
	  \param subband Number of the subband you want to retrieve.
	 */
    BeamSubband * getSubband( int subband );

    /*!
      \brief Provide a summary of the object's interal parameters

      \param os -- Output stream to which the summary is written.
    */
    void summary(std::ostream &os);

    /*!
      \brief Get the ra of the beam

      \return ra -- The ra of the beam pointing direction
    */
    std::string ra();

    /*!
      \brief Get the declination of the beam

      \return dec -- The declination of the beam pointing direction
    */
    std::string dec();

    /*!
      \brief Get the number of sub-bands

      \return subbands -- The number of sub-bands.
    */
    int n_subbands ();

 }; // end BeamGroup class

} // end DAL namespace


#endif
