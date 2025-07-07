#include "ABB.h"
#include <stdio.h>
#include "TS.h"
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"
#define N 8//lonxitude do vector de inicialización da T

TABB TS;
TIPOELEMENTOABB vectorInicTS[N] = {{PACKAGE, "package"}, {IMPORT, "import"}, {FUNCION, "func"}, {CANAL, "chan"}, {VARIABLE, "var"}, {FOR, "for"}, {RANGO, "range"},{GO, "go"}};

/**
 *Chama á función de inserción da estrutura escollida para a TS
 * 
 * @param elem elemento que se quere insertar na TS
 */
void insertarTS(TIPOELEMENTOABB elem){
    insertarElementoAbb(&TS, elem);
}

/**
 * Destrúe a táboa de símbolos
 */
void destruirTS(){
    if(TS!=NULL){
        destruirAbb(&TS);
    }
}


/**
 *Imprime o lexema e compoñente léxica do elemento 
 * 
 * @param elemento elemento da TS cuxo lexema e compoñente léxico se queren imprimir
 */
void _imprimir_elemento(TIPOELEMENTOABB elemento){
    printf("\n(%d, %s)\n", elemento.componente_lexico, elemento.lexema);
}



/**
 *Percorrido recursivo inorden: I-R-D para a impresion da informacion asociada aos elementos da TS (lexema e compoñente léxico)
 * 
 * @param A árbore de búsqueda binaria que se vai percorrer
 */
void _inorden(TABB A){
    TIPOELEMENTOABB elemento;
    if(!esAbbVacio(A)){
        _inorden(izqAbb(A)); 
        leerElementoAbb(A, &elemento); //lese o elemento da árbore
        _imprimir_elemento(elemento); //imprímese
        _inorden(derAbb(A));
    }
}


/**
 *Imprime todos os elementos da TS
 */
void imprimirTS(){
    /*realizamos el percorrido inorden da árbore se esta es non está vacía*/
    if(!esAbbVacio(TS)){
        printf("\n\n\nIMPRESIÓN TÁBOA DE SÍMBOLOS\n");
        printf("/**************************************/");
        _inorden(TS);
        printf("/**************************************/\n\n");
    }
    else{
        printf("Non hai símbolos rexistrados na táboa de símbolos\n\n\n");
    }
}


/**
 * Inicializa a TS, creándoa primeiro e insertando un array de duplas (lexema, compoñente léxica) correspondentes ás palabras reservadas do ficheiro concurrentSum.go
 * de forma ordeada na TS
 */
void inicializarTS(){
    TIPOELEMENTOABB vectorDinamTS[N];
    crearAbb(&TS); //en primeiro lugar, creamos a árbore 
    for(int i=0; i<N; i++){ //insertamos cada unha das palabras reservadas na TS
        vectorDinamTS[i].lexema = (char *)malloc(strlen(vectorInicTS[i].lexema) + 1);
        if (vectorDinamTS[i].lexema != NULL) { 
            vectorDinamTS[i].componente_lexico = vectorInicTS[i].componente_lexico;
            strcpy(vectorDinamTS[i].lexema, vectorInicTS[i].lexema); 
        }
        insertarTS(vectorDinamTS[i]);
    }
}


/**
 *Busca un elemento na TS en función da súa chave (lexema) e, en caso afirmativo (atoparse na TS), devolve a compoñente léxica.
 * En caso negativo, inserta o novo elemento na TS
 * 
 * @param lexema cadea de carácteres correspondentes ao lexema atopado
 * @return compoñente léxica correspondete ao lexema enviado como parámetro
 */
int buscarTS(char *lexema){   
    TIPOELEMENTOABB elem; //xeramos un  elemento da árbore (struct cos campos lexema e compoñente léxico)
    buscarNodoAbb(TS, lexema, &elem); //aplicamos a función de búsqueda na ABB a partir do lexema
    if (esMiembroAbb(TS, elem)){ //en caso de que o lexema se atope na TS, devolvemos a compoñente léxica
        return elem.componente_lexico;
    }
    else{ //no caso de que o lexema non se atope na TS, insertarémolo coa compoñente léxica ID
        elem.lexema = (char *)malloc(strlen(lexema) + 1); // +1 para o carácter nulo '\0'
        if (elem.lexema != NULL) { //comprobamos que a memoria foi asignada correctamente
            strcpy(elem.lexema, lexema); // Copiamos a cadea
        }
        elem.componente_lexico = ID; //asignamos a compoñente léxica ID porque se non é unha palabra reservada será un identificador
        insertarTS(elem); // esta función hai que rematala porque non podemos insertar un elemento sen que teña compoñente léxica
        
        /*Descomentar se se quere observar a inserción dos identificadores na TS*/
        //printf("O lexema %s foi insertado correctamente\n", lexema);
        
        return ID;
    }
    
}