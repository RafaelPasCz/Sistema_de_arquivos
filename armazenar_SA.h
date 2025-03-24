// Funções envolvidas no processo de pegar um sistema de arquivos armazenado na memória, e escrevê-lo no HD

#include "structs.h"

int salvar_sistema_arquivos(){
    // Função que pega as estruturas de dados do sistema de arquivos, e armazena elas no disco

    FILE* file = fopen(nome_arquivo, "wb");
    if(!file){
        printf("\n++--- Erro na abertura do arquivo! Impossível armazenar o sistema.");
        return 1;
    }

    // Primeiro, escrevemos o boot record no arquivo
    fwrite(&br_sistema, BLOCK_SIZE, 1, file);


    return 0;
}