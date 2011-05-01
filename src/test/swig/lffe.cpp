/*-------------------------------------------------------------------------*
| $Id:: cs1.h 389 2007-06-13 08:47:09Z baehren                          $ |
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

#include "lffe.h"

int LFFE::get_ant1(int n){
	return row[n].ant1;
}
int LFFE::get_ant2(int n) {
	return row[n].ant2;
}
dalcomplex LFFE::get_data(int n, int polarization, int channel){
	return row[n].data[polarization][channel];
}
double LFFE::get_time(int n){
	return row[n].time;
}
double LFFE::get_u(int n) {
	return row[n].u;
}
double LFFE::get_v(int n) {
	return row[n].v;
}
double LFFE::get_w(int n) {
	return row[n].w;
}
