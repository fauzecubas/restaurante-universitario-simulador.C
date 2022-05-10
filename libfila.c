#include <stdio.h>
#include <stdlib.h>
#include "libfila.h"

fila_t *fila_cria()
{
	fila_t *fila;						  /* cria fila */
	fila = malloc(sizeof(fila_t));		  /* alocamemória do tamanho TAD fila_t */
	fila->ini = malloc(sizeof(nodo_f_t)); /* aloca memória p/ o nodo de inicio da fila */
	fila->fim = malloc(sizeof(nodo_f_t)); /* aloca memória p/ o nodo de fim da fila */
	fila->fim->prox = NULL;				  /* o fim da fila aponta para NULL */
	fila->tamanho = 0;
	if (fila == NULL || fila->ini == NULL || fila->fim == NULL)
	{
		printf("Falha faltal. Impossível alocar mamória.");
		return NULL;
	}
	return fila;
}

fila_t *fila_destroi(fila_t *f)
{
	if (f->tamanho > 2)
	{
		struct nodo_f *p = f->ini->prox;	   /* desaloca memória */
		int i;								   /* dos demais nodos */
		for (i = 2; i < f->tamanho; i = i + 1) /* INCLUSIVE do fim */
		{
			struct nodo_f *pAux = p;
			p = p->prox;
			free(pAux);
		}
		free(p);
		free(f->ini); /* desaloca memória do inicio da fila */
		free(f);	  /* desaloca memoria da fila */
	}
	else
	{
		free(f->ini); /* desaloca memória do inicio da fila */
		free(f->fim); /* desaloca memoria do fim da fila */
		free(f);	  /* desaloca memoria da fila */
	}
	return NULL;
}

int fila_vazia(fila_t *f)
{
	if (f && f->tamanho <= 0)
	{
		return 1;
	}
	return 0;
}

int fila_tamanho(fila_t *f)
{
	if (f)
	{
		return f->tamanho;
	}

	return 0;
}

int queue(fila_t *f, int elemento)
{
	if (f->tamanho == 0)
	{ /* fila ainda vazia */
		f->ini->chave = elemento;
		f->tamanho = f->tamanho + 1;
		return 1;
	}
	else if (f->tamanho == 1)
	{
		struct nodo_f *p = malloc(sizeof(struct nodo_f));
		if (p == NULL)
			return 0;
		p->chave = elemento;
		p->prox = NULL;
		f->ini->prox = p;
		f->fim = p;
		f->tamanho = f->tamanho + 1;
		return 1;
	}
	else
	{
		struct nodo_f *p = malloc(sizeof(struct nodo_f));
		if (p == NULL)
			return 0;
		p->chave = elemento;
		p->prox = NULL;
		f->fim->prox = p;
		f->fim = p;
		f->tamanho = f->tamanho + 1;
		return 1;
	}
}

int dequeue(fila_t *f, int *elemento)
{
	if (f->tamanho == 0)
		return 0;
	*elemento = f->ini->chave;
	nodo_f_t *p = f->ini;
	f->ini = f->ini->prox;
	free(p);
	f->tamanho = f->tamanho - 1;
	return 1;
}

void fila_imprime(fila_t *f)
{
	if (f->tamanho == 0)
		return;
	struct nodo_f *p = f->ini;
	int i;
	if (f->tamanho == 1)
	{
		printf("%d\n", p->chave);
		return;
	}
	for (i = 1; i < f->tamanho; i = i + 1)
	{
		printf("%d ", p->chave);
		p = p->prox;
	}
	printf("%d\n", f->fim->chave);
}
