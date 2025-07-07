#include "analisis_lexico.h"
#include "sistema_entrada.h"
#include "definiciones.h"
#include "TS.h"
#include "ABB.h"
#include "errores.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*ALFANUM é unha constante que devolverá o autómata alfanumérico á función seguinte_comp_lexico()
 * en caso de aceptar un lexema. Unha vez aceptada dita cadea, será dita función a que se encargue
 * de comprobar se o lexema se atopa na TS ou non e, en función diso, devolver unha ou outra 
 * compoñente léxica.
 */
#define ALFANUM 400


/*Almacena a última compoñente léxica, para a súa utilización na decisión de que o compilador 
 * insira ou non ';' ao final de liña*/
int ultimo_comp_lexico = 0;

/*Indica o número de liña actual do código fonte. Xorde pola necesidade de saber se ultimo_comp_lexico
 se corresponde coa compoñente léxica da liña actual ou dalgunha anterior, para así saber se é necesario
 que o compilador insira de forma automática ';' ou non*/
int num_linea = 1;

/*Indica o número de liña no que se aceptou o último lexema, para a súa utilización na decisión de que
 o compilador insira ou non ';' ao final da liña*/
int linea_ultimo_comp_lex = 0; //temos que ir actualizándoa, coidado con onde se actualiza unha e outra

/*Indica se o último ';' foi engadido (en caso de terse que engadir) automaticamente polo compilador. 
 * A súa presenza é necesaria, xa que se pode dar o caso de que o último carácter antes de 'EOF' non 
 * sexa '\n', polo que precisamos dunha variable que, en caso de recibirse o carácter 'EOF' dende 
 * o sistema de entrada, determine se xa foi engadido o ';' (en caso de ser necesario) ou non e, 
 * polo tanto, proceder á ao envío da compoñente léxica correspondente a 'EOF' */
int last_semicolon = 0;

/*Almacena o número de carácteres a devolver ao sistema de entrada mediante a función devolver(int num_caracteres_devolver).
 *Foi creada porque inicialmente se pensaba que sería necesaria para un caso do autómata de números e logo observouse a súa 
 * relevancia para unha posible xeralización do analizador léxico, xa que en ocasións, os autómatas poden necesitar analizar 
 * máis dun carácter do lexema para determinar se se acepta ou non. Polo tanto, aínda que o seu valor nesta versión do analizador
 * léxico se mantén intacto (1 durante toda a execución, poderá ser de gran utilidade nunha futura mellora  */
int num_caracteres_devolver = 1;


/**
 * Devolve a liña actual do código fonte que se está a procesar
 */
int obtener_linea_actual(){
    return num_linea;
}

/**
 * Acepta ou non un determinado lexema como operador e, en función diso, devolve 
 * a compoñente léxica correspondente, o valor ascii ou 0 (erro)
 */
int automata_operadores(char caracter)
{
    int estado = 0;
    while(1)
    {
        switch(estado)
        {
            case 0: //estado 0 inicial resultante de recibir ':', +', '-' ou '<' 
                if(caracter == ':'){
                    estado = 1;
                }
                else if(caracter == '+'){
                    estado = 2;
                }
                else if(caracter == '-'){
                    estado = 3;
                }
                else if (caracter =='<'){
                    estado = 4;
                }
                break;
                
            case 1: //estado resultante de recibir ':'
                caracter = seguinte_caracter();
                if(caracter == '='){ //':='
                    return OP_DECLARACION;
                }
                else{ //non se acepta
                    devolver(num_caracteres_devolver); //é necesario devolver o carácter actual
                    return ':'; //valor ascii
                }
                break;
            
            case 2: //estado resultante de recibir '+'
                caracter = seguinte_caracter();
                if(caracter == '='){ //'+='
                    return OP_SUMA_ASIGN;
                } //'++'
                else if(caracter == '+'){
                    return INCREMENTO;
                }
                else{ //non se acepta
                    devolver(num_caracteres_devolver); 
                    return '+'; //valor ascii
                }
                break;
                
            case 3: //estado resultante de recibir '-'
                caracter = seguinte_caracter();
                if(caracter == '='){ //'-='
                    return OP_RESTA_ASIGN;
                }
                else if(caracter == '-'){ //'--'
                    return INCREMENTO;
                }
                else{ //non se acepta
                    devolver(num_caracteres_devolver); //é necesario devolver o carácter actual
                    return '-'; //valor ascii
                }
                break;
                
            case 4: //estado resultante de recibir '<'
                caracter = seguinte_caracter();
                if(caracter == '-'){
                    return OP_CANAL;
                }
                else{ //non se acepta
                    devolver(num_caracteres_devolver); //é necesario devolver o carácter actual
                    return '<'; //valor ascii
                }
                break;
        }
    }   
}


