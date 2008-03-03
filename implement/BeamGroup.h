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
    */
    BeamGroup();

    /*!
      \brief Argumented constructor
    */
    BeamGroup (dalDataset &dataset, std::string const &name);

    /*!
      \brief Initialize the object's internal parameters
    */
    void init();

    bool setBeamGroup (dalDataset &dataset, std::string const &name);

    void getSubbandData_X( int subband, int start, int length );
    void getSubbandData_Y( int subband, int start, int length );

    /*!
      \brief Provide a summary of the object's interal parameters
    */
    inline void summary()
    {
       summary(cout);
    }

    BeamSubband * getSubband( int );

    /*!
      \brief Provide a summary of the object's interal parameters

      \param os -- Output stream to which the summary is written.
    */
    void summary(std::ostream &os);

    std::string ra();
    /*!
      \brief Get the declination (angle?) of the beam

      \return ra -- The declination (angle?) of the beam pointing direction
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
