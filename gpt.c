Olá! Estou implementando um simulador de sistema de arquivos em C. Até agora está tudo indo bem, só estou tendo um problema na função carregar_lista_blocos_livres:
Estou testando com um arquivo composto de 3 blocos, e nele a lista de blocos livres carregada teria 16 entradas (32 bytes por entrada * 16 = 512 bytes, o tamanho de um bloco).
Acontece que as duas ultimas entradas estão saindo com valores muito estranhos, sendo que eles deveriam estar zerados, veja:
 ------- Entrada 13: -------
Status:
Nome:
Extensao :
Tipo:
Primeiro bloco: 0
Tamanho (bytes): 0
Blocos usados: 0
Padding: 0

 ------- Entrada 14: -------
Status:
Nome:
Extensao AACy│·KÊ:
Tipo: │
Primeiro bloco: 53776
Tamanho (bytes): 2635033392
Blocos usados: 298
Padding: 336

 ------- Entrada 15: -------
Status: *
Nome:
Extensao XE36h+i2z2KZi27t+SJoy/70lRocH7ZpAAAAAAOgAAAAAIAACAAAAB/1h6iQJayoL4CwPHJs88SecKU83dQ5GMPmWnPkLIm4WAAAAD0W2Z4TpQLT0Q+EE9tDjlFbQxwYNl3yce7TxrDO1AwvmQE+GjrnRFvxEXEcDT9bokkx3zcBRF/jTlT0RFdH6licPnzjyaHtchY0e0MJkCmM6VBS+YYVZFvUsHTQCRnfNhAAAAAjUvBIPJcGlHLhOTmSpirfujxnUeoDa33S9XwH8nYsibQvHaiYgMaL+xfvhjxeyYqhuFbyy4kKe8QWzP8932Kyw==:
Tipo: h
Primeiro bloco: 1261599282
Tamanho (bytes): 926050650
Blocos usados: 1246964596
Padding: 31087

Segue o código para voce consultar:
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BLOCK_SIZE 512

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


struct Bloco{
    char conteudo[BLOCK_SIZE];
};

typedef struct Boot_record boot_record;
typedef struct Entrada entrada;
typedef struct Bloco bloco;


#include <string.h>
#include <math.h>
#include <stdlib.h>


char nome_arquivo[256];             // Arquivo a ser usado para simular o sistema de arquivos
boot_record br_sistema;             // boot record do sistema de arquivos em memória
entrada *entrada_sistema = NULL;    // Lista que armazena a tabela de entradas do sistema de arquivos em memória (alocado com malloc)
bloco   *dados_sistema = NULL;      // Lista que armazena os blocos da seção de dados do sistema de arquivos em memória



void get_nomeArquivo(){             // Função que pede o nome do arquivo ao usuário, e armazena ele numa var global
    printf("=-- Insira o nome do arquivo a ser usado\nR: ");

    while (getchar() != '\n');      // Limpa o buffer

    if (fgets(nome_arquivo, sizeof(nome_arquivo), stdin)) {
        nome_arquivo[strcspn(nome_arquivo, "\n")] = '\0';   // Remove o '\n' do final
    }   // Lemos o nome do arquivo, junto com alguns tratamentos de strings. Cortesia do StackOverflow
    
    printf("Arquivo escolhido: %s\n", nome_arquivo);
}


