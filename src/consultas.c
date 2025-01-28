#include "consulta.h"
#include <stdlib.h>  // Para malloc e free
#include <string.h>  // Para strcmp e strncpy

// Função para criar uma nova consulta
Consulta criar_consulta(int max_resultados, const char* criterio, NoFiltro* filtros) {
    Consulta nova_consulta;
    nova_consulta.max_resultados = max_resultados;
    strncpy(nova_consulta.criterio, criterio, sizeof(nova_consulta.criterio) - 1);
    nova_consulta.criterio[sizeof(nova_consulta.criterio) - 1] = '\0';
    nova_consulta.filtros = filtros;
    return nova_consulta;
}

// Função recursiva para avaliar se um voo atende aos filtros
int avaliar_filtros(const Voo* voo, const NoFiltro* filtros) {
    if (filtros == NULL) return 1; // Caso base: sem filtros, tudo é válido

    // Avaliar os filtros de forma recursiva
    if (filtros->operador == '&&') {
        return avaliar_filtros(voo, filtros->esq) && avaliar_filtros(voo, filtros->dir);
    }

    // Avaliar o filtro atual
    if (strcmp(filtros->atributo, "org") == 0) {
        return strcmp(voo->origem, filtros->valor_str) == 0;
    } else if (strcmp(filtros->atributo, "dst") == 0) {
        return strcmp(voo->destino, filtros->valor_str) == 0;
    } else if (strcmp(filtros->atributo, "prc") == 0) {
        if (filtros->operador_rel == '<') {
            return voo->preco < filtros->valor_num;
        } else if (filtros->operador_rel == '>') {
            return voo->preco > filtros->valor_num;
        } else if (filtros->operador_rel == '=') {
            return voo->preco == filtros->valor_num;
        }
    } else if (strcmp(filtros->atributo, "sto") == 0) {
        if (filtros->operador_rel == '<') {
            return voo->paradas < (int)filtros->valor_num;
        } else if (filtros->operador_rel == '>') {
            return voo->paradas > (int)filtros->valor_num;
        } else if (filtros->operador_rel == '=') {
            return voo->paradas == (int)filtros->valor_num;
        }
    }
    // Adicionar mais atributos conforme necessário

    return 0; // Caso o atributo não seja reconhecido
}

// Função recursiva para liberar memória da árvore sintática
void liberar_filtros(NoFiltro* filtros) {
    if (filtros != NULL) {
        liberar_filtros(filtros->esq);
        liberar_filtros(filtros->dir);
        free(filtros);
    }
}
