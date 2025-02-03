#include "sort.h"

/**
 * Compara dois voos com base nos critérios do trigrama.
 * @param a Ponteiro para o primeiro voo.
 * @param b Ponteiro para o segundo voo.
 * @param trigrama String de três caracteres indicando a ordem dos critérios (ex: "psd").
 * @return Valor negativo se a < b, positivo se a > b, 0 se forem iguais.
 */
int comparar_voos(const Flight *a, const Flight *b, const char *trigrama) {
    for (int i = 0; i < 3; i++) {
        switch (trigrama[i]) {
            case 'p': // Comparar por preço (crescente)
                if (a->preco != b->preco)
                    return (a->preco > b->preco) ? 1 : -1;
                break;
            case 'd': // Comparar por duração (crescente)
                if (a->duracao != b->duracao)
                    return (a->duracao > b->duracao) ? 1 : -1;
                break;
            case 's': // Comparar por número de paradas (crescente)
                if (a->paradas != b->paradas)
                    return (a->paradas > b->paradas) ? 1 : -1;
                break;
        }
    }
    return 0; // Se todos os critérios forem iguais
}

/**
 * Particiona o vetor de voos para o QuickSort.
 * @param voos Vetor de voos.
 * @param baixo Índice inferior.
 * @param alto Índice superior.
 * @param trigrama Critério de ordenação.
 * @return Índice do pivô após a partição.
 */
int particionar(Flight *voos, int baixo, int alto, const char *trigrama) {
    Flight pivo = voos[alto]; // Escolher o último elemento como pivô
    int i = baixo - 1;

    for (int j = baixo; j < alto; j++) {
        if (comparar_voos(&voos[j], &pivo, trigrama) < 0) {
            i++;
            Flight temp = voos[i];
            voos[i] = voos[j];
            voos[j] = temp;
        }
    }

    Flight temp = voos[i + 1];
    voos[i + 1] = voos[alto];
    voos[alto] = temp;

    return i + 1;
}

/**
 * Implementação do QuickSort para ordenar os voos.
 * @param voos Vetor de voos.
 * @param baixo Índice inferior.
 * @param alto Índice superior.
 * @param trigrama Critério de ordenação.
 */
void quick_sort_voos(Flight *voos, int baixo, int alto, const char *trigrama) {
    if (baixo < alto) {
        int pi = particionar(voos, baixo, alto, trigrama);

        quick_sort_voos(voos, baixo, pi - 1, trigrama);
        quick_sort_voos(voos, pi + 1, alto, trigrama);
    }
}

/**
 * Ordena os voos com base no critério do trigrama da consulta e seleciona os primeiros `n`.
 * @param voos Vetor de voos a serem ordenados.
 * @param total_voos Número total de voos no vetor.
 * @param trigrama String de três caracteres indicando a ordem dos critérios.
 * @param max_resultados Número máximo de voos a serem considerados após a ordenação.
 */
void ordenar_voos(Flight *voos, int total_voos, const char *trigrama, int max_resultados) {
    if (!voos || total_voos <= 1) return;

    // 🔍 Ordena todos os voos antes de aplicar a limitação
    quick_sort_voos(voos, 0, total_voos - 1, trigrama);

    // 🔍 Agora, apenas mantém os primeiros `max_resultados`
    if (total_voos > max_resultados) {
        total_voos = max_resultados;
    }
}
