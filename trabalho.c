#include "structs.h"
#include <string.h>
typedef struct boot_record boot;
boot teste;
boot teste2;

unsigned int cria_cabeca() {
    FILE *file;
    bloco cabeca;
    uint32_t end_fis;
    uint32_t next_block_marker = 0xFFFFFFFF; // Marcador de fim da lista
    unsigned char bytes[4];

    // 1. Calcular o endereço físico do bloco cabeça (número do bloco)
    // incluimos pra não escrever em cima da tabela de entradas
    end_fis = teste.blocos_reservados + teste.num_blocos_tabela_entradas; // Bloco após os reservados

    // 2. Codificar 0xFFFFFFFF em 4 bytes (little-endian)
    bytes[0] = (next_block_marker >> 0) & 0xFF;  // Byte menos significativo
    bytes[1] = (next_block_marker >> 8) & 0xFF;
    bytes[2] = (next_block_marker >> 16) & 0xFF;
    bytes[3] = (next_block_marker >> 24) & 0xFF; // Byte mais significativo

    // 3. Alocar e inicializar o conteúdo do bloco
    cabeca.conteudo = (char*)malloc(teste.bytes_por_bloco * sizeof(char));
    if (!cabeca.conteudo) {
        perror("Erro ao alocar memória para o bloco cabeça");
        exit(EXIT_FAILURE);
    }

    // Preencher os primeiros 4 bytes com 0xFFFFFFFF, mudar depois se precisar
    for (int i = 0; i < 4; i++) {
        cabeca.conteudo[i] = bytes[i];
    }

    // preencher o restante do bloco com alguma coisa
    for (int i = 4; i < teste.bytes_por_bloco; i++) {
        cabeca.conteudo[i] = 0x41;
    }

    // 4. Escrever o bloco no disco
    file = fopen("boot.dat", "rb+");
    if (!file) {
        perror("Erro ao abrir arquivo boot.dat");
        exit(EXIT_FAILURE);
    }

    fseek(file, end_fis * teste.bytes_por_bloco, SEEK_SET); // Posicionar no bloco correto
    fwrite(cabeca.conteudo, teste.bytes_por_bloco, 1, file);
    fclose(file);

    // 5. Liberar memória e retornar
    free(cabeca.conteudo);
    return end_fis;
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
    printf("Insira as informacoes de formatacao\n");
    printf("Insira o numero de bytes por bloco(1 a 65535)\n");
    scanf("%hu",&teste.bytes_por_bloco);

    printf("insira o tamanho do disco em bytes\n");
    scanf("%i",&tamanho_disco);
    teste.num_blocos_totais = tamanho_disco/teste.bytes_por_bloco; //blocos totais do sistema, incluindo reservados

    printf("insira o numero de blocos reservados (incluindo boot record)\n");
    scanf("%hu",&teste.blocos_reservados);

    printf("insira o numero de blocos na tabela de entradas\n");
    scanf("%i",&teste.num_blocos_tabela_entradas);

    printf("insira o numero de blocos reservados para o diretório raiz\n");
    scanf("%i",&teste.num_blocos_reservados_raiz);

    //aqui contamos o numero total de blocos que não poderão ser utilizados
    total_blocos_reservados = teste.blocos_reservados + teste.num_blocos_tabela_entradas + teste.num_blocos_reservados_raiz;
    //e subtraimos pra calcular o numero de blocos na seção de dados
    teste.num_blocos_secao_dados = teste.num_blocos_totais - total_blocos_reservados;
    //inicialmente o numero de blocos livres é o mesmo que o numero de blocos na seção de dados
    teste.num_blocos_livres = teste.num_blocos_secao_dados;
    //nenhuma entrada no sistema
    teste.quant_entradas_sistema = 0;
    //criando cabeca da lista
    teste.cabeca_lista = cria_cabeca();
}

void testar_boot(){
    FILE *file = fopen("boot.dat", "wb");
    fwrite(&teste, sizeof(boot), 1, file);
    fclose(file);
    file = fopen("boot.dat", "rb");
    fread(&teste2, sizeof(boot), 1, file);
    fclose(file);
    printf("Dados lidos: \nbytes_por_bloco: %hu",teste2.bytes_por_bloco);
    printf("\nblocos reservados: %hu",teste2.num_blocos_reservados_raiz);
    printf("\nnum blocos livres: %i",teste2.num_blocos_livres);
    printf("\nnum blocos tabela de entradas: %i",teste2.num_blocos_tabela_entradas);
    printf("\nnum blocos secao de dados: %i",teste2.num_blocos_secao_dados);
    printf("\nnum blocos totais: %i",teste2.num_blocos_totais);
    printf("\nnum blocos reservados raiz: %i",teste2.num_blocos_reservados_raiz);
    printf("\nquant entradas sistema: %i",teste2.quant_entradas_sistema);
    printf("\ncabeca da lista: %i",teste2.cabeca_lista);

    //testar se um bloco escrito está certo
    file = fopen("boot.dat","rb");
    bloco lido;
    fseek(file, teste2.cabeca_lista * teste.bytes_por_bloco, SEEK_SET);
    lido.conteudo = (char*)malloc(teste2.bytes_por_bloco * sizeof(char));
    fread(&lido, sizeof(char) * teste2.bytes_por_bloco, 1, file);
    int i;
    for(i = 0;i < teste2.bytes_por_bloco;i++){
        printf("%c", lido.conteudo[i]);
    }
}




int main(){
	printf("hello world\n");
	formatar();
	testar_boot();
}