void inicializar_secao_dados(char *nome_arquivo) {
    //  Recebe o nome de um arquivo, abre ele
    //  Abre ele
    //  Obtém o começo, e o tamanho da seção de dados
    //  A partir do primeiro endereço, percorre a seção de dados, criando a lista de blocos livres
        // Adiciona um ponteiro de 4 bytes para o próximo bloco, no começo de cada bloco
    //  Escreve no arquivo, e libera os espaços alocados

    FILE *file;
    uint32_t inicio_secao = br_sistema.blocos_reservados + br_sistema.num_blocos_tabela_entradas;   // Obtem o endereço do primeiro bloco da seção de dados
    uint32_t num_blocos = br_sistema.num_blocos_secao_dados;    // Obtem o tamnho total da seção de dados

    file = fopen(nome_arquivo, "rb+");          
    if (!file) {                                                // Verifica se foi aberto corretamente
        printf("\n\nArquivo %s", nome_arquivo);
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < num_blocos; i++) {
        uint32_t endereco_atual = inicio_secao + i; // Vai caminhando por cada bloco da seção de dados
        uint32_t proximo_endereco;                  // Variavel para calcular o proximo endereço

        if (i == num_blocos - 1) {
            proximo_endereco = 0xFFFFFFFF;          // Se chegamos ao ultimo bloco, proximo = -1
        } else {
            proximo_endereco = endereco_atual + 1; //proximo endereço é apenas o atual + 1
        }

        unsigned char bytes[4]; //fazemos a conversão de um inteiro para 4 bytes
        bytes[0] = (proximo_endereco >> 0) & 0xFF;
        bytes[1] = (proximo_endereco >> 8) & 0xFF;
        bytes[2] = (proximo_endereco >> 16) & 0xFF;
        bytes[3] = (proximo_endereco >> 24) & 0xFF;

        char *conteudo_bloco = (char*)malloc(br_sistema.bytes_por_bloco);   // Alocamos o conteudo para o bloco
        if (!conteudo_bloco) {
            perror("Erro ao alocar memória para o bloco");
            exit(EXIT_FAILURE);
        }

        memcpy(conteudo_bloco, bytes, 4);                                   // Os primeiros 4 bytes guardam o proximo endereço
        memset(conteudo_bloco + 4, 0x00, br_sistema.bytes_por_bloco - 4);   // O restante enchemos de 0

        fseek(file, endereco_atual * br_sistema.bytes_por_bloco, SEEK_SET); // Movemos o cursor, deslocamento atual * tamanho do bloco
        fwrite(conteudo_bloco, br_sistema.bytes_por_bloco, 1, file);        // Escrevemos o conteudo do bloco

        free(conteudo_bloco);   // Liberamos o ponteiro
    }
    fclose(file);               // Fecha o arquivo após a execução
}



int formatar(char *nome_arquivo){

    int total_blocos_reservados, tamanho_disco, n_entradas, tamanho_total_entradas;
    int continuar = 1;          // Usado na validação dos inputs
    

    while (continuar){                      // Recebemos e validamos os inputs do usuário
        printf("\n=--=- Informacoes de Formatacao\n");    
        printf("=-- Insira o tamanho do disco, em bytes\nR: ");
        scanf("%i",&tamanho_disco);
        
        printf("=-- Insira o numero de entradas na tabela de entradas\nR: ");
        scanf("%i",&n_entradas);
        
        // TODO - É 4 se considerarmos o root directory (1 bloco)
        if (tamanho_disco >= (3 * BLOCK_SIZE)                           // O disco armazena no mínimo 3 blocos (boot_r, tabela, dados)?
            && n_entradas <= ((int)(tamanho_disco / BLOCK_SIZE)) - 2 ){ // Não pode ter mais entradas do que blocos de dados
            // Não é necessário verificar se o tamanho da partição é suficiente pra tabela de entradas.
            
            continuar = 0;  // Se os inputs forem válidos, continua a execução

        } else {            // Se não, pede novamente os inputs
            printf("\n\n=-- DADOS INVALIDOS!!!\n");
        }
    }

    br_sistema.bytes_por_bloco = BLOCK_SIZE;     // Quantidade fixa de blocos por byte.
    br_sistema.blocos_reservados = 1;            // Quantidade de blocos reservados: 1 (boot record) (adicionar mais 1? root dir?)
    br_sistema.quant_entradas_sistema = 0;       // inicia em 0 porque o sistema está vazio
    br_sistema.num_blocos_reservados_raiz = 1;   // Numero de blocos reservados para o root directory
    br_sistema.num_blocos_totais = tamanho_disco/br_sistema.bytes_por_bloco;  // separamos o disco em blocos

    tamanho_total_entradas = n_entradas * sizeof(entrada);          // tamanho total da tabela de entradas em bytes

    if(tamanho_total_entradas < br_sistema.bytes_por_bloco){             // alocamos apenas um bloco se o tamanho da tabela é menor que um bloco
        br_sistema.num_blocos_tabela_entradas = 1;
    }else{                                                          // separamos em blocos e arredondamos para cima
        br_sistema.num_blocos_tabela_entradas = (tamanho_total_entradas + br_sistema.bytes_por_bloco - 1) / br_sistema.bytes_por_bloco;
    }

    total_blocos_reservados = br_sistema.blocos_reservados + br_sistema.num_blocos_tabela_entradas;   // para calcular o inicio da seção de dados
    br_sistema.num_blocos_secao_dados = br_sistema.num_blocos_totais - total_blocos_reservados;       
                                            // numero de blocos totais do sistema - (reservados + tabela de entradas)
    br_sistema.num_blocos_livres = br_sistema.num_blocos_secao_dados;

    br_sistema.cabeca_lista = total_blocos_reservados;       // Obtemos o endereço do primeiro bloco livre

    FILE *file = fopen(nome_arquivo, "wb");             // Abrimos o arquivo

    if (!file) {
        printf("\n\nErro ao abrir o arquivo %s!\n", nome_arquivo);
        return 1;
    }

    fwrite(&br_sistema, sizeof(boot_record), 1, file);
    fclose(file);

    return 0;
}


