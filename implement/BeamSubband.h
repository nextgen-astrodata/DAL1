/*-------------------------------------------------------------------------*
 | $Id:: s                         $ |
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
#define BEAMSUBBAND_H

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

#ifndef DALTABLE_H
#include <dalTable.h>
#endif


namespace DAL {

/*!
   \class BeamFormed
    
   \ingroup DAL
    
   \brief High-level interface between beam-formed data and the DAL
    
   \author Joseph Masters

 */
 
  class BeamSubband {

  private:

    //! HDF5 file handle ID
    hid_t H5groupID_p;

    //! Group object of the Data Access Library 
    dalTable *table_p;

    //! HDF5 file handle ID
    hid_t H5fileID_p;

    //! HDF5 table handle ID
    hid_t H5tableID_p;

  public:

    BeamSubband();

    void init();

    inline void summary()
    {
       summary(cout);
    }

    void summary(std::ostream &os);

 }; // end BeamSubband class

} // end DAL namespace

#endif
