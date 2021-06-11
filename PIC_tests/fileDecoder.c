#include <stdio.h>
#include <stdlib.h>
#include "txtCoordsParser.h"

const char *fname = "../ElaboratoInterfaceSansonEris/canvasData/coords.txt";
 
int main(void) {

    int X[9999], Y[9999], Z[9999], tot;
    coordinateMotori(fname, X, Y, Z, tot);

    for (int i=0 ; i<tot ; i++) {
      printf("n:%i\t X:%i\t Y:%i\t Z:%i\n",i+1, X[i],Y[i],Z[i]);       
    }

    return 0;
}