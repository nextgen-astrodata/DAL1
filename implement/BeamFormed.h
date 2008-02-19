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

#ifndef BEAMFORMED_H
#define BEAMFORMED_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef DAL_H
#include <dal.h>
#endif

#ifndef DALDATASET_H
#include <dalDataset.h>
#endif


#ifndef BEAMGROUP_H
#include <BeamGroup.h>
#endif

namespace DAL {

/*!
   \class BeamFormed
    
   \ingroup DAL
    
   \brief High-level interface between beam-formed data and the DAL
    
   \author Joseph Masters

 */
 
  class BeamFormed {

  private:

    //! Filename of the dataset
    std::string filename_p;

    //! HDF5 file handle ID
    hid_t H5fileID_p;

    //! DAL Dataset object to handle the basic I/O
    dalDataset *dataset_p;
 
    bool init();

    //! Vector of beam groups within the dataset
    std::vector<BeamGroup> beamGroups_p;

  public:


    /*!
      \brief Empty constructor

    */
    BeamFormed();

    /*!
      \brief Argumented constructor

      \param filename -- Name of the file from which to read in the data
    */
    BeamFormed(std::string const &filename);

    /*!
      \brief Destructor
    */
    ~BeamFormed();
	
    /*!
      \brief Provide a summary of the internal status
    */
    inline void summary () {
      summary (cout, true);
    }

    /*!
      \brief Provide a summary of the internal status
    */
    void summary (ostream &os, bool const &listBeams);

    /*!
      \brief Print beam groups embedded within the dataset

    */
    std::vector<std::string> beams();

    /*!
      \brief Get the name of the data file

      \return filename -- The name of the data file
    */
    std::string filename();

    /*!
      \brief Get the name of the telescope

      \return telescope -- The name of the telescope with which the data were
              recorded; returns an empty string in case no keyword value could
	      be extracted.
    */
    std::string telescope();

    int nstations();

    std::string datatype();

    std::string emband();

    std::string observation_id();

    std::string proj_id();

    std::string point_ra();

    std::string point_dec();
    /*!
      \brief Get the name of the observer

      \return observer -- The name of the observer; returns an empty string in
              case no keyword value could be extracted.
    */
    std::string observer();

    double epoch_mjd();

    std::string epoch_date();

    std::string epoch_utc();

    std::string epoch_lst();

    int main_beam_diam();
    int center_freq();
    int bandwidth();
    double integration_time();
    int breaks();
    int dispersion_measure();
    int number_of_samples();
    double sampling_time();
    int number_of_beams();
    int sub_beam_diameter();
    int weather_temperature();
    int weather_humidity();
    int station_temperatures();

    std::string notes();

  };

} // end namespace DAL

#endif
