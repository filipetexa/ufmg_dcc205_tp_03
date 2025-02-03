#include "query.h"
#include "expr_parser.h"

// Árvores AVL globais para indexação dos voos
struct AVLTree *indice_origem;
struct AVLTree *indice_destino;
struct AVLTree *indice_preco;
struct AVLTree *indice_assentos;
struct AVLTree *indice_duracao;
struct AVLTree *indice_paradas;

/**
 * Inicializa as árvores AVL para indexação.
 */
void inicializar_indices() {
    indice_origem = criar_avl();
    indice_destino = criar_avl();
    indice_preco = criar_avl();
    indice_assentos = criar_avl();
    indice_duracao = criar_avl();
    indice_paradas = criar_avl();
}

/**
 * Popula as árvores AVL com os voos.
 * @param voos Vetor de voos.
 * @param n Quantidade total de voos.
 */
void construir_indices(Flight *voos, int n) {
    char chave[20];

    for (int i = 0; i < n; i++) {
        indice_origem->raiz = inserir_avl(indice_origem->raiz, voos[i].origem, i);
        indice_destino->raiz = inserir_avl(indice_destino->raiz, voos[i].destino, i);

        sprintf(chave, "%.2f", voos[i].preco);
        indice_preco->raiz = inserir_avl(indice_preco->raiz, chave, i);

        sprintf(chave, "%d", voos[i].assentos);
        indice_assentos->raiz = inserir_avl(indice_assentos->raiz, chave, i);

        sprintf(chave, "%d", voos[i].duracao);
        indice_duracao->raiz = inserir_avl(indice_duracao->raiz, chave, i);

        sprintf(chave, "%d", voos[i].paradas);
        indice_paradas->raiz = inserir_avl(indice_paradas->raiz, chave, i);
    }
}

/**
 * Filtra os voos conforme a consulta usando AVL e árvore de expressões.
 * @param voos Vetor de voos.
 * @param n Quantidade total de voos.
 * @param consulta Ponteiro para a consulta a ser aplicada.
 * @param resultados Ponteiro para armazenar os voos filtrados.
 * @return Número de voos que passaram no filtro.
 */
int filtrar_voos(Flight *voos, int n, const Query *consulta, Flight **resultados) {
    *resultados = (Flight *)malloc(n * sizeof(Flight));
    if (*resultados == NULL) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(EXIT_FAILURE);
    }

    // Converte a string da consulta em árvore de expressão
    ExprNode *expressao = parse_expressao(consulta->expressao);

    // Busca os voos correspondentes nas árvores AVL
    AVLNode *resultado_avl = avaliar_expressao(expressao);

    // Coleta os voos filtrados conforme os índices obtidos na AVL
    int count = 0;
    if (resultado_avl) {
        for (int i = 0; i < resultado_avl->num_indices; i++) {
            (*resultados)[count++] = voos[resultado_avl->indices[i]];
        }
    }

    // Libera a memória da árvore de expressão
    liberar_expressao(expressao);

    return count;
}

/**
 * Lê uma consulta do arquivo de entrada.
 * @param arquivo Arquivo de entrada.
 * @param consulta Estrutura para armazenar a consulta lida.
 * @return 1 se a leitura for bem-sucedida, 0 caso contrário.
 */
int carregar_consulta(FILE *arquivo, Query *consulta) {
    if (fscanf(arquivo, "%d %3s %255[^\n]", &consulta->max_voos, consulta->trigrama, consulta->expressao) != 3) {
        fprintf(stderr, "Erro ao ler a consulta.\n");
        return 0;
    }
    return 1;
}
