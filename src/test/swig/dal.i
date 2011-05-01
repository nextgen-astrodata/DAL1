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

%module dal
%include "std_string.i"
%include "cpointer.i"

%{
/* Includes the header in the wrapper code */
#include "uvw.h"
#include "cs1.h"
#include "lffe.h"
#include "dal.h"
//#include "lffe_buf.h"
#include "dalColumn.h"
#include "dalFilter.h"
#include "dalGroup.h"
#include "dalTable.h"
#include "dalDataset.h"
%}

/* Parse the header file to generate wrappers */
%include "uvw.h"
%include "cs1.h"
%include "lffe.h"
%include "dal.h"
//%include "lffe_buf.h"
%include "dalColumn.h"
%include "dalFilter.h"
%include "dalGroup.h"
%include "dalTable.h"
%include "dalDataset.h"

/* Wrap a class interface around an "int *" */
/* %pointer_class(void, voidp); */
%pointer_class(char, charp);
%pointer_class(short, shortp);
%pointer_class(int, intp);
%pointer_class(double, doublep);
%pointer_class(float, floatp);
%pointer_class(UVW, uvwp);
%pointer_class(CS1, cs1p);
%pointer_class(LFFE, lffep);
//%pointer_class(LFFE_BUF, lffebufp);
