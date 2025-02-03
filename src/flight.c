#include "flight.h"

/**
 * Converte uma string de data e hora para time_t.
 * @param datetime String no formato ISO 8601 (ex: "2022-11-01T17:54:00" ou "2022-11-01T17:54:00.000-06:00").
 * @return Tempo convertido em segundos desde a época Unix.
 */
time_t converter_datetime(const char *datetime) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));

    // Verificar o formato da string para decidir qual parsing usar
    if (strchr(datetime, '.') != NULL) {
        // Formato com milissegundos e fuso horário (2022-11-01T17:54:00.000-06:00)
        if (sscanf(datetime, "%d-%d-%dT%d:%d:%d",
                   &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
                   &tm.tm_hour, &tm.tm_min, &tm.tm_sec) != 6) {
            fprintf(stderr, "Erro ao converter data/hora: %s\n", datetime);
            return -1;
        }
    } else {
        // Formato sem milissegundos e sem fuso horário (2022-11-01T17:54:00)
        if (sscanf(datetime, "%d-%d-%dT%d:%d:%d",
                   &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
                   &tm.tm_hour, &tm.tm_min, &tm.tm_sec) != 6) {
            fprintf(stderr, "Erro ao converter data/hora: %s\n", datetime);
            return -1;
        }
    }

    // Ajustar os valores para struct tm
    tm.tm_year -= 1900;  // Ajusta ano
    tm.tm_mon -= 1;      // Ajusta mês (0-11)

    // Converter para timestamp UTC
    return mktime(&tm);
}


/**
 * Lê os voos do arquivo de entrada e os armazena em um vetor.
 * @param arquivo Ponteiro para o arquivo de entrada (stdin).
 * @param voos Ponteiro para o vetor de voos (será alocado dentro da função).
 * @param n Ponteiro para armazenar o número total de voos lidos.
 */
void carregar_voos(FILE *arquivo, Flight **voos, int *n) {
    // Lê o número de voos
    if (fscanf(arquivo, "%d", n) != 1) {
        fprintf(stderr, "Erro ao ler o número de voos.\n");
        exit(EXIT_FAILURE);
    }

    // Aloca memória para os voos
    *voos = (Flight *)malloc((*n) * sizeof(Flight));
    if (*voos == NULL) {
        fprintf(stderr, "Erro de alocação de memória.\n");
        exit(EXIT_FAILURE);
    }

    // Lê cada voo e armazena no vetor
    for (int i = 0; i < *n; i++) {
        char partida_str[30], chegada_str[30];

        if (fscanf(arquivo, "%s %s %f %d %s %s %d",
                   (*voos)[i].origem, (*voos)[i].destino,
                   &(*voos)[i].preco, &(*voos)[i].assentos,
                   partida_str, chegada_str, &(*voos)[i].paradas) != 7) {
            fprintf(stderr, "Erro ao ler os dados do voo %d.\n", i + 1);
            exit(EXIT_FAILURE);
        }

        // Converte datas para timestamp
        (*voos)[i].partida = converter_datetime(partida_str);
        (*voos)[i].chegada = converter_datetime(chegada_str);
        (*voos)[i].duracao = (int)difftime((*voos)[i].chegada, (*voos)[i].partida);
    }
}

/**
 * Exibe a lista de voos (usado para depuração).
 * @param voos Vetor de voos.
 * @param n Número de voos no vetor.
 */
void exibir_voos(Flight *voos, int n) {
    printf("Lista de Voos:\n");
    for (int i = 0; i < n; i++) {
        printf("%s -> %s | Preço: %.2f | Assentos: %d | Partida: %ld | Chegada: %ld | Paradas: %d | Duração: %d seg\n",
               voos[i].origem, voos[i].destino,
               voos[i].preco, voos[i].assentos,
               voos[i].partida, voos[i].chegada,
               voos[i].paradas, voos[i].duracao);
    }
}

/**
 * Converte um timestamp (time_t) para uma string no formato YYYY-MM-DDTHH:MM:SS.
 * @param timestamp Valor de tempo Unix a ser convertido.
 * @param buffer Buffer onde a string formatada será armazenada.
 */
void formatar_data(time_t timestamp, char *buffer) {
    struct tm *tm_info = localtime(&timestamp); // Converter para UTC
    strftime(buffer, 20, "%Y-%m-%dT%H:%M:%S", tm_info);
}

