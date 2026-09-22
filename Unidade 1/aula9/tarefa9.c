#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define MAX_NOME 100
#define NUM_ARQUIVOS 5

typedef struct No {
    int arquivo[MAX_NOME];
    int tamanho_arquivo;
    struct No *proximo;
} No;

// Cria um novo nó e preenche o arquivo com números aleatórios
No* criar_no() {
    No *novo = (No *) malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro na alocacao de memoria.\n");
        exit(1);
    }
    // Define aleatoriamente o tamanho do arquivo
    novo->tamanho_arquivo = 1 + rand() % MAX_NOME;
    // Preenche o arquivo com algarismos de 0 a 9
    for (int i = 0; i < novo->tamanho_arquivo; i++) {
        novo->arquivo[i] = rand() % 10;
    }
    novo->proximo = NULL;
    return novo;
}

// Insere um novo arquivo no final da lista
void inserir(No **lista) {
    No *novo = criar_no();
    if (*lista == NULL) {
        *lista = novo;
        return;
    }
    No *atual = *lista;
    while (atual->proximo != NULL) {
        atual = atual->proximo;
    }
    atual->proximo = novo;
}

// Imprime os arquivos da lista
void imprimir_lista(No *lista) {
    No *atual = lista;
    int numero_arquivo = 1;
    while (atual != NULL) {
        printf("Arquivo %d (tamanho = %d):\n", numero_arquivo, atual->tamanho_arquivo);
        for (int i = 0; i < atual->tamanho_arquivo; i++) {
            printf("%d ", atual->arquivo[i]);
        }
        printf("\n\n");
        atual = atual->proximo;
        numero_arquivo++;
    }
}

// Libera a memória da lista
void liberar_lista(No *lista) {
    No *atual = lista;
    while (atual != NULL) {
        No *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
}

int main() {
    No *lista = NULL;
    srand(time(NULL));

    // A parte do código com a inicialização dos arquivos não precisa ser paralelizada
    for (int i = 0; i < NUM_ARQUIVOS; i++) {
        inserir(&lista);
    }
    imprimir_lista(lista);

    // 1. Inicializa o histograma global e a trava (lock)
    int histograma_global[10] = {0};
    omp_lock_t lock_histograma;
    omp_init_lock(&lock_histograma);

    // 2. Abre a região paralela para a equipe de threads
    #pragma omp parallel
    {
        // 3. A diretiva single garante que apenas UMA thread (o gerente) percorra a lista
        #pragma omp single
        {
            No *atual = lista;
            while (atual != NULL) {
                
                // 4. Cria uma unidade de trabalho (task) para cada arquivo na lista
                #pragma omp task
                {
                    // Usa um histograma local para minimizar a contenção do lock
                    int histograma_local[10] = {0};
                    
                    // Conta as ocorrências no arquivo atual
                    for (int i = 0; i < atual->tamanho_arquivo; i++) {
                        int digito = atual->arquivo[i];
                        histograma_local[digito]++;
                    }

                    // 5. Usa o lock explícito para garantir exclusão mútua no vetor compartilhado
                    omp_set_lock(&lock_histograma);
                    for (int i = 0; i < 10; i++) {
                        histograma_global[i] += histograma_local[i];
                    }
                    omp_unset_lock(&lock_histograma);
                }
                
                atual = atual->proximo;
            }
        } 
        // Existe uma barreira implícita no final da diretiva single que aguarda a conclusão
    }

    // 6. Destrói o lock após o uso
    omp_destroy_lock(&lock_histograma);

    // 7. Imprime o resultado final
    printf("--- Histograma Global ---\n");
    for (int i = 0; i < 10; i++) {
        printf("Algarismo %d: %d ocorrencias\n", i, histograma_global[i]);
    }

    liberar_lista(lista);
    return 0;
}