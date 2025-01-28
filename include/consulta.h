#ifndef CONSULTA_H
#define CONSULTA_H

#include "voo.h"

// Estrutura para representar nós da árvore sintática
typedef struct NoFiltro {
    char operador;         // Operador lógico: '&&' (versão básica)
    char atributo[10];     // Atributo (ex: "org", "prc", "dur")
    char operador_rel;     // Operador relacional: '=', '<', '>'
    union {
        float valor_num;   // Valor numérico (ex: preço, duração)
        char valor_str[10];// Valor string (ex: origem/destino)
    };
    struct NoFiltro* esq;  // Subárvore esquerda
    struct NoFiltro* dir;  // Subárvore direita
} NoFiltro;

// Estrutura para representar uma consulta
typedef struct {
    int max_resultados;    // Número máximo de resultados
    char criterio[4];      // Critério de ordenação (ex: "pds")
    NoFiltro* filtros;     // Raiz da árvore sintática
} Consulta;

// Funções para manipular a TAD Consulta

/**
 * Cria uma nova consulta.
 *  max_resultados Número máximo de resultados.
 *  criterio Critério de ordenação.
 *  filtros Raiz da árvore sintática de filtros.
 *  Consulta inicializada.
 */
Consulta criar_consulta(int max_resultados, const char* criterio, NoFiltro* filtros);

/**
 * Avalia se um voo atende aos filtros da consulta.
 *  voo Voo a ser avaliado.
 *  filtros Raiz da árvore sintática de filtros.
 *  1 se o voo atende aos filtros, 0 caso contrário.
 */
int avaliar_filtros(const Voo* voo, const NoFiltro* filtros);

/**
 * Libera a memória da árvore sintática de filtros.
 *  filtros Raiz da árvore sintática.
 */
void liberar_filtros(NoFiltro* filtros);

#endif // CONSULTA_H
