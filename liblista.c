#include <stdio.h>
#include <stdlib.h>
#include "liblista.h"

lista_t *lista_cria()
{
	lista_t *lista;					 /* cria lista */
	lista = malloc(sizeof(lista_t)); /* aloca memória do tamanho TAD lista_t */
	lista->tamanho = 0;
	lista->ini = malloc(sizeof(nodo_l_t));

	if (lista == NULL || lista->ini == NULL)
	{
		printf("Falha fatal. Impossível alocar memória.");
		return NULL;
	}

	return lista;
}

lista_t *lista_destroi(lista_t *l)
{
	if (l->tamanho > 2)
	{
		struct nodo_l *p = l->ini->prox; /* desaloca memória */
		int i;							 /* dos demias nodos */
		for (i = 2; i < l->tamanho; i = i + 1)
		{
			struct nodo_l *pAux = p;
			p = p->prox;
			free(pAux);
			pAux = NULL;
		}
		free(p);
		p = NULL;
	}
	free(l->ini); /* desaloca memória do item inicial da lista */
	free(l);	  /* desaloca memoria da lista */
	l->ini = NULL;
	l = NULL;
	return l;
}

int lista_vazia(lista_t *l)
{
	if (l && l->tamanho > 0)
		return 0; /* lista não vazia */
	return 1;	  /* lista vazia */
}

int lista_tamanho(lista_t *l)
{
	if (l)
		return l->tamanho; /* se lista existir, retorna o tam */
	return 0;
}

int lista_insere_inicio(lista_t *l, int elemento)
{
	if (l->tamanho == 0)
	{								 /* lista ainda vazia */
		l->ini->elemento = elemento; /* atribui diretamente */
		l->ini->prox = NULL;		 /* no inicio */
		l->tamanho = l->tamanho + 1; /* da lista */
		return 1;
	}
	struct nodo_l *p = malloc(sizeof(struct nodo_l)); /* aloca memória */
	if (p == NULL)
		return 0; /* falha de alocação de memória */
	p->elemento = elemento;
	p->prox = l->ini; /* o prox agora será o inicio */
	l->ini = p;		  /* inicio agora é p */
	l->tamanho = l->tamanho + 1;
	return 1;
}

int lista_insere_fim(lista_t *l, int elemento)
{
	if (l->tamanho == 0)
	{								 /* lista ainda vazia */
		l->ini->elemento = elemento; /* atribui diretamente */
		l->ini->prox = NULL;		 /* no inicio */
		l->tamanho = l->tamanho + 1; /* da lista */
		return 1;
	}

	struct nodo_l *p = l->ini; /* ponteiro apontando para o ini da lista */
	int i;
	for (i = 1; i < l->tamanho; i = i + 1)
	{				 /* incremento p/ */
		p = p->prox; /* chegar no ultimo el */
	}

	struct nodo_l *el = malloc(sizeof(struct nodo_l)); /* aloco memoria */
	if (el == NULL)
		return 0;			 /* falha de alocação de memória */
	el->elemento = elemento; /* atribuo o valor desejado */
	p->prox = el;			 /* antigoUltimoElemento->prox = elementoInserido */
	l->tamanho = l->tamanho + 1;
	return 1;
}

int lista_insere_ordenado(lista_t *l, int elemento)
{
	/*----------CASO DE LISTA AINDA VAZIA, ATRIBUIÇÃO DIRETA----------*/
	if (l->tamanho == 0)
	{								 /* lista ainda vazia */
		l->ini->elemento = elemento; /* atribui diretamente */
		l->ini->prox = NULL;		 /* no inicio */
		l->tamanho = l->tamanho + 1; /* da lista */
		return 1;
	}
	/*-----------------------------------------------------------------*/

	/*--CASO ONDE O ELEMENTO INSERIDO DEVE OCUPAR A POSIÇÃO DE INÍCIO--*/
	if (l->ini->elemento >= elemento)
	{
		struct nodo_l *el = malloc(sizeof(struct nodo_l)); /* aloca memória */
		if (el == NULL)
			return 0; /* falha de alocação de memória */
		el->elemento = elemento;
		el->prox = l->ini;
		l->ini = el;
		l->tamanho = l->tamanho + 1;
		return 1;
	}
	/*-----------------------------------------------------------------*/

	/*--CASO DE BUSCA NA LISTA DA POSIÇÃO DE INSERÇÃO CORRETA--*/
	struct nodo_l *p = l->ini; /* ponteiro apontando para o ini da lista */
	struct nodo_l *pAnt;
	int i;
	for (i = 1; i <= l->tamanho; i = i + 1)
	{ /* incremento p/ */
		if (p->elemento >= elemento)
		{ /* acha a pos correta */
			break;
		}
		pAnt = p;
		p = p->prox; /* chegar no ultimo el */
	}
	struct nodo_l *el = malloc(sizeof(struct nodo_l)); /* aloca memória */
	if (el == NULL)
		return 0; /* falha de alocação de memória */
	el->elemento = elemento;
	el->prox = pAnt->prox;
	pAnt->prox = el;
	l->tamanho = l->tamanho + 1;
	return 1;
	/*-----------------------------------------------------------------*/
}

int lista_retira_inicio(lista_t *l, int *elemento)
{
	if (l->tamanho == 0)
		return 0;
	elemento = &l->ini->elemento;
	l->ini = l->ini->prox;
	l->tamanho = l->tamanho - 1;
	return 1;
}

int lista_retira_fim(lista_t *l, int *elemento)
{
	if (l->tamanho == 0)
		return 0;
	struct nodo_l *p = l->ini;
	int i;
	for (i = 1; i < l->tamanho; i = i + 1)
	{
		p = p->prox;
	}
	free(p);
	l->tamanho = l->tamanho - 1;
	return 1;
}

int lista_retira_elemento(lista_t *l, int *elemento)
{
	if (l->tamanho == 0)
		return 0;

	/*--CASO ONDE O ELEMENTO PROCURADO ESTÁ NA POSIÇÃO DE INÍCIO--*/
	if (l->ini->elemento == *elemento)
	{
		l->ini = l->ini->prox;
		l->tamanho = l->tamanho - 1;
		return 1;
	}
	/*-----------------------------------------------------------------*/

	/*--CASO ONDE DEVO PROCURAR O ELEMENTO A SER RETIRADO--*/
	struct nodo_l *p = l->ini->prox;
	struct nodo_l *pAnt = l->ini;
	int i;
	for (i = 2; i < l->tamanho; i = i + 1)
	{
		if (p->elemento == *elemento)
			break;
		pAnt = p;
		p = p->prox;
	}
	if (p->elemento == *elemento)
	{
		pAnt->prox = p->prox;
		free(p);
		l->tamanho = l->tamanho - 1;
		return 1;
	}
	/*-----------------------------------------------------*/

	return 0;
}

int lista_pertence(lista_t *l, int elemento)
{
	if (l->tamanho == 0)
		return 0;
	struct nodo_l *p = l->ini;
	int i;
	for (i = 0; i < l->tamanho; i = i + 1)
	{
		if (p->elemento == elemento)
			return 1;
		p = p->prox;
	}
	return 0;
}

void lista_imprime(lista_t *l)
{
	if (l->tamanho == 0)
		return;
	struct nodo_l *p = l->ini;
	int i;
	for (i = 0; i < l->tamanho; i = i + 1)
	{
		printf("%d", p->elemento);
		p = p->prox;
		if (p)
			printf(" ");
	}
	printf("\n");
}
