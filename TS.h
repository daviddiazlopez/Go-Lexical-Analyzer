#ifndef TS_H
#define TS_H
#include "ABB.h"

/**
 * Inicializa a TS, creándoa primeiro e insertando un array de duplas (lexema, compoñente léxica) correspondentes ás palabras reservadas do ficheiro concurrentSum.go
 * de forma ordeada na TS
 */
void inicializarTS();

/**
 *Chama á función de destrución da estrutura escollida para a TS
 */
void destruirTS();

/**
 *Chama á función de inserción da estrutura escollida para a TS
 * 
 * @param elem elemento que se quere insertar na TS
 */
void insertarTS(TIPOELEMENTOABB elem);

/**
 *Busca un elemento na TS en función da súa chave (lexema) e, en caso afirmativo (atoparse na TS), devolve a compoñente léxica.
 * En caso negativo, inserta o novo elemento na TS
 * 
 * @param lexema cadea de carácteres correspondentes ao lexema atopado
 * @return compoñente léxica correspondete ao lexema enviado como parámetro
 */
int buscarTS(char *lexema);

/**
 *Imprime todos os elementos da TS
 */
void imprimirTS();

#endif /* TS_H */

