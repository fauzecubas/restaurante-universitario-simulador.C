#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libpilha.h"
#include "libfila.h"
#include "liblista.h"

/*--------------TAD pessoa---------------*/
typedef struct
{
	int ticket;		 /* identificador único */
	int vacinado;	 /* se está vacinado (1) ou não (0) */
	int de_mascara;	 /* se está de máscara (1) ou não (0) */
	double dinheiro; /* quantidade de dinheiro */
} pessoa_t;
/*---------------------------------------*/

/*--------------TAD tickets ñ utilizados---------------*/
typedef struct
{
	int ticket;
	int motivo; /* 1. acabou a comida - 2. não vacinado - 3. sem mascara disponível - 4. sem dinheiro para refeição - 5. não tem dinheiro para a máscara */
} ticket_n_utilizado;
/*-----------------------------------------------------*/

/*--------------TAD contabilidade---------------*/
typedef struct
{
	double dinRef;
	double dinMasc;
	ticket_n_utilizado **tickets;
	int topo;
} cont_t;
/*----------------------------------------------*/

cont_t *cont_cria()
{
	cont_t *cont;				   /* cria contabilidade */
	cont = malloc(sizeof(cont_t)); /* aloca memoria para o TAD cont */
	cont->tickets = malloc(sizeof(ticket_n_utilizado) * 150);
	if (cont == NULL)
	{
		printf("Falha fatal. Impossível alocar memória.\n");
		return NULL;
	}
	cont->dinRef = 0;  /* inicializa o dinheiro gasto com refeições */
	cont->dinMasc = 0; /* inicializa o dinheiro gasto com mascaras */
	cont->topo = -1;   /* inicializa o topo dos tickets em -1 */
	return cont;
}

cont_t *cont_destroi(cont_t *cont)
{
	free(cont->tickets);
	cont->tickets = NULL;
	return cont;
}

pessoa_t **pessoas_cria(int num)
{
	pessoa_t **pessoas;						  /* cria pessoas do ru */
	pessoas = malloc(sizeof(pessoa_t) * num); /* aloca memoria para o TAD de pessoas */
	if (pessoas == NULL)
	{
		printf("Falha fatal. Impossível alocar memória.\n");
		return NULL;
	}
	return pessoas;
}

pessoa_t **pessoas_destroi(pessoa_t **pessoas, int num)
{
	int i;
	for (i = 1; i <= num; i++)
	{
		free(pessoas[i]);
	}
	return NULL;
}

double geraDinheiro()
{ /* retorna R$1,30 ou R$3,80 com proporção de 60/40 */
	int v = rand() % 101;
	if (v <= 60)
	{
		return 1.3;
	}
	return 3.8;
}

void inicializaPessoas(pessoa_t **pessoas)
{
	int i;
	for (i = 1; i <= 1000; i = i + 1)
	{
		pessoas[i] = malloc(sizeof(pessoa_t)); /* aloca espaço */
		pessoa_t *pessoa = pessoas[i];
		pessoa->ticket = i;
		int v = rand() % 101;
		if (v <= 99)
		{
			pessoa->vacinado = 1;
		}
		else
		{
			pessoa->vacinado = 0;
		}
		int m = rand() % 101;
		if (m <= 97)
		{
			pessoa->de_mascara = 1;
		}
		else
		{
			pessoa->de_mascara = 0;
		}
		pessoa->dinheiro = geraDinheiro();
	}
}

void insereTicketsFila(fila_t *f)
{
	int i;
	for (i = 1; i <= 1000; i++)
	{
		queue(f, i);
	}
}

void ticketNaoUtilizado(cont_t *cont, int ticket, int motivo)
{
	cont->tickets[cont->topo + 1] = malloc(sizeof(ticket_n_utilizado)); /* aloca espaço na memoria */
	cont->tickets[cont->topo + 1]->ticket = ticket;						/* atribui ticket */
	cont->tickets[cont->topo + 1]->motivo = motivo;						/* atribui motivo */
	cont->topo = cont->topo + 1;
}

void retiraTicket(pessoa_t *pessoa)
{
	pessoa->ticket = -1;
}

void pagaRefeicao(pessoa_t *pessoa, cont_t *cont)
{
	pessoa->dinheiro = pessoa->dinheiro - 1.3;
	cont->dinRef = cont->dinRef + 1.3;
}

