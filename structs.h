// Structs, variáveis globais, e bibliotecas compartilhadas entre todos os headers

#ifndef HEADER_FILE
#define HEADER_FILE
#define BLOCK_SIZE 512

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


char nome_arquivo[256];             // Arquivo a ser usado para simular o sistema de arquivos


struct Boot_record{
    unsigned short bytes_por_bloco;             // 2 bytes
    unsigned short blocos_reservados;           // 2 bytes
    unsigned int num_blocos_livres;             // 4 bytes      O que que é isso? cabeca_lista deve ir aqui
    unsigned int num_blocos_tabela_entradas;    // 4 bytes          Não sei, acho que inventei sem querer, achei que esta no boot record
    unsigned int num_blocos_secao_dados;        // 4 bytes
    unsigned int num_blocos_totais;             // 4 bytes
    unsigned int num_blocos_reservados_raiz;    // 4 bytes
    uint32_t cabeca_lista;                      // 4 bytes      Não teria que usar uint32_t em tudo?
    unsigned int quant_entradas_sistema;        // 4 bytes,         tentei fazer isso, mas tava dando bosta na hora de fazer aritmetica com os valores
} __attribute__((packed));


struct Entrada{
    unsigned char status;                       // 1 byte para status
    char nome[12];                              // 12 bytes, nome do arquivo
    char ext[4];                                // 4 bytes, extensão
    char tipo;
    unsigned int primeiro_bloco;
    unsigned int tamanho;
    unsigned int numero_blocos_usados;
    unsigned short padding;
} __attribute__((packed));


struct Bloco{
    char conteudo[BLOCK_SIZE];
} __attribute__((packed));

typedef struct Boot_record boot_record;
typedef struct Entrada entrada;
typedef struct Bloco bloco;


boot_record br_sistema;             // boot record do sistema de arquivos em memória
entrada *entrada_sistema = NULL;    // Lista que armazena a tabela de entradas do sistema de arquivos em memória (alocado com malloc)
bloco   *dados_sistema = NULL;      // Lista que armazena os blocos da seção de dados do sistema de arquivos em memória


/*
    O boot recor seguindo a fio a especificação do gerente seria assim:

struct Boot_record{
    uint16_t bytes_por_bloco;                   // 2 bytes
    uint16_t blocos_reservados;                 // 2 bytes
    uint32_t cabeca_lista;                      // 4 bytes
    uint32_t num_blocos_tabela;                 // 4 bytes
    uint32_t num_blocos_secao_dados;            // 4 bytes
    uint32_t num_blocos_totais;                 // 4 bytes, numero de blocos totais do sistema
    uint32_t num_blocos_reservados_raiz;        // 4 bytes
    uint32_t quant_entradas_sistema;            // 4 bytes
    uint32_t foo;                               // 4 bytes, completa os 32 bytes do boot record
};
*/



/*
    Entradas da tabela, só que usando uint_t:

struct Boot_record{
    uint8_t status;                             // 1 byte
    char nome[12];                              // 12 bytes, nome do arquivo
    char ext[4];                                // 4 bytes, extensão
    uint8_t tipo;                               // 1 byte, tipo.    Por que, apesar de nao termos subdiretórios? facilita futuras expansões
    uint32_t primeiro_bloco                     // 4 bytes, Ponteiro para o primiro bloco
    uint32_t tamanho                            // 4 bytes, tamanho do arquivo, em bytes
    uint32_t numero_blocos_usados;              // 4 bytes, quantos blocos sequenciais são usados
    uint16_t foo;                               // 2 bytes, completa 32 bytes
};
*/



#endif