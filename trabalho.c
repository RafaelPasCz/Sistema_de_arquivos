#include "structs.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define BLOCK_SIZE 512


boot_record br_sistema;         // boot record do sistema de arquivos em memória
boot_record teste2;             // boot record carregado na memoria em outras execuções
char nome_arquivo[256];         // Arquivo a ser usado para simular o sistema de arquivos



void get_nomeArquivo(){         // Função que pede o nome do arquivo ao usuário, e armazena ele numa var global
    printf("=-- Insira o nome do arquivo a ser usado\nR: ");

    while (getchar() != '\n');  // Limpa o buffer

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

        char *conteudo_bloco = (char*)malloc(br_sistema.bytes_por_bloco); //alocamos o conteudo para o bloco
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


unsigned int* listar_blocos_livres(char *nome_arquivo) {
    FILE *file = fopen(nome_arquivo, "rb");
    if (!file) {
        perror("Erro ao abrir arquivo");
        exit(EXIT_FAILURE);
    }

    // lê o boot record para obter a cabeça da lista e tamanho do bloco
    unsigned int* lista_livres;
    lista_livres = (unsigned int*)malloc(teste2.num_blocos_livres * sizeof(unsigned int));
    unsigned int bloco_atual = teste2.cabeca_lista; //pegamos a cabeça da lista no boot record
    const unsigned short bytes_por_bloco = teste2.bytes_por_bloco; //e pegamos o tamanho do bloco
    int i = 0;

    printf("Percorrendo lista de blocos livres:\n");
    char buffer[4]; // aloca buffer para ler apenas os 4 bytes do próximo bloco
    while (bloco_atual != 0xFFFFFFFF) { //enquanto não chegamos ao fim da lista
        lista_livres[i] = bloco_atual;
        //posiciona  o cursor no início do bloco atual
        fseek(file, bloco_atual * bytes_por_bloco, SEEK_SET);

        // lê os primeiros 4 bytes (próximo endereço)
        fread(buffer, sizeof(char), 4, file);

        // converte de little-endian para uint32_t
        unsigned int proximo = (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | buffer[0];

        // exibe em hexadecimal
      //  printf("Bloco %u: 0x", bloco_atual);
      //  for (int i = 0; i < 4; i++) {
      //      printf("%02X", (char)buffer[i]);
      //  }
      //  printf(" -> Proximo: %u\n", proximo);

        bloco_atual = proximo; // Avança para o próximo bloco
        i++;
    }
    fclose(file);
    return lista_livres;
}





//função de comparação para o qsort
//se a = 5 e b = 3, a função retorna 2, que é positivo, então b vem antes de a
//se a = 3 e b = 5, a função retorna -2, que é negativo, então a vem antes de b
int comparar_uint(const void *a, const void *b) {
    return (*(unsigned int*)a - *(unsigned int*)b);
}

unsigned int* ordenar_lista(unsigned int* lista) {
    if(lista == NULL || teste2.num_blocos_livres == 0) {
        return NULL;
    }

    //alocar memória para a lista ordenada
    unsigned int* lista_ordenada = (unsigned int*)malloc(teste2.num_blocos_livres * sizeof(unsigned int));

    if(!lista_ordenada) {
        perror("Erro ao alocar memória para lista ordenada");
        return NULL;
    }

    //copiar a lista original
    memcpy(lista_ordenada, lista, teste2.num_blocos_livres * sizeof(unsigned int));

    // Ordenar usando quicksort
    qsort(lista_ordenada,
          teste2.num_blocos_livres,
          sizeof(unsigned int),
          comparar_uint);

    return lista_ordenada;
}

unsigned int* procurar_espaco(int espaco_necessario, unsigned int* lista) {
    if (lista == NULL || teste2.num_blocos_livres < espaco_necessario) {
        return NULL;
    }

    //aloca memória para o resultado, início e final da sequencia contigua
    unsigned int* resultado = (unsigned int*)malloc(2 * sizeof(unsigned int));
    if (!resultado) {
        perror("Erro ao alocar memória para resultado");
        return NULL;
    }

    int contador = 1;
    unsigned int inicio_sequencia = lista[0]; //inicializa a sequencia no começo da lista

    for (int i = 1; i < teste2.num_blocos_livres; i++) {
        //verifica se o bloco atual é consecutivo ao anterior
        if (lista[i] == lista[i - 1] + 1) {
            contador++;
        } else {
            //reinicia a contagem e atualiza o início da sequência
            contador = 1;
            inicio_sequencia = lista[i];
        }

        //se encontrou uma sequência válida
        if (contador >= espaco_necessario) {
            resultado[0] = inicio_sequencia; //retorna o vetor com o inicio
            resultado[1] = inicio_sequencia + espaco_necessario - 1; //e o fim da sequencia livre
            return resultado;
        }
    }

    //nenhuma sequência encontrada
    free(resultado);
    return NULL;
}


int formatar(char *nome_arquivo){
    //  Recebe o nome do arquivo aonde será feita a simulação do sistema de arquivos
    //  Pede para o usuário o tamanho do disco (simulado), e o número de entradas da tabela de entradas
    //  Valida os dados inseridos
    //  Gera a stuct do boot record, calcula e insere os dados na struct
    //  Salva a struct no arquivo
    //  Retorna 0 caso tudo der certo

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


void ler_boot(){ 
    //  Abre o arquivo escolhido antes pelo usuário
    //  Lê o arquivo, armazena o boot record numa struct
    //  Printa os dados armazenadosna struct

    FILE *file;
    file = fopen(nome_arquivo, "rb");
    fread(&teste2, sizeof(boot_record), 1, file);
    fclose(file);

    printf("Dados lidos: \nbytes_por_bloco: %hu",teste2.bytes_por_bloco);
    printf("\nblocos reservados: %hu",teste2.blocos_reservados);
    printf("\nnum blocos livres: %i",teste2.num_blocos_livres);
    printf("\nnum blocos tabela de entradas: %i",teste2.num_blocos_tabela_entradas);
    printf("\nnum blocos secao de dados: %i",teste2.num_blocos_secao_dados);
    printf("\nnum blocos totais: %i",teste2.num_blocos_totais);
    printf("\nnum blocos reservados raiz: %i",teste2.num_blocos_reservados_raiz);
    printf("\nquant entradas sistema: %i",teste2.quant_entradas_sistema);
    printf("\ncabeca da lista: %i\n",teste2.cabeca_lista);


}


int main(){
    unsigned int* lista_livres;
    unsigned int* lista_ordenado;
    unsigned int* espaco_livre; // contem o deslocamento inicial e final do espaço livre necessário
    int operacao = 0, i = 0, continuar = 1;

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
	            ler_boot();
                break;
            
            case 3:
                printf("Tchau!\n");
                continuar = 0;
                break;
            
            default:
                printf("Opcao invalida!\n");

        }

    }




    /*
    */
    printf("\n\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    get_nomeArquivo();
    //lista todos os blocos livres do sistema
    lista_livres = listar_blocos_livres(nome_arquivo);
    printf("listando blocos livres\n");
    for(i = 0; i < teste2.num_blocos_livres; i++){
        printf("%u ",lista_livres[i]);
    }

    //testar ordenação de lista
    lista_ordenado = ordenar_lista(lista_livres);
    printf("\nlistando ordenado\n");
    for(i = 0; i < teste2.num_blocos_livres; i++){
        printf("%u ",lista_ordenado[i]);
    }

    //testar a busca por espaços, recebe o numero de espaços necessário e a lista ordenada
    espaco_livre = procurar_espaco(5,lista_ordenado);
    printf("\nespaco de 2 blocos livres: %i a %i\n", espaco_livre[0],espaco_livre[1]);
    return 0;
}
