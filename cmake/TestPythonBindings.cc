
#include <iostream>

/* Python header file(s) */
#include <Python.h>

/* Boost header files */
#include <boost/python.hpp>
#include <boost/python/object.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>

/* Num-Util header file(s) */
#include <num_util.h>

namespace bpl = boost::python;

//_______________________________________________________________________________
//                                                                           main

int main () 
{
  int status = 0;
  int nelem  = 0;

  bpl::list bpl_list;
  
  // Add items/values to the list
  for (int ii=0; ii<nelem; ii++) {
    bpl_list.append(1);
  }
  // Get the number of list elements
  int nofElements = bpl::len(bpl_list);
  
  return status;
}
