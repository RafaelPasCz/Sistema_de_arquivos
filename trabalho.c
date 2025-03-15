#include <stdio.h>
#include <stdlib.h>

struct boot_record{
    unsigned short bytes_por_bloco;          //2 bytes 
    unsigned short blocos_reservados;        //2 bytes
    unsigned int num_blocos_livres;          //4 bytes
    unsigned int num_blocos_tabela_entradas; //...
    unsigned int num_blocos_secao_dados;
    unsigned int num_blocos_totais;
    unsigned int num_blocos_reservados_raiz;
    unsigned int quant_entradas_sistema;
    int padding;                             //4 bytes para completar
};

struct entrada{
    char status;   //apenas um byte para status
    char nome[12]; 
    char ext[4];
    char tipo;
    unsigned int primeiro_bloco;
    unsigned int tamanho;
    unsigned int numero_blocos_usados;
    unsigned short padding;
};

struct Node{
	int num; //numero do bloco
	struct Node *prox; //proximo bloco
};

typedef struct Node node;
typedef node* LISTA; //aponta para o inicio da lista;

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


void insereFinal(LISTA *lista,int x){

	node *novo = (node*) malloc (sizeof(node));
    novo->num = x;
	if(novo == NULL)
	{
		printf("Erro ao alocar memoria(inserefinal)\n");
		exit(0);
	}
	novo->prox = NULL;
	if((*lista) == NULL)
	{
		*lista = novo; //cria lista se ela não existe
	}
		else
		{
			node *aux = *lista;
			while(aux->prox != NULL)
			{
				aux = aux->prox;  //ate chegar no fim da lista
			}
			aux->prox=novo; //adiciona novo
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
		node* aux;
		aux = *lista;
		while(aux != NULL)
			{
				printf("%5i", aux->num);
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
			node *tmp;
			while((*lista) != NULL)
			{
				tmp = *lista;
				*lista = (*lista)->prox;
				free(tmp);
			}
		}
}


void insereordenado(LISTA* lista, int valor)
{
    node *tmp;
    tmp = *lista;
    node* novo = (node*) malloc (sizeof(node));
    novo->num=valor;
        while((tmp->prox)->num < novo->num) 
        {
            tmp = tmp->prox;
        }
        if((tmp->prox)->num >= novo->num)
            {
                novo->prox = tmp->prox;
                tmp->prox = novo;
            }
}

void removeelemento(LISTA* lista,int v)
{
    int flag=0;
    node* tmp,*tmpp; //atual e proximo 

    tmp=*lista;
    tmpp = tmp->prox;
    while(tmpp->num != v && tmpp->prox != NULL)
    {
        tmpp = tmpp->prox; 
        tmp = tmp->prox;
    }
        if(tmpp->num == v) //se o proximo é o elemento a ser deletado
        {
            tmp->prox = tmpp->prox; //o proximo do atual é igual ao proximo do proximo
            tmpp=NULL;
            free(tmpp); //deleta
            printf("elemento removido com sucesso\n");
        }
        else if(tmpp->prox==NULL && tmpp->num != v && flag != 1)
            {
                printf("elemento nao presente na lista\n");
            }
}


void eliminatdselem(LISTA* lista,int x)
{
    node *tmp,*aux,*rem;
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

}

int testar_lista(LISTA* lista){

    int opc;
    int x;
    do
    {
		printf("insira a funcao que deseja executar\n");
		printf("1- insere no final\n");
		printf("2- exibe lista\n");
		printf("3- libera lista\n");
		printf("4- insere ordenado\n");
		printf("5- remove elemento especifico\n");
		printf("0- sair do programa\n");
		scanf("%i",&opc);
		switch(opc)
		{
		case 1:
            printf("insira o numero a ser inserido: ");
            scanf("%i",&x);
			insereFinal(lista,x);
			break;
		case 2:
			exibe(lista);
			break;
		case 3:
			libera(lista);
			break;
        case 4:
            printf("insira o numero a ser inserido: ");
            scanf("%i",&x);
            insereordenado(lista,x);
            break;
        case 5:
            printf("insira o numero a ser removido: ");
            scanf("%i",&x);
            removeelemento(lista,x);
            break;
		case 0:
			free(lista);
			printf("saindo do programa\n");
			return 0;
			break;
		};
	}while (opc != 0);

}


int main(){
	printf("hello world\n");
    int opc;
	LISTA *lista = cria_lista();
    while(1){
        printf("testar lista?(1 ou 0)\n");
        scanf("%i",&opc);
        if(opc == 1){
            testar_lista(lista);
        }
        else
            return 0;
    }


}
