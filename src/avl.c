#include <stdbool.h>  // Para o uso de bool
#include "avl.h"

// Cria uma nova árvore AVL
AVLTree *criar_avl() {
    AVLTree *tree = malloc(sizeof(AVLTree));
    if (!tree) {
        fprintf(stderr, "Erro ao alocar memória para AVL.\n");
        exit(EXIT_FAILURE);
    }
    tree->raiz = NULL;
    return tree;
}

// Retorna a altura do nó
int altura(AVLNode *n) {
    if (n == NULL) return 0;
    return n->altura;
}

// Atualiza a altura do nó
void atualizar_altura(AVLNode *n) {
    if (n != NULL) {
        n->altura = 1 + (altura(n->esq) > altura(n->dir) ? altura(n->esq) : altura(n->dir));
    }
}

// Retorna o fator de balanceamento do nó
int get_balanceamento(AVLNode *n) {
    if (n == NULL) return 0;
    return altura(n->esq) - altura(n->dir);
}

// Realiza rotação simples à direita
AVLNode *rotacao_direita(AVLNode *y) {
    AVLNode *x = y->esq;
    AVLNode *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    atualizar_altura(y);
    atualizar_altura(x);

    return x;
}

// Realiza rotação simples à esquerda
AVLNode *rotacao_esquerda(AVLNode *x) {
    AVLNode *y = x->dir;
    AVLNode *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    atualizar_altura(x);
    atualizar_altura(y);

    return y;
}

