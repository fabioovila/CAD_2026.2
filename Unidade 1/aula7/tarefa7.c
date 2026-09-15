#include <stdio.h>
#include <omp.h>

// Função auxiliar para testar se um número é primo
int eh_primo(int num) {
    if (num < 2) return 0;
    for (int d = 2; d * d <= num; d++) {
        if (num % d == 0) return 0;
    }
    return 1;
}

// =================================================================
// PARTE 1: Testes de Escopo de Variáveis (Gerenciamento de Memória)
// =================================================================
void executar_testes_memoria(int n) {
    int contador;

    printf("--- PARTE 1: TESTES DE ESCOPO DE VARIAVEIS (n = %d) ---\n\n", n);

    // 1. Apenas private
    contador = 0;
    #pragma omp parallel for private(contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("1. private:       %d (Fica 0, original nao atualizada)\n", contador);

    // 2. Apenas firstprivate
    contador = 0;
    #pragma omp parallel for firstprivate(contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("2. firstprivate:  %d (Fica 0 pelo mesmo motivo)\n", contador);

    // 3. Apenas lastprivate
    contador = 0;
    #pragma omp parallel for lastprivate(contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("3. lastprivate:   %d (Lixo de memoria, comecou sem xerox)\n", contador);

    // 4. firstprivate e lastprivate
    contador = 0;
    #pragma omp parallel for firstprivate(contador) lastprivate(contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("4. first + last:  %d (Incompleto, pega so a ultima thread)\n", contador);

    // 5. default(none) com reduction
    contador = 0;
    #pragma omp parallel for default(none) shared(n) reduction(+:contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("5. default(none): %d (Obriga a declarar, correto com reduction)\n", contador);

    // 6. Paralela utilizando reduction
    contador = 0;
    #pragma omp parallel for reduction(+:contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("6. reduction:     %d (Correto! Vencedora!)\n\n", contador);
}

// =================================================================
// PARTE 2: Testes de Escalonamento (Desempenho e Divisão de Carga)
// =================================================================
void executar_testes_escalonamento(int n) {
    int contador;
    double inicio, fim;

    printf("--- PARTE 2: TESTES DE ESCALONAMENTO (n = %d) ---\n\n", n);

    // 1. Estratégia STATIC
    contador = 0;
    inicio = omp_get_wtime();
    #pragma omp parallel for reduction(+:contador) schedule(static)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    fim = omp_get_wtime();
    printf("1. STATIC:  Tempo = %f s | Primos = %d\n", fim - inicio, contador);

    // 2. Estratégia DYNAMIC
    contador = 0;
    inicio = omp_get_wtime();
    #pragma omp parallel for reduction(+:contador) schedule(dynamic)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    fim = omp_get_wtime();
    printf("2. DYNAMIC: Tempo = %f s | Primos = %d\n", fim - inicio, contador);

    // 3. Estratégia GUIDED
    contador = 0;
    inicio = omp_get_wtime();
    #pragma omp parallel for reduction(+:contador) schedule(guided)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    fim = omp_get_wtime();
    printf("3. GUIDED:  Tempo = %f s | Primos = %d\n", fim - inicio, contador);

    // 4. Estratégia AUTO
    contador = 0;
    inicio = omp_get_wtime();
    #pragma omp parallel for reduction(+:contador) schedule(auto)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    fim = omp_get_wtime();
    printf("4. AUTO:    Tempo = %f s | Primos = %d\n", fim - inicio, contador);

    // 5. Estratégia RUNTIME
    contador = 0;
    inicio = omp_get_wtime();
    #pragma omp parallel for reduction(+:contador) schedule(runtime)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    fim = omp_get_wtime();
    printf("5. RUNTIME: Tempo = %f s | Primos = %d\n", fim - inicio, contador);
}

// =================================================================
// FUNÇÃO PRINCIPAL
// =================================================================
int main() {
    // Escolha um valor razoável para testes.
    // Dica: Aumente para 10.000.000 para evidenciar a diferença de tempo no Parte 2!
    int n = 100000; 
    
    printf("Iniciando bateria completa de testes OpenMP...\n\n");
    
    executar_testes_memoria(n);

    // melhor versão: reduction, farei os teste de escalonamento usando essa cláusula

    executar_testes_escalonamento(n);
    
    printf("\nBateria de testes finalizada com sucesso!\n");
    return 0;
}