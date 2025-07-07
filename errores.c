#include "errores.h"
#include <stdio.h>
#include <stdlib.h>

void lanzar_error(int error, int num_linea){
    switch(error)
    {
        case FICHERO_NO_DISPONIBLE: //non se imprime o número de liña porque se trata dun erro de envío de argumentos, non do código fonte
            printf("\nERRO: o ficheiro co código fonte non se enviou como argumento\n\n");
            break;
            
        case EXCEDE_TAMANHO_LEXEMA:
            printf("\nERRO: lexema excede o tamaño máximo permitido na liña %d\n\n", num_linea);
            break;
            
        case STRING_NO_CERRADO:
            printf("\nERRO: string non pechado na liña %d\n\n", num_linea);
            break;
            
        case COMENTARIO_GENERAL_NO_CERRADO:
            printf("\nERRO: comentario xeral non pechado na liña %d\n\n", num_linea);
            break;
            
        case HEX_MAL_FORMADO:
            printf("\nERRO: número hexadecimal mal formado na liña %d\n\n", num_linea);
            break;
            
        case PUNTEROS_SIST_ENTRADA: //non se imprime o número de liña porque se trata dun erro de xestión da memoria intermedia, non do código fonte
            printf("\nERRO: existen fallos coa xestión de 'inicio' e 'delantero' no sistema de entrada\n\n");
            break;
        
        case EXP_MAL_FORMADO:
            printf("\nERRO: número exponencial mal formado na liña %d\n\n", num_linea);
            break;
            
        case OCT_MAL_FORMADO:
            printf("\nERRO: número octal mal formado na liña %d\n\n", num_linea);
            break;
            
        case BIN_MAL_FORMADO:
            printf("\nERRO: número binario mal formado na liña %d\n\n", num_linea);
            break;
    }
}
