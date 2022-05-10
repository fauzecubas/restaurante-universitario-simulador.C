#include <stdio.h>
#include <stdlib.h>
#include "libpilha.h"

pilha_t* pilha_cria (int nelem) {
	pilha_t *pilha; /* cria pilha */
	pilha = malloc(sizeof(pilha_t)); /* aloca memória de tamanho TAD pilha_t */
	pilha->elems = malloc(sizeof(int)*nelem); /* aloca memória p/ o num de elementos da pilha */
	if (pilha == NULL || pilha->elems == NULL) {
		printf("Falha fatal. Impossível alocar memória.");
		return NULL;
	}
	pilha->nelem = nelem; 
	pilha->topo = -1; /* inicializa o topo em -1 para indicar que a pilha n possui elementos */
	return pilha;
}

int push (pilha_t* pilha, int elem) {
	pilha->topo = pilha->topo + 1;
	if (pilha->topo >= pilha->nelem) {
		pilha->topo = pilha->topo - 1; /* corrige o topo da pilha */
		return -1; /* pilha cheia */
	}
	pilha->elems[pilha->topo] = elem; /* faz o push no topo da pilha */
	return pilha->topo + 1; /* retorna o numero de elementos na pilha (topo+1) */
}

int pop (pilha_t* pilha) {
	if (pilha->topo == -1) {
		return 0; /* pilha vazia */
	}
	pilha->topo = pilha->topo - 1; /* diminui em 1 o tamanho da pilha */
	return pilha->elems[pilha->topo + 1]; /* retorna o elemento que estava no topo */
}

int pilha_topo (pilha_t* pilha) {
	if (pilha->topo == -1) {
		return 0; /* pilha vazia */
	}
	return pilha->elems[pilha->topo]; /* retorna o topo da pilha */
}

int pilha_tamanho (pilha_t* pilha) {
	return pilha->topo + 1;
}

int pilha_vazia (pilha_t* pilha) {
	if (!pilha || pilha->topo == -1) {
		return 1;
	}
	return 0;
}

pilha_t* pilha_destroi (pilha_t* pilha) {
	free(pilha->elems);	/* desaloca memória dos elementos da pilha */
	free(pilha);	/* desaloca memoria da pilha */
	pilha = NULL;
	return pilha;
}

void pilha_imprime(pilha_t* pilha) {
	if (pilha->topo == -1) return;
	int i;
	for (i = 0; i <= pilha->topo; i = i + 1) {
		printf("%d", pilha->elems[i]);
		if (i < pilha->topo) printf(" ");
	}
	printf("\n");
}
