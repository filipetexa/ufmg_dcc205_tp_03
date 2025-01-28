#include <stdio.h>
#include <stdlib.h>
#include "voo.h"
#include "lista_voos.h"
#include "consulta.h"
#include "arvore_avl.h"

// Função para ler a lista de voos do arquivo de entrada
ListaVoos* ler_lista_voos(FILE* arquivo, int* num_voos) {
    fscanf(arquivo, "%d", num_voos); // Lê o número de voos

    ListaVoos* lista = criar_lista_voos(*num_voos);
    if (lista == NULL) {
        fprintf(stderr, "Erro ao criar lista de voos.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *num_voos; i++) {
        char origem[4], destino[4];
        float preco;
        int assentos, paradas;
        char partida_str[25], chegada_str[25];
        time_t partida, chegada;

        fscanf(arquivo, "%s %s %f %d %s %s %d", origem, destino, &preco, &assentos, partida_str, chegada_str, &paradas);

        // Converter strings de data/hora para time_t (UTC)
        struct tm partida_tm = {0}, chegada_tm = {0};
        strptime(partida_str, "%Y-%m-%dT%H:%M:%S", &partida_tm);
        strptime(chegada_str, "%Y-%m-%dT%H:%M:%S", &chegada_tm);
        partida = mktime(&partida_tm);
        chegada = mktime(&chegada_tm);

        Voo voo = criar_voo(origem, destino, preco, assentos, partida, chegada, paradas);
        adicionar_voo(lista, voo);
    }

    return lista;
}

// Função para ler consultas do arquivo de entrada
Consulta* ler_consultas(FILE* arquivo, int* num_consultas) {
    fscanf(arquivo, "%d", num_consultas); // Lê o número de consultas

    Consulta* consultas = (Consulta*)malloc(*num_consultas * sizeof(Consulta));
    if (consultas == NULL) {
        fprintf(stderr, "Erro ao alocar consultas.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *num_consultas; i++) {
        int max_resultados;
        char criterio[4], filtros_str[100];

        fscanf(arquivo, "%d %s %[^\n]", &max_resultados, criterio, filtros_str);

        // TODO: Implementar parsing de filtros para criar a árvore sintática
        NoFiltro* filtros = NULL;

        consultas[i] = criar_consulta(max_resultados, criterio, filtros);
    }

    return consultas;
}

// Função principal
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo_entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* arquivo = fopen(argv[1], "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir arquivo");
        return EXIT_FAILURE;
    }

    // Ler lista de voos
    int num_voos;
    ListaVoos* lista_voos = ler_lista_voos(arquivo, &num_voos);

    // Ler consultas
    int num_consultas;
    Consulta* consultas = ler_consultas(arquivo, &num_consultas);

    // Processar cada consulta
    for (int i = 0; i < num_consultas; i++) {
        printf("%d %s %s\n", consultas[i].max_resultados, consultas[i].criterio, "FILTROS"); // Placeholder para filtros

        // TODO: Processar a consulta e imprimir os resultados
    }

    // Limpar memória
    liberar_lista_voos(lista_voos);
    for (int i = 0; i < num_consultas; i++) {
        liberar_filtros(consultas[i].filtros);
    }
    free(consultas);

    fclose(arquivo);
    return EXIT_SUCCESS;
}
