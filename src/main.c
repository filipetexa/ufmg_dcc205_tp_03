#include <stdio.h>
#include <stdlib.h>
#include "flight.h"
#include "query.h"
#include "sort.h"


void imprimir_voo(Flight voo) {
    char partida_str[20], chegada_str[20];

    // Converter timestamps para strings formatadas
    formatar_data(voo.partida, partida_str);
    formatar_data(voo.chegada, chegada_str);

    // Ajuste na formatação do preço
    double preco = voo.preco;
    int preco_inteiro = (int)preco;  // Parte inteira do preço
    int centavos = (int)(preco * 100) % 100;  // Obtém os dois últimos dígitos decimais (centavos)

    if (centavos == 0) {
        // Se não houver centavos, imprime como um número inteiro
        printf("%s %s %.0f %d %s %s %d\n",
               voo.origem, voo.destino,
               preco, voo.assentos,
               partida_str, chegada_str,
               voo.paradas);
    } else {
        // Converte para string e remove zero desnecessário no final
        char preco_formatado[10];
        snprintf(preco_formatado, sizeof(preco_formatado), "%.2f", preco);

        // Remove zero final se necessário
        if (preco_formatado[strlen(preco_formatado) - 1] == '0') {
            preco_formatado[strlen(preco_formatado) - 1] = '\0';
        }
        printf("%s %s %s %d %s %s %d\n",
               voo.origem, voo.destino,
               preco_formatado, voo.assentos,
               partida_str, chegada_str,
               voo.paradas);
    }
}


int main(int argc, char *argv[])
{
    // Verificar se o nome do arquivo foi fornecido como argumento
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Abrir o arquivo de entrada
    FILE *entrada = fopen(argv[1], "r");
    if (entrada == NULL)
    {
        fprintf(stderr, "Erro ao abrir o arquivo %s.\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Inicializar as árvores AVL
    inicializar_indices();

    // Variáveis para armazenar os voos e consultas
    Flight *voos;
    int num_voos;

    // Carregar a lista de voos
    carregar_voos(entrada, &voos, &num_voos);

    // Construir os índices AVL com os voos carregados
    construir_indices(voos, num_voos);

    // Ler o número de consultas
    int num_consultas;
    if (fscanf(entrada, "%d", &num_consultas) != 1)
    {
        fprintf(stderr, "Erro ao ler o número de consultas.\n");
        free(voos);
        fclose(entrada);
        return EXIT_FAILURE;
    }

    // Processar cada consulta
    for (int i = 0; i < num_consultas; i++)
    {
        Query consulta;
        if (!carregar_consulta(entrada, &consulta))
        {
            free(voos);
            fclose(entrada);
            return EXIT_FAILURE;
        }

        // Depuração: Imprimir as árvores AVL antes da filtragem das consultas
        // imprimir_todas_as_avls();

        // Filtrar os voos que atendem à consulta
        Flight *voos_filtrados;
        int num_filtrados = filtrar_voos(voos, num_voos, &consulta, &voos_filtrados);

        // Ordenar os voos filtrados conforme o trigrama
        ordenar_voos(voos_filtrados, num_filtrados, consulta.trigrama, consulta.max_voos);

        // Imprimir a consulta
        printf("%d %s %s\n", consulta.max_voos, consulta.trigrama, consulta.expressao);

        // Imprimir os voos filtrados no formato correto
        for (int j = 0; j < consulta.max_voos && j < num_filtrados; j++)
        {
            imprimir_voo(voos_filtrados[j]);
        }

        free(voos_filtrados); // Liberar a memória dos voos filtrados
    }

    // Fechar o arquivo e liberar memória
    fclose(entrada);
    free(voos);

    return EXIT_SUCCESS;
}
