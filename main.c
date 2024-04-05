#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <string.h>
#include "tp.h"

int main(int argc, char *argv[]) {

    if(argc < 3) {
        printf("Execução incorreta!\n");
        printf("Digite: ./tp2 <estratégia> entrada.txt\n");
        exit(1);
    }

    int opt = atoi(argv[1]);
    char *input = argv[2];

    // Uso das funções getrusage e gettimeofday para medir o tempo gasto pela aplicação:
    struct rusage usage_inicio, usage_fim;  // Estruturas para a função getrusage.
    struct timeval tempo_inicio, tempo_fim; // Estruturas para a função gettimeofday.

    // Verificando se foi digitado uma opção inválida:
    if(opt != 1 && opt != 2) {
        printf("Opção inválida!\n");
        printf("Digite estratégia 1 ou 2.\n");
        exit(1);
    }

    getrusage(RUSAGE_SELF, &usage_inicio);
    gettimeofday(&tempo_inicio, NULL);

    LerArquivo(input, opt);

    getrusage(RUSAGE_SELF, &usage_fim);
    gettimeofday(&tempo_fim, NULL);

    // Calcular o tempo de usuário:
    float usuario = (usage_fim.ru_utime.tv_sec - usage_inicio.ru_utime.tv_sec) + 1e-6* (usage_fim.ru_utime.tv_usec - usage_inicio.ru_utime.tv_usec);

    // Calcular o tempo de sistema:
    float sistema = (usage_fim.ru_stime.tv_sec - usage_inicio.ru_stime.tv_sec) + 1e-6* (usage_fim.ru_stime.tv_usec - usage_inicio.ru_stime.tv_usec);

    // Calcular o tempo decorrido no relógio:
    float tempo = (tempo_fim.tv_sec - tempo_inicio.tv_sec) +  1e-6 *(tempo_fim.tv_usec - tempo_inicio.tv_usec);

    printf("User: %0.6fs\n", usuario);
    printf("System: %0.6fs\n", sistema);
    printf("Time: %0.6fs\n", tempo);

    return 0;
}