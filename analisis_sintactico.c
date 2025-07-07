#include "analisis_sintactico.h"
#include "analisis_lexico.h"
#include <stdlib.h>
#include <stdio.h>
#include "ABB.h"

/*NOTA IMPORTANTE*/
/*A pesar de que xa se comentou en analizador_lexico.c, cómpre recordar que a impresión dos ';'
 inseridos automaticamente polo compilador realízase do seguinte xeito: (59, (null))
 Considerouse adecuado, polo tanto, que nestes casos o lexema fose NULL, para que así
 quedase plasmada na saída do programa a diferenza de cando cando ';' aparece explicitamente 
 * no código fonte: (59, ;), fronte a cando ';' é inserido automaticamente polo compilador */


void analisis_sintactico(){
    TIPOELEMENTOABB elem;
    elem = seguinte_comp_lexico();
    printf("\n\n\nIMPRESIÓN (compoñente léxica, lexema)\n");
        printf("/**************************************/\n");
    while(elem.componente_lexico!=EOF)//actúa como consumidor solicitando compoñentes léxicas ata que se lea todo o ficheiro
    {
        printf("(%d, %s)\n", elem.componente_lexico, elem.lexema);
        if(elem.lexema!=NULL){ //libérase a memoria dinámica empregada para almacenar os lexemas
            free(elem.lexema);
            elem.lexema = NULL;
        }
        elem = seguinte_comp_lexico();
    }
    printf("/**************************************/\n");
}