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

#ifndef BEAMGROUP_H
#include <BeamGroup.h>
#endif

namespace DAL {
  
  /*!
    \class BeamFormed
    
    \ingroup DAL
    
    \brief High-level interface between beam-formed data and the DAL
    
    \author Joseph Masters
    
    \test tBeamFormed.cpp
    
    While this class implements the top-level structure of a HDF5 file storing
    beam-formed data, the lower-level structures are accessible through the
    classes BeamGroup and BeamSubband.
    
    A LOFAR Beam-Formed file is stored per observation as an HDF5 file, grouped by
    beam and then subband.  All attribute data, described below, is defined in the
    <i>LOFAR Beam-Formed Data Format ICD</i>.
    
    \verbatim
    beam-formed-file.h5         <-------  Root level of dataset
    |--beam000                  <-------  First beam group
    |  |--SB000                 \
    |  |--SB001                  )------  Table for each subband
    |  `--SB00N                 /
    |--beam001                  <-------  Second beam group
    |  |--SB000                 \
    |  |--SB001                  )------  Table for each subband
    |  `--SB00N                 /
    `--beam00N                  <-------  Nth beam group
       |--SB000                 \
       |--SB001                  )------  Table for each subband
       `--SB00N                 /
    \endverbatim

    <ul>
      <li><b>Root-level</b> The root level of the file contains the majority of
      associated meta-data, describing the circumstances of the observation. These
      data attributes include time, frequency, weather and other important
      characteristics of the dataset.

      <li><b>Beam Group</b> Each observation beam is stored as a seperate group
      within the file, each containing its own subband tables and pointing
      information.

      <li><b>Subband Data tables</b> The subband tables are where the bulk of the
      data reside. Each beam has a fixed number of tables; one per subband. The subband
      frequency information is stored as an attribute of the table.

      <li><b>Full Resolution</b> The default data stored in each subband table is full
      resolution, polarized voltages from the dipoles. These voltages are stored as
      16-bit complex pairs for both X and Y.  In other words, each sample is stored
      as (X-real, X-imaginary)(Y-real, Y-imaginary) for a total of 64-bits.

      <li><b>Total Intensity</b> Although complete, full resolution data is very
      large due to a very high sampling rate. In some cases, the observer may not
      be especially
      interested in polarization data and instead choose to receive only the total
      intensities of the samples.  Because the total intensity information can be
      stored as a single 32-bit floating point value, the size of the file is
      effectively halved.  In this case, each subband table consists of one total
      intensity value per sample instead of two complex pairs as described above.

      <li><b>Downsampled</b> Furthermore, the observer may prefer a smaller file
      with data averaged along the time axis, or ``downsampled''. A relatively
      modest downsample factor (i.e. 128) will drastically reduce the size of the
      data file and, in many cases, still provide interesting, high-resolution
      data for analysis.

      <li><b>Channelized</b> Another option for the user is to bin the data into
      frequency channels, effectively downsampling the data along the time-axis
      by a factor of the number of frequency bins. <br>
      In this case, the subband table would have total intensities for each channel,
      per sample.
    </ul>

    <h3>Examples</h3>
    
    <ol>
      <li>Create an object of type BeamFormed within your class/application:
      \code
      #include <dal/BeamFormed.h>
      
      DAL::BeamFormed bf (filename);
      \endcode

      <li>Retrieve attributes in the root group of the file:
      \code
      std::string filename      = bf.filename();
      std::string telescope     = bf.telescope();
      std::string observationID = bf.observation_id();
      \endcode
    </ol>
  */
  
  class BeamFormed {
    
  private:
    
    // Filename of the dataset
    std::string filename_p;
    
    // HDF5 file handle ID
    hid_t H5fileID_p;
    
    // DAL Dataset object to handle the basic I/O
    dalDataset * dataset_p;
    
    bool init();
    
    // Vector of beam groups within the dataset
    std::vector<BeamGroup*> beamGroups_p;
    
    bool status;
    
  public:
    
      /*!
	\brief Default constructor
       */
      BeamFormed();
      /*!
	\brief Argumented constructor
       */
      BeamFormed(std::string const &filename);
      ~BeamFormed();


      // ---------------------------------------------------------- print_vector

      /*!
        \brief Print a list of vector elements
        \param os        -- output stream [I]
        \param vec       -- vector [I]
      */
      template<class T>
      void print_vector ( std::ostream& os,
                          std::vector<T> &vec)
      {
        for (uint n(0); n<vec.size(); n++)
          {
            os << vec[n] << ", ";
          }
      }

      std::vector< std::string > h5get_str_array_attr( std::string attrname,
          hid_t obj_id );
      /*! \brief Provide a summary of the internal status */
      inline void summary ()
      {
        summary (cout, true);
      }
      void summary (ostream &os, bool const &listBeams);
      std::vector<std::string> beams();
      BeamGroup * getBeam( int beam );
      std::string filename();
      std::string telescope();
      int nstations();
      std::string datatype();
      std::string emband();
      std::vector<std::string> sources();
      std::string observation_id();
      std::string proj_id();
      std::string point_ra();
      std::string point_dec();
      std::string observer();
      std::string epoch_mjd();
      std::string epoch_date();
      std::string epoch_utc();
      std::string epoch_lst();
      int main_beam_diam();
      int center_freq();
      int bandwidth();
      double total_integration_time();
      int breaks();
      int dispersion_measure();
      int number_of_samples();
      double sampling_time();
      int number_of_beams();
      int sub_beam_diameter();
      int weather_temperature();
      int weather_humidity();
      std::vector<int> station_temperatures();
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

      // ---------------------------------------------------------- vector2list

      /* utility to turn a vector into a python list */

      template <class T>
      bpl::list vector2list( std::vector<T> vec )
      {
        bpl::list mylist;

        typename std::vector<T>::iterator it;
        for ( it=vec.begin(); it < vec.end(); it++ )
          mylist.append( *it );

        return mylist;
      }

#endif // PYTHON

    }; // BeamFormed class

} // namespace DAL

#endif // BEAMFORMED_H
