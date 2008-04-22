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

#include "Enumerations.h"

namespace DAL { // Namespace DAL -- begin
  
  // ============================================================================
  //
  //  Methods
  //
  // ============================================================================
  
  std::string attribute_name (Attributes const &num)
  {
    std::string name;
    
    switch (num) {
      // --- General attributes for telescope and observation --------
    case TELESCOPE:
      name = "TELESCOPE";
      break;
    case OBSERVER:
      name = "OBSERVER";
      break;
    case PROJECT:
      name = "PROJECT";
      break;
    case OBSERVATION_ID:
      name = "OBS_ID";
      break;
    case OBSERVATION_MODE:
      name = "OBS_MODE";
      break;
      // --- Attributes for an individual LOFAR station --------------
    case STATION_ID:
      name = "STATION_ID";
      break;
    case STATION_POSITION_VALUE:
      name = "STATION_POSITION_VALUE";
      break;
    case STATION_POSITION_UNIT:
      name = "STATION_POSITION_UNIT";
      break;
    case STATION_POSITION_FRAME:
      name = "STATION_POSITION_FRAME";
      break;
      // --- Attributes for an individual signal channel (dipole) ----
    case RSP_ID:
      name = "RSP_ID";
      break;
    case RCU_ID:
      name = "RCU_ID";
      break;
    case TIME:
      name = "TIME";
      break;
    case SAMPLE_NUMBER:
      name = "SAMPLE_NR";
      break;
    case SAMPLE_FREQUENCY:
      name = "SAMPLE_FREQ";
      break;
    case NYQUIST_ZONE:
      name = "NYQUIST_ZONE";
      break;
    case SAMPLES_PER_FRAME:
      name = "SAMPLES_PER_FRAME";
      break;
    case DATA_LENGTH:
      name = "DATA_LENGTH";
      break;
    case FEED:
      name = "FEED";
      break;
    case ANTENNA_POSITION:
      name = "ANT_POSITION";
      break;
    case ANTENNA_POSITION_VALUE:
      name = "ANTENNA_POSITION_VALUE";
      break;
    case ANTENNA_POSITION_UNIT:
      name = "ANTENNA_POSITION_UNIT";
      break;
    case ANTENNA_POSITION_FRAME:
      name = "ANTENNA_POSITION_FRAME";
      break;
    case ANTENNA_ORIENTATION:
      name = "ANT_ORIENTATION";
      break;
    case ANTENNA_ORIENTATION_VALUE:
      name = "ANTENNA_ORIENTATION_VALUE";
      break;
    case ANTENNA_ORIENTATION_UNIT:
      name = "ANTENNA_ORIENTATION_UNIT";
      break;
    case ANTENNA_ORIENTATION_FRAME:
      name = "ANTENNA_ORIENTATION_FRAME";
      break;
    case BEAM_DIRECTION:
      name = "BEAM_DIR";
      break;
      // --- Trigger algorithm ---------------------------------------
    case TRIGGER_TYPE:
      name = "TRIG_TYPE";
      break;
    case TRIGGER_OFFSET:
      name = "TRIG_OFST";
      break;
    case TRIGGERED_ANTENNAS:
      name = "TRIG_ANTS";
      break;
    }
    
    return name;
  }
  
  
} // Namespace DAL -- end
