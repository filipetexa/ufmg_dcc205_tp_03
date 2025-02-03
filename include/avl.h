#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Enumeração dos operadores suportados na árvore de expressão.
 */
typedef enum {
    OP_EQUAL,        // Igual (==)
    OP_LESS_EQUAL,   // Menor ou igual (<=)
    OP_GREATER_EQUAL,// Maior ou igual (>=)
    OP_LESS,         // Menor (<)
    OP_GREATER,      // Maior (>)
    OP_AND           // Operador lógico AND (&&)
} OperatorType;

/**
 * Estrutura de um nó da árvore AVL.
 */
typedef struct AVLNode {
    char chave[100];  // Valor da chave associada ao nó (exemplo: código do aeroporto)
    int *indices;     // Lista de índices dos voos associados a essa chave
    int num_indices;  // Número de índices armazenados no nó
    int altura;       // Altura do nó na árvore AVL
    struct AVLNode *esq;  // Ponteiro para o filho à esquerda
    struct AVLNode *dir;  // Ponteiro para o filho à direita
} AVLNode;

/**
 * Estrutura de uma árvore AVL.
 */
typedef struct AVLTree {
    AVLNode *raiz;  // Ponteiro para o nó raiz da árvore AVL
} AVLTree;

/**
 * Variáveis globais para armazenar os índices AVL de diferentes atributos dos voos.
 */
extern struct AVLTree *indice_origem;    // Índice AVL para o campo "origem"
extern struct AVLTree *indice_destino;   // Índice AVL para o campo "destino"
extern struct AVLTree *indice_preco;     // Índice AVL para o campo "preço"
extern struct AVLTree *indice_assentos;  // Índice AVL para o campo "assentos"
extern struct AVLTree *indice_duracao;   // Índice AVL para o campo "duração"
extern struct AVLTree *indice_paradas;   // Índice AVL para o campo "paradas"

/**
 * Funções para manipulação da árvore AVL.
 */
AVLTree *criar_avl();  // Inicializa e retorna uma nova árvore AVL

/**
 * Insere um novo nó na árvore AVL.
 * @param raiz Ponteiro para o nó raiz da árvore.
 * @param chave Chave a ser inserida (exemplo: código do aeroporto).
 * @param indice Índice do voo associado à chave.
 * @return Retorna a raiz da árvore após a inserção.
 */
AVLNode *inserir_avl(AVLNode *raiz, const char *chave, int indice);

/**
 * Busca um nó na árvore AVL.
 * @param raiz Ponteiro para o nó raiz da árvore.
 * @param chave Chave a ser buscada.
 * @return Retorna o nó encontrado ou NULL se não existir.
 */
AVLNode *buscar_avl(AVLNode *raiz, const char *chave);

/**
 * Libera a memória de uma árvore AVL.
 * @param raiz Ponteiro para o nó raiz da árvore.
 */
void liberar_avl(AVLNode *raiz);

/**
 * Intersecciona os resultados de dois nós AVL.
 * @param a Ponteiro para a primeira AVLNode.
 * @param b Ponteiro para a segunda AVLNode.
 * @return Retorna uma nova AVLNode contendo apenas os índices presentes em ambas as árvores.
 */
AVLNode *intersect_avl_nodes(AVLNode *a, AVLNode *b);

/**
 * Realiza uma busca na árvore AVL baseada em um campo específico.
 * @param campo Nome do campo a ser buscado.
 * @param valor Valor a ser comparado.
 * @param operador Operador lógico para a busca (ex: >=, <=, ==).
 * @return Retorna um nó AVL contendo os resultados da busca.
 */
AVLNode *buscar_avl_por_campo(const char *campo, const char *valor, OperatorType operador);

/**
 * Busca todos os valores menores ou iguais ao valor fornecido na AVL.
 * @param raiz Ponteiro para o nó raiz da árvore.
 * @param valor Valor limite para a busca.
 * @param incluir_igual Define se o valor limite deve ser incluído na busca.
 * @return Retorna um nó AVL contendo os resultados.
 */
AVLNode *buscar_menores_avl(AVLNode *raiz, const char *valor, int incluir_igual);

/**
 * Busca todos os valores maiores ou iguais ao valor fornecido na AVL.
 * @param raiz Ponteiro para o nó raiz da árvore.
 * @param valor Valor limite para a busca.
 * @param incluir_igual Define se o valor limite deve ser incluído na busca.
 * @return Retorna um nó AVL contendo os resultados.
 */
AVLNode *buscar_maiores_avl(AVLNode *raiz, const char *valor, int incluir_igual);

#endif // AVL_H
