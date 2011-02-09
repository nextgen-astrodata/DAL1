
#include <hdf5.h>

int main ()
{
#ifdef H5_HAVE_PARALLEL
  return 1;
#else
  return 0;
#endif
}
