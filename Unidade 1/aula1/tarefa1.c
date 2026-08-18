#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void preencher_matriz(double *A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i * n + j] = (double)(i + j + 1);
        }
    }
}

void preencher_vetor(double *x, int n) {
    for (int i = 0; i < n; i++) {
        x[i] = (double)(i + 1);
    }
}

void multiplicar_por_linhas(double *A, double *x, double *y, int n) {
    for (int i = 0; i < n; i++) {
        y[i] = 0.0;
        for (int j = 0; j < n; j++) {
            y[i] += A[i * n + j] * x[j];
        }
    }
}

void multiplicar_por_colunas(double *A, double *x, double *y, int n) {
    for (int i = 0; i < n; i++) {
        y[i] = 0.0;
    }

    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            y[i] += A[i * n + j] * x[j];
        }
    }
}

int main(int argc, char *argv[]) {
    int n;
    double *A, *x, *y;
    clock_t inicio, fim;
    double tempo_linhas, tempo_colunas;

    if (argc != 2) {
        printf("Uso: %s <tamanho>", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);

    if (n <= 0) {
        printf("Tamanho invalido.\n");
        return 1;
    }

    A = (double *)malloc((size_t)n * n * sizeof(double));
    x = (double *)malloc((size_t)n * sizeof(double));
    y = (double *)malloc((size_t)n * sizeof(double));

    if (A == NULL || x == NULL || y == NULL) {
        printf("Erro ao alocar memoria.\n");
        free(A);
        free(x);
        free(y);
        return 1;
    }

    preencher_matriz(A, n);
    preencher_vetor(x, n);

    inicio = clock();
    multiplicar_por_linhas(A, x, y, n);
    fim = clock();
    tempo_linhas = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\nTempo por linhas: %.6f segundos\n", tempo_linhas);

    preencher_matriz(A, n);
    preencher_vetor(x, n);

    inicio = clock();
    multiplicar_por_colunas(A, x, y, n);
    fim = clock();
    tempo_colunas = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("Tempo por colunas: %.6f segundos\n\n", tempo_colunas);

    free(A);
    free(x);
    free(y);

    return 0;
}