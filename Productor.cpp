/*
 * Programa: Productor.cpp
 * Compilar: g++ -o Productor Productor.cpp -lpthread -lrt
 * Ejecutar: ./Productor
 */
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

using namespace std;

const int n = 10;

int
main(int argc, const char *argv[]) {

  sem_t *llenos, *vacios, *mutex;

  if ((llenos = sem_open("llenos", O_RDWR)) < 0) {
    perror("sem_open");
    exit(1);
  }

  if ((vacios = sem_open("vacios", O_RDWR)) < 0) {
    perror("sem_open");
    exit(1);
  }

  if ((mutex = sem_open("mutex", O_RDWR)) < 0) {
    perror("sem_open");
    exit(1);
  }

  int mem;
  
  if ((mem = shm_open("memoria", O_RDWR, 0600)) == -1) {
    perror("shm_open");
    exit(1);
  }

  int *memoria;

  if ((memoria = (int *) mmap(NULL, sizeof(int) * 13, PROT_READ | PROT_WRITE,
                             MAP_SHARED, mem, 0)) == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }

  int *buffer = memoria + 4;
  int *n      = memoria;
  int *cantidad = memoria + 1;
  int *entra  = memoria + 2;
  
  int valor = 0;
  for (;;) {
    valor++;
    sem_wait(vacios);
    sem_wait(mutex);
    // Esta en la region critica
    cout << "Productor produciendo: " << valor << endl;
    buffer[*entra] = valor;
    *entra = (*entra + 1) % *n;
    *cantidad = *cantidad + 1; 
    sem_post(mutex);
    sem_post(llenos);
  }
  
  return 0;
}
