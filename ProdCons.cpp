/*
 * Programa: ProdCons.cpp
 * Compilar: g++ -o ProdCons ProdCons.cpp -lpthread
 * Ejecutar: ./ProdCons
 */
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

using namespace std;

const int n = 10;
sem_t llenos, vacios, mutex;

int entra = 0, sale = 0;
int buffer[n];

void*
productor(void *) {

  int valor = 0;
  for (;;) {
    valor++;
    sem_wait(&vacios);
    sem_wait(&mutex);
    // Esta en la region critica
    cout << "Productor produciendo" << endl;
    buffer[entra] = valor;
    entra = (entra + 1) % n;
    sem_post(&mutex);
    sem_post(&llenos);
  }
  return NULL;
}

void*
consumidor(void *) {
  for (;;) {
    int valor;
    sem_wait(&llenos);
    sem_wait(&mutex);
    valor = buffer[sale];
    sale = (sale + 1) % n;
    cout << "Valor llegado: " << valor << endl;
    sleep(2);
    sem_post(&mutex);
    sem_post(&vacios);
  }
  return NULL;
}

int
main(){

  sem_init(&llenos, 0, 0);
  sem_init(&vacios, 0, n);
  sem_init(&mutex, 0, 1);
  pthread_t hilo_prod;
  pthread_t hilo_cons;
  pthread_create(&hilo_prod, NULL, productor, NULL);
  pthread_create(&hilo_cons, NULL, consumidor, NULL);
  void* valRet;
  pthread_join(hilo_prod, &valRet);
  return 0;
}
