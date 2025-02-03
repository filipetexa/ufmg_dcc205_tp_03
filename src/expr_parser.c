#include "expr_parser.h"

/**
 * Imprime a árvore de expressão em formato hierárquico.
 */
void imprimir_arvore_expressao(ExprNode *raiz, int nivel) {
    if (!raiz) return;

    // Ajusta indentação para visualização em árvore
    int espaco = nivel * 4;

    // Imprime primeiro o lado direito da árvore
    imprimir_arvore_expressao(raiz->right, nivel + 1);

    // Imprime o nó atual com indentação proporcional ao nível
    for (int i = 0; i < espaco; i++) printf(" ");
    printf("|-- (%s %s %s)\n",
           raiz->field,
           (raiz->operator == OP_EQUAL) ? "==" :
           (raiz->operator == OP_LESS_EQUAL) ? "<=" :
           (raiz->operator == OP_GREATER_EQUAL) ? ">=" :
           (raiz->operator == OP_LESS) ? "<" :
           (raiz->operator == OP_GREATER) ? ">" :
           (raiz->operator == OP_AND) ? "&&" : "?",
           raiz->value);

    // Imprime o lado esquerdo da árvore
    imprimir_arvore_expressao(raiz->left, nivel + 1);
}

/**
 * Cria um novo nó para a árvore de expressão.
 */
ExprNode *novo_no_expressao(const char *field, OperatorType op, const char *value) {
    ExprNode *node = malloc(sizeof(ExprNode));
    if (!node) {
        fprintf(stderr, "Erro ao alocar memória para ExprNode.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(node->field, field);
    node->operator = op;
    strcpy(node->value, value);
    node->left = node->right = NULL;
    return node;
}

/**
 * Identifica o operador lógico a partir da string da consulta.
 */
OperatorType identificar_operador(const char *op_str) {
    if (strcmp(op_str, "==") == 0) return OP_EQUAL;
    if (strcmp(op_str, "<=") == 0) return OP_LESS_EQUAL;
    if (strcmp(op_str, ">=") == 0) return OP_GREATER_EQUAL;
    if (strcmp(op_str, "<") == 0) return OP_LESS;
    if (strcmp(op_str, ">") == 0) return OP_GREATER;
    if (strcmp(op_str, "&&") == 0) return OP_AND;

    fprintf(stderr, "Erro: Operador desconhecido '%s'\n", op_str);
    exit(EXIT_FAILURE);
}

/**
 * Converte a string da consulta em uma árvore de expressão.
 */
ExprNode *parse_expressao(const char *expr) {
    char expr_copia[256];
    strcpy(expr_copia, expr);

    // Remove caracteres de quebra de linha (\r, \n)
    for (int i = 0; expr_copia[i]; i++) {
        if (expr_copia[i] == '\r' || expr_copia[i] == '\n') {
            expr_copia[i] = '\0';
        }
    }

    char *token = strtok(expr_copia, " ()");
    ExprNode *root = NULL, *current = NULL;

    while (token) {
        char field[10] = {0}, op_str[3] = {0}, value[20] = {0};

        // Ignora tokens inválidos
        if (strlen(token) == 0 || token[0] == '\r' || token[0] == '\n') {
            token = strtok(NULL, " ()");
            continue;
        }

        if (strcmp(token, "&&") == 0) {
            // Cria um nó para o operador lógico &&
            ExprNode *and_node = malloc(sizeof(ExprNode));
            if (!and_node) {
                fprintf(stderr, "Erro ao alocar memória para operador &&.\n");
                exit(EXIT_FAILURE);
            }
            and_node->operator = OP_AND;
            and_node->left = root;
            and_node->right = NULL;
            and_node->field[0] = '\0';
            and_node->value[0] = '\0';

            root = and_node;
            current = and_node;
            token = strtok(NULL, " ()");
            continue;
        }

        // Lê campo, operador e valor da expressão
        if (sscanf(token, "%9[^=<>]%2[=<>]%19s", field, op_str, value) < 3) {
            fprintf(stderr, "Erro ao interpretar expressão: %s\n", token);
            token = strtok(NULL, " ()");
            continue;
        }

        OperatorType op = identificar_operador(op_str);
        ExprNode *new_node = novo_no_expressao(field, op, value);

        if (!root) {
            root = new_node;
        } else if (current && current->operator == OP_AND) {
            current->right = new_node;
        } else {
            fprintf(stderr, "Erro na estrutura da árvore de expressão.\n");
        }

        current = new_node;
        token = strtok(NULL, " ()");
    }

    return root;
}

/**
 * Avalia a árvore de expressão e retorna os voos que atendem aos critérios.
 */
AVLNode *avaliar_expressao(const ExprNode *root) {
    if (!root) return NULL;

    // Se for operador &&, retorna interseção dos resultados das subárvores
    if (root->operator == OP_AND) {
        AVLNode *left_results = avaliar_expressao(root->left);
        AVLNode *right_results = avaliar_expressao(root->right);

        if (!left_results) return right_results;
        if (!right_results) return left_results;

        return intersect_avl_nodes(left_results, right_results);
    }

    // Busca voos que correspondem ao critério na AVL
    return buscar_avl_por_campo(root->field, root->value, root->operator);
}

/**
 * Libera a memória da árvore de expressão.
 */
void liberar_expressao(ExprNode *root) {
    if (!root) return;
    liberar_expressao(root->left);
    liberar_expressao(root->right);
    free(root);
}
