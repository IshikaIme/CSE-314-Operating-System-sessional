#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>

using namespace std;

struct read_write_lock
{
    sem_t mutex;
    sem_t wrt;
	sem_t test;
	sem_t test2;
	sem_t rmutex;
    int readers;
	int writers;

};

void InitalizeReadWriteLock(struct read_write_lock * rw);
void ReaderLock(struct read_write_lock * rw);
void ReaderUnlock(struct read_write_lock * rw);
void WriterLock(struct read_write_lock * rw);
void WriterUnlock(struct read_write_lock * rw);
