#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define INCREMENTOS 1000000

int contador = 0;

void *incrementar(void *arg) {
	for (int i = 0; i < INCREMENTOS; i++) {
		contador++;
	}
	return NULL;
}

int main() {
	pthread_t threads[NUM_THREADS];
	printf("Contador inicial: %d\n", contador);

	// Criar as threads
	for (int i = 0; i < NUM_THREADS; i++) {
		int erro = pthread_create(
			&threads[i],
			NULL,
			incrementar,
			NULL
		);
		if (erro != 0) {
			fprintf(stderr, "Erro ao criar thread\n");
			exit(EXIT_FAILURE);
		}
	}

	// Esperar todas as threads
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	printf("\nContador final: %d\n", contador);
	return 0;
}