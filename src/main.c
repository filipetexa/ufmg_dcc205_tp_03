#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "voo.h"
#include "lista_voos.h"
#include "consulta.h"
#include "arvore_avl.h"
#include "filtros.h"

// Função para ler a lista de voos do arquivo de entrada
ListaVoos* ler_lista_voos(FILE* arquivo, int* num_voos) {
    fscanf(arquivo, "%d", num_voos);

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
    fscanf(arquivo, "%d", num_consultas);

    Consulta* consultas = (Consulta*)malloc(*num_consultas * sizeof(Consulta));
    if (consultas == NULL) {
        fprintf(stderr, "Erro ao alocar consultas.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *num_consultas; i++) {
        int max_resultados;
        char criterio[4], filtros_str[256];

        fscanf(arquivo, "%d %s %[^\n]", &max_resultados, criterio, filtros_str);

        NoFiltro* filtros = construir_arvore_filtros(filtros_str);
        consultas[i] = criar_consulta(max_resultados, criterio, filtros);
    }

    return consultas;
}

// Função de comparação para ordenação dos voos
int comparar_voos(const void* a, const void* b, void* criterio) {
    Voo* voo1 = (Voo*)a;
    Voo* voo2 = (Voo*)b;
    char* ordem = (char*)criterio;

    for (int i = 0; i < 3; i++) {
        switch (ordem[i]) {
            case 'p': // Ordenar por preço
                if (voo1->preco != voo2->preco)
                    return (voo1->preco > voo2->preco) ? 1 : -1;
                break;
            case 'd': // Ordenar por duração
                if (voo1->duracao != voo2->duracao)
                    return (voo1->duracao > voo2->duracao) ? 1 : -1;
                break;
            case 's': // Ordenar por paradas
                if (voo1->paradas != voo2->paradas)
                    return (voo1->paradas > voo2->paradas) ? 1 : -1;
                break;
        }
    }
    return 0;
}

// Função para processar uma consulta
void processar_consulta(const Consulta* consulta, const ListaVoos* lista_voos) {
    Voo* voos_filtrados = (Voo*)malloc(lista_voos->tamanho * sizeof(Voo));
    if (voos_filtrados == NULL) {
        fprintf(stderr, "Erro ao alocar memória para voos filtrados.\n");
        return;
    }

    int num_resultados = 0;

    // Aplicar os filtros
    for (int i = 0; i < lista_voos->tamanho; i++) {
        if (avaliar_filtros(&lista_voos->voos[i], consulta->filtros)) {
            voos_filtrados[num_resultados++] = lista_voos->voos[i];
        }
    }

    // Ordenar os resultados conforme o critério de ordenação
    qsort_r(voos_filtrados, num_resultados, sizeof(Voo), comparar_voos, (void*)consulta->criterio);

    // Imprimir a consulta original
    printf("%d %s ", consulta->max_resultados, consulta->criterio);
    // Aqui estamos imprimindo os filtros como placeholder
    printf("FILTROS\n"); // Substituir depois por função que imprime os filtros corretamente

    // Exibir no máximo `max_resultados`
    for (int i = 0; i < consulta->max_resultados && i < num_resultados; i++) {
        Voo v = voos_filtrados[i];
        printf("%s %s %.2f %d %ld %ld %d\n", v.origem, v.destino, v.preco, v.assentos,
               v.partida, v.chegada, v.paradas);
    }

    free(voos_filtrados);
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
        processar_consulta(&consultas[i], lista_voos);
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
