#include <stdio.h>
#include <stdlib.h>
#include "sistema_entrada.h"
#include "errores.h"
#include "analisis_lexico.h"
#include "ABB.h"

//buffers ou bloques globais
char bloque_A[MAX_BUFFER];
char bloque_B[MAX_BUFFER];

/*Indica en que bloque se atopa en cada momento o punteiro inicio:
    - 0: atópase no bloque B
    - 1: atópase no bloque A*/
int inicio_en_bloqueA = 1;

/*Indica en que bloque se atopa en cada momento o punteiro delantero:
    - 0: atópase no bloque B
    - 1: atópase no bloque A*/
int delantero_en_bloqueA = 1;

/*Indica se o lexema cuxos carácteres están sendo lidos excede o tamaño máximo
 permitido:
 *  - 0: non excede o tamaño máximo permitido
 *  - 1: excede o tamaño máximo permitido */
int tam_lex_excedido = 0;

FILE *fichero = NULL; //Punteiro ao ficheiro

/*Punteiros necesarios para a implementación do dobre centinela*/
char *inicio = NULL;
char *delantero = NULL;

/*Variables necesarias para, antes de cargar un novo bloque, saber se ese foi lido completamente. Necesarias para, por exemplo, en caso de que a función devolver() 
 devolva varios carácteres e delantero retroceda ao bloque anterior, non cargue o novo bloque porque este aínda non foi lido*/
int bloque_A_leido = 0;
int bloque_B_leido = 1; //inicialízase con valor 1 para que na primeira iteración se cargue o bloque B aínda que este non fose lido todavía, xa que está vacío

/*Indica o número de liña no que se produciu o erro "Tamaño lexema excedido"*/
int num_linea_excedido = 0;

/**
 *Función que devolve se delantero se atopa na última posición (eof) do bloque A 
 * @return 1: delantero en eof do bloque A, 0: delantero non en EOF do bloque A
 */
int eof_en_bloqueA() {
    return (delantero == &bloque_A[MAX_BUFFER-1]);
}

/**
 *Función que devolve se delantero se atopa na última posición (eof) do bloque B 
 * @return 1: delantero en eof do bloque B, 0: delantero non en EOF do bloque B
 */
int eof_en_bloqueB() {
    return (delantero == &bloque_B[MAX_BUFFER-1]);
}


/**
 * Carga de carácteres do código fonte o bloque especificado, excepto a última posición, na que se atopa
 * o carácter centinela (EOF)
 * @param bloque bloque que imos cargar con carácteres do código fonte
 */
void cargar_bloque(char *bloque){
    if (fichero == NULL){
        perror("Error ao ler o ficheiro");
        return;
    }
    /*cargamos todas as posicións do bloque excepto a última, que será sempre EOF*/
    for (int i = 0; i<(MAX_BUFFER-1); i++){
        bloque[i] = fgetc(fichero);
        if(bloque[i]==EOF){ //en caso de atopar o carácter de fin de ficheiro, deixamos de cargar
            break;
        }
    }
}


/**
 * Inicializa o sistema de entrada, enchendo de carácteres o bloque A e incluíndo os carácteres
 * centinela (EOF) na última posición de cada un dos dous bloques
 * @param f punteiro ao ficheiro en modo lectura que contén o código fonte
 */
void inicializar_sist_entr(FILE *f){
    fichero = f; 
            
    cargar_bloque(bloque_A); //na inicialización tan só se carga o bloque A
    
    inicio = bloque_A; //inicialmente, inicio e delantero apuntarán á primeira posición do bloque A
    delantero = bloque_A;
    
    bloque_A[MAX_BUFFER-1] = EOF; //incluímos os carácteres centinela na última posición dos bloques
    bloque_B[MAX_BUFFER-1] = EOF;

}


/**
 * Axusta o punteiro 'delantero' cando un carácter é devolto polo analizador léxico
 * @param num_caracteres
 */
void devolver(int num_caracteres){
    for(int i = 0; i<num_caracteres; i++){
        if(delantero == &bloque_A[0]){ //en caso de estar na primeira posición do bloque A, poñémolo na última (sen contar EOF) do bloque B
            delantero = &bloque_B[MAX_BUFFER-2];
            delantero_en_bloqueA = 0;
        }
        else if(delantero == &bloque_B[0]){ //en caso de estar na primeira posición do bloque A, poñémolo na última (sen contar EOF) do bloque B
            delantero = &bloque_A[MAX_BUFFER-2];
            delantero_en_bloqueA = 0;
        }
        else{
            delantero--;
        }
    }
}