void carregar_boot(){ 
    //  Abre o arquivo escolhido antes pelo usuário
    //  Lê o arquivo, armazena o boot record numa struct
    //  Printa os dados armazenadosna struct

    FILE *file;
    file = fopen(nome_arquivo, "rb");
    fread(&br_sistema, sizeof(boot_record), 1, file);
    fclose(file);

    printf("Dados lidos: \nbytes_por_bloco: %hu", br_sistema.bytes_por_bloco);
    printf("\nblocos reservados: %hu", br_sistema.blocos_reservados);
    printf("\nnum blocos livres: %i", br_sistema.num_blocos_livres);
    printf("\nnum blocos tabela de entradas: %i", br_sistema.num_blocos_tabela_entradas);
    printf("\nnum blocos secao de dados: %i", br_sistema.num_blocos_secao_dados);
    printf("\nnum blocos totais: %i", br_sistema.num_blocos_totais);
    printf("\nnum blocos reservados raiz: %i", br_sistema.num_blocos_reservados_raiz);
    printf("\nquant entradas sistema: %i", br_sistema.quant_entradas_sistema);
    printf("\ncabeca da lista: %i\n", br_sistema.cabeca_lista);
}


int carregar_lista_blocos_livres(){



    FILE *file;
    file = fopen(nome_arquivo, "rb");       // Abrimos o arquivo, para pegar a tabela de entradas dele
    uint32_t blocos_tabela = br_sistema.num_blocos_tabela_entradas; // Armazenamos o tamanho em blocos da tabela de entradas
    size_t tamanho_tabela_entradas = blocos_tabela * BLOCK_SIZE;    // Armazenamos o tamanho em bytes da tabela de entradas

    free(entrada_sistema);                  // Liberamos a alocação anterior da tabela de entradas do sistema
    
    entrada_sistema = (entrada *)malloc(tamanho_tabela_entradas);   // Alocamos espaço em memória para a tabela
    if(entrada_sistema == NULL){
        printf("Erro ao alocar memoria para a tabela de entradas!");
        return 1;
    }

    // Agora, fseek no arquivo para o começo da tabela de entradas
    // Depois for loop que vai lendo o coiso

    fseek(file, 1 * BLOCK_SIZE, SEEK_SET);  // Movemos o ponteiro de leitura para o começo da tabela de entradas

    fread(entrada_sistema, tamanho_tabela_entradas, 1, file);       // Lê todos os arquivos da tabela de entrada, armazena eles na lista


    fclose(file);


    // Agora é pra tudo estar dentro dessa lista! Printamos ela por conveniencia:
    // Primeiro calculamos o tamanho do array:
    int tamanho = blocos_tabela * 16;   // 512 / 32 = 16 entradas por bloco

    printf("\n\n ----------- TABELA DE ENTRADAS -----------");
    for(int i = 0; i < tamanho; i++){
        printf("\n\n ------- Entrada %i: -------", i);
        printf("\nStatus: %c", entrada_sistema[i].status);
        printf("\nNome: %s", entrada_sistema[i].nome);
        printf("\nExtensao %s: ", entrada_sistema[i].ext);
        printf("\nTipo: %c", entrada_sistema[i].tipo);
        printf("\nPrimeiro bloco: %u", entrada_sistema[i].primeiro_bloco);
        printf("\nTamanho (bytes): %u", entrada_sistema[i].tamanho);
        printf("\nBlocos usados: %u", entrada_sistema[i].numero_blocos_usados);
        printf("\nPadding: %u", entrada_sistema[i].padding);
    }

    return 0;
}


