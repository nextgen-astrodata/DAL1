/***************************************************************************
 *   Copyright (C) 2006 by Joseph Masters                                  *
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
#ifndef DALATTRIBUTE_H
#define DALATTRIBUTE_H

#ifndef DAL_H
#include "dal.h"
#endif

class dalAttribute {

	string name; // attribute name
	string units; // attribute units
	string description; // column description
	
  public:
	int getName(); // return the attribute name
	int getUnits(); // return the attribute units
	int getDescription(); // return the attribute description
	int setName(); // set the attribute name
	int setUnits(); // set the attribute units
	int setDescription(); // set the attribute description
};
#endif