/**
 * Devolve o tamaño do lexema que se atopa entre os punteiros delantero e inicio, tendo en conta as 
 tres posibilidades que se poden dar en canto á localización de inicio e delantero
 * @return tamaño do lexema
 */
int calcular_tamano_lexema(){
    //printf("BLOQUES\n%d\t%d\n", inicio_en_bloqueA,delantero_en_bloqueA);
    /*se inicio e delantero están no mesmo bloque*/
    if(inicio_en_bloqueA == delantero_en_bloqueA){
        return delantero - inicio; //non sei se -1 dependendo de en que momento se mova delantero
    }
    /*se inicio está no bloque A e delantero no bloque B*/
    else if (inicio_en_bloqueA==1 && delantero_en_bloqueA==0){
        return ((&bloque_A[MAX_BUFFER-1]-inicio)+(delantero-&bloque_B[0])); //de novo mirar o de -1/-2 en función de cando aumentemos delantero
    }
    /*se inicio está no bloque B e delantero no bloque A*/
    else if (inicio_en_bloqueA==0 && delantero_en_bloqueA==1){
        return ((&bloque_B[MAX_BUFFER-1]-inicio)+(delantero-&bloque_A[0]));
    }
    return -1;
}



/**
 * Le o lexema existente entre os punteiros "inicio" e delantero, gardando carácter a carácter
 * @param lexema punteiro lexema no que se van gardando cada un dos carácteres atopados
 */
void leer_lexema(char *lexema){
    int indice = 0;
    /*imos tomando carácter a carácter ata que inicio chegue á posición de fin*/
    while(inicio!=delantero){
        /*en caso de que se atopen no mesmo bloque, simplemente imos incrementando inicio*/
        if(inicio_en_bloqueA == delantero_en_bloqueA){
            lexema[indice] = *inicio;
            inicio++;
            indice++;
        }
        /*en caso de que inicio estea no bloque A e delantero no bloque B, incrementamos
         inicio ata chegar a EOF do bloque A, momento no que situamos inicio no bloque B*/
        else if (inicio_en_bloqueA==1 && delantero_en_bloqueA==0){
            while (inicio != &bloque_A[MAX_BUFFER-1]){
                lexema[indice] = *inicio;
                inicio++;
                indice++;
            }
            bloque_A_leido = 1; //no momento no que inicio sae da última posición do bloque A é cando se pode considerar lido o bloque A
            inicio = bloque_B;
            inicio_en_bloqueA = 0;
        }
        /*en caso de que inicio estea no bloque B e delantero no bloque A, incrementamos
         inicio ata chegar a EOF do bloque B, momento no que situamos inicio no bloque A*/
        else if (inicio_en_bloqueA==0 && delantero_en_bloqueA==1){
            while (inicio != &bloque_B[MAX_BUFFER-1]){
                lexema[indice] = *inicio;
                inicio++;
                indice++;
            }
            bloque_B_leido = 1; //no momento no que inicio sae da última posición do bloque B é cando se pode considerar lido o bloque B
            inicio = bloque_A;
            inicio_en_bloqueA = 1;
        }
    }
}

/**
 * Sitúa o punteiro inicio na posición do bloque na que se atope delantero, xa que, como non
 * se acepta o lexema, se produce un erro ou son carácteres do código fonte sen significado gramatical, 
 * simplemente se ignoran os carácteres existentes entre ambos punteiros
 */
void saltar_lexema(){
    if(inicio_en_bloqueA == delantero_en_bloqueA){ //en caso de atoparse no mesmo bloque, faise que inicio apunte a delantero
        inicio = delantero;
    }
    else if(inicio_en_bloqueA == 1 && delantero_en_bloqueA==0){ //en caso de estar en bloques diferentes, hai que axustar certas variables 
        inicio = delantero;
        inicio_en_bloqueA = delantero_en_bloqueA;
        bloque_A_leido = 1;
    }
    else if(inicio_en_bloqueA == 0 && delantero_en_bloqueA==1){
        inicio = delantero;
        inicio_en_bloqueA = delantero_en_bloqueA;
        bloque_B_leido = 1;
    }
    
}


/**
 * Garda no campo 'lexema' da variable 'elem' o lexema que se atopa entre os punteiros inicio e delantero
 * Tamén comproba se se excedeu o tamaño máximo de lexema permitido, gardando NULL no campo 'lexema' nese caso
 * e emitindo un erro
 * @param elem variable de tipo TIPOELEMENTOABB cuxo campo 'lexema' vai ser modificado
 */
