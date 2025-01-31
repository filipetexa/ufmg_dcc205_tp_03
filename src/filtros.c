#include "filtros.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// Função auxiliar para criar um nó de filtro
NoFiltro* criar_no_filtro(const char* atributo, char operador_rel, const char* valor) {
    NoFiltro* novo_no = (NoFiltro*)malloc(sizeof(NoFiltro));
    if (!novo_no) {
        fprintf(stderr, "Erro ao alocar memória para o nó de filtro.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(novo_no->atributo, atributo);
    novo_no->operador_rel = operador_rel;
    novo_no->esq = novo_no->dir = NULL;

    // Verificar se o valor é numérico ou string
    if (isdigit(valor[0]) || valor[0] == '-') {
        novo_no->valor_num = atof(valor);
    } else {
        strcpy(novo_no->valor_str, valor);
    }

    return novo_no;
}

// Função auxiliar para remover espaços desnecessários da string de filtros
void limpar_espacos(char* str) {
    char* i = str, *j = str;
    while (*j != '\0') {
        if (*j != ' ') {
            *i = *j;
            i++;
        }
        j++;
    }
    *i = '\0';
}

// Função para construir a árvore de filtros a partir da string de entrada
NoFiltro* construir_arvore_filtros(const char* filtros_str) {
    if (filtros_str == NULL || strlen(filtros_str) == 0) return NULL;

    char filtros[256];
    strcpy(filtros, filtros_str);
    limpar_espacos(filtros);

    if (filtros[0] != '(' || filtros[strlen(filtros) - 1] != ')') {
        fprintf(stderr, "Erro no formato dos filtros: Parênteses esperados.\n");
        return NULL;
    }

    // Remover os parênteses externos
    filtros[strlen(filtros) - 1] = '\0';
    char* expressao = filtros + 1;

    // Buscar operador lógico (&&)
    char* operador = strstr(expressao, "&&");
    if (operador) {
        *operador = '\0';
        NoFiltro* no = (NoFiltro*)malloc(sizeof(NoFiltro));
        if (!no) {
            fprintf(stderr, "Erro ao alocar memória para nó de operador lógico.\n");
            exit(EXIT_FAILURE);
        }
        no->operador = '&&';
        no->esq = construir_arvore_filtros(expressao);
        no->dir = construir_arvore_filtros(operador + 2);
        return no;
    }

    // Caso seja um nó folha (ex: "prc<=500")
    char atributo[10], operador_rel, valor[20];
    if (sscanf(expressao, "%[^<=>]%c%s", atributo, &operador_rel, valor) == 3) {
        return criar_no_filtro(atributo, operador_rel, valor);
    }

    return NULL;
}

// Função recursiva para avaliar se um voo atende aos filtros
int avaliar_filtros(const Voo* voo, const NoFiltro* filtros) {
    if (filtros == NULL) return 1; // Se não há filtros, o voo é válido

    // Se for um operador lógico
    if (filtros->operador == '&&') {
        return avaliar_filtros(voo, filtros->esq) && avaliar_filtros(voo, filtros->dir);
    }

    // Comparação dos atributos do voo
    if (strcmp(filtros->atributo, "org") == 0) {
        return strcmp(voo->origem, filtros->valor_str) == 0;
    } else if (strcmp(filtros->atributo, "dst") == 0) {
        return strcmp(voo->destino, filtros->valor_str) == 0;
    } else if (strcmp(filtros->atributo, "prc") == 0) {
        return (filtros->operador_rel == '<') ? (voo->preco < filtros->valor_num) :
               (filtros->operador_rel == '>') ? (voo->preco > filtros->valor_num) :
               (voo->preco == filtros->valor_num);
    } else if (strcmp(filtros->atributo, "sto") == 0) {
        return (filtros->operador_rel == '<') ? (voo->paradas < (int)filtros->valor_num) :
               (filtros->operador_rel == '>') ? (voo->paradas > (int)filtros->valor_num) :
               (voo->paradas == (int)filtros->valor_num);
    }

    return 0; // Se o atributo não for reconhecido
}

// Função recursiva para liberar a memória da árvore sintática
void liberar_filtros(NoFiltro* filtros) {
    if (filtros != NULL) {
        liberar_filtros(filtros->esq);
        liberar_filtros(filtros->dir);
        free(filtros);
    }
}
