
#ifndef HDF5_TABLE_H
#define HDF5_TABLE_H

#include <stdlib.h>

#define NFIELDS       (hsize_t)   5
#define NRECORDS      (hsize_t)   8

/*____________________________________________________________________
  Define structure storing particle properties
*/
typedef struct Particle
{
  char   name[16];
  int    lati;
  int    longi;
  float  pressure;
  double temperature;
} Particle;

/*____________________________________________________________________
  Define a subset of Particle, with latitude and longitude fields
*/
typedef struct Position
{
  int    lati;
  int    longi;
} Position;

/*____________________________________________________________________
  Define a subset of Particle, with name and pressure fields
*/
typedef struct NamePressure
{
  char   name[16];
  float  pressure;
} NamePressure;

/*____________________________________________________________________
  Define field information
*/
const char *field_names[NFIELDS] = {"Name",
				    "Latitude",
				    "Longitude",
				    "Pressure",
				    "Temperature" };

#endif
