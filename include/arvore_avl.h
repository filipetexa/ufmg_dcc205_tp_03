#ifndef ARVORE_AVL_H
#define ARVORE_AVL_H

#include "voo.h"

// Estrutura de nó da Árvore AVL
typedef struct NoAVL {
    char chave[10];          // Valor indexado (ex: "LAX", "ORD")
    int* indices_voos;       // Lista de índices de voos na Lista de Voos
    int tamanho;             // Número de voos associados à chave
    int capacidade;          // Capacidade alocada para o vetor de índices
    struct NoAVL* esq;       // Subárvore esquerda
    struct NoAVL* dir;       // Subárvore direita
    int altura;              // Altura do nó
} NoAVL;

// Funções para manipular a Árvore AVL

/**
 * Cria um novo nó na árvore AVL.
 * @param chave Valor da chave do nó.
 * @return Ponteiro para o nó criado.
 */
NoAVL* criar_no_avl(const char* chave);

/**
 * Insere uma nova chave na árvore AVL.
 * @param raiz Raiz da árvore AVL.
 * @param chave Valor da chave a ser inserida.
 * @param indice_voo Índice do voo a ser associado à chave.
 * @return Nova raiz da árvore após a inserção.
 */
NoAVL* inserir_no_avl(NoAVL* raiz, const char* chave, int indice_voo);

/**
 * Libera a memória alocada para a árvore AVL.
 * @param raiz Raiz da árvore AVL.
 */
void liberar_arvore_avl(NoAVL* raiz);

/**
 * Busca um nó na árvore AVL.
 * @param raiz Raiz da árvore AVL.
 * @param chave Valor da chave a ser buscada.
 * @return Ponteiro para o nó correspondente ou NULL se não encontrado.
 */
NoAVL* buscar_no_avl(NoAVL* raiz, const char* chave);

#endif // ARVORE_AVL_H
