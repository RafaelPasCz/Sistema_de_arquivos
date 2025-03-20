#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


struct Boot_record{
    unsigned short bytes_por_bloco;          //2 bytes
    unsigned short blocos_reservados;        //2 bytes
    unsigned int num_blocos_livres;          //4 bytes
    unsigned int num_blocos_tabela_entradas; //...
    unsigned int num_blocos_secao_dados;
    unsigned int num_blocos_totais;
    unsigned int num_blocos_reservados_raiz;
    uint32_t cabeca_lista;
    unsigned int quant_entradas_sistema;
};

struct Entrada{
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
    char* conteudo;
};
typedef struct Boot_record boot_record;
typedef struct Entrada entrada;
typedef struct Bloco bloco;