/**
 * Acepta ou non un determinado lexema como comentario e, en función diso, devolve 
 * a compoñente léxica correspondente, o valor ascii ou 0 (erro)
 */
int automata_comentarios()
{
    //xa comezamos no estado 1, sen pasar por un estado 0 inicial, porque '/' é o único carćater que te pode levar ao autómata de comentarios
    int estado = 1;
    char caracter = seguinte_caracter();
    while (1)
    {
        switch(estado)
        {
            //estado resultante de recibir unha única '/'
            case 1:
                // se recibimos unha segunda '/', será un line comment
                if(caracter == '/'){
                    estado = 2;
                }
                //se recibimos un *, será un general comment
                else if(caracter == '*'){
                    estado = 3;
                }
                //noutro caso non será un comentario, non se acepta e devolvemos o código ascii
                else{
                    devolver(num_caracteres_devolver);//é necesario devolver o carácter actual
                    return '/';
                }
                break;

            //estado resultante de recibir '//' (line comment)    
            case 2:
                caracter = seguinte_caracter();
                //o comentario non se acepta ata que chega un salto de liña
                while (caracter != '\n'){
                    caracter = seguinte_caracter();
                }
                devolver(num_caracteres_devolver); //non se considera que \n forme parte do comentario polo que o devolvemos
                return COMENTARIO;
                break;

            //estado resultante de recibir '/*' (general comment)    
            case 3:
                caracter = seguinte_caracter();
                //para pechar un general comment, primeiro hai que recibir un '*'
                while(caracter != '*' && caracter != EOF){
                    if(caracter == '\n'){
                        num_linea++;
                    }
                    caracter = seguinte_caracter();
                }
                if (caracter == EOF){
                    lanzar_error(COMENTARIO_GENERAL_NO_CERRADO, num_linea);
                    return 0;
                }
                estado = 4;
                break;

            //estado resultante de recibir '*' de peche do general comment
            case 4:
                caracter = seguinte_caracter();
                //en caso de recibir '/', o general comment estaría pechado e por tanto aceptado
                if(caracter == '/'){
                    return COMENTARIO;
                }
                //noutro caso, o '*' formaría parte do general comment
                else{
                    if(caracter == '\n'){
                        num_linea++;
                    }
                    estado = 3;
                }
                break;
        }
    }
    
}


/**
 * Acepta ou non un determinado lexema como string e, en función diso, devolve 
 * a compoñente léxica correspondente ou 0 (erro)
 */
int automata_strings()
{
    int estado = 1; // o estado inicial xa é 1, porque o único carácter co que se pode acceder a este autómata é "
    char caracter = seguinte_caracter();
    while (1)
    {
        switch(estado)
        {
            //estado resultante de recibir unha "
            case 1:
                while (caracter != '\\' && caracter != '"' && caracter !=EOF)
                {
                    caracter = seguinte_caracter();
                }
                /*en caso de recibirse un carácter de escape ('\'), non se inclúe na cadea
                 * xa que este se emprega para escribir caracteres especiais no código fonte
                 * pero non forman parte do valor real da cadea*/
                if(caracter == '\\'){
                    estado = 2;
                }
                /*en caso de recibirse '"', consideraranse as comiñas de peche de cadea, 
                 polo que se acepta o lexema no autómata de strings*/
                else if (caracter == '"'){
                    return STRING;
                }
                /*se chega un EOF antes das '"' de peche hai un erro de cadea */
                else if(caracter == EOF){
                    lanzar_error(STRING_NO_CERRADO, num_linea);
                    return 0;
                }
                break;

            //estado resultante de recibir '\' durante a formación dunha string
            case 2:
                caracter = seguinte_caracter();
                //en caso de recibir '\' ou '"' logo de '\' a string pode ser válida
                if(caracter == '\\' || caracter == '"'){
                    estado = 3;
                }
                /*a pesar de que, tras '\', podería haber valores válidos (octais),
                 non se contemplan nesta versión de analizador léxico, polo que se 
                 considera que o lexema non se acepta*/
                else {
                    return 0;
                }
                break;

            //estado resultante de recibir '"' ou '\' logo de recibir '\' na formación dunha string    
            case 3:
                caracter = seguinte_caracter();
                if(caracter == '\\'){
                    estado = 2;
                }
                else if (caracter == '"'){
                    return STRING;
                }
                /*en caso de recibir calquera outro carácter vólvese ao estado 1 para continuar coa
                formación da string*/
                else{
                    estado = 1;
                }
                break;
        }
    }
    
}


