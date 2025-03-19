#include "structs.h"


LISTA* cria_lista(){
	LISTA* inicio = (LISTA*) malloc (sizeof(LISTA));
	if(inicio != NULL)
	{
		*inicio = NULL;
	}
		else
		{
			printf("\nERRO EM cria_lista\n");
			exit(0);
		}
	return inicio;
}


void insereFinal(LISTA *lista,char x){

	bloco *novo = (bloco*) malloc (sizeof(bloco));
    novo->conteudo[0] = x;
	if(novo == NULL)
	{
		printf("Erro ao alocar memoria(inserefinal)\n");
		exit(0);
	}
	novo->prox = NULL;
	if((*lista) == NULL)
	{
		*lista = novo;
	}
		else
		{
			bloco *aux = *lista;
			while(aux->prox != NULL)
			{
				aux = aux->prox;
			}
			aux->prox=novo;
		}
}

void exibe(LISTA *lista)
{
	if((*lista) == NULL)
	{
		printf("lista vazia\n");
	}
	else
		{
		bloco* aux;
		aux = *lista;
		while(aux != NULL)
			{
				printf("%5c", aux->conteudo[0]);
				aux = aux->prox;
			}
		}
	printf("\n");
}

void libera(LISTA *lista)
{
	if((*lista) == NULL)
	{
		printf("a lista ja esta vazia\n");
	}
	else
		{
			bloco *tmp;
			while((*lista) != NULL)
			{
				tmp = *lista;
				*lista = (*lista)->prox;
				free(tmp);
			}
		}
}


/*void insereordenado(LISTA* lista, char valor)
{
    bloco *tmp;
    tmp = *lista;
    bloco* novo = (bloco*) malloc (sizeof(bloco));
    novo->conteudo[0]=valor;
        while((tmp->prox)->num < novo->num)
        {
            tmp = tmp->prox;
        }
        if((tmp->prox)->num >= novo->num)
            {
                novo->prox = tmp->prox;
                tmp->prox = novo;
            }
}*/

/*void removeelemento(LISTA* lista,int v)
{
    int flag=0;
    bloco* tmp,*tmpp;

    tmp=*lista;
    tmpp = tmp->prox;
    while(tmpp->num != v && tmpp->prox != NULL)
    {
        tmpp = tmpp->prox;
        tmp = tmp->prox;
    }
        if(tmpp->num == v)
        {
            tmp->prox = tmpp->prox;
            tmpp=NULL;
            free(tmpp);
            printf("elemento removido com sucesso\n");
        }
        else if(tmpp->prox==NULL && tmpp->num != v && flag != 1)
            {
                printf("elemento nao presente na lista\n");
            }
}*/


/*void eliminatdselem(LISTA* lista,int x)
{
    bloco *tmp,*aux,*rem;
    int cont=0;
    if(*lista==NULL)
        {
            printf("lista vazia\n");
        }
    tmp=*lista;
    aux = tmp->prox;
        while(aux->prox != NULL)
            {
            if(aux->num==x)
                {
                    rem=aux;
                    tmp->prox = aux->prox;
                    aux=aux->prox;
                    free(rem);
                    cont++;
                    printf("%i",cont);
                }
                else
                    {
                        aux=aux->prox;
                        tmp=tmp->prox;
                    }
            }
        printf("%i elementos removidos com sucesso\n",cont);

}*/

int testar_lista(LISTA* lista){

    int opc;
    char x;
    do
    {
		printf("insira a funcao que deseja executar\n");
		printf("1- insere no final\n");
		printf("2- exibe lista\n");
		printf("3- libera lista\n");
	//	printf("4- insere ordenado\n");
	//	printf("5- remove elemento especifico\n");
		printf("0- sair do programa\n");
		scanf("%i",&opc);
		switch(opc)
		{
		case 1:
            printf("insira o caracter a ser inserido: ");
            scanf(" %c",&x);
			insereFinal(lista,x);
			break;
		case 2:
			exibe(lista);
			break;
		case 3:
			libera(lista);
			break;
      /*  case 4:
            printf("insira o numero a ser inserido: ");
            scanf("%i",&x);
            insereordenado(lista,x);
            break;
        case 5:
            printf("insira o numero a ser removido: ");
            scanf("%i",&x);
            removeelemento(lista,x);
            break;*/
		case 0:
			free(lista);
			printf("saindo do programa\n");
			return 0;
			break;
		};
	}while (opc != 0);

}


int procurar_espaco(){
    //percorrer a lista de blocos
    //pegar os endereços de bloco
    //ordenar os endereços em ordem crescente
    //procurar maior numero de endereços consecutivos
}

typedef struct boot_record boot;
boot teste;
boot teste2;
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
    //apenas inicializando o padding para alinnhamento
    teste.padding = 0;
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
