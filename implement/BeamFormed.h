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
    dalDataset * dataset_p;

    bool init();

    //! Vector of beam groups within the dataset
    std::vector<BeamGroup*> beamGroups_p;

    bool status;

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
      \brief Print a list of vector elements
    */
    template<class T>
    void print_vector ( std::ostream& os,
                         std::vector<T> &vec);

    /*!
      \brief Get a list of values for a string array attribute
    */
   std::vector< std::string > h5get_str_array_attr( std::string attrname,
                                                    hid_t obj_id );
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
      \brief Get a beam group object
    */
    BeamGroup * getBeam( int beam );

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

    /*!
      \brief Get the number of stations
    */
    int nstations();

    /*!
      \brief Get the datatype
    */
    std::string datatype();

    /*!
      \brief Get the emband
    */
    std::string emband();

    /*!
      \brief Get the source list
    */
    std::vector<std::string> sources();

    /*!
      \brief Get the observation identifier
    */
    std::string observation_id();

    /*!
      \brief Get the project identifier
    */
    std::string proj_id();

    /*!
      \brief Get the pointing (ra)
    */
    std::string point_ra();

    /*!
      \brief Get the pointing (dec)
    */
    std::string point_dec();

    /*!
      \brief Get the name of the observer

      \return observer -- The name of the observer; returns an empty string in
              case no keyword value could be extracted.
    */
    std::string observer();

    /*!
      \brief Get the epoch mjd
    */
    std::string epoch_mjd();

    /*!
      \brief Get the epoch date
    */
    std::string epoch_date();

    /*!
      \brief Get the epoch utc
    */
    std::string epoch_utc();

    /*!
      \brief Get the epoch lst
    */
    std::string epoch_lst();

    /*!
      \brief Get the main beam diameter
    */
    int main_beam_diam();

    /*!
      \brief Get the center frequency
    */
    int center_freq();

    /*!
      \brief Get the bandwidth
    */
    int bandwidth();

    /*!
      \brief Get the integration time
    */
    double integration_time();

    /*!
      \brief Get the number of breaks
    */
    int breaks();

    /*!
      \brief Get the dispersion measure
    */
    int dispersion_measure();

    /*!
      \brief Get the number of samples
    */
    int number_of_samples();

    /*!
      \brief Get the sampling time
    */
    double sampling_time();

    /*!
      \brief Get the number of beams
    */
    int number_of_beams();

    /*!
      \brief Get the sub beam diameter
    */
    int sub_beam_diameter();

    /*!
      \brief Get the weather temperature
    */
    int weather_temperature();

    /*!
      \brief Get the weather humidity
    */
    int weather_humidity();

    /*!
      \brief Get the station temperatures
    */
    std::vector<int> station_temperatures();

    /*!
      \brief Get the notes
    */
    std::string notes();

/************************************************************************
 *
 * The following functions are boost wrappers to allow some previously
 *   defined functions to be easily called from a python prompt.
 *
 ************************************************************************/
#ifdef PYTHON

    void summary_boost();
    bpl::list beams_boost();
    bpl::list source_boost();

    /* utility to turn a vector into a python list */
    template <class T>
    bpl::list vector2list( std::vector<T> vec );

#endif

  };

} // end namespace DAL

#endif
