#include <stdio.h>
#include <stdlib.h>
#include "TS.h"
#include "analisis_lexico.h"
#include "analisis_sintactico.h"
#include "sistema_entrada.h"
#include "errores.h"


int main(int argc, char** argv) {
    if(argc < 2){ //compróbase se se enviou o ficheiro como argumento
        lanzar_error(FICHERO_NO_DISPONIBLE, 0); //envíase '0' como número de liña xa que non se trata dun erro concreto do código fonte
        exit(1);
    }
    
    FILE *fichero = fopen(argv[1], "r"); // Abre o ficheiro en modo lectura
    if (fichero == NULL) {
        perror("Erro ao abrir o ficheiro");
        exit(1);
    }
    
    /*INICIALIZACIÓN DE ESTRUTURAS*/
    inicializarTS(); //inicialízase a TS
    inicializar_sist_entr(fichero); //inicialízase o sistema de entrada
    imprimirTS(); //imprímese a TS para comprobar a súa correcta inicialización
    
    /*COMPILACIÓN*/
    analisis_sintactico(); //comeza o proceso de comilación
    imprimirTS(); //imprímese de novo a TS
    
    //NOTA: ter en conta que Println aparece de primeiro por comezar por maiúscula e no meu ABB prevalecen maiúsculas considéranse menores que as minúsculas
    
    /*LIBERACIÓN DE MEMORIA E DESTRUCIÓN DE ESTRUTURAS*/
    destruirTS(); //destrúese a TS
    fclose(fichero); //péchase o ficheiro
    
    return (EXIT_SUCCESS);
}