/**
 * Acepta ou non un determinado lexema como cadea alfanumérica e, en función diso, devolve 
 * a compoñente léxica correspondente, o valor ascii ou 0 (erro)
 */
int automata_alfanum(char caracter)
{
    int estado = 0;
    while(1)
    {
        switch(estado)
        {
            //estado resultante de recibir unha letra ou '_'
            case 0:
                if(isalpha(caracter)){
                    estado = 1;
                }
                else if(caracter == '_'){
                    estado = 2;
                }
                else{
                    return 0;
                }
                break;
                
            //estado resultante de recibir unha letra    
            case 1:
                /*en caso de que o caracter sexa unha letra, aceptarase calquera
                 combinación alfanumérica e de '_' posterior*/
                caracter = seguinte_caracter();
                while (isdigit(caracter) || isalpha(caracter) || caracter == '_')
                {
                    caracter = seguinte_caracter();
                }
                devolver(num_caracteres_devolver); /*é necesario devolver o carácter que fai que se acepte a cadea*/
                return ALFANUM;
                break;
                
            //estado resultante de recibir unha '_'
            case 2:
                caracter = seguinte_caracter();
                /*en caso de que o seguinte carácter sexa un díxito ou letra, acéptase como
                 identificador, xa que go permite identificadores comezados por '_' e seguidos
                 de díxitos ou letras*/
                if(isdigit(caracter) || isalpha(caracter)){
                    caracter = seguinte_caracter();
                    while (isdigit(caracter) || isalpha(caracter) || caracter == '_')
                    {
                        caracter = seguinte_caracter();
                    }
                    return ALFANUM;
                }
                /*en caso de que o seguinte carácter sexa diferente a un díxito ou letra, non se acepta e
                 devólvese o valor ASCII*/
                else{
                    devolver(num_caracteres_devolver); //é necesario devolver o carácter actual
                    return '_';
                }
                break;
        }
    }
}

/**
 *Determina se un carácter pertence ao sistema hexadecimal (0-9, A-F, a-f)
 * 
 * @param caracter a analizar
 * @return 1: é hexadecimal; 0: non é hexadecimal
 */
int es_hexadecimal(char caracter) {
    // Verificar se o carácter está no rango '0'-'9', 'A'-'F' ou 'a'-'f'
    if (isdigit(caracter) || 
        (caracter >= 'A' && caracter <= 'F') || 
        (caracter >= 'a' && caracter <= 'f')) {
        return 1;  // Trátase dun carácter hexadecimal
    } else {
        return 0;  // Non é un carácter hexadecimal
    }
}


/**
 * Determina se un carácter pertence ao sistema octal (0-7)
 * @param caracter carácter a analizar
 * @return 1: é octal; 0: non é octal
 */
int es_octal(char caracter) {
    return (caracter >= '0' && caracter <= '7');
}

/**
 * Determina se un carácter pertence ao sistema binario (0-1)
 * @param caracter carácter a analizar
 * @return 1: é binario; 0: non é binario
 */
int es_binario(char caracter) {
    return (caracter == '0' || caracter == '1');
}


/**
 * Acepta ou non un determinado lexema como:
 *  - Integer literal (número enteiro)
 *  - Floating-point literal (número decimal con ou sen exponente, hexadecimal, octal ou binario)
 *  - Imaginary literal (número imaxinario)
 *  e, en función diso, devolve a compoñente léxica correspondente, o valor ascii ou 
 * 0 (erro)
 * 
 * NOTA: a pesar de que GO agrupa 'decimal_float_lit', 'hex_float_lit', 'binary_lit'
 * e 'octal_lit' dentro de floating-point literals, considerouse adecuado crear 
 * unha compoñente léxica para cada unha destas casuísticas
 */
