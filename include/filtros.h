#ifndef FILTROS_H
#define FILTROS_H

#include "voo.h"

// Estrutura para representar nós da árvore sintática
typedef struct NoFiltro {
    char operador;         // Operador lógico: '&&'
    char atributo[10];     // Atributo (ex: "org", "prc", "dur")
    char operador_rel;     // Operador relacional: '=', '<', '>'
    union {
        float valor_num;   // Valor numérico (ex: preço, duração)
        char valor_str[10];// Valor string (ex: origem/destino)
    };
    struct NoFiltro* esq;  // Subárvore esquerda
    struct NoFiltro* dir;  // Subárvore direita
} NoFiltro;

/**
 * Constrói a árvore de filtros a partir da string de entrada.
 * @param filtros_str String contendo os filtros.
 * @return Ponteiro para a raiz da árvore sintática.
 */
NoFiltro* construir_arvore_filtros(const char* filtros_str);

/**
 * Avalia se um voo atende aos filtros da consulta.
 * @param voo Voo a ser avaliado.
 * @param filtros Raiz da árvore sintática de filtros.
 * @return 1 se o voo atende aos filtros, 0 caso contrário.
 */
int avaliar_filtros(const Voo* voo, const NoFiltro* filtros);

/**
 * Libera a memória da árvore sintática de filtros.
 * @param filtros Raiz da árvore sintática.
 */
void liberar_filtros(NoFiltro* filtros);

#endif // FILTROS_H