// Insere um novo nó na AVL e retorna a nova raiz
// Insere um novo nó na AVL e adiciona os índices sem sobrescrever
AVLNode *inserir_avl(AVLNode *raiz, const char *chave, int indice) {
    if (!raiz) {
        AVLNode *novo = malloc(sizeof(AVLNode));
        strcpy(novo->chave, chave);
        novo->indices = malloc(sizeof(int));
        novo->indices[0] = indice;
        novo->num_indices = 1;
        novo->altura = 1;
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (strcmp(chave, raiz->chave) < 0) {
        raiz->esq = inserir_avl(raiz->esq, chave, indice);
    } else if (strcmp(chave, raiz->chave) > 0) {
        raiz->dir = inserir_avl(raiz->dir, chave, indice);
    } else {
        // ✅ Apenas adicionamos o índice, sem sobrescrever
        int new_size = raiz->num_indices + 1;
        raiz->indices = realloc(raiz->indices, new_size * sizeof(int));
        raiz->indices[raiz->num_indices++] = indice;
    }

    atualizar_altura(raiz);
    int balance = get_balanceamento(raiz);

    // Aplicação das rotações se necessário
    if (balance > 1 && strcmp(chave, raiz->esq->chave) < 0) return rotacao_direita(raiz);
    if (balance < -1 && strcmp(chave, raiz->dir->chave) > 0) return rotacao_esquerda(raiz);
    if (balance > 1 && strcmp(chave, raiz->esq->chave) > 0) {
        raiz->esq = rotacao_esquerda(raiz->esq);
        return rotacao_direita(raiz);
    }
    if (balance < -1 && strcmp(chave, raiz->dir->chave) < 0) {
        raiz->dir = rotacao_direita(raiz->dir);
        return rotacao_esquerda(raiz);
    }

    return raiz;
}


// Busca na AVL pelo valor especificado e retorna o nó correspondente
AVLNode *buscar_avl(AVLNode *raiz, const char *chave) {
    if (raiz == NULL || strcmp(raiz->chave, chave) == 0)
        return raiz;

    if (strcmp(chave, raiz->chave) < 0)
        return buscar_avl(raiz->esq, chave);
    else
        return buscar_avl(raiz->dir, chave);
}

// Libera a memória alocada para a árvore AVL
void liberar_avl(AVLNode *raiz) {
    if (raiz) {
        liberar_avl(raiz->esq);
        liberar_avl(raiz->dir);
        free(raiz->indices);
        free(raiz);
    }
}

void imprimir_resultado_busca(const char *campo, const char *valor, OperatorType operador, AVLNode *resultado) {
    printf("\n🔍 Resultado da busca para: (%s %s %s)\n",
           campo,
           (operador == OP_EQUAL) ? "==" :
           (operador == OP_LESS_EQUAL) ? "<=" :
           (operador == OP_GREATER_EQUAL) ? ">=" :
           (operador == OP_LESS) ? "<" :
           (operador == OP_GREATER) ? ">" : "?",
           valor);

    if (!resultado) {
        printf("   ⚠️ Nenhum resultado encontrado\n");
    } else {
        printf("   📌 Índices encontrados: ");
        for (int i = 0; i < resultado->num_indices; i++) {
            printf("%d ", resultado->indices[i]);
        }
        printf("\n");
    }

}

// Busca na AVL por um campo específico
AVLNode *buscar_avl_por_campo(const char *campo, const char *valor, OperatorType operador) {
    AVLTree *indice = NULL;

    // Seleciona a árvore AVL correta com base no campo
    if (strcmp(campo, "org") == 0) indice = indice_origem;
    if (strcmp(campo, "dst") == 0) indice = indice_destino;
    if (strcmp(campo, "prc") == 0) indice = indice_preco;
    if (strcmp(campo, "sea") == 0) indice = indice_assentos;
    if (strcmp(campo, "dur") == 0) indice = indice_duracao;
    if (strcmp(campo, "sto") == 0) indice = indice_paradas;

    if (!indice || !indice->raiz) return NULL;

    // Verifica o operador e chama a busca adequada
    AVLNode *resultado = NULL;
    if (operador == OP_EQUAL) {
        resultado = buscar_avl(indice->raiz, valor);
    } else if (operador == OP_LESS || operador == OP_LESS_EQUAL) {
        resultado = buscar_menores_avl(indice->raiz, valor, (operador == OP_LESS_EQUAL));
    } else if (operador == OP_GREATER || operador == OP_GREATER_EQUAL) {
        resultado = buscar_maiores_avl(indice->raiz, valor, (operador == OP_GREATER_EQUAL));
    }

    // 🔍 Depuração: Imprimir o resultado da busca AVL
    // imprimir_resultado_busca(campo, valor, operador, resultado);

    return resultado;
}


// 🔍 Função auxiliar para armazenar índices de uma AVL em um array
void extrair_indices(AVLNode *raiz, int *indices, int *num_indices) {
    if (!raiz) return;
    for (int i = 0; i < raiz->num_indices; i++) {
        indices[(*num_indices)++] = raiz->indices[i];
    }
    extrair_indices(raiz->esq, indices, num_indices);
    extrair_indices(raiz->dir, indices, num_indices);
}

// 🔍 Função auxiliar para verificar se um índice está no conjunto
bool existe_no_conjunto(int *conjunto, int tamanho, int valor) {
    for (int i = 0; i < tamanho; i++) {
        if (conjunto[i] == valor) return true;
    }
    return false;
}

// 🔍 Nova interseção que usa uma estrutura temporária (set)
AVLNode *intersect_avl_nodes(AVLNode *a, AVLNode *b) {
    if (!a || !b) return NULL; // Se um dos conjuntos for NULL, não há interseção

    int indices_a[1000], indices_b[1000]; // Arrays temporários para armazenar índices
    int num_indices_a = 0, num_indices_b = 0;

    // 🔍 Extrai os índices das duas árvores em arrays temporários
    extrair_indices(a, indices_a, &num_indices_a);
    extrair_indices(b, indices_b, &num_indices_b);

    // 🔍 Criamos uma nova AVL para armazenar a interseção final
    AVLNode *intersection = NULL;

    // 🔍 Percorremos `indices_b` e adicionamos na interseção apenas se estiverem em `indices_a`
    for (int i = 0; i < num_indices_b; i++) {
        if (existe_no_conjunto(indices_a, num_indices_a, indices_b[i])) {
            intersection = inserir_avl(intersection, "intersec", indices_b[i]);
        }
    }

    return intersection;
}




/**
 * 🔍 Insere todos os nós de uma árvore AVL dentro de outra AVL
 * Isso garante que preservamos todos os índices encontrados sem interseção
 */
AVLNode *inserir_todos_nos(AVLNode *destino, AVLNode *origem) {
    if (!origem) return destino;

    // Mantém os índices sem sobrescrever os anteriores
    for (int i = 0; i < origem->num_indices; i++) {
        destino = inserir_avl(destino, origem->chave, origem->indices[i]);
    }

    // Insere os nós da esquerda e direita
    if (origem->esq) destino = inserir_todos_nos(destino, origem->esq);
    if (origem->dir) destino = inserir_todos_nos(destino, origem->dir);

    return destino;
}


/**
 * Função auxiliar para coletar todos os índices de nós com valores <= ao especificado.
 */
void coletar_menores(AVLNode *raiz, int valor, int incluir_igual, int *indices, int *num_indices) {
    if (!raiz) return;

    // Criando uma pilha manualmente (simulando recursão)
    AVLNode *pilha[1000];  // Ajuste o tamanho conforme necessário
    int topo = -1;
    
    // Começamos pelo nó raiz
    AVLNode *atual = raiz;

    while (atual || topo != -1) {
        // Percorre a subárvore esquerda primeiro
        while (atual) {
            pilha[++topo] = atual;
            atual = atual->esq;  // Vai para os menores valores primeiro
        }

        // Pega o último nó da pilha
        atual = pilha[topo--];

        int chave_num = atoi(atual->chave);

        // Verifica se devemos incluir esse nó no resultado
        if (chave_num < valor || (incluir_igual && chave_num == valor)) {
            for (int i = 0; i < atual->num_indices; i++) {
                indices[(*num_indices)++] = atual->indices[i];
            }
        }

        // Agora vamos para a subárvore direita
        atual = atual->dir;
    }
}


/**
 * Busca todos os valores menores ou iguais ao especificado e retorna no formato AVL.
 */
AVLNode *buscar_menores_avl(AVLNode *raiz, const char *valor, int incluir_igual) {
    if (!raiz) return NULL;

    int valor_num = atoi(valor);

    // Criamos um array temporário para armazenar os índices encontrados
    int *indices_temp = malloc(1000 * sizeof(int)); // Ajuste conforme necessário
    int num_indices = 0;

    // Coletamos todos os índices válidos antes de inserir na AVL
    coletar_menores(raiz, valor_num, incluir_igual, indices_temp, &num_indices);

    // Criamos a AVL final com esses índices
    AVLNode *resultado = NULL;
    for (int i = 0; i < num_indices; i++) {
        resultado = inserir_avl(resultado, valor, indices_temp[i]);
    }

    free(indices_temp); // Liberamos a memória do array temporário
    return resultado;
}



/**
 * Função auxiliar para coletar todos os índices de nós com valores >= ao especificado.
 */
void coletar_maiores(AVLNode *raiz, int valor, int incluir_igual, int *indices, int *num_indices) {
    if (!raiz) return;

    // Criando uma pilha manualmente (simulando recursão)
    AVLNode *pilha[1000];  // Ajuste o tamanho conforme necessário
    int topo = -1;
    
    // Começamos pelo nó raiz
    AVLNode *atual = raiz;

    while (atual || topo != -1) {
        // Percorre a subárvore direita primeiro
        while (atual) {
            pilha[++topo] = atual;
            atual = atual->dir;  // Vai para os maiores valores primeiro
        }

        // Pega o último nó da pilha
        atual = pilha[topo--];

        int chave_num = atoi(atual->chave);

        // Verifica se devemos incluir esse nó no resultado
        if (chave_num > valor || (incluir_igual && chave_num == valor)) {
            for (int i = 0; i < atual->num_indices; i++) {
                indices[(*num_indices)++] = atual->indices[i];
            }
        }

        // Agora vamos para a subárvore esquerda
        atual = atual->esq;
    }
}



/**
 * Busca todos os valores maiores ou iguais ao especificado e retorna no formato AVL.
 */
AVLNode *buscar_maiores_avl(AVLNode *raiz, const char *valor, int incluir_igual) {
    if (!raiz) return NULL;

    int valor_num = atoi(valor);

    // Criamos um array temporário para armazenar os índices encontrados
    int *indices_temp = malloc(1000 * sizeof(int)); // Ajuste conforme necessário
    int num_indices = 0;

    // Coletamos todos os índices válidos antes de inserir na AVL
    coletar_maiores(raiz, valor_num, incluir_igual, indices_temp, &num_indices);

    // Criamos a AVL final com esses índices
    AVLNode *resultado = NULL;
    for (int i = 0; i < num_indices; i++) {
        resultado = inserir_avl(resultado, valor, indices_temp[i]);
    }

    free(indices_temp); // Liberamos a memória do array temporário
    return resultado;
}
