#ifndef LISTA_VOOS_H
#define LISTA_VOOS_H

#include "voo.h"

typedef struct {
    Voo* voos;           // Vetor dinâmico de voos
    int tamanho;         // Número atual de voos na lista
    int capacidade;      // Capacidade máxima atual do vetor
} ListaVoos;

// Funções para manipular a Lista de Voos

/**
 * Inicializa uma nova lista de voos.
 * capacidade_inicial Capacidade inicial da lista.
 * Lista de voos inicializada.
 */
ListaVoos* criar_lista_voos(int capacidade_inicial);

/**
 * Adiciona um novo voo à lista de voos.
 * lista Ponteiro para a lista de voos.
 * voo Voo a ser adicionado.
 * 0 em caso de sucesso, -1 em caso de falha.
 */
int adicionar_voo(ListaVoos* lista, Voo voo);

/**
 * Libera a memória alocada para a lista de voos.
 * lista Ponteiro para a lista de voos.
 */
void liberar_lista_voos(ListaVoos* lista);

#endif // LISTA_VOOS_H
