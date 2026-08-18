#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_PROCESSOS 4
#define INCREMENTOS 1000000

int contador = 0;


int main() {
	printf("Contador inicial: %d\n", contador);
	for (int i = 0; i < NUM_PROCESSOS; i++) {
		pid_t pid = fork();
		if (pid < 0) {
			perror("Erro ao criar processo");
			exit(EXIT_FAILURE);
		}
		if (pid == 0) {
			// Código executado pelo processo filho
			for (int j = 0; j < INCREMENTOS; j++) {
				contador++;
			}
			printf("Filho %d: contador = %d\n", getpid(), contador);
			exit(EXIT_SUCCESS);
		}
	}

	// Processo pai espera todos os filhos
	for (int i = 0; i < NUM_PROCESSOS; i++) {
		wait(NULL);
	}
	printf("\nContador no processo pai: %d\n", contador);
	return 0;
}