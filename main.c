// Nome: Luiz Henrique Viras Nobre

#include <stdio.h>
#include <string.h>

#define TAMANHO_STRING 100

// Vetor predefinido
char vetor[TAMANHO_STRING * 1000]; // Tamanho do vetor predefinido

char *ponteiro = vetor; // Ponteiro para a próxima posição disponível no vetor

// Função para alocar memória a partir do vetor predefinido
void *aloca(size_t tamanho) {
	if ((ponteiro + tamanho) <= (vetor + sizeof(vetor))) {
		void *endereco = (void *)ponteiro;
		ponteiro += tamanho;
		return endereco;
	} 
	else {
		fprintf(stderr, "Erro: Sem memória disponível.\n");
		return NULL;
	}
}

// Função para liberar memória alocada
void libera(void *endereco) {

}

// Estrutura para armazenar a tupla
typedef struct {
	int chave;
	char *string;
} Tupla;

// Estrutura para os nós da lista duplamente encadeada
typedef struct No {
	Tupla tupla;
	struct No *proximo;
	struct No *anterior;
} No;

// Estrutura para a lista duplamente encadeada
typedef struct {
	No *inicio;
} ListaDuplamenteEncadeada;

// Função para criar um novo nó
No *criar_no(int chave, const char *string) {
	No *novo_no = (No *)aloca(sizeof(No));
	if (novo_no) {
		novo_no->tupla.chave = chave;
		novo_no->tupla.string = (char *)aloca(strlen(string) + 1);
		if (novo_no->tupla.string) {
		strcpy(novo_no->tupla.string, string);
		novo_no->proximo = NULL;
		novo_no->anterior = NULL;
		} 
		else {
			printf("Erro ao alocar memória para a string.\n");
			libera(novo_no);
			novo_no = NULL;
		}
	}
	return novo_no;
}

// Função para inserir um novo elemento na lista
void inserir_elemento(ListaDuplamenteEncadeada *lista, int chave, const char *string) {
	No *novo_no = criar_no(chave, string);
	if (!novo_no) {
		printf("Erro ao criar o novo elemento.\n\n");
		return;
	}
	
	No *atual = lista->inicio;
	No *anterior = NULL;
	
	while (atual && atual->tupla.chave < chave) {
		anterior = atual;
		atual = atual->proximo;
	}
	
	if (anterior) {
		anterior->proximo = novo_no;
		novo_no->anterior = anterior;
	} 
	else {
		lista->inicio = novo_no;
	}
	
	if (atual) {
		novo_no->proximo = atual;
		atual->anterior = novo_no;
	}
	printf("Elemento incluído com sucesso!\n\n");
}

// Função para alterar o valor da string associada a uma chave
void alterar_elemento(ListaDuplamenteEncadeada *lista, int chave, const char *nova_string) {
	No *atual = lista->inicio;
	
	while (atual && atual->tupla.chave != chave) {
		atual = atual->proximo;
	}
	
	if (atual) {
		libera(atual->tupla.string);
		atual->tupla.string = (char *)aloca(strlen(nova_string) + 1);
		if (atual->tupla.string) {
			strcpy(atual->tupla.string, nova_string);
			printf("Elemento alterado com sucesso!\n\n");
		} 
		else {
			printf("Erro ao alocar memória para a nova string.\n");
		}
	} 
	else {
		printf("Chave não encontrada.\n\n");
	}
}

// Função para excluir um elemento da lista
void excluir_elemento(ListaDuplamenteEncadeada *lista, int chave) {
	No *atual = lista->inicio;
	
	while (atual && atual->tupla.chave != chave) {
		atual = atual->proximo;
	}
	
	if (atual) {
		if (atual->anterior) {
			atual->anterior->proximo = atual->proximo;
		} 
		else {
			lista->inicio = atual->proximo;
		}
		
		if (atual->proximo) {
			atual->proximo->anterior = atual->anterior;
		}
		
		libera(atual->tupla.string);
		libera(atual);
		printf("Elemento excluído com sucesso!\n\n");
	} 
	else {
		printf("Chave não encontrada.\n\n");
	}
}

// Função para listar o conteúdo da lista
void listar_elementos(ListaDuplamenteEncadeada *lista) {
	No *atual = lista->inicio;
	
	if (atual) {
		printf("Conteúdo da lista:\n");
		
		while (atual) {
			printf("Chave: %d, String: %s\n", atual->tupla.chave, atual->tupla.string);
			atual = atual->proximo;
		}
	} 
	else {
		printf("Lista vazia.\n\n");
	}
}

// Função para liberar a memória ocupada pela lista
void liberar_lista(ListaDuplamenteEncadeada *lista) {
	No *atual = lista->inicio;
	while (atual) {
		No *prox = atual->proximo;
		libera(atual->tupla.string);
		libera(atual);
		atual = prox;
	}
}

int main() {
	ListaDuplamenteEncadeada lista;
	lista.inicio = NULL;
	
	int opcao;
	int chave;
	char string[TAMANHO_STRING];
	
	do {
		printf("Escolha uma opção:\n");
		printf("1. Incluir elemento\n");
		printf("2. Alterar elemento\n");
		printf("3. Excluir elemento\n");
		printf("4. Listar elementos\n");
		printf("0. Sair\n");
		scanf("%d", &opcao);
		
		switch (opcao) {
			case 1:
				printf("Informe a chave: ");
				scanf("%d", &chave);
				printf("Informe a string: ");
				scanf("%s", string);
				inserir_elemento(&lista, chave, string);
				break;
			
			case 2:
				printf("Informe a chave a ser alterada: ");
				scanf("%d", &chave);
				printf("Informe a nova string: ");
				scanf("%s", string);
				alterar_elemento(&lista, chave, string);
				break;
			
			case 3:
				printf("Informe a chave a ser excluída: ");
				scanf("%d", &chave);
				excluir_elemento(&lista, chave);
				break;
			
			case 4:
				listar_elementos(&lista);
				printf("\n");
				break;
			
			case 0:
				printf("Saindo...\n");
				break;
				
			default:
				printf("Opção inválida!\n\n");
				break;
		}
	} while (opcao != 0);
	
	liberar_lista(&lista);
	
	return 0;
}
