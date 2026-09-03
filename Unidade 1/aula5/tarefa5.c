#include <stdio.h>
#include <stdbool.h>
#include <omp.h>

// gcc -fopenmp tarefa5.c -o tarefa5

bool is_prime(int x) {
    if (x <= 1) {
        return false;
    }
    for (int y = 2; y * y <= x; y++) {
        if (x % y == 0) {
            return false;
        }
    }

    return true;
}

int main() {
    double tempo_inicio = omp_get_wtime();
    int n = 1000000;
    int quantidade_primos = 0;

    #pragma omp parallel for
    for (int x = 2; x <= n; x++) {
        if (is_prime(x)) {
            quantidade_primos++;
        }
    }

    printf("Existem %d numeros primos entre 2 e %d.\n", quantidade_primos, n);
    double tempo_fim = omp_get_wtime();
    printf("Tempo de execucao: %f segundos.\n", tempo_fim - tempo_inicio);

    return 0;
}

/*
    Com n = 10000, o tempo médio de execução da versão sequencial foi 0.000350 segundos
    Já na versão paralelizada, o resultado sempre dava mais tempo de execução, chegando até a 0.002112 segundos

    A versão paralelizada usa somente a diretiva #pragma omp parallel for. Para esse valor
    de n, ela foi mais lenta porque o custo de criação, gerenciamento e sincronização das
    threads foi maior que o benefício de dividir um laço relativamente pequeno.

    O resultado esperado é 1229 números primos, mas o uso direto de quantidade_primos++
    por várias threads cria uma condição de corrida. Duas threads podem ler o mesmo valor
    do contador e, ao incrementá-lo, uma atualização pode sobrescrever a outra. Por isso,
    o resultado pode coincidir com o sequencial em algumas execuções, mas não é garantido
    e pode ficar incorreto, especialmente com valores maiores de n.

    Esse exemplo mostra dois desafios iniciais da programação paralela: preservar a correção
    quando há dados compartilhados e distribuir uma carga suficiente entre as threads para
    compensar o custo da paralelização. Além disso, os testes de primalidade podem ter custos
    diferentes, o que pode fazer algumas threads terminarem antes de outras.
*/