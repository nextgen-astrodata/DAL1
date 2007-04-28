%module dal
%include "std_string.i"
%{
/* Includes the header in the wrapper code */
#include "dal.h"
#include "dalAttribute.h"
#include "dalColumn.h"
#include "dalFilter.h"
#include "dalGroup.h"
#include "dalTable.h"
#include "dalDataset.h"
%}
/* Parse the header file to generate wrappers */
%include "dal.h"
%include "dalAttribute.h"
%include "dalColumn.h"
%include "dalFilter.h"
%include "dalTable.h"
%include "dalDataset.h"
