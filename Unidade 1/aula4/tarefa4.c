#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Função f(x) cuja integral queremos calcular
// Exemplo: f(x) = x^2 (pode ser alterada para qualquer outra função)
double f(double x) {
    return x * x;
}

// Implementação sequencial baseada estritamente na imagem fornecida
double calcular_integral_trapezio(double a, double b, int n) {
    double h = (b - a) / n; // Largura de cada subdivisão
    
    // x_0 é o limite inferior 'a' e x_n é o limite superior 'b'
    double x_0 = a;
    double x_n = b;

    // Tradução literal da parte interna dos colchetes: f(x_0)/2 + f(x_n)/2
    double soma_colchetes = (f(x_0) / 2.0) + (f(x_n) / 2.0);

    // Somatório de i=1 até n-1 de f(x_i)
    for (int i = 1; i < n; i++) {
        double x_i = a + i * h;
        soma_colchetes += f(x_i);
    }

    // Multiplicação final externa por h: h * [...]
    double integral = h * soma_colchetes;
    
    return integral;
}

int main() {
    const double a = 0.0;
    const double b = 1.0;
    const int n = 10000;

    printf("--- Cálculo de Integral Definida (Fórmula Estrita da Imagem) ---\n");
    printf("Intervalo: [%.1f, %.1f] | Subdivisões: %d\n", a, b, n);

    double resultado = calcular_integral_trapezio(a, b, n);

    printf("\nResultado aproximado da integral: %.10f\n", resultado);

    return 0;
}


/*Para implementar o somatório do método do trapézio, é necessário implementar um loop que aumenta iteradamente o valor de uma variável. Como a variável soma_colchetes é compartilhada, pode ocorrer condição de corrida se as threads acessarem e modificarem essa variável simultaneamente. Além disso, na soma de todos os resultados das threads, deve haver uma garantia de que essa operação seja feita de forma segura, evitando que múltiplas threads tentem atualizar a mesma variável ao mesmo tempo. 

Tentar proteger essa variável compartilhada a cada iteração do loop geraria um custo de sincronização (overhead) altíssimo, tornando o programa paralelo mais lento que o sequencial. O método de redução é uma técnica que pode ser utilizada para resolver esse problema, onde cada thread calcula uma parte da soma em uma variável privada (aumentando a granularidade do trabalho) e, ao final, os resultados parciais são combinados de forma segura, evitando inconsistência e perda de desempenho quando o problema crescer.*/