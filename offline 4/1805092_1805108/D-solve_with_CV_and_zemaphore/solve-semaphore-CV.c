#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_ITERATION 10

pthread_cond_t tobacco_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t paper_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t matches_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t agent_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int tobacco = 0;
int paper = 0;
int matches = 0;
int agent = 1;

void smoke(int smoker) {
    printf("Smoker %d is smoking now \n", smoker);
}

void *agent_thread(void *arg) {
    while(1) {
        pthread_mutex_lock(&mutex);
        while (agent == 0) {
            pthread_cond_wait(&agent_cond, &mutex);
        }
        agent = 0;
        int r = rand() % 3;
        if (r == 0) {
            printf("Agent is now placing matches and paper on the table\n");
            tobacco = 1;
            pthread_cond_signal(&tobacco_cond);
        } else if (r == 1) {
            printf("Agent is now placing matches and tobacco on the table\n");
            paper = 1;
            pthread_cond_signal(&paper_cond);
        } else {
            printf("Agent is now placing tobacco and paper on the table\n");
            matches = 1;
            pthread_cond_signal(&matches_cond);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *smoker_thread(void *data) {
    int smoker = *((int *)data);
    printf("Smoker %d has created\n", smoker);
    while (1) {
        pthread_mutex_lock(&mutex);
        if (smoker == 0) {
            while (tobacco == 0) {
                pthread_cond_wait(&tobacco_cond, &mutex);
            }
            tobacco = 0;
            printf("Smoker with tobacco has started smoking\n");
            smoke(smoker);
            printf("Smoker with tobacco has finished smoking\n");
            agent = 1;
            pthread_cond_signal(&agent_cond);
        } else if (smoker == 1) {
            while (paper == 0) {
                pthread_cond_wait(&paper_cond, &mutex);
            }
            paper = 0;
            printf("Smoker with paper has started smoking\n");
            smoke(smoker);
            printf("Smoker with paper has finished smoking\n");
            agent = 1;
            pthread_cond_signal(&agent_cond);
        } else {
            while (matches == 0) {
                pthread_cond_wait(&matches_cond, &mutex);
            }
            matches = 0;
            printf("Smoker with matches has started smoking\n");
            smoke(smoker);
            printf("Smoker with matches has finished smoking\n");
            agent = 1;
            pthread_cond_signal(&agent_cond);
        }
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    pthread_t agent_thread_id;
    pthread_t smoker_thread_id[3];
    int smoker[3] = {0, 1, 2};

    pthread_create(&agent_thread_id, NULL, agent_thread, NULL);
    for (int i = 0; i < 3; i++) {
        pthread_create(&smoker_thread_id[i], NULL, smoker_thread, &smoker[i]);
    }

    pthread_join(agent_thread_id, NULL);
    for (int i = 0; i < 3; i++) {
        pthread_join(smoker_thread_id[i], NULL);
    }

    return 0;
}
