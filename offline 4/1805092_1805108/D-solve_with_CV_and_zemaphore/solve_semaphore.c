#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include "zemaphore.h"

zem_t tobacco;
zem_t paper;
zem_t matches;
zem_t agent;
// int count = 3;
void smoke(int smoker) {
    printf("Smoker %d is smoking now \n", smoker);
    // count--;
}


void *agent_thread(void *arg) {
    while (1) {
        zem_down(&agent);
        int r = rand() % 3;
        // printf("agent is creating %d \n", r);
        if (r == 0) {
            //  zem_up(&paper);
            // zem_up(&matches);
            printf("agent is now placing matches and paper in the table\n");
            zem_up(&tobacco);

           
        } else if (r == 1) {
            // zem_up(&tobacco);
            // zem_up(&matches);
            printf("agent is now placing matches and tobacco in the table\n");
            zem_up(&paper);
        } else {
            // zem_up(&tobacco);
            // zem_up(&paper);
            printf("agent is now placing tobacco and paper in the table\n");
            zem_up(&matches);
        }
    }

    return NULL;
}


void *smoker_thread(void *data) {
   int smoker = *((int *)data);
   printf("smoker %d has  created \n", smoker);
    while (1) {
        if (smoker == 0) {
            zem_down(&tobacco);
            // zem_down(&agent);
            printf("smoker with tobacco has started smoking\n");
            smoke(smoker);
            printf("smoker with tobacco has finished smoking\n");
            zem_up(&agent);
        } else if (smoker == 1) {
            zem_down(&paper);
            // zem_down(&agent);
            printf("smoker with paper has started smoking\n");
            smoke(smoker);
            printf("smoker with paper has finished smoking\n");

            zem_up(&agent);
        } else {
            zem_down(&matches);
            // zem_down(&agent);
            printf("smoker with matches has started smoking\n");

            smoke(smoker);
            printf("smoker with matches has finished smoking\n");

            zem_up(&agent);
        }
    }

    return NULL;
}


int main(int argc, char *argv[]) {
    zem_init(&tobacco, 0);
    zem_init(&paper, 0);
    zem_init(&matches, 0);
    zem_init(&agent, 1);
    int mythread_id[3];
    for (int i = 0; i < 3; i++)
    {
        /* code */
        mythread_id[i] = i;
    }
    
    // mythread_id[0] = 0;
    // mythread_id[1] = 1;
    // mythread_id[2] = 2;

    pthread_t smoker1, smoker2, smoker3, agent;
    pthread_create(&smoker1, NULL, smoker_thread, (void *)&mythread_id[0]);
    pthread_create(&smoker2, NULL, smoker_thread, (void *)&mythread_id[1]);
    pthread_create(&smoker3, NULL, smoker_thread, (void *)&mythread_id[2]);
    pthread_create(&agent, NULL, agent_thread, NULL);

    pthread_join(smoker1, NULL);
    pthread_join(smoker2, NULL);
    pthread_join(smoker3, NULL);
    pthread_join(agent, NULL);

    return 0;
}





