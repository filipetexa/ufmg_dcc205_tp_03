#ifndef VO0_H
#define VO0_H

#include <time.h>

typedef struct
{
    char origem[4];  // Código IATA da origem
    char destino[4]; // Código IATA do destino
    float preco;     // Preço do voo
    int assentos;    // Número de assentos disponíveis
    time_t partida;  // Data e hora de partida (UTC em segundos)
    time_t chegada;  // Data e hora de chegada (UTC em segundos)
    int paradas;     // Número de paradas
    long duracao;    // Duração total do voo (em segundos)
} Voo;

// Funções básicas para a TAD Voo

/**
 * Cria um novo voo.
 * @param origem Código IATA da origem.
 * @param destino Código IATA do destino.
 * @param preco Preço do voo.
 * @param assentos Número de assentos disponíveis.
 * @param partida Data e hora de partida em UTC.
 * @param chegada Data e hora de chegada em UTC.
 * @param paradas Número de paradas.
 * @return Voo inicializado.
 */
Voo criar_voo(const char *origem, const char *destino, float preco, int assentos,
              time_t partida, time_t chegada, int paradas);

/**
 * Calcula a duração total do voo em segundos.
 * @param partida Hora de partida em segundos.
 * @param chegada Hora de chegada em segundos.
 * @return Duração em segundos.
 */
long calcular_duracao(time_t partida, time_t chegada);

#endif // VO0_H
