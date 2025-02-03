#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flight.h"
#include "query.h"

/**
 * Compara dois voos com base no critério definido pelo trigrama.
 * @param a Ponteiro para o primeiro voo.
 * @param b Ponteiro para o segundo voo.
 * @param trigrama String de três caracteres indicando a ordem dos critérios (ex: "psd").
 * @return Valor negativo se a < b, positivo se a > b, 0 se forem iguais.
 */
int comparar_voos(const Flight *a, const Flight *b, const char *trigrama);

/**
 * Ordena os voos com base no critério do trigrama da consulta.
 * @param voos Vetor de voos a serem ordenados.
 * @param total_voos Número total de voos no vetor.
 * @param trigrama String de três caracteres indicando a ordem dos critérios.
 * @param max_resultados Número máximo de voos a serem retornados após a ordenação.
 */
void ordenar_voos(Flight *voos, int total_voos, const char *trigrama, int max_resultados);

#endif // SORT_H
