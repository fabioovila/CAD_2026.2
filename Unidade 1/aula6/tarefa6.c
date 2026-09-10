#include <stdio.h>
#include <omp.h>

// Função que vamos integrar: f(x) = x^2
// O resultado exato da integral de 0 a 1 é 1/3 (aproximadamente 0.333333)
double f(double x) {
    return x * x;
}

int main() {
    double a = 0.0;
    double b = 1.0;
    
    // Vetor com diferentes quantidades de subdivisões (n) para testar
    int n_valores[] = {1000, 10000, 100000, 10000000};
    int num_testes = 4;

    printf("Resultados da Integral (Metodo do Trapezio)\n");
    printf("===========================================\n\n");

    for (int t = 0; t < num_testes; t++) {
        int n = n_valores[t];
        double h = (b - a) / n;
        double inicio, fim;
        double integral;

        printf("--- Teste com n = %d subdivisoes ---\n", n);

        // 1. Sequencial
        integral = (f(a) + f(b)) / 2.0;
        inicio = omp_get_wtime();
        for (int i = 1; i < n; i++) {
            integral += f(a + i * h);
        }
        integral *= h;
        fim = omp_get_wtime();
        printf("Sequencial:       Resultado = %.6f | Tempo = %f s\n", integral, fim - inicio);

        // 2. Paralela SEM tratamento (Condicao de corrida)
        integral = (f(a) + f(b)) / 2.0;
        inicio = omp_get_wtime();
        #pragma omp parallel for
        for (int i = 1; i < n; i++) {
            integral += f(a + i * h); // Erro aqui!
        }
        integral *= h;
        fim = omp_get_wtime();
        printf("Sem tratamento:   Resultado = %.6f | Tempo = %f s\n", integral, fim - inicio);

        // 3. Paralela com CRITICAL
        integral = (f(a) + f(b)) / 2.0;
        inicio = omp_get_wtime();
        #pragma omp parallel for
        for (int i = 1; i < n; i++) {
            double calc_local = f(a + i * h);
            #pragma omp critical
            {
                integral += calc_local;
            }
        }
        integral *= h;
        fim = omp_get_wtime();
        printf("Com Critical:     Resultado = %.6f | Tempo = %f s\n", integral, fim - inicio);

        // 4. Paralela com ATOMIC
        integral = (f(a) + f(b)) / 2.0;
        inicio = omp_get_wtime();
        #pragma omp parallel for
        for (int i = 1; i < n; i++) {
            double calc_local = f(a + i * h);
            #pragma omp atomic
            integral += calc_local;
        }
        integral *= h;
        fim = omp_get_wtime();
        printf("Com Atomic:       Resultado = %.6f | Tempo = %f s\n", integral, fim - inicio);

        // 5. Paralela com REDUCTION
        integral = (f(a) + f(b)) / 2.0;
        inicio = omp_get_wtime();
        #pragma omp parallel for reduction(+:integral)
        for (int i = 1; i < n; i++) {
            integral += f(a + i * h);
        }
        integral *= h;
        fim = omp_get_wtime();
        printf("Com Reduction:    Resultado = %.6f | Tempo = %f s\n\n", integral, fim - inicio);
    }

    return 0;
}