/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
 *-------------------------------------------------------------------------*
 ***************************************************************************
 *   Copyright (C) 2008                                                    *
 *   Lars B"ahren (lbaehren@gmail.com)                                     *
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

#ifndef ENUMERATIONS_H
#define ENUMERATIONS_H

// Standard library header files
#include <iostream>
#include <string>

namespace DAL   // Namespace DAL -- begin
  {

  /*!
    \file Enumerations.h

    \ingroup DAL

    \brief List of attributes used within a LOFAR time-series dataset

    \author Lars B&auml;hren

    \date 2008/01/31

    \test tLOFAR_Attributes.cc

    <h3>Prerequisite</h3>

    <ul type="square">
      <li>CR::LOFAR_TBB
      <li>DAL::LOFAR_StationGroup
      <li>DAL::LOFAR_DipoleDataset
    </ul>

    <h3>Synopsis</h3>

    <h3>Example(s)</h3>

  */

  enum Attributes
  {
    // ---------------------------------------------------------------
    //  General attributes for telescope and observation
    // ---------------------------------------------------------------
    /*!
      Name of the telescope with which the observation has been performed.
    */
    TELESCOPE,
    /*!
      Name of the person either responsible for carrying out the observation
      or having requested the observation; combination of both possible.
    */
    OBSERVER,
    /*!
      Name of the project for which this observation has been carried out.
    */
    PROJECT,
    /*!
      Unique identifier for this observation.
     */
    OBSERVATION_ID,
    /*!
      Description of the telescope mode in which this observation was carried
      out.
     */
    OBSERVATION_MODE,
    /*!
      Identifier for the LOFAR station
    */
    STATION_ID,
    /*!
      Position of a LOFAR station
    */
    STATION_POSITION_VALUE,
    STATION_POSITION_UNIT,
    STATION_POSITION_FRAME,
    /*!
      Identifier for a Remote Station Processing (RSP) board
    */
    RSP_ID,
    /*!
      Identifier for a Receiver Unit (RCU)
    */
    RCU_ID,
    /*!
      Date of the observation. Standard unix date i.e. (GMT-)seconds since
      1.1.1970
    */
    TIME,
    /*!
      The number of sample passed from the point marked by <tt>TIME</tt> and
      the first sample in the stored dataset
    */
    SAMPLE_NUMBER,
    /*!
      Sample frequency in the analog-digital conversion step
    */
    SAMPLE_FREQUENCY_VALUE,
    SAMPLE_FREQUENCY_UNIT,
    SAMPLES_PER_FRAME,
    FEED,
    ANTENNA_POSITION_VALUE,
    ANTENNA_POSITION_UNIT,
    ANTENNA_POSITION_FRAME,
    ANTENNA_ORIENTATION_VALUE,
    ANTENNA_ORIENTATION_UNIT,
    ANTENNA_ORIENTATION_FRAME,
    BEAM_DIRECTION_VALUE,
    BEAM_DIRECTION_UNIT,
    BEAM_DIRECTION_FRAME,
    /*!
      Nyquist zone within which the analog-digital conversion step is performed
    */
    NYQUIST_ZONE,
    // ---------------------------------------------------------------
    //  Attributes associated with the trigger algorithm running on
    //  on the FPGA or the local control unit (LCU)
    // ---------------------------------------------------------------
    TRIGGER_TYPE,
    TRIGGER_OFFSET,
    /*!
      List of antennas from which a trigger originated
    */
    TRIGGERED_ANTENNAS
  };

  // -------------------------------------------------------------------- Methods

  /*!
    \brief Get the name of an attribute

    \param enum -- Numberical identifier for the attribute

    \return name -- The name of the attribute, i.e. the string which can be
            used to access an element within an HDF5 file.
  */
  std::string attribute_name (Attributes const &num);


} // Namespace DAL -- end

#endif /* ENUMERATIONS_H */

