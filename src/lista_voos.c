#include "lista_voos.h"
#include <stdlib.h>  // Para malloc, free e realloc
#include <string.h>  // Para memcpy

// Função para criar uma nova lista de voos
ListaVoos* criar_lista_voos(int capacidade_inicial) {
    if (capacidade_inicial <= 0) return NULL;

    ListaVoos* lista = (ListaVoos*)malloc(sizeof(ListaVoos));
    if (lista == NULL) return NULL;

    lista->voos = (Voo*)malloc(capacidade_inicial * sizeof(Voo));
    if (lista->voos == NULL) {
        free(lista);
        return NULL;
    }

    lista->tamanho = 0;
    lista->capacidade = capacidade_inicial;

    return lista;
}

// Função para adicionar um voo à lista
int adicionar_voo(ListaVoos* lista, Voo voo) {
    if (lista == NULL || lista->voos == NULL) return -1;

    // Redimensionar o vetor se necessário
    if (lista->tamanho >= lista->capacidade) {
        int nova_capacidade = lista->capacidade * 2;
        Voo* novo_array = (Voo*)realloc(lista->voos, nova_capacidade * sizeof(Voo));
        if (novo_array == NULL) return -1;

        lista->voos = novo_array;
        lista->capacidade = nova_capacidade;
    }

    // Adicionar o voo na próxima posição disponível
    lista->voos[lista->tamanho] = voo;
    lista->tamanho++;

    return 0;
}

// Função para liberar a memória da lista de voos
void liberar_lista_voos(ListaVoos* lista) {
    if (lista != NULL) {
        free(lista->voos);  // Libera o vetor de voos
        free(lista);        // Libera a estrutura da lista
    }
}
