#include "leitura_arquivos.h"
#include "carregar_SA.h"
#include "armazenar_SA.h"
#include "formatar.h"



int main(){
    // unsigned int* lista_livres;
    // unsigned int* lista_ordenado;
    // unsigned int espaco_livre; // contem o deslocamento inicial e final do espaço livre necessário
    // unsigned int i = 0;
    
    int operacao = 0, continuar = 1;
    char arquivo[256];
	printf("hello world\n");

    while(continuar){
        printf("\n\n==----------- Sistema de Arquivos -----------==\n");
        printf("=--=- O que deseja fazer?\n");
        printf("=--[1] Formatar uma particao a ser simulada\n");
        printf("=--[2] Carregar uma particao simulada salva (HD -> RAM)\n");
        printf("=--[3] Salvar uma particao simulada (RAM -> HD)\n");
        printf("=--[4] Carregar arquivo do disco para o sistema simulado (HD -> RAM)\n");
        printf("=--[5] Carregar arquivo do sistema simulado para o disco(RAM -> HD)\n");
        printf("=--[0] Sair\nR: ");
        scanf("%i", &operacao);

        switch(operacao){
            case 1:
                get_nomeArquivo();

                formatar();                 // Cria o boot record
                inicializar_secao_dados();  // Inicializa todos os blocos da seção de dados

                memset(&br_sistema, 0, sizeof(br_sistema));   // Limpa os dados armazenados nas structs

                break;

            case 2:
                // Pega um sistema de arquivos salvo no disco, carrega ele na memória
                get_nomeArquivo();
                carregar_sistema_arquivos();
                break;

            case 3:
                // Pega o sistema de arquivos carregado na memória, salva ele no disco
                get_nomeArquivo();
                salvar_sistema_arquivos();
                break;

            case 4:
                // Funções para copiar arquivo do disco para o sistema
                printf("=-- Insira o nome do arquivo a ser copiado\nR: ");

                while (getchar() != '\n');      // Limpa o buffer

                if (fgets(arquivo, sizeof(arquivo), stdin)) {
                        arquivo[strcspn(arquivo, "\n")] = '\0';   // Remove o '\n' do final
                }   // Lemos o nome do arquivo, junto com alguns tratamentos de strings. Cortesia do StackOverflow

                printf("Arquivo escolhido: %s\n", arquivo);

                escrever_entrada(arquivo);

                break;

            case 5:
                // Funções para copiar um arquivo 
                

            case 0:
                printf("Tchau!\n");
                continuar = 0;
                break;

            default:
                printf("Opcao invalida!\n");

        }

    }





}