void pagaMascara(pessoa_t *pessoa, cont_t *cont)
{
	pessoa->dinheiro = pessoa->dinheiro - 2.5;
	pessoa->de_mascara = 1; /* AGORA ELA ESTÁ DE MASCARA */
	cont->dinMasc = cont->dinMasc + 2.5;
}

void todosRestantesNaoAtendidos(fila_t *fila_atendimentos, pessoa_t **pessoas, lista_t *lista_n_atendidos, cont_t *cont)
{
	int motivo = 1;
	while (!fila_vazia(fila_atendimentos))
	{
		int ticket;
		dequeue(fila_atendimentos, &ticket); /* pega o ticket de cada pessoa em ordem da fila */
		pessoa_t *pessoa = pessoas[ticket];
		lista_insere_inicio(lista_n_atendidos, ticket); /* INSERE NA LISTA DE NÃO ATENDIDOS */
		ticketNaoUtilizado(cont, ticket, motivo);
		retiraTicket(pessoa);
	}
}

void fiscalizacao(fila_t *fila_atendimentos, pessoa_t **pessoas, lista_t *lista_n_atendidos, pilha_t *p_mascaras, pilha_t *p_refeicoes, cont_t *cont)
{
	while (!fila_vazia(fila_atendimentos))
	{
		int ticket;
		dequeue(fila_atendimentos, &ticket); /* pega o ticket de cada pessoa em ordem da fila */

		pessoa_t *pessoa = pessoas[ticket];

		if (!pessoa->vacinado)
		{
			lista_insere_inicio(lista_n_atendidos, ticket); /* SE N ESTIVER VACINADA, NÃO SERÁ ATENDIDA */
			int motivo = 2;
			ticketNaoUtilizado(cont, ticket, motivo);
			retiraTicket(pessoa);
		}
		else
		{
			if (pessoa->de_mascara)
			{									  /*---------COM MASCARA---------*/
				int temComida = pop(p_refeicoes); /* VERIFICA SE TEM COMIDA */
				if (!temComida)
				{ /* CASO NÃO TENHA MAIS COMIDA */
					todosRestantesNaoAtendidos(fila_atendimentos, pessoas, lista_n_atendidos, cont);
					fila_destroi(fila_atendimentos);
					return;
				}
				if (pessoa->dinheiro > 1.29)
				{
					pagaRefeicao(pessoa, cont);
				}
				else
				{
					lista_insere_inicio(lista_n_atendidos, ticket); /* SE N TIVER DINHEIRO P/ A COMIDA, NÃO SERÁ ATENDIDA */
					int motivo = 4;
					ticketNaoUtilizado(cont, ticket, motivo);
					retiraTicket(pessoa);
				}
			}
			else
			{ /*---------SEM MASCARA----------*/
				if (pessoa->dinheiro >= 2.5)
				{									  /* CASO TENHA DINHEIRO PRA MASCARA */
					int temMascara = pop(p_mascaras); /* VERIFICA SE TEM MASCARA */
					if (!temMascara)
					{													/* CASO NÃO TENHA MASCARA */
						lista_insere_inicio(lista_n_atendidos, ticket); /* TICKET NAO SERA UTILIZADO */
						int motivo = 3;
						ticketNaoUtilizado(cont, ticket, motivo);
						retiraTicket(pessoa);
					}
					else
					{
						pagaMascara(pessoa, cont);
						queue(fila_atendimentos, ticket); /* REINSERIDO NO FINAL DA FILA */
					}
				}
				else
				{													/* SEM DINHEIRO PARA A MÁSCARA */
					lista_insere_inicio(lista_n_atendidos, ticket); /* SE N TIVER DINHEIRO P/ A MASCARA, NÃO SERÁ ATENDIDA */
					int motivo = 5;
					ticketNaoUtilizado(cont, ticket, motivo);
					retiraTicket(pessoa);
				}
			}
		}
	}
}

