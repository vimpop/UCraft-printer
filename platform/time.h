#ifndef TIME_H
#define TIME_H

/* A time value that is accurate to the nearest
   microsecond but also has a range of years.  */
struct timeval
{
#ifdef __USE_TIME_BITS64
  __time64_t tv_sec;		/* Seconds.  */
  __suseconds64_t tv_usec;	/* Microseconds.  */
#else
  long tv_sec;		/* Seconds.  */
  long tv_usec;	/* Microseconds.  */
#endif
};

#endif //TIME_H