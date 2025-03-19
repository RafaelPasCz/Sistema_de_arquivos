#include "structs.h"
#include <stdint.h>
typedef struct boot_record boot;
boot teste;
boot teste2;

unsigned int cria_cabeca(){
    int i;
    bloco cabeca;
    cabeca.conteudo = (char*)malloc(teste.bytes_por_bloco * sizeof(char));
    cabeca.conteudo[0] = 0xff;
    cabeca.conteudo[1] = 0xff;
    cabeca.conteudo[2] = 0xff;
    cabeca.conteudo[3] = 0xff;
    printf("\nentrou2\n");
    for(i=4;i < teste.bytes_por_bloco; i++){
        cabeca.conteudo[i] = 0x00;
    }
    FILE *file = fopen("boot.dat", "ab");
    fseek(file, (teste.bytes_por_bloco * teste.blocos_reservados),SEEK_SET);
    fwrite(&cabeca, teste.bytes_por_bloco, 1, file);
    fclose(file);
    unsigned int endereco_cabeca = (long int)&cabeca/teste.bytes_por_bloco;
    return endereco_cabeca;
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
    printf("\nquant entradas sistema: %i\n",teste2.quant_entradas_sistema);
    printf("\ncabeca da lista: %u\n",teste2.cabeca_lista);

}


int main(){
	printf("hello world\n");
	formatar();
    /*int opc;
	LISTA *lista = cria_lista();
    while(1){
        printf("testar lista?(1 ou 0)\n");
        scanf("%i",&opc);
        if(opc == 1){
            testar_lista(lista);
        }
        else
            return 0;
    }*/


}
