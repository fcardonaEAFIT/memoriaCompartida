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
  
  if (sem_unlink("llenos") == -1) {
    perror("sem_unlink");
    exit(1);
  }
  
  if (sem_unlink("vacios") == -1) {
    perror("sem_unlink");
    exit(1);
  }
      
  if (sem_unlink("mutex")  == -1) {
    perror("sem_unlink");
    exit(1);
  }

  int mem;

  if (shm_unlink("memoria") == -1) {
    perror("shm_unlink");
    exit(1);
  }


  return 0;
}