int carregar_secao_dados(){
    //  Abre o arquivo escolhido pelo usuário
    //  Lê o arquivo, armazena a seção de dados na memória
    //  Printa os blocos armazenados

    
    // Ok. Possivel ordem de fazer as coisas:
    // Abre o arquivo
    // fseek pro começo do bloco da seção de dados
    // pega o valor do número de blocos da seção de dados

    // aloca espaço em memória blocos_dados * BLOCK_SIZE
        // usando o ponteiro global blocos_sistema
        // podse: blocos_sistema = (bloco *)malloc(blocos_dados * sizeof(bloco)))
        // Daí vai fazer o mesmo esquema feito anteriormente, de ir adicionando indices de array nesse espaço da memória

    // Quando esses valores estiverem carregados, tudo ok pode retornar.
    // Talvez ate printar eles na tela enquanto pega cada um

    // Dai depois disso tem que fazer a função de colocar eles da memória pro arquivo.




    FILE *file;
    file = fopen(nome_arquivo, "rb");       // Abrimos o arquivo, para pegar a tabela de entradas dele
    uint32_t blocos_dados = br_sistema.num_blocos_secao_dados;  // Armazenamos o tamanho em blocos da seção de dados
    size_t tamanho_secao_dados = blocos_dados * BLOCK_SIZE;     // Armazenamos o tamanho em bytes da seção de dados
    uint32_t inicio_dados = br_sistema.blocos_reservados + br_sistema.num_blocos_tabela_entradas;   // Endereço do começo da seção de dados

    free(dados_sistema);                    // Liberamos a alocação anterior da seção de dados do sistema
    
    dados_sistema = (bloco *)malloc(tamanho_secao_dados);       // Alocamos espaço em memória para a tabela

    if(dados_sistema == NULL){
        printf("Erro ao alocar memoria para a seção de dados!");
        return 1;
    }

    // Agora, fseek no arquivo para o começo da seção de dados
    // Depois a gente lê tudo e armazena na memória alocada anteriormente

    fseek(file, inicio_dados * BLOCK_SIZE, SEEK_SET);       // Movemos o ponteiro de leitura para o começo da seção de dados

    fread(dados_sistema, tamanho_secao_dados, 1, file);     // Lê todos os blocos da seção de dados, armazena eles na lista

    fclose(file);


    // Agora é pra tudo estar dentro dessa lista! Printamos ela por conveniencia:
    printf("\n\n ----------- SECAO DE DADOS -----------");

    for(uint32_t i = 0; i < blocos_dados; i++){                  // Iteramos por cada bloco da seção de dados
        printf("\nBloco %i:\n", i + inicio_dados);
        for (int j = 0; j < BLOCK_SIZE; j++) {              // Iteramos por cada byte do bloco
            printf("%02X ", (unsigned char)dados_sistema[i].conteudo[j]);
            if ((j + 1) % 16 == 0) {
                printf("\n");                               // Quebra de linha a cada 16 bytes, mesmo estilo do HexEd.it
            }
        }
        printf("\n\n");
    }

    return 0;






}


int main(){
    unsigned int* lista_livres;
    unsigned int* lista_ordenado;
    unsigned int* espaco_livre; // contem o deslocamento inicial e final do espaço livre necessário
    int operacao = 0, continuar = 1;
    unsigned int i = 0;

	printf("hello world\n");

    while(continuar){
        printf("\n\n==----------- Sistema de Arquivos -----------==\n");
        printf("=--=- O que deseja fazer?\n");        
        printf("=--[1] Formatar uma particao simulada\n");
        printf("=--[2] Carregar uma particao simulada\n");
        printf("=--[3] Sair\nR: ");
        scanf("%i", &operacao);

        switch(operacao){
            case 1:
                get_nomeArquivo();

                formatar(nome_arquivo);                 // Cria o boot record
                inicializar_secao_dados(nome_arquivo);  // Inicializa todos os blocos da seção de dados

                memset(&br_sistema, 0, sizeof(br_sistema));   // Limpa os dados armazenados nas structs 

                break;

            case 2:
                //função de ler boot para outras execuções
                get_nomeArquivo();
	            carregar_boot();
                carregar_lista_blocos_livres();
                carregar_secao_dados();

                break;
            
            case 3:
                printf("Tchau!\n");
                continuar = 0;
                break;
            
            default:
                printf("Opcao invalida!\n");

        }

    }

}

