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
#define N 1

//Globale Variablen
int semid;

//Funktionen deklarieren
void createSEM(int value);
void initSEM(int value);
void waitSEM(void);
void signalSEM(void);
void deleteSEM(void);

#endif
