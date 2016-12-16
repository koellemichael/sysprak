#include "semaphor.h"

/**
 * Erstellt eine Semaphorengruppe mit 1 Semaphor und initialisiert diesen mit value.
 */
void createSEM(int value){
  if((semid = semget(KEY, N, PERMISSION)) == -1){                               //Semaphorengruppe erstellen
      perror("Error while generating Shared Memory!");                          //Fehlerbehandlung falls Fehler bei Erstellung
      exit(EXIT_FAILURE);
  }
  initSEM(value);
}

/**
 * Initialisiert jeden Semaphor in der Semaphorengruppe mit dem Wert Value.
 * @param value Der Wert mit dem die Semaphoren initialisiert werden
 */
void initSEM(int value){
  int init[N];                                                                  //init Array erstellen
  for(int i=0; i<N; i++) {                                                      //init mit 1 befüllen
    init[i] = value;
  }
  semctl(semid, N, SETALL, init);                                               //Semaphoren initialisieren
}

/**
 * Dekrementiert den ersten Semaphor in der Semaphorengruppe.
 */
void waitSEM(void){
  struct sembuf wait;                                                           //struct für die Semaphorenoperation erstellen
  wait.sem_num = 0;                                                             //Operation wird angewendet auf den ersten Semaphor
  wait.sem_op = -1;                                                             //Operation -1
  wait.sem_flg = SEM_UNDO;                                                      //Falls der Prozess vorzeitig terminiert wird die Semaphore wieder zurückgesetzt
  semop(semid, &wait, 1);                                                       //Operation auf den Semaphor ausführen
}

/**
 * Inkrementiert den ersten Semaphor in der Semaphorengruppe.
 */
void signalSEM(void){
  struct sembuf signal;                                                         //struct für die Semaphorenoperation erstellen
  signal.sem_num = 0;                                                           //Operation wird angewendet auf den ersten Semaphor
  signal.sem_op = 1;                                                            //Operation +1
  signal.sem_flg = 0;
  semop(semid, &signal, 1);                                                     //Operation auf den Semaphor ausführen
}

/**
 * Löscht die Semaphorengruppe.
 */
void deleteSEM(void){
  semctl(semid, N, IPC_RMID, 0);                                                //Semaphorengruppe löschen
}
