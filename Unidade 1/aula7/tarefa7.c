#include <stdio.h>
#include <omp.h>

// Função auxiliar clássica para testar se é primo
int eh_primo(int num) {
    if (num < 2) return 0;
    for (int d = 2; d * d <= num; d++) {
        if (num % d == 0) return 0;
    }
    return 1;
}

void teste_private(int n) {
    int contador = 0;
    #pragma omp parallel for private(contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("1. private:       %d (Fica 0, pois a original nao eh atualizada)\n", contador);
}

void teste_firstprivate(int n) {
    int contador = 0;
    #pragma omp parallel for firstprivate(contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("2. firstprivate:  %d (Fica 0 pelo mesmo motivo)\n", contador);
}

void teste_lastprivate(int n) {
    int contador = 0;
    #pragma omp parallel for lastprivate(contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("3. lastprivate:   %d (Lixo de memoria, comecou sem xerox)\n", contador);
}

void teste_first_last(int n) {
    int contador = 0;
    #pragma omp parallel for firstprivate(contador) lastprivate(contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("4. first + last:  %d (Incompleto, pega so a contagem da ultima thread)\n", contador);
}

void teste_default_none(int n) {
    int contador = 0;
    // O default(none) OBRIGA a declarar 'n' e 'contador'.
    // Usamos reduction para que essa versao de o resultado correto.
    #pragma omp parallel for default(none) shared(n) reduction(+:contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("5. default(none): %d (Obriga a declarar tudo explicitamente)\n", contador);
}

void teste_reduction(int n) {
    int contador = 0;
    #pragma omp parallel for reduction(+:contador)
    for (int i = 2; i <= n; i++) {
        if (eh_primo(i)) contador++;
    }
    printf("6. reduction:     %d (Correto!)\n", contador);
}

int main() {
    int n = 100000;
    printf("Buscando primos de 2 ate %d usando OpenMP...\n\n", n);
    
    teste_private(n);
    teste_firstprivate(n);
    teste_lastprivate(n);
    teste_first_last(n);
    teste_default_none(n);
    teste_reduction(n);
    
    return 0;
}