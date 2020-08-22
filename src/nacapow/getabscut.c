#include <sys/resource.h>
#include <sys/time.h>

void getabscut_(double *tarray)
{
  struct rusage RU;
  struct timeval tv;

  getrusage(RUSAGE_SELF, &RU);
  gettimeofday(&tv, (struct timezone *)0);

  tarray[0]=RU.ru_utime.tv_sec + (double)RU.ru_utime.tv_usec*1e-6;
  tarray[1]=RU.ru_stime.tv_sec + (double)RU.ru_stime.tv_usec*1e-6;
  tarray[2]=tv.tv_sec+(double)tv.tv_usec*1e-6;
  return;
}
