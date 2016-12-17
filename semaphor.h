#ifndef sema                                                                    //Wenn das Makro noch nicht existiert
#define sema                                                                    //Definiere das Makro

//Bibliotheken einbinden
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>

//Konstanten
#define PERMISSION 0644
#define KEY IPC_PRIVATE

//Funktionen deklarieren
int createSEM(int n, int value);
void initSEM(int semid, int n, int value);
void waitSEM(int semid, int n);
void signalSEM(int semid, int n);
void deleteSEM(int semid, int n);

#endif