void cont_tickets_imprime(cont_t *cont)
{
	int i;
	/* 1. acabou a comida - 2. não vacinado - 3. sem mascara disponível - 4. sem dinheiro para refeição - 5. não tem dinheiro para a máscara */
	for (i = 0; i <= cont->topo; i++)
	{
		if (cont->tickets[i]->motivo == 1)
		{
			printf("TICKET %d NÃO UTILIZADO - MOTIVO: ACABOU A COMIDA\n", cont->tickets[i]->ticket);
		}
		else if (cont->tickets[i]->motivo == 2)
		{
			printf("TICKET %d NÃO UTILIZADO - MOTIVO: NÃO VACINADO\n", cont->tickets[i]->ticket);
		}
		else if (cont->tickets[i]->motivo == 3)
		{
			printf("TICKET %d NÃO UTILIZADO - MOTIVO: SEM MÁSCARAS DISPONÍVEIS\n", cont->tickets[i]->ticket);
		}
		else if (cont->tickets[i]->motivo == 4)
		{
			printf("TICKET %d NÃO UTILIZADO - MOTIVO: NÃO POSSUIA DINHEIRO PARA A REFEIÇÃO\n", cont->tickets[i]->ticket);
		}
		else if (cont->tickets[i]->motivo == 5)
		{
			printf("TICKET %d NÃO UTILIZADO - MOTIVO: NÃO POSSUIA DINHEIRO PARA A MÁSCARA\n", cont->tickets[i]->ticket);
		}
	}
}

void contabilidadeDoDia(cont_t *cont)
{
	cont_tickets_imprime(cont);
	printf("Número de tickets não utilizados: %d\n", cont->topo + 1);
	printf("Dinheiro arrecadado com refeições: R$ %.2f\n", cont->dinRef);
	printf("Dinheiro arrecadado com mascaras: R$ %.2f\n", cont->dinMasc);
	printf("TOTAL arrecadado: R$ %.2f\n", (cont->dinRef + cont->dinMasc));
}

void inicializaPilhas(pilha_t *masc, int qtdMasc, pilha_t *ref, int qtdRef)
{
	int i;
	if (qtdMasc >= qtdRef)
	{
		for (i = 1; i <= qtdMasc; i++)
		{
			push(masc, 1);
			if (i <= qtdRef)
				push(ref, 1);
		}
	}
	else
	{
		for (i = 1; i <= qtdRef; i++)
		{
			push(ref, 1);
			if (i <= qtdMasc)
				push(masc, 1);
		}
	}
}

int main()
{
	srand(time(NULL));
	cont_t *cont = cont_cria();
	int qtdMascaras = rand() % 101;
	int qtdRefeicoes = rand() % 101 + 900;
	pilha_t *p_mascaras = pilha_cria(qtdMascaras);	 /* 1. gera pilha de mascaras */
	pilha_t *p_refeicoes = pilha_cria(qtdRefeicoes); /* 2. gera pilha de refeições */
	inicializaPilhas(p_mascaras, qtdMascaras, p_refeicoes, qtdRefeicoes);

	pessoa_t **pessoas = pessoas_cria(1000); /* cria conjunto de mil pessoas */
	inicializaPessoas(pessoas);				 /* inicializa as pessoas de forma aleatoria */

	fila_t *fila_atendimentos = fila_cria();   /* 3. cria fila de atendimentos */
	lista_t *lista_n_atendidos = lista_cria(); /* 4. cria lista de tickets n atendidos */
	insereTicketsFila(fila_atendimentos);	   /* insere o ticket de todas as 1000 pessoas na fila */

	fiscalizacao(fila_atendimentos, pessoas, lista_n_atendidos, p_mascaras, p_refeicoes, cont); /* fiscalização seguindo a ordem de tickets da fila de atendimentos */

	contabilidadeDoDia(cont);
	printf("quantidade de refeições disponibilizadas: %d\n", qtdRefeicoes);
	printf("quantidade de refeições que sobraram: %d\n", pilha_tamanho(p_refeicoes));
	printf("quantidade de mascaras disponibilizadas: %d\n", qtdMascaras);
	printf("quantidade de mascaras que sobraram: %d\n", pilha_tamanho(p_mascaras));

	cont = cont_destroi(cont);				  /* destroi tickets da conrtabilidade */
	p_mascaras = pilha_destroi(p_mascaras);	  /* destroi pilha de mascaras */
	p_refeicoes = pilha_destroi(p_refeicoes); /* destroi pilha de refeições */
	pessoas = pessoas_destroi(pessoas, 1000); /* destroi pessoas */
	if (fila_atendimentos)
		fila_atendimentos = fila_destroi(fila_atendimentos);
	lista_n_atendidos = lista_destroi(lista_n_atendidos); /* destroi lista de n atendidos */

	return 0;
}
