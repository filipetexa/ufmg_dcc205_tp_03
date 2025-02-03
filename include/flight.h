#ifndef FLIGHT_H
#define FLIGHT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ORIGEM 4   // Código IATA de 3 letras + '\0'
#define MAX_DESTINO 4  // Código IATA de 3 letras + '\0'

/**
 * Estrutura que representa um voo.
 */
typedef struct {
    char origem[MAX_ORIGEM];     // Aeroporto de origem (ex: "DEN")
    char destino[MAX_DESTINO];   // Aeroporto de destino (ex: "ORD")
    float preco;                 // Preço da passagem
    int assentos;                // Número de assentos disponíveis
    time_t partida;              // Data e hora de partida (convertido para segundos)
    time_t chegada;              // Data e hora de chegada (convertido para segundos)
    int paradas;                 // Número de paradas
    int duracao;                 // Duração total do voo em segundos (chegada - partida)
} Flight;

/**
 * Lê os voos do arquivo de entrada e os armazena em um vetor.
 * @param arquivo Ponteiro para o arquivo de entrada (stdin).
 * @param voos Ponteiro para o vetor de voos (será alocado dentro da função).
 * @param n Ponteiro para armazenar o número total de voos lidos.
 */
void carregar_voos(FILE *arquivo, Flight **voos, int *n);

/**
 * Converte uma string de data e hora para time_t.
 * @param datetime String no formato ISO 8601 (ex: "2022-11-01T17:54:00.000-06:00").
 * @return Tempo convertido em segundos desde a época Unix.
 */
time_t converter_datetime(const char *datetime);

/**
 * Exibe a lista de voos (usado para depuração).
 * @param voos Vetor de voos.
 * @param n Número de voos no vetor.
 */
void exibir_voos(Flight *voos, int n);


#endif // FLIGHT_H