void aceptar_lexema(TIPOELEMENTOABB *elem){
    int len;
    /*lerase o lexema tan só en caso de que este non exceda o tamaño máximo permitido*/
    if(tam_lex_excedido == 0){
        len = calcular_tamano_lexema(); //calculamos o tamaño do lexema aceptado en función das posicións entre delantero e inicio
        if(len < 0){
            lanzar_error(PUNTEROS_SIST_ENTRADA, 0); //envíase '0' como número de liña xa que non se trata dun erro concreto no código fonte senón de xestión da memoria intermedia
            exit(1);
        }
        elem->lexema = (char *)malloc((len+1) * sizeof(char)); //reservamos memoria dinámica
        if (elem->lexema == NULL) { // Comprobamos se a memoria foi asignada correctamente
            printf("Erro ao asignar memoria!\n");
            exit(1);  
        }
        leer_lexema(elem->lexema);
        elem->lexema[len] = '\0'; //engadimos o carácter de fin de cadea para evitar posíbeis erros
    }
    /*en caso de que xa se detectase que o lexema actual excedía o tamaño máximo permitido,
     * igualámolo a null para que o analizador léxico saiba que este non debe ser enviado ao 
     * analizador sintáctico para a súa impresión */
    else{
        elem->lexema = NULL;
        lanzar_error(EXCEDE_TAMANHO_LEXEMA, num_linea_excedido); //lanzamos o erro
        tam_lex_excedido = 0; //actualizamos o valor da variable unha vez superado o lexema cuxo tamaño se excedía do permitido
    }
}


/**
 * Devolve o seguinte carácter do código fonte, tendo en conta a dobre centinela empregada
 * para almacenar no sistema de entrada. Tamén detecta se un lexema excede o tamaño máximo
 * permitido, modificando as variables necesarias e realiza os axustes dos punteiros que 
 * sexan requiridos.
 * @return o seguinte carácter do código fonte
 */
char seguinte_caracter(){
    //en caso de ler EOF, este pode ser de fin de bloque A, fin de bloque B ou fin de ficheiro
    if(*delantero == EOF){
        /*en caso de que sexa eof de fin de bloque A, cargaremos o bloque B, axustaremos 
         os punteiros e o valor da variable de localización de delantero*/
        if (eof_en_bloqueA()){
            if(bloque_B_leido == 1){ //tan só se cargará o bloque B se este xa foi lido completamente
                /*a condición para detectar que un lexema superou o tamaño máximo permitido é que
                se leve recarga dun bloque sen que este aínda non fose procesado, é dicir, que
                'delantero' trate de recargar un bloque no que se atopa 'inicio'. 
                Neste casp, actualizamos o valor da variable 'tam_lex_excedido' para que a función
                aceptar_lexema() 'saiba' que o lexema non debe ser lido*/
               if (inicio_en_bloqueA==0){
                   if(tam_lex_excedido==0){ //condición necesaria para que, se un lexema excede máis de unha vez o tamaño permitido, se manteña o número de liña na que se produciu o primeiro exceso
                       num_linea_excedido = obtener_linea_actual(); //gárdase a liña na que se produciu o erro
                   }
                   tam_lex_excedido = 1;
                   inicio = bloque_B; //óptase por poñer 'inicio' na primeira posición do novo bloque
               }
               cargar_bloque(bloque_B);
               delantero = bloque_B;
               delantero_en_bloqueA = 0;  
            }
            else{
                delantero = bloque_B;
                delantero_en_bloqueA = 0;
            }
        }
        /*en caso de que sexa eof de fin de bloque B, cargaremos o bloque A, axustaremos 
         os punteiros e o valor va variable de localización de delantero*/
        else if(eof_en_bloqueB()){
            if(bloque_A_leido == 1){ //tan só se cargará o bloque A se este xa foi lido completamente
                if(inicio_en_bloqueA==1){ //o mesmo que o explicado no condicional anterior
                    if(tam_lex_excedido == 0){ //condición necesaria para que, se un lexema excede máis de unha vez o tamaño permitido, se manteña o número de liña na que se produciu o primeiro exceso
                        num_linea_excedido = obtener_linea_actual(); //gárdase a liña na que se produciu o erro
                    }
                    tam_lex_excedido = 1;
                    inicio =  bloque_A;
                }
                cargar_bloque(bloque_A);
                delantero = bloque_A;
                delantero_en_bloqueA = 1;
            }
            else{
                delantero = bloque_A;
                delantero_en_bloqueA = 1;
            }
        }
            
    }
    
    return *delantero++;
}