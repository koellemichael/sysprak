#include "semaphor.h"

/**
 * Erstellt eine Semaphorengruppe mit n Semaphoren und initialisiert diesen mit value.
 * @param n Anzahl der Semaphoren in der Semaphorengruppe
 * @param value Wert mit dem die Semaphoren initialisiert werden sollen
 * @return ID der Semaphorengruppe
 */
int createSEM(int n, int value){
  int semid = -1;
  if((semid = semget(KEY, n, PERMISSION)) == -1){                               //Semaphorengruppe erstellen
      perror("Error while creating semaphors");                                 //Fehlerbehandlung falls Fehler bei Erstellung
      exit(EXIT_FAILURE);
  }
  initSEM(semid,n,value);                                                       //Semaphoren initialisieren
  return semid;
}

/**
 * Initialisiert jeden Semaphor in der Semaphorengruppe mit dem Wert Value.
 * @param semid ID der Semaphorengruppe
 * @param n Anzahl der Semaphoren in der Semaphorengruppe
 * @param value Der Wert mit dem die Semaphoren initialisiert werden
 */
void initSEM(int semid, int n, int value){
  int init[n];                                                                  //init Array erstellen
  for(int i=0; i<n; i++) {                                                      //init mit 1 befüllen
    init[i] = value;
  }
  if((semctl(semid, n, SETALL, init)) == -1){                                   //Semaphoren initialisieren
      perror("Error while initializing semaphors");                             //Fehlerbehandlung falls Fehler bei Initialisierung
      exit(EXIT_FAILURE);
  }
}

/**
 * Dekrementiert den n-ten Semaphor in der Semaphorengruppe.
 * @param semid ID der Semaphorengruppe
 * @param n n-ter Semaphor der dekrementiert werden soll
 */
void waitSEM(int semid, int n){
  struct sembuf wait;                                                           //struct für die Semaphorenoperation erstellen
  wait.sem_num = n;                                                             //Operation wird angewendet auf den ersten Semaphor
  wait.sem_op = -1;                                                             //Operation -1
  wait.sem_flg = SEM_UNDO;                                                      //Falls der Prozess vorzeitig terminiert wird die Semaphore wieder zurückgesetzt
  if((semop(semid, &wait, 1))==-1){                                             //Operation auf den Semaphor ausführen
    perror("Error at wait semaphor");
    exit(EXIT_FAILURE);
  }
}

/**
 * Inkrementiert den n-ten Semaphor in der Semaphorengruppe.
 * @param semid ID der Semaphorengruppe
 * @param n n-ter Semaphor der inkrementiert werden soll
 */
void signalSEM(int semid, int n){
  struct sembuf signal;                                                         //struct für die Semaphorenoperation erstellen
  signal.sem_num = n;                                                           //Operation wird angewendet auf den ersten Semaphor
  signal.sem_op = 1;                                                            //Operation +1
  signal.sem_flg = 0;
  if((semop(semid, &signal, 1))==-1){                                           //Operation auf den Semaphor ausführen
    perror("Error at signal semaphor");
    exit(EXIT_FAILURE);
  }
}

/**
 * Löscht eine Semaphorengruppe.
 * @param semid ID der Semaphorengruppe
 * @param n Anzahl der Semaphoren in der Semaphorengruppe
 */
void deleteSEM(int semid, int n){
  if((semctl(semid, n, IPC_RMID, 0))==-1){                                      //Semaphorengruppe löschen
    perror("Error while deleting semaphor");
    exit(EXIT_FAILURE);
  }
}
