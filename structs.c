#include <stdio.h>
#include <stdlib.h>

struct entrada_arquivo{
    char status;
    char nome[12];
    char ext[4];
    char tipo;
    unsigned int primeiro_bloco;
    unsigned int tamanho;
    unsigned int numero_blocos_usados;
};

struct boot_record{
    unsigned short bytes_por_bloco;          //2 bytes
    unsigned short blocos_reservados;        //2 bytes
    unsigned int num_blocos_livres;          //4 bytes
    unsigned int num_blocos_tabela_entradas; //...
    unsigned int num_blocos_secao_dados;
    unsigned int num_blocos_totais;
    unsigned int num_blocos_reservados_raiz;
    unsigned int quant_entradas_sistema;
    int padding;                             //4 bytes para completar
};

struct entrada_diretorio{
    char status;   //apenas um byte para status
    char nome[12];
    char ext[4];
    char tipo;
    unsigned int primeiro_bloco;
    unsigned int tamanho;
    unsigned int numero_blocos_usados;
    unsigned short padding;
};


struct Bloco{
    struct Bloco *prox;
	char conteudo[508];
};



typedef struct Bloco bloco;
typedef bloco* LISTA; //aponta para o inicio da lista;