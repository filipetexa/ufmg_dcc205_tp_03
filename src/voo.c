#include "voo.h"
#include <string.h> // Para strcpy

// Função para criar um novo voo
Voo criar_voo(const char *origem, const char *destino, float preco, int assentos,
              time_t partida, time_t chegada, int paradas)
{
    Voo novo_voo;

    // Copiar os códigos IATA
    strncpy(novo_voo.origem, origem, sizeof(novo_voo.origem) - 1);
    novo_voo.origem[sizeof(novo_voo.origem) - 1] = '\0';
    strncpy(novo_voo.destino, destino, sizeof(novo_voo.destino) - 1);
    novo_voo.destino[sizeof(novo_voo.destino) - 1] = '\0';

    // Definir os outros atributos
    novo_voo.preco = preco;
    novo_voo.assentos = assentos;
    novo_voo.partida = partida;
    novo_voo.chegada = chegada;
    novo_voo.paradas = paradas;

    // Calcular a duração do voo
    novo_voo.duracao = calcular_duracao(partida, chegada);

    return novo_voo;
}

// Função para calcular a duração total do voo
long calcular_duracao(time_t partida, time_t chegada)
{
    return (long)difftime(chegada, partida);
}
