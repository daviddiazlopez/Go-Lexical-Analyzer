#ifndef ANALISIS_LEXICO_H
#define ANALISIS_LEXICO_H
#include "ABB.h"

/**
 * Devolve o seguinte compoñente léxico do código fonte 
 */
TIPOELEMENTOABB seguinte_comp_lexico();

/**
 * Devolve a liña actual do código fonte que se está a procesar
 */
int obtener_linea_actual();

#endif /* ANALISIS_LEXICO_H */

