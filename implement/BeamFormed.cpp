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
#include "BeamFormed.h"
#endif

namespace DAL {

  BeamFormed::BeamFormed(){};

  BeamFormed::BeamFormed(std::string const &filename)
  {
    filename_p = filename;
    init();
  }

  BeamFormed::~BeamFormed()
  {
    dataset_p->close();
  }

  void BeamFormed::summary (std::ostream &os)
  {
    os << "[BeamFormed Data] Summary of object properties"     << endl;

    os << "-- Filename .......... : " << filename()              << endl;
    os << "-- Telesope .......... : " << telescope()             << endl;
    os << "-- Number of Stations. : " << nstations()             << endl;
    os << "-- Observer .......... : " << observer()              << endl;
    os << "-- Datatype .......... : " << datatype()              << endl;
    os << "-- Emband   .......... : " << emband()                << endl;
    os << "-- Observation Id .... : " << observation_id()        << endl;
    os << "-- Project Id ........ : " << proj_id()               << endl;
    os << "-- Point RA .......... : " << point_ra()              << endl;
    os << "-- Point DEC ......... : " << point_dec()             << endl;
    os << "-- Epoch Date ........ : " << epoch_date()            << endl;
    os << "-- Epoch UTC ......... : " << epoch_utc()             << endl;
    os << "-- Epoch LST ......... : " << epoch_lst()             << endl;
//     os << "-- Notes ............. : " << notes()                 << endl;
  }

  std::string BeamFormed::filename ()
  {
    std::string attribute_filename ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * filename = reinterpret_cast<char*>(dataset_p->getAttribute("FILENAME"));
	attribute_filename = string(filename);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute FILENAME" << endl;
	attribute_filename = "";
      }
    }
   return attribute_filename;
  }

  std::string BeamFormed::telescope ()
  {
    std::string attribute_telescope ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * telescope = reinterpret_cast<char*>(dataset_p->getAttribute("TELESCOPE"));
	attribute_telescope = string(telescope);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute TELESCOPE" << endl;
	attribute_telescope = "";
      }
    }
   return attribute_telescope;
  }

  int BeamFormed::nstations ()
  {
    int nstations;
    if (dataset_p->getName() != "UNDEFINED") {
      try {
	int * nstations_p = reinterpret_cast<int*>(dataset_p->getAttribute("NUMBER_OF_STATIONS"));
	nstations = *nstations_p;

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute NUMBER_OF_STATIONS" << endl;
	nstations = -1;
      }
    }
    return nstations;
  }

  std::string BeamFormed::observer ()
  {
    std::string attribute_observer ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * observer = reinterpret_cast<char*>(dataset_p->getAttribute("OBSERVER"));
	attribute_observer = string(observer);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute OBSERVER" << endl;
	attribute_observer = "";
      }
    }
   return attribute_observer;
  }

  std::string BeamFormed::datatype ()
  {
    std::string attribute_datatype ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * datatype = reinterpret_cast<char*>(dataset_p->getAttribute("DATATYPE"));
	attribute_datatype = string(datatype);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute DATATYPE" << endl;
	attribute_datatype = "";
      }
    }
   return attribute_datatype;
  }

  std::string BeamFormed::emband ()
  {
    std::string attribute_emband ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * emband = reinterpret_cast<char*>(dataset_p->getAttribute("EMBAND"));
	attribute_emband = string(emband);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute EMBAND" << endl;
	attribute_emband = "";
      }
    }
   return attribute_emband;
  }

  std::string BeamFormed::observation_id ()
  {
    std::string attribute_observation_id ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * observation_id = reinterpret_cast<char*>(dataset_p->getAttribute("OBSERVATION_ID"));
	attribute_observation_id = string(observation_id);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute OBSERVATION_ID" << endl;
	attribute_observation_id = "";
      }
    }
   return attribute_observation_id;
  }

  std::string BeamFormed::proj_id ()
  {
    std::string attribute_proj_id ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * proj_id = reinterpret_cast<char*>(dataset_p->getAttribute("PROJ_ID"));
	attribute_proj_id = string(proj_id);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute PROJ_ID" << endl;
	attribute_proj_id = "";
      }
    }
   return attribute_proj_id;
  }

  std::string BeamFormed::point_ra ()
  {
    std::string attribute_point_ra ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * point_ra = reinterpret_cast<char*>(dataset_p->getAttribute("POINT_RA"));
	attribute_point_ra = string(point_ra);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute POINT_RA" << endl;
	attribute_point_ra = "";
      }
    }
   return attribute_point_ra;
  }

  std::string BeamFormed::point_dec ()
  {
    std::string attribute_point_dec ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * point_dec = reinterpret_cast<char*>(dataset_p->getAttribute("POINT_DEC"));
	attribute_point_dec = string(point_dec);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute POINT_DEC" << endl;
	attribute_point_dec = "";
      }
    }
   return attribute_point_dec;
  }

  std::string BeamFormed::epoch_date ()
  {
    std::string attribute_epoch_date ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * epoch_date = reinterpret_cast<char*>(dataset_p->getAttribute("EPOCH_DATE"));
	attribute_epoch_date = string(epoch_date);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute EPOCH_DATE" << endl;
	attribute_epoch_date = "";
      }
    }
   return attribute_epoch_date;
  }

  std::string BeamFormed::epoch_utc ()
  {
    std::string attribute_epoch_utc ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * epoch_utc = reinterpret_cast<char*>(dataset_p->getAttribute("EPOCH_UTC"));
	attribute_epoch_utc = string(epoch_utc);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute EPOCH_UTC" << endl;
	attribute_epoch_utc = "";
      }
    }
   return attribute_epoch_utc;
  }

  std::string BeamFormed::epoch_lst ()
  {
    std::string attribute_epoch_lst ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * epoch_lst = reinterpret_cast<char*>(dataset_p->getAttribute("EPOCH_LST"));
	attribute_epoch_lst = string(epoch_lst);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute EPOCH_LST" << endl;
	attribute_epoch_lst = "";
      }
    }
   return attribute_epoch_lst;
  }

  std::string BeamFormed::notes ()
  {
    std::string attribute_notes ("");

    if (dataset_p->getName() != "UNDEFINED") {
      try {
	char * notes = reinterpret_cast<char*>(dataset_p->getAttribute("NOTES"));
	attribute_notes = string(notes);

      } catch (std::string message) {
	std::cerr << "-- Error extracting attribute NOTES" << endl;
	attribute_notes = "";
      }
    }
   return attribute_notes;
  }


  bool BeamFormed::init ()
  {
    bool status (true);

    // Connection to dataset via DAL layer

    dataset_p = new dalDataset();
    if (dataset_p->open(filename_p.c_str())) {
      std::cerr << "[BeamFormed::init] Error opening file into dalDataset!"
		<< std::endl;
      return false;
    }
    return status;
  }


} // end namespace DAL
