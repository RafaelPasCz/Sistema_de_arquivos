#include "structs.h"


void deletar_arquivo(int index) {
    // Função responsável por deletar um arquivo do sistema de arquivos.
    // Recebe o índice do arquivo a ser deletado, e a função marca a entrada como excluída
    // e libera os blocos usados pelo arquivo.

    // Verifica se o índice é válido e se a entrada está em uso
    if (index < 0 || index >=  br_sistema.num_blocos_tabela_entradas * 16) {
        printf("\nÍndice inválido!\n");
        return;
    }

    if (entrada_sistema[index].status == 0x00 || entrada_sistema[index].status == 0xE5) {
        printf("\nNenhum arquivo encontrado no índice especificado.\n");
        return;
    }

    // Marcamos a entrada como excluída
    entrada_sistema[index].status = 0xE5;
    printf("\nArquivo no índice %d marcado como excluído.\n", index);


    // Libera os blocos usados pelo arquivo
    uint32_t bloco_inicio_deletado = entrada_sistema[index].primeiro_bloco;
    uint32_t numero_blocos_deletado = entrada_sistema[index].numero_blocos_usados;

    // Agora, o que dá pra fazer:
    //  Percorrer a lista de blocos livres
    //  Ter em mãos o primeiro bloco do arquivo deletado
    //  Quando fizermos um "pulo" que for passar do primeiro bloco, saimos do loop
    //  Pegamos o bloco_livre_anterior, fazemos ele apontar pro primeiro bloco
    //  Primeiro bloco aponta pro segundo, terceiro, etc
    //  Último aponta para onde o bloco_livre_anterior apontava.
    



    uint32_t cabeca_lista = br_sistema.cabeca_lista;        // Primeiro bloco livre
    int offset = 1 + br_sistema.num_blocos_tabela_entradas; // Offset a ser utilizado quando formos acessar o vetor dados_sistema[]

    uint32_t ptr_bloco = cabeca_lista;                      // Armazena o bloco sendo analizado atualmente
    uint32_t ptr_inicio_atual = cabeca_lista;               // Armazena o começo do conjunto contíguo de blocos atual 
    uint32_t ptr_fim_anterior = cabeca_lista;               // Armazena o final do conjunto contíguo de blocos anterior
    uint32_t ptr = 0;                                       // Usado em comparações


    for (int i = 0; i < 4; i++) {           // Pegamos o ponteiro armazenado pela cabeça da lista
        ptr |= ((uint32_t)(uint8_t)dados_sistema[ptr_bloco - offset].conteudo[i]) << (i * 8);
    }


    // Agora, caminhamos pela lista de blocos livres
    while(ptr_bloco != 0xFFFFFFFF){         // Percorremos a lista até chegar no último bloco livre.
        // Primeiro vemos se nós já encontramos uma sequência de blocos contíguos grande o suficiente.
        if(ptr > ){
            break;                          // Encontramos nossa sequência de blocos contíguos! Saímos do loop
        }
        // Pegamos os 4 primeiros bytes do bloco atual, para obter o ponteiro para o próximo bloco
        ptr = 0;
        for (int i = 0; i < 4; i++) {
            ptr |= ((uint32_t)(uint8_t)dados_sistema[ptr_bloco - offset].conteudo[i]) << (i * 8);
        }

        // Seguimos para o próximo bloco
        if(ptr == (ptr_bloco + 1)){         // Se o próximo bloco livre está grudado no bloco atual...
            ptr_bloco = ptr;                // Avançamos para o próximo bloco
            sequencia = sequencia + 1;      // Aumentamos em 1 a sequência
        
        }else{                              // Se não...
            ptr_fim_anterior = ptr_bloco;   // Armazenamos o final do conjunto contíguo de blocos anterior
            ptr_inicio_atual = ptr;         // Armazenamos o começo da próxima sequência de blocos contígua
            ptr_bloco  = ptr;               // Avançaos para o próximo bloco
            sequencia  = 1;                 // Reiniciamos o contador de sequência
        }
        printf("\nCabeca: %i, sequencia: %i, ptr: %i, ptr_bloco: %i", cabeca_lista, sequencia, ptr, ptr_bloco);
    }























    // Atualiza o número de blocos livres no boot record
    br_sistema.num_blocos_livres += entrada_sistema[index].numero_blocos_usados;

    // Limpa os dados da entrada na tabela de entradas
    // memset(&entrada_sistema[index], 0, sizeof(entrada));

    printf("Blocos liberados e lista de blocos livres atualizada.\n");
}