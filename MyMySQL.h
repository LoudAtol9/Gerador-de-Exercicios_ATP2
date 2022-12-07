#ifndef MYMYSQL_H_
#define MYMYSQL_H_


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TYPE_INT '#'
#define TYPE_FLOAT '$'
#define TYPE_CHAR '*'
#define END_OF_OBJ ';'


// Structs
typedef struct variavel
{
    void* ptr_var;
    int len;
    int tipo;

} VAR;

typedef struct variavel_int
{
    int* ptr_var;
    int len;

} VAR_INT;

typedef struct variavel_float
{
    float* ptr_var;
    int len;

} VAR_FLOAT;

typedef struct variavel_char
{
    char* ptr_var;
    int len;

} VAR_CHAR;


/*
** Struct que possuiu lista de ponteiros de
** (int) , (float) , (char)
** n_count indica qnts variáveis estao salvas nas listas
*/
typedef struct pacote
{
    int int_count;
    int float_count;
    int char_count;

    VAR_INT* int_lst[10];
    VAR_FLOAT* float_lst[10];
    VAR_CHAR* char_lst[10];

} PACOTE;


/*
** Cabeçalhos das funções implementadas no módulo MyMySQL.c ,
** que servem para realizar criar e salvar no Banco de Dados
*/




// ------- Inicialização -------

/*
** Recebe o nome do arquivo e abre em binário,
** ou cria se não existir 
*/
FILE* abrir_arq(char* file_nome);

/*
** Cria o index do DB, 
** Recebe o número max de classes que esse DB terá,
** Só pode ser executado UMA VEZ NO COMEÇO
*/
void inicia_db(FILE* file_ptr, int qnts_cls);

/*
** Cria uma struct do tipo PACOTE
** Retorna o ponteiro alocado no Heap
*/
PACOTE* criador_PACOTE();





// ------- Leitura do DB -------

/*
** Busca e salva no Heap a variavel requisitada do DB, 
** Recebe o nome da classe, do objeto e da variavel como (char*)
** Retorna o index da lista do pacote correspondente ao tipo
*/
int busca_var_por_nome(PACOTE* pact, FILE* file_ptr, char* nome_cls, char* nome_obj, char* nome_var);

/*
** Busca e salva no Heap a variavel requisitada do DB, 
** Recebe o id (int) da classe, do objeto e o nome da variavel em
** Retorna o index da lista do pacote correspondente ao tipo
*/
int busca_var_por_ID(PACOTE* pact, FILE* file_ptr, int id_cls, int id_obj, char* nome_var);




// ------- Editar o DB -------

/*
** Altera o valor requisitado no db, 
** Recebe o ponteiro do valor e seu tamanho
** Caso seja uma str dê o tamanho um menor
*/
int editar_var_por_nome(FILE* file_ptr, char* nome_cls, char* nome_obj, char* nome_var, void* var, int tam_var);

/*
** Altera o valor requisitado no db, 
** Recebe o ponteiro do valor e seu tamanho
** Caso seja uma str dê o tamanho um menor
** NAO USE COM ID OBJETO = 0
*/
int editar_var_por_ID(FILE* file_ptr, int id_cls, int id_obj, char* nome_var, void* var, int tam_var);




// ------- Criar no DB -------

/*
** Cria uma nova classe no db 
** Recebe uma lista (e seu tamanho "qnt") com :
* -->   nomes das variaveis(char**)
* -->   tipos das variaveis(int*)
* -->   tamanho das variaveis(int*)
*/
void criar_classe(FILE* file_ptr, char* nome_cls, int qnt_obj, char** nomes_var, int* tipos_var, int* tam_var, int qnt);

/*
** Cria um novo objeto de uma classe db 
** Recebe o nome da classe e do novo objeto :
** Os valores do Objeto precisam ser modificados dps
*/
void criar_objeto(FILE* file_ptr, char* nome_cls, char* nome_obj);




// ------- Interface (UX) -------

/*
** Imprime todos os valores dos 
** Objetos dentro de uma classe
*/
void print_cls_db(FILE* file_ptr, char* nome_cls);

// Fazer da Busca, Editar e Criar




// ------- "Tradutores" -------

/*
** Trasnforma uma str em inteiro
** Recebe a str e seu tamanho
*/
int char_2_int(char* str, int len);

/*
** Trasnforma uma str em float
** Recebe a str e seu tamanho
*/
float char_2_float(char* str, int len);

#endif // MYMYSQL_H_
