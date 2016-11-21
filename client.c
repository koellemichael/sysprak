#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "performConnection.h"

#define GAMEKINDNAME "Bashni"
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"

int main (int argc, char **argv){
  int *fd = malloc(2*sizeof(int));
  performConnection(fd);
}