int automata_numeros(char caracter){
    int estado = 0; //inicialmente, accédese ao estado común do autómata
    while (1)
    {
        switch(estado)
        {
            case 0: 
                if(isdigit(caracter) && caracter!='0'){
                    estado = 1;
                }
                else if(caracter=='-'){
                    estado = 2;
                }
                else if(caracter=='0'){ 
                    estado = 3;
                }
                else if(caracter=='.'){
                    estado = 4;
                }
                else{ //se non se trata de ningún destes carácteres, non se poderá aceptar o lexema
                    return 0;
                }
                break;

            case 1: //estado resultante de recibir un díxito distinto de 0
                caracter = seguinte_caracter();
                while(isdigit(caracter)){
                    caracter = seguinte_caracter();
                }
                if(caracter == 'e' || caracter == 'E'){
                    estado = 6;
                }
                else if(caracter == '.'){ //floating-point literal
                    estado = 4;
                }
                else{ //en caso de recibir calquera outro carácter procédese a aceptar o lexema como número enteiro (exemplo: 123)
                    devolver(num_caracteres_devolver);
                    return NUMERO_ENTERO;
                }
                break;

            case 2: //estado resultante de recibir '-'
                caracter = seguinte_caracter();
                if(isdigit(caracter)){
                    estado = 1; 
                }
                /*en caso de, tras o '-', non vir un díxito, devólvese o valor ascii*/
                else{
                    devolver(1); //é necesario devolver o carácter actual
                    return '-';
                }
                break;

            case 3: //estado resultante de recibir  0 (pode ser hexadecimal, octal ou binario)
                caracter = seguinte_caracter();
                if(caracter == 'x' || caracter == 'X'){ //hexadecimal
                    estado = 10;
                }
                else if (caracter == '.'){ //decimal
                    estado = 4;
                }
                else if (caracter == 'i'){//imaxinario
                    estado = 9;
                }
                else if(caracter == 'o' || caracter == 'O'){ //octal '0o...' ou '0O...'
                    estado = 14;
                }
                else if(es_octal(caracter)){ //octal '0[dixito_octal]' 
                    estado = 15;
                }
                else if(caracter =='_'){ //octal '0_...'
                    estado = 16;
                }
                else if(caracter == 'B' || caracter =='b'){//binario '0B' ou '0b'
                    estado = 17;
                }
                else{//noutro caso acépase o lexema como '0'
                    devolver(num_caracteres_devolver);
                    return NUMERO_ENTERO;
                }
                break;

            case 4: //estado resultante de recibir '.' tras díxitos
                caracter = seguinte_caracter();
                if(isdigit(caracter)){ 
                    estado = 5;
                }
                else if(caracter == 'e' || caracter=='E'){
                    estado = 6;
                }
                /*en caso de, tras díxitos e '.', non vir nin un díxito nin 'e'/'E'acéptase como enteiro (3. por exemplo)*/
                else{ 
                    devolver(num_caracteres_devolver); //é necesario devolver o carácter actual
                    return NUMERO_ENTERO;
                }
                break;

            case 5: //estado resultante de recibir un díxito tras un '.'
                caracter = seguinte_caracter();
                while(isdigit(caracter)){
                    caracter = seguinte_caracter();
                }
                if (caracter == 'e' || caracter=='E'){ //parte exponencial
                    estado = 6;
                }
                else if (caracter == 'i'){ //parte imaxinaria
                    estado = 9;
                }
                else{ //noutro caso, a cadea acéptase por número decimal(exemplo 12.34)
                    devolver(num_caracteres_devolver);
                    return NUMERO_DECIMAL;
                }
                break;

            case 6: //estado resultante de recibir 'e'/'E' na formación dun número con parte exponencial
                caracter = seguinte_caracter();
                if(isdigit(caracter)){
                    estado = 8;
                }
                else if (caracter == '+' || caracter == '-'){
                    estado = 7;
                }
                else{ //en caso contrario, trátase dun exponencial mal formado
                    lanzar_error(EXP_MAL_FORMADO, num_linea);
                    return 0;
                }
                break;

            case 7: //estado resultado de recibir un +/- logo dun E/e
                caracter = seguinte_caracter();
                if(isdigit(caracter)){
                    estado = 8;
                }
                else{ //en caso contrario, trátase dun exponencial mal formado
                    lanzar_error(EXP_MAL_FORMADO, num_linea);
                    return 0;
                }
                break;

            case 8: //estado resultante de recibir un díxito nun número con expoñentes (e/E)
                caracter = seguinte_caracter();
                while(isdigit(caracter)){
                    caracter = seguinte_caracter();
                }
                if(caracter == 'i') //1.e+0i
                {
                    estado = 9;
                }
                else{
                    devolver(num_caracteres_devolver);
                    return NUMERO_DECIMAL;
                }
                
                break;

            case 9: //estado resultante de recibir unha i correspondente á parte imaxinaria
                return NUMERO_IMAGINARIO;
                break;

            case 10: //estado resultante de recibir unha x após recibir un 0
                caracter = seguinte_caracter();
                /*comróbase que o carácter recibido despois da x é válido para a formación dun número
                 hexadecimal en Go, é dicir dos rangos 0-9, a-f, A-F ou unha barra baixa*/
                if(es_hexadecimal(caracter) || caracter == '_'){ 
                    estado = 11;
                }
                else{ //en caso contrario, producirase un erro por número hexadecimal mal formado
                    lanzar_error(HEX_MAL_FORMADO, num_linea);
                    devolver(num_caracteres_devolver);
                    return 0;
                }
                break;

            case 11: /*estado resultante de recibir un carácter pertencente a '0-9', 'a-f' e 'A-F'
                      na formación dun número hexadecimal*/ 
                caracter = seguinte_caracter();
                while (es_hexadecimal(caracter)){
                    caracter = seguinte_caracter();
                }
                if(caracter == '_'){
                    estado = 12;
                }
                else{ //en caso de recibir outro carácter, acéptase o lexema (0xBadFace42, por exemplo)
                    devolver(num_caracteres_devolver); //devolvemos o caracter que provocou o erro
                    return NUMERO_HEXADECIMAL;
                }
                break;

            case 12: //estado resultante de recibir '_' na formación dun número hexadecimal
                caracter = seguinte_caracter();
                if(es_hexadecimal(caracter)){
                    estado = 11;
                }
                else{ /*en go non está permitido que un número hexadecimal remate en '_', ten que estar
                       seguida dun dos caracteres permitidos para a formación de números hexadecimais en go*/
                    lanzar_error(HEX_MAL_FORMADO, num_linea);
                    devolver(num_caracteres_devolver); //devolvemos o caracter que provocou o erro
                    return 0;
                }
                break;
                
            case 13: //estado resultante de recibir '.'
                caracter = seguinte_caracter();
                if(isdigit(caracter)){
                    estado = 5;
                }
                /*non se permiten .i ou .e-11*/
                else{
                    devolver(num_caracteres_devolver);
                    return '.';
                }
                
                break;
                
            case 14: //estado resultante de recibir '0o' ou '0O'
                caracter = seguinte_caracter();
                if(es_octal(caracter)==1){
                    estado = 15;
                }
                else if(caracter == '_'){ //'0o_' ou '0O:'
                    estado = 16;
                }
                else{
                    lanzar_error(OCT_MAL_FORMADO, num_linea);
                    devolver(num_caracteres_devolver);//devolvemos o caracter que provocou o erro
                    return 0;
                }
                break;
                
            case 15: //estado resultante de recibur '0' e un díxito octal despois
                caracter = seguinte_caracter();
                while (es_octal(caracter)==1){
                    caracter=seguinte_caracter();
                }
                if(caracter=='_'){ //recíbese '_' durante a formación dun octal
                    estado = 16;
                }
                else{ //noutro caso, acéptase
                    devolver(num_caracteres_devolver);
                    return NUMERO_OCTAL;
                }
                break;
                
            case 16: //estado resultante de recibir '0_' ou '_' durante a formación dun octal
                caracter = seguinte_caracter();
                if(es_octal(caracter)){
                    estado = 15;
                }
                else{ //un octal non pode rematar en '_'
                    lanzar_error(OCT_MAL_FORMADO, num_linea); //devolvemos '_'
                    devolver(num_caracteres_devolver);
                    return 0;
                }
                break;
                
            case 17: //estado resultante de recibir '0b' ou '0B'
                caracter = seguinte_caracter();
                while(es_binario(caracter)==1){
                    caracter = seguinte_caracter();
                }
                if(caracter == '_'){
                    estado = 18;
                }
                else{
                    devolver(num_caracteres_devolver);
                    return NUMERO_BINARIO;
                }
                
            case 18: //estado resultante de recibir '_' durante a formación dun número binario
                caracter = seguinte_caracter();
                if(es_binario(caracter)){
                    estado = 17;
                }
                else{ //un binario non pode rematar en '_'
                    lanzar_error(BIN_MAL_FORMADO, num_linea);
                    devolver(num_caracteres_devolver);
                    return 0;
                }
        }

    }
       
}

