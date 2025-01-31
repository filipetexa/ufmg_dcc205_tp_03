#include "arvore_avl.h"
#include <stdlib.h>  // Para malloc, free
#include <string.h>  // Para strcmp e strncpy

// Função auxiliar para calcular a altura de um nó
static int altura(NoAVL* no) {
    return no == NULL ? 0 : no->altura;
}

// Função auxiliar para calcular o fator de balanceamento
static int fator_balanceamento(NoAVL* no) {
    return no == NULL ? 0 : altura(no->esq) - altura(no->dir);
}

// Função auxiliar para atualizar a altura de um nó
static void atualizar_altura(NoAVL* no) {
    if (no != NULL) {
        no->altura = 1 + (altura(no->esq) > altura(no->dir) ? altura(no->esq) : altura(no->dir));
    }
}

// Rotação à direita
static NoAVL* rotacao_direita(NoAVL* y) {
    NoAVL* x = y->esq;
    NoAVL* T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    atualizar_altura(y);
    atualizar_altura(x);

    return x;
}

// Rotação à esquerda
static NoAVL* rotacao_esquerda(NoAVL* x) {
    NoAVL* y = x->dir;
    NoAVL* T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    atualizar_altura(x);
    atualizar_altura(y);

    return y;
}

// Função para criar um novo nó AVL
NoAVL* criar_no_avl(const char* chave) {
    NoAVL* novo_no = (NoAVL*)malloc(sizeof(NoAVL));
    if (novo_no == NULL) return NULL;

    strncpy(novo_no->chave, chave, sizeof(novo_no->chave) - 1);
    novo_no->chave[sizeof(novo_no->chave) - 1] = '\0';
    novo_no->indices_voos = (int*)malloc(10 * sizeof(int)); // Capacidade inicial para 10 índices
    if (novo_no->indices_voos == NULL) {
        free(novo_no);
        return NULL;
    }
    novo_no->tamanho = 0;
    novo_no->capacidade = 10;
    novo_no->esq = novo_no->dir = NULL;
    novo_no->altura = 1;

    return novo_no;
}

// Função para inserir um nó na árvore AVL
NoAVL* inserir_no_avl(NoAVL* raiz, const char* chave, int indice_voo) {
    if (raiz == NULL) return criar_no_avl(chave);

    // Inserção na subárvore esquerda ou direita
    if (strcmp(chave, raiz->chave) < 0) {
        raiz->esq = inserir_no_avl(raiz->esq, chave, indice_voo);
    } else if (strcmp(chave, raiz->chave) > 0) {
        raiz->dir = inserir_no_avl(raiz->dir, chave, indice_voo);
    } else {
        // Inserir o índice do voo na lista de índices
        if (raiz->tamanho >= raiz->capacidade) {
            raiz->capacidade *= 2;
            int* nova_lista = (int*)realloc(raiz->indices_voos, raiz->capacidade * sizeof(int));
            if (nova_lista == NULL) return raiz;
            raiz->indices_voos = nova_lista;
        }
        raiz->indices_voos[raiz->tamanho++] = indice_voo;
        return raiz;
    }

    // Atualizar a altura do nó atual
    atualizar_altura(raiz);

    // Balanceamento
    int balance = fator_balanceamento(raiz);

    // Rotação LL
    if (balance > 1 && strcmp(chave, raiz->esq->chave) < 0) {
        return rotacao_direita(raiz);
    }

    // Rotação RR
    if (balance < -1 && strcmp(chave, raiz->dir->chave) > 0) {
        return rotacao_esquerda(raiz);
    }

    // Rotação LR
    if (balance > 1 && strcmp(chave, raiz->esq->chave) > 0) {
        raiz->esq = rotacao_esquerda(raiz->esq);
        return rotacao_direita(raiz);
    }

    // Rotação RL
    if (balance < -1 && strcmp(chave, raiz->dir->chave) < 0) {
        raiz->dir = rotacao_direita(raiz->dir);
        return rotacao_esquerda(raiz);
    }

    return raiz;
}

// Função para buscar um nó na árvore AVL
NoAVL* buscar_no_avl(NoAVL* raiz, const char* chave) {
    if (raiz == NULL || strcmp(chave, raiz->chave) == 0) {
        return raiz;
    }

    if (strcmp(chave, raiz->chave) < 0) {
        return buscar_no_avl(raiz->esq, chave);
    } else {
        return buscar_no_avl(raiz->dir, chave);
    }
}

// Função para liberar a memória da árvore AVL
void liberar_arvore_avl(NoAVL* raiz) {
    if (raiz != NULL) {
        liberar_arvore_avl(raiz->esq);
        liberar_arvore_avl(raiz->dir);
        free(raiz->indices_voos);
        free(raiz);
    }
}
