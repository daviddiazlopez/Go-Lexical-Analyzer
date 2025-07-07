#ifndef SISTEMA_ENTRADA_H
#define SISTEMA_ENTRADA_H
#include "ABB.h"
#include <stdio.h>

/*Déixase este tamaño de bloque para observar varios erros nos que se excedeu o tamaño máximo do lexema permitido*/
#define MAX_BUFFER 10
/**
 * Inicializa o sistema de entrada, enchendo de carácteres o bloque A e incluíndo os carácteres
 * centinela (EOF) na última posición de cada un dos dous bloques
 * @param f punteiro ao ficheiro en modo lectura que contén o código fonte
 */
void inicializar_sist_entr(FILE *f);

/**
 * Axusta o punteiro 'delantero' cando un carácter é devolto polo analizador léxico
 * @param num_caracteres
 */
void devolver(int num_caracteres);

/**
 * Sitúa o punteiro inicio na posición do bloque na que se atope delantero, xa que, como non
 * se acepta o lexema, se produce un erro ou son carácteres do código fonte sen significado gramatical, 
 * simplemente se ignoran os carácteres existentes entre ambos punteiros
 */
void saltar_lexema();

/**
 * Garda no campo 'lexema' da variable 'elem' o lexema que se atopa entre os punteiros inicio e delantero
 * Tamén comproba se se excedeu o tamaño máximo de lexema permitido, gardando NULL no campo 'lexema' nese caso
 * e emitindo un erro
 * @param elem variable de tipo TIPOELEMENTOABB cuxo campo 'lexema' vai ser modificado
 */
void aceptar_lexema(TIPOELEMENTOABB *elem);

/**
 * Devolve o seguinte carácter do código fonte, tendo en conta a dobre centinela empregada
 * para almacenar no sistema de entrada. Tamén detecta se un lexema excede o tamaño máximo
 * permitido, modificando as variables necesarias e realiza os axustes dos punteiros que 
 * sexan requiridos.
 * @return o seguinte carácter do código fonte
 */
char seguinte_caracter();



#endif /* SISTEMA_ENTRADA_H */

