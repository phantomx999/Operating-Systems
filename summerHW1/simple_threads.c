#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 10

void *foo(void *arg)
{
  int *retval;
  int myval;

  myval = (int) arg;
  printf("Thread %d: MYVAL=%d\n", myval, myval);

  retval=(int *) malloc(sizeof(int));
  *retval = myval+10;
  printf("Thread %d: RETVAL=%d\n", myval, *retval);
  return(retval);
}


main()
{
  pthread_t threads[NUM_THREADS];
  int *vals[NUM_THREADS];
  int i;

  printf("\nMain: Creating threads\n\n");
  for (i=0; i<NUM_THREADS; i++)
    pthread_create(threads+i, NULL, foo, (void *)i);

  printf("\nMain: Going to Sleep\n\n");
  sleep(10);
  printf("\nMain: Joining on threads\n\n");

  for (i=NUM_THREADS-1; i>=0; i--)
    {
      pthread_join(threads[i], (void **) (vals+i));
      printf("Received retval from thread %d: %d\n", i, *vals[i]);
    }
}
