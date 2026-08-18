#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função auxiliar para calcular diferença de tempo
double tempo_decorrido(struct timespec inicio, struct timespec fim) {
    return (fim.tv_sec - inicio.tv_sec) + (fim.tv_nsec - inicio.tv_nsec) / 1e9;
}

// Laço 1: Inicialize um vetor com um cálculo simples
void inicializar_vetor(double *v, int n) {
    for (int i = 0; i < n; i++) {
        v[i] = (double)(i + 1) * 1.5;
    }
}

// Laço 2: Some elementos de forma acumulativa (com dependência)
double somar_acumulativo(double *v, int n) {
    double soma = 0.0;
    for (int i = 0; i < n; i++) {
        soma += v[i];  // Dependência: soma depende do valor anterior
    }
    return soma;
}

// Laço 3: Quebre dependência usando múltiplas variáveis
double somar_sem_dependencia(double *v, int n) {
    double soma1 = 0.0;
    double soma2 = 0.0;
    double soma3 = 0.0;
    double soma4 = 0.0;
    
    // Processa 4 elementos por iteração, usando 4 acumuladores
    int i;
    for (i = 0; i < n - 3; i += 4) {
        soma1 += v[i];
        soma2 += v[i + 1];
        soma3 += v[i + 2];
        soma4 += v[i + 3];
    }
    
    // Processa elementos restantes
    while (i < n) {
        soma1 += v[i];
        i++;
    }
    
    return soma1 + soma2 + soma3 + soma4;
}

int main(int argc, char *argv[]) {
    int n;
    double *v;
    struct timespec inicio, fim;
    double tempo_init, tempo_acumulativo, tempo_sem_dep;
    double resultado1, resultado2;
    
    if (argc != 2) {
        printf("Uso: %s <tamanho_vetor>\n", argv[0]);
        return 1;
    }
    
    n = atoi(argv[1]);
    
    if (n <= 0) {
        printf("Tamanho invalido.\n");
        return 1;
    }
    
    v = (double *)malloc((size_t)n * sizeof(double));
    if (v == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }
    
    // Laço 1: Inicializar vetor
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    inicializar_vetor(v, n);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    tempo_init = tempo_decorrido(inicio, fim);
    
    // Laço 2: Soma acumulativa (com dependência)
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    resultado1 = somar_acumulativo(v, n);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    tempo_acumulativo = tempo_decorrido(inicio, fim);
    
    // Reinicializar vetor
    inicializar_vetor(v, n);
    
    // Laço 3: Soma sem dependência (múltiplas variáveis)
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    resultado2 = somar_sem_dependencia(v, n);
    clock_gettime(CLOCK_MONOTONIC, &fim);
    tempo_sem_dep = tempo_decorrido(inicio, fim);
    
    printf("Tamanho do vetor: %d\n", n);
    printf("================================================\n");
    printf("Tempo inicializar vetor:      %.9f segundos\n", tempo_init);
    printf("Tempo soma acumulativa:       %.9f segundos\n", tempo_acumulativo);
    printf("Tempo soma sem dependencia:   %.9f segundos\n", tempo_sem_dep);
    printf("================================================\n");
    
    // Verificar se os resultados são iguais (com tolerância para ponto flutuante)
    double tolerancia = 1e-9;
    if (!(resultado1 - resultado2 < tolerancia && resultado2 - resultado1 < tolerancia)) {
        printf("Resultados DIFERENTES: Houve erro na computacao.\n");
        printf("Diferenca: %.15e\n", resultado1 - resultado2);
    }
    
    free(v);
    return 0;
}