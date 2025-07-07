
#ifndef ERRORES_H
#define ERRORES_H

/**
 * Imprime por pantalla o erro indicado como argumento e a liña na que este se produce
 * @param error erro producido
 * @param num_linea número de liña no que se produce o erro
 */
void lanzar_error(int error, int num_linea);

#define FICHERO_NO_DISPONIBLE 1
#define EXCEDE_TAMANHO_LEXEMA 2
#define STRING_NO_CERRADO 3
#define COMENTARIO_GENERAL_NO_CERRADO 4
#define HEX_MAL_FORMADO 5
#define PUNTEROS_SIST_ENTRADA 6
#define EXP_MAL_FORMADO 7
#define OCT_MAL_FORMADO 8
#define BIN_MAL_FORMADO 9




#endif /* ERRORES_H */

