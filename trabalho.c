#include "structs.h"
#include <string.h>
#include <math.h>
typedef struct boot_record boot;
boot teste;
boot teste2;

void inicializar_secao_dados() {
    FILE *file;
    uint32_t inicio_secao = teste.blocos_reservados + teste.num_blocos_tabela_entradas; //tabela de entradas não faz parte daa seção de dados
    uint32_t num_blocos = teste.num_blocos_secao_dados; //definido nos calculos do boot recotd

    file = fopen("boot.dat", "rb+");
    if (!file) {
        perror("Erro ao abrir arquivo boot.dat");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < num_blocos; i++) {
        uint32_t endereco_atual = inicio_secao + i; //inicio da seção de dados é o primeiro endereço
        uint32_t proximo_endereco; //inicializamos a variavel para calcular o proximo endereço

        if (i == num_blocos - 1) {
            proximo_endereco = 0xFFFFFFFF; //se chegamos ao ultimo bloco, proximo = -1
        } else {
            proximo_endereco = endereco_atual + 1; //proximo endereço é apenas o atual + 1
        }

        unsigned char bytes[4]; //fazemos a conversão de um inteiro para 4 bytes
        bytes[0] = (proximo_endereco >> 0) & 0xFF;
        bytes[1] = (proximo_endereco >> 8) & 0xFF;
        bytes[2] = (proximo_endereco >> 16) & 0xFF;
        bytes[3] = (proximo_endereco >> 24) & 0xFF;

        char *conteudo_bloco = (char*)malloc(teste.bytes_por_bloco); //alocamos o conteudo para o bloco
        if (!conteudo_bloco) {
            perror("Erro ao alocar memória para o bloco");
            exit(EXIT_FAILURE);
        }

        memcpy(conteudo_bloco, bytes, 4); //os primeiros 4 bytes guardam o proximo endereço
        memset(conteudo_bloco + 4, 0x00, teste.bytes_por_bloco - 4); //o restante enchemos de 0

        fseek(file, endereco_atual * teste.bytes_por_bloco, SEEK_SET); //atualizamos o cursor, deslocamento atual * tamanho do bloco
        fwrite(conteudo_bloco, teste.bytes_por_bloco, 1, file); //escrevemos o conteudo do bloco

        free(conteudo_bloco); //liberamos o ponteiro
    }
    fclose(file);
}



int procurar_espaco(){
    //percorrer a lista de blocos
    //pegar os endereços de bloco
    //ordenar os endereços em ordem crescente
    //procurar maior numero de endereços consecutivos
}


int formatar(){
    int total_blocos_reservados;
    int tamanho_disco;
    int n_entradas;
    int tamanho_total_entradas;
    printf("Insira as informacoes de formatacao\n");

    teste.bytes_por_bloco = 512;
    printf("insira o tamanho do disco em bytes\n");
    scanf("%i",&tamanho_disco);
    teste.num_blocos_totais = tamanho_disco/teste.bytes_por_bloco; //separamos o disco em blocos

    printf("insira o numero de blocos reservados (incluindo boot record)\n");
    scanf("%hu",&teste.blocos_reservados);

    printf("insira o numero de entradas na tabela de entradas\n");
    scanf("%i",&n_entradas); //pegamos o numero de entradas que o usuario quer
    tamanho_total_entradas = n_entradas * sizeof(entrada); //tamanho total da tabela de entradas em bytes
    printf("%i\n",tamanho_total_entradas);
    printf("%i\n",teste.bytes_por_bloco);
    if(tamanho_total_entradas < teste.bytes_por_bloco)
        teste.num_blocos_tabela_entradas = 1;
    else
        teste.num_blocos_tabela_entradas = (tamanho_total_entradas + teste.bytes_por_bloco - 1) / teste.bytes_por_bloco; //separamos em blocos e arredondamos para cima
    printf("insira o numero de blocos reservados para o diretório raiz\n");
    scanf("%i",&teste.num_blocos_reservados_raiz);

    total_blocos_reservados = teste.blocos_reservados + teste.num_blocos_tabela_entradas; //para calcular o inicio da seção de dados
    teste.num_blocos_secao_dados = teste.num_blocos_totais - total_blocos_reservados; //numero de blocos totais do sistema - (reservados + tabela de entradas)
    teste.num_blocos_livres = teste.num_blocos_secao_dados;
    teste.quant_entradas_sistema = 0; //inicia em 0 porque o sistema está vazio
    teste.cabeca_lista = total_blocos_reservados; //

    FILE *file = fopen("boot.dat", "wb");
    fwrite(&teste, sizeof(boot), 1, file);
    fclose(file);

    inicializar_secao_dados(); // Inicializa todos os blocos da seção de dados
    return 0;
}

void testar_boot(){
    FILE *file;
    file = fopen("boot.dat", "rb");
    fread(&teste2, sizeof(boot), 1, file);
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

    file = fopen("boot.dat","rb");
    bloco lido;
    lido.conteudo = (char*)malloc(teste2.bytes_por_bloco * sizeof(char));
    fseek(file, teste2.cabeca_lista * teste2.bytes_por_bloco, SEEK_SET);
    fread(lido.conteudo, sizeof(char), teste2.bytes_por_bloco, file); // Correção aqui
    fclose(file);

    printf("Conteudo do primeiro bloco da lista:\n");
    for(int i = 0; i < teste2.bytes_por_bloco; i++){
        printf("%02X ", (char)lido.conteudo[i]);
    }
    printf("\n");
    free(lido.conteudo);
}



int main(){
	printf("hello world\n");
	formatar();
	testar_boot();
}
