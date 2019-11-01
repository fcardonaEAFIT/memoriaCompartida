#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <cstdio>
#include <cstdlib>
#include <errno.h>
#include <iostream>

using namespace std;

int
main() {
  extern int errno;
  const unsigned int PERM = 0600;
  const unsigned int SHPOS = 14;
  
  sem_t *llenos, *vacios, *mutex;
  if ((llenos = sem_open("llenos",
                         O_CREAT | O_EXCL, PERM, 0)) == SEM_FAILED) {
    perror("sem_open");
    exit(1);
  }
  
  if ((vacios = sem_open("vacios",
                         O_CREAT | O_EXCL, PERM, 10)) == SEM_FAILED) {
    perror("sem_open");
    exit(1);
  }
      
  if ((mutex  = sem_open("mutex",
                         O_CREAT | O_EXCL, PERM, 1))  == SEM_FAILED) {
    perror("sem_open");
    exit(1);
  }

  int mem;

  if ((mem = shm_open("memoria", O_CREAT | O_EXCL | O_RDWR, PERM)) == -1) {
    perror("shm_open");
    exit(1);
  }

  off_t len = sizeof(int) * SHPOS;
  cout << "mem: " << mem << "len: " << len << endl;
  
  if (ftruncate(mem,len) < 0) {
    perror("ftruncate");
    cerr << "Error: " << errno << endl;
    exit(1);
  }

  int *buffer;

  if ((buffer = (int *) mmap(NULL, sizeof(int) * SHPOS, PROT_READ | PROT_WRITE,
                             MAP_SHARED, mem, 0)) == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }

  buffer[0] = 10;  // n
  buffer[1] = 0;   // cantidad
  buffer[2] = 0;   // entra
  buffer[3] = 0;   // sale
  return 0;
}
