/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2010                                                    *
 *   Lars B"ahren <bahren@astron.nl>                                       *
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

#ifndef ITS_EXPERIMENTMETA_H
#define ITS_EXPERIMENTMETA_H

// Standard library header files
#include <iostream>
#include <string>
#include <vector>

namespace DAL { // Namespace DAL -- begin
  
  /*!
    \class ITS_ExperimentMeta
    
    \ingroup DAL
    \ingroup data_hl
    
    \brief Storage of meta information from an LOFAR ITS experiment
    
    \author Lars B&auml;hren

    \date 2010/05/15

    \test tITS_ExperimentMeta.cc
    
    <h3>Prerequisite</h3>
    
    <ul type="square">
      <li>[start filling in your text here]
    </ul>
    
    <h3>Synopsis</h3>
    
    The basic format of the <i>experiment.meta</i> file as produced at an LOFAR ITS
    observation is independent on the specific design of the experiment; however
    there are subtle differences in the set of keyword/value combinations found in
    the metafile:
    <ul>
      <li>In <b>capture mode</b> raw time series ADC data are recorded, therefore
      no processing plugins are activated.
      \verbatim
      [OBSERVATION]
      description=NDA-ITS observations of Jupiter
      experiment_type=capturing
      basename=jupiter_20060130_2
      starttime=Fri Jan  1 12:00:00 2010
      interval=0
      capturemode=SYNCSTOP
      capturesize=S16_512M
      antennas=0 1 2 3 4 ... 58 59 
      signextension=true
      skipcapture=false
      iterations=1
      current_status=scheduled
      current_iteration=0
      observation_id=1138600386
      observation_owner=ERTC
      submittime=2006-01-30T05:53:16.00Z
      # machineformat=ieee-le
      # precision=int16
      #EOF
      triggertime[1]=2006-01-30T06:03:01.04Z
      file=/var/opt/aviary/dat/jupiter_20060130_2/e01.i0001.dat
      file=/var/opt/aviary/dat/jupiter_20060130_2/e02.i0001.dat
      file=/var/opt/aviary/dat/jupiter_20060130_2/e03.i0001.dat
      file=/var/opt/aviary/dat/jupiter_20060130_2/e04.i0001.dat
      \endverbatim

      <li>In the case of a correlation experiment, two separate plugins are
      activated: the <tt>fftPlugin</tt> running on each of the data aquisition PCs
      and the <tt>correlatorPlugin</tt> running on the central machine (as
      concentrator process).
      \verbatim
      [OBSERVATION]
      description=Monitoring of system stability
      experiment_type=processing
      plugin1=fftPlugin -c 8192 -e 1550:3900 -d --window=hanning
      plugin2=correlatorPlugin -c 2351 -i 255
      basename=monitoring_20050217_512
      starttime=now
      interval=0
      capturemode=SYNCSTART
      capturesize=S16_2M
      antennas=0 1 2 3 4 ... 58 59 
      signextension=true
      skipcapture=false
      iterations=1
      current_status=scheduled
      current_iteration=0
      observation_id=1108671001
      observation_owner=ERTC
      submittime=2005-02-17T22:30:42.00Z
      # machineformat=ieee-le
      # precision=int16
      #EOF
      triggertime[1]=2005-02-17T23:40:08.61Z
      file=/var/opt/aviary/dat/monitoring_20050217_512/i0001.dat
      \endverbatim
    </ul>
      
    <h3>Example(s)</h3>
    
  */  
  class ITS_ExperimentMeta {

    //! Location of the "experiment.meta" file for an observation
    std::string metafile_p;
    
    //! Description of the experiment
    std::string description_p;
    //! Type of experiment (processing, capture)
    std::string experimentType_p;
    
  public:
    
    // === Construction =========================================================
    
    //! Default constructor
    ITS_ExperimentMeta ();
    
    //! Argumented constructor
    ITS_ExperimentMeta (std::string const &metafile);
    
    /*!
      \brief Copy constructor
      
      \param other -- Another ITS_ExperimentMeta object from which to create this new
             one.
    */
    ITS_ExperimentMeta (ITS_ExperimentMeta const &other);
    
    // === Destruction ==========================================================

    //! Destructor
    ~ITS_ExperimentMeta ();
    
    // === Operators ============================================================
    
    /*!
      \brief Overloading of the copy operator
      
      \param other -- Another ITS_ExperimentMeta object from which to make a copy.
    */
    ITS_ExperimentMeta& operator= (ITS_ExperimentMeta const &other); 
    
    // === Parameter access =====================================================

    //! Get the location of the "experiment.meta" file for an observation
    inline std::string metafile () const {
      return metafile_p;
    }
    
    /*!
      \brief Get the name of the class
      
      \return className -- The name of the class, ITS_ExperimentMeta.
    */
    inline std::string className () const {
      return "ITS_ExperimentMeta";
    }

    //! Provide a summary of the object's internal parameters and status
    inline void summary () {
      summary (std::cout);
    }

    //! Provide a summary of the object's internal parameters and status
    void summary (std::ostream &os);    

    // === Methods ==============================================================
    
    //! Read in in file with experiment metadata
    bool readMetafile (std::string const &metafile);
    
  private:

    //! Initialize internal parameters
    void init ();
    
    //! Unconditional copying
    void copy (ITS_ExperimentMeta const &other);
    
    //! Unconditional deletion 
    void destroy(void);
    
  }; // Class ITS_ExperimentMeta -- end
  
} // Namespace DAL -- end

#endif /* ITS_EXPERIMENTMETA_H */
  
