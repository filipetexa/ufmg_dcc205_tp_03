#ifndef QUERY_H
#define QUERY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flight.h"
#include "avl.h"
#include "expr_parser.h"

/**
 * Índices AVL para busca eficiente por diferentes atributos dos voos.
 */
extern struct AVLTree *indice_origem;    
extern struct AVLTree *indice_destino;   
extern struct AVLTree *indice_preco;     
extern struct AVLTree *indice_assentos;  
extern struct AVLTree *indice_duracao;   
extern struct AVLTree *indice_paradas;   

/**
 * Estrutura de uma consulta de voo.
 */
typedef struct {
    int max_voos;        // Número máximo de voos a exibir
    char trigrama[4];    // Código do aeroporto
    char expressao[256]; // Expressão da consulta
} Query;

/**
 * Inicializa os índices AVL.
 */
void inicializar_indices();

/**
 * Constrói os índices AVL com os voos carregados.
 */
void construir_indices(Flight *voos, int n);

/**
 * Filtra os voos usando a árvore de expressões e AVL.
 */
int filtrar_voos(Flight *voos, int n, const Query *consulta, Flight **resultados);

/**
 * Lê uma consulta do arquivo de entrada.
 */
int carregar_consulta(FILE *arquivo, Query *consulta);

#endif // QUERY_H
