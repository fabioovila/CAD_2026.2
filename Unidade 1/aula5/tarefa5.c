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
    printf("Numero maximo de threadhs (fogoes) disponiveis: %d\n", omp_get_max_threads());
    int n = 1000000;
    int quantidade_primos = 0;
    
    double tempo_inicio = omp_get_wtime();
    #pragma omp parallel for reduction(+:quantidade_primos) schedule(dynamic, 1000)
    for (int x = 2; x <= n; x++) {
        if (is_prime(x)) {
            quantidade_primos++;
        }
    }
    double tempo_fim = omp_get_wtime();

    printf("Existem %d numeros primos entre 2 e %d.\n", quantidade_primos, n);
    printf("Tempo de execucao: %f segundos.\n", tempo_fim - tempo_inicio);

    return 0;
}

/*
    Com n = 10000, o tempo médio de execução da versão sequencial foi 0.000350 segundos.
    Na versão paralelizada, o tempo foi maior, chegando a 0.002112 segundos.

    Mesmo testando com valores de n bem maiores, não observei uma diferença significativa
    no tempo de execução entre as versões. Isso pode acontecer porque o tempo de criação,
    gerenciamento e sincronização das threads, além da distribuição dinâmica das tarefas,
    compensa o benefício de dividir o laço entre várias threads. O resultado também pode
    depender da quantidade de threads disponíveis e da carga de trabalho de outros processos
    no computador.

    O resultado esperado para n = 10000 é 1229 números primos. O uso direto de
    quantidade_primos++ por várias threads criaria uma condição de corrida: duas threads
    poderiam ler o mesmo valor do contador e, ao incrementá-lo, uma atualização poderia
    sobrescrever a outra. A cláusula reduction(+:quantidade_primos) evita esse problema,
    criando uma cópia privada do contador para cada thread e combinando os resultados ao
    final da região paralela.

    A cláusula schedule(dynamic, 1000) distribui os valores do laço em blocos de 1000
    iterações. Como os testes de primalidade podem ter custos diferentes, essa distribuição
    pode ajudar a equilibrar o trabalho entre as threads, mas também acrescenta custo de
    gerenciamento. Neste teste, portanto, a paralelização garantiu a correção do resultado,
    mas não produziu um ganho de tempo perceptível.
*/