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
/**
 * \file dal.cpp
 * \author Joseph Masters
 * \date 03-20-07
 */
#ifndef TIMESERIES_H
#define TIMESERIES_H

#ifndef DALBASETYPES_H
#include <dalBaseTypes.h>
#endif

const Int32 ETHEREAL_HEADER_LENGTH = 46;
const Int32 FIRST_EXTRA_HDR_LENGTH = 40;
const Int32 EXTRA_HDR_LENGTH = 16;

typedef struct AntennaStruct {
	unsigned int rsp_id;
	unsigned int rcu_id;
	unsigned int time;
	unsigned int sample_nr;
	unsigned int samples_per_frame;
	short data;
	char feed[16];
	double ant_position[ 3 ];
	double ant_orientation[ 3 ];
} AntennaStruct;

typedef struct TBB_Header {
	unsigned char stationid;
	unsigned char rspid;
	unsigned char rcuid;
	unsigned char sample_freq;
	UInt32 seqnr;
	Int32 time;
	UInt32 sample_nr;
	UInt16 n_samples_per_frame;
	UInt16 n_freq_bands;
	char bandsel[64];
	Int16 spare;
	UInt16 crc;
};

typedef struct TransientSample {
	Int16 value;
};

typedef struct SpectralSample {
	complex<Int16> value;
};

#endif