/**
 * Devolve 1 o 0 en función de se o compilador debe inserir automaticamente un ';' ao final
 * da liña, en función da última compoñente léxica desa mesma liña de código:
 *  - 0: non inseire o ';'
 *  - 1: insire o ';'
 */
int insertar_semicolon()
{
    if((ultimo_comp_lexico == ID || ultimo_comp_lexico == NUMERO_ENTERO || ultimo_comp_lexico == NUMERO_DECIMAL || ultimo_comp_lexico == NUMERO_IMAGINARIO
            || ultimo_comp_lexico == STRING || ultimo_comp_lexico == BREAK || ultimo_comp_lexico == CONTINUE || ultimo_comp_lexico == FALLTHROUGH 
            || ultimo_comp_lexico == RETURN || ultimo_comp_lexico == INCREMENTO || ultimo_comp_lexico == DECREMENTO || ultimo_comp_lexico == ')' 
            || ultimo_comp_lexico == ']' || ultimo_comp_lexico == '}') && (linea_ultimo_comp_lex == num_linea) ){
        return 1;
    }
    return 0;
}


/**
 * Función principal que contén a lóxica de chamada aos diferentes autómatas en función dos carácteres recibidos, así como da actualiazción de variables
 * fundamentais, a formación da estrutura (lexema, compoñente léxica) a devolver ao analizador sintáctico, chamada ás funcións que permiten a búsqueda
 * das cadeas alfanuméricas na táboa de símbolos e outras tarefas menores
 * @return seguinte compoñente léxica do código fonte 
 */
