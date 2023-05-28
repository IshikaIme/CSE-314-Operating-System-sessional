#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  rw->readers = 0;
  sem_init(&rw->mutex, 0, 1);
  sem_init(&rw->wrt, 0, 1);
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  sem_wait(&rw->mutex);
  rw->readers++;
  if (rw->readers == 1)
    sem_wait(&rw->wrt);
  sem_post(&rw->mutex);
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  sem_wait(&rw->mutex);
  rw->readers--;
  if (rw->readers == 0)
    sem_post(&rw->wrt);
  sem_post(&rw->mutex);
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  sem_wait(&rw->wrt);

}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  sem_post(&rw->wrt);
}
