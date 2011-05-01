/*-------------------------------------------------------------------------*
 | $Id::                                                                 $ |
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

#include "dal.h"

/*
class CS1 {
private:
	int ant1;
	int ant2;
	dalcomplex data[4][256];
	double time;
	double u;
	double v;
	double w;
public:
	int get_ant1();
	int get_ant2();
	dalcomplex get_data(int polarization, int channel);
	double get_time();
	double get_u();
	double get_v();
	double get_w();
};
*/

typedef struct CS1_ROW {
	int ant1;
	int ant2;
	dalcomplex data[4][256];
	double time;
	double u;
	double v;
	double w;
} CS1_ROW;

class CS1 {
private:
	CS1_ROW row[50];
public:
	int get_ant1(int n);
	int get_ant2(int n);
	dalcomplex get_data(int n, int polarization, int channel);
	double get_time(int n);
	double get_u(int n);
	double get_v(int n);
	double get_w(int n);
};
