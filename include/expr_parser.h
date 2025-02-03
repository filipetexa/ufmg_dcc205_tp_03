#ifndef EXPR_PARSER_H
#define EXPR_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"

/**
 * Nó da árvore de expressões.
 */
typedef struct ExprNode {
    OperatorType operator;  // Operador lógico (==, <=, >=, &&)
    char field[10];  // Campo da consulta (ex: "org", "dst", "prc")
    char value[20];  // Valor a ser comparado (ex: "LAX", "100.50")
    struct ExprNode *left;  // Subárvore esquerda
    struct ExprNode *right; // Subárvore direita
} ExprNode;

/**
 * Constrói uma árvore de expressão a partir de uma string de consulta.
 */
ExprNode *parse_expressao(const char *expr);

/**
 * Avalia a árvore de expressão e retorna os voos que satisfazem a consulta.
 */
AVLNode *avaliar_expressao(const ExprNode *root);

/**
 * Libera a memória da árvore de expressão.
 */
void liberar_expressao(ExprNode *root);

/**
 * Imprime a árvore de expressão formatada.
 */
void imprimir_arvore_expressao(ExprNode *raiz, int nivel);

#endif // EXPR_PARSER_H
