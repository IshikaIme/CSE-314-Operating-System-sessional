#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "zemaphore.h"

#define NUM_THREADS 3
#define NUM_ITER 10

zem_t flag1;
zem_t flag2;
zem_t flag3;

void *justprint(void *data)
{
  int thread_id = *((int *)data);
  // zem_down(&flag1);

  for(int i=0; i < NUM_ITER; i++)
    {
      if (thread_id == 0)
      {
        /* code */
         zem_down(&flag1);
      }
      else if (thread_id == 1)
      {
        /* code */
        zem_down(&flag2);
      }
      else if (thread_id == 2)
      {
        /* code */
        zem_down(&flag3);
      }
      
     
      printf("This is thread %d\n", thread_id);
      if (thread_id == 0)
      {
        /* code */
         zem_up(&flag2);
      }
      else if (thread_id == 1)
      {
        /* code */
        zem_up(&flag3);
      }
      else if (thread_id == 2)
      {
        /* code */
        zem_up(&flag1);
      }
    }
  // zem_up(&flag1);
  return 0;
}

int main(int argc, char *argv[])
{

  pthread_t mythreads[NUM_THREADS];
  int mythread_id[NUM_THREADS];

    zem_init(&flag1, 1);
    zem_init(&flag2, 0);
    zem_init(&flag3, 0);
  
  for(int i =0; i < NUM_THREADS; i++)
    {
      mythread_id[i] = i;
      
      pthread_create(&mythreads[i], NULL, justprint, (void *)&mythread_id[i]);
      // zem_down(&flag1);
    }
  
  for(int i =0; i < NUM_THREADS; i++)
    {
      pthread_join(mythreads[i], NULL);
    }
  
  return 0;
}
