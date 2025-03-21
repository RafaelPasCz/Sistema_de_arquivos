#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


struct Boot_record{
    unsigned short bytes_por_bloco;             // 2 bytes
    unsigned short blocos_reservados;           // 2 bytes
    unsigned int num_blocos_livres;             // 4 bytes      O que que é isso? cabeca_lista deve ir aqui
    unsigned int num_blocos_tabela_entradas;    // 4 bytes
    unsigned int num_blocos_secao_dados;        // 4 bytes
    unsigned int num_blocos_totais;             // 4 bytes
    unsigned int num_blocos_reservados_raiz;    // 4 bytes
    uint32_t cabeca_lista;                      // 4 bytes      Não teria que usar uint32_t em tudo?            
    unsigned int quant_entradas_sistema;        // 4 bytes, 
};


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


struct Entrada{
    char status;                                // 1 byte para status
    char nome[12];                              // 12 bytes, nome do arquivo
    char ext[4];                                // 4 bytes, extensão
    char tipo;
    unsigned int primeiro_bloco;
    unsigned int tamanho;
    unsigned int numero_blocos_usados;
    unsigned short padding;
};


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


struct Bloco{
    char* conteudo;
};

typedef struct Boot_record boot_record;
typedef struct Entrada entrada;
typedef struct Bloco bloco;
