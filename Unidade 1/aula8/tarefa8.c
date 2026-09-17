#include <stdio.h>
#include <stdlib.h>
//#include <omp.h>

#define N 100000

int main() {
    double *f = (double *)malloc(N * sizeof(double));
    double *derivada = (double *)malloc(N * sizeof(double));
    double integral = 0.0;
    double h = 0.001; // Passo 

    // 1. Inicialização do vetor (f(x) = x^2) - Sequencial ou Paralelo simples
    for (int i = 0; i < N; i++) {
        f[i] = (i * h) * (i * h); 
    }

    // Habilitando o paralelismo aninhado (nested) crucial para a tarefa
    omp_set_nested(1);

    // Iniciando a região paralela de seções (Paralelismo de Tarefas)
    #pragma omp parallel sections num_threads(2)
    {
        // ==========================================
        // TAREFA A: Cálculo da Integral 
        // ==========================================
        #pragma omp section
        {
            // Criando uma nova equipe de 2 threads dentro da seção
            #pragma omp parallel num_threads(2)
            {
                // Paralelismo de Dados com redução
                #pragma omp for reduction(+:integral)
                for (int i = 0; i < N - 1; i++) {
                    integral += ((f[i] + f[i+1]) / 2.0) * h;
                }

                // Apenas uma thread desta equipe imprime o resultado
                #pragma omp single
                {
                    printf("[Integral] Concluída por 1 thread da equipe. Resultado: %f\n", integral);
                }
            }
        }

        // ==========================================
        // TAREFA B: Cálculo da Derivada
        // ==========================================
        #pragma omp section
        {
            // Criando outra equipe de 2 threads dentro desta seção
            #pragma omp parallel num_threads(2)
            {
                // Paralelismo de Dados
                #pragma omp for
                for (int i = 1; i < N - 1; i++) {
                    derivada[i] = (f[i+1] - f[i-1]) / (2.0 * h);
                }

                // Apenas uma thread desta equipe imprime o aviso
                #pragma omp single
                {
                    printf("[Derivada] Vetor calculado com sucesso por 1 thread da equipe!\n");
                }
            }
        }
    }

    free(f);
    free(derivada);
    return 0;
}