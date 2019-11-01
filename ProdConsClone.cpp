/*
 * Programa: ProdConsClone.cpp
 * Compilar: g++ -o ProdConsClone ProdConsClone.cpp -lpthread
 * Ejecutar: ./ProdCons
 */
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sched.h>

using namespace std;

const int n = 10;
sem_t llenos, vacios, mutex;

int entra = 0, sale = 0;
int buffer[n];

int
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
  return 0;
}

int
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
  return 0;
}

const int STACK_SIZE = 1024;
int
main(){

  sem_init(&llenos, 0, 0);
  sem_init(&vacios, 0, n);
  sem_init(&mutex, 0, 1);
  char *pila1 = new char[STACK_SIZE];
  char *pila2 = new char[STACK_SIZE];
  clone(productor, pila1 + STACK_SIZE, CLONE_VM | CLONE_FILES, NULL);
  clone(consumidor, pila2 + STACK_SIZE, CLONE_VM | CLONE_FILES, NULL);
  return 0;
}