TIPOELEMENTOABB seguinte_comp_lexico()
{
    TIPOELEMENTOABB elemento;
    char caracter;
    int estado = 0;
    
    /*Permite a saída do bucle while cando hai unha compoñente léxica que entregar ao analizador sintáctico
        - 0: aínda non se obtivo unha compoñente léxica para entregar ao analizador sintáctico
        - 1: obtívose unha compoñente léxica, polo que finaliza a función*/
    int lexema_aceptado = 0; 
    
    /*Permite o almacenamento da compoñente léxica devolta polos autómatas, que permite saber se o lexema debe ser lido ou saltado:
        - 0: erro nalgún dos autómatas, polo que se salta o lexema
        - !0: valor da compoñente léxica devolta polo autómata */
    int comp_lexico = 0;
    
    //executamos o bucle ata que se acepte un lexema, é dicir, ata poder devolver unha dupla (lexema, compoñente léxica) ao analizador sintáctico
    while (lexema_aceptado == 0){ 
        caracter = seguinte_caracter();
        //unha das tarefas do analizador léxico é eliminar aqueles carácteres do código fonte sen significado gramatical
        if(caracter==' ' || caracter=='\t'){
            comp_lexico = 0;
        }  
        else{
            //en caso de tratarse do carácter de fin de liña, compróbase se é necesario que o compilador de go engada automaticamente un ';'
            /*NOTA: tan só se insire automaticamente ';' se algún dos lexemas da liña do código fonte é aceptado, polo que non inclúe 
             * aqueles lexemas que exceden o tamaño máximo permitido.*/
            if (caracter == '\n')
            {
                estado = 0;
            }
            //en caso de que sexa un díxito ou un . (decimais .12) chámase ao autómata de números
            else if(isdigit(caracter) ||caracter=='.')
            {
                estado=1;
            }
            // en caso de que sexa '-', poderá ser un número negativo ou un operador 
            else if(caracter == '-'){ 
                caracter = seguinte_caracter();
                if(isdigit(caracter)){
                    estado = 1; //autómata de números
                }
                else{
                    estado = 5; //autómata operadores
                }
                devolver(num_caracteres_devolver);
                caracter = '-'; //para enviar '-' aos autómatas
            }
            //en caso de que sexa unha letra (A-Z ou a-z), chámase ao autómata alfanumérico de identificadores
            else if(isalpha(caracter) || caracter=='_')
            {
                estado = 2;
            }
            //en caso de que sexa '"', chámase ao autómata de strings
            else if (caracter == '"')
            {
                estado = 3;
            }
            //en caso de que sexa '/', chámase ao autómata de comentarios
            else if(caracter == '/')
            {
                estado = 4;
            }
            //en caso de recibir algún dos seguintes carácteres, chámase ao autómata de operadores
            else if(caracter==':' || caracter=='+' || caracter=='<')
            {
                estado = 5;
            }
            //en caso de non tratarse de ningún destes carácteres, devólvese o valor ascii
            else
            {
                estado = 6;
            }

            switch(estado)
            {
                //case 0 para '\n'
                case 0:
                    if(insertar_semicolon() == 1){
                        /*en caso de que sexa necesario que o compilador insira automaticamente ';', entón devólvese a compoñente léxica correspondente,
                         deixando a NULL o lexema para que se interprete que este foi engadido manualmente polo compilador e non aparece de forma explícita
                         no código fonte*/
                        elemento.componente_lexico = ';';
                        elemento.lexema = NULL; //faise que o lexema sexa NULL, para así entender que foi engadido automaticamente polo compilador de go
                        lexema_aceptado = 1;
                    }
                    num_linea++;
                    comp_lexico = 0; //é necesario para que non se acepte o lexema '\n' e decida saltarse no 'else' de despois do switch
                    //case 1 para números
                case 1:
                    /*pasámoslle o caracter ao autómata de números para que poida decidir a que estado
                     * pasar en función de se se trata dun díxito, un '-' ou un '.'*/
                    comp_lexico = automata_numeros(caracter);
                    break;

                //case 2 para cadeas alfanuméricas    
                case 2: 
                    /*non é necesario enviar o carácter xa que todas as cadeas alfanuméricas comezan por 
                     letra ou '_' pero parten do mesmo estado sen importar cal de estas sexa o primeiro
                     carácter*/
                    comp_lexico = automata_alfanum(caracter); 
                    break;

                //case 3 para cadeas de carácteres strings (string literals en go)
                case 3:
                    comp_lexico = automata_strings();
                    break;

                //case 4 para comentarios
                case 4:
                    /*non é necesario enviar o carácter xa que todos os comentarios comezan por '/'*/
                    comp_lexico = automata_comentarios(); 
                    break;

                //case 5 para operadores (:=, +=, -=, <-)    
                case 5:
                    comp_lexico = automata_operadores(caracter);
                    break;

                //case 6 para calquera outro carácter
                case 6:
                    comp_lexico = caracter; //a compoñente léxica será o valor ascii do carácter
                    break;
                    
                default:
                    break;
            }
            
        }
        
        /*en caso de que sexa distinto de 0, quererá decir que o lexema se aceptou*/
        if(comp_lexico != 0 && caracter!=EOF){
            aceptar_lexema(&elemento);
            if(elemento.lexema != NULL){ //indicativo de que non se produciu o erro de tamaño máximo de lexema permitido
                lexema_aceptado = 1;
                if(comp_lexico == ALFANUM){
                    comp_lexico = buscarTS(elemento.lexema);  //buscamos na TS se está e se non insertamos 
                }
                elemento.componente_lexico = comp_lexico;

                ultimo_comp_lexico = comp_lexico;
                linea_ultimo_comp_lex = num_linea;
            }
            else{ //excédese o tamaño máximo do lexema
                saltar_lexema();
            }
            
        }
        else if (caracter == EOF){
            if(insertar_semicolon() == 1 && last_semicolon==0){ //compróbase se é necesario engadir ';' antes de 'EOF'
                        /*en caso de que sexa necesario que o compilador insira automaticamente ';', entón devólvese a compoñente léxica correspondente,
                         deixando a NULL o lexema para que se interprete que este foi engadido manualmente polo compilador e non aparece de forma explícita
                         no código fonte*/
                        elemento.componente_lexico = ';';
                        elemento.lexema = NULL; //faise que o lexema sexa NULL, para así entender que foi engadido automaticamente polo compilador de go
                        lexema_aceptado = 1;
                        last_semicolon=1; //actualízase a variable de modo que, cando se volva recibir 'EOF' se envíe a súa compoñente léxica ao analizador sintáctico
                        devolver(num_caracteres_devolver); //devólvese 'eof'
                    }
            else{
                elemento.componente_lexico = EOF;
                elemento.lexema = NULL;
                lexema_aceptado = 1;
            }
        }
        else{ //se o lexema non se acepta ou se produce un erro, sáltase o lexema
            saltar_lexema();
        }
    }	
    return elemento;
}
 
