
# TP3 - Busca de Passagens Aéreas

## Descrição do Projeto
Este projeto foi desenvolvido como parte do Trabalho Prático 3 (TP3) da disciplina de Estruturas de Dados e Algoritmos. O objetivo é criar um sistema eficiente para buscar passagens aéreas com base em consultas personalizadas, utilizando estruturas de dados avançadas para garantir desempenho e flexibilidade.

## Problema
A busca por passagens aéreas é um problema comum em sistemas de reservas, onde o usuário deseja encontrar voos que atendam a critérios específicos, como origem, destino, preço, e número de paradas. Além disso, os resultados precisam ser ordenados por diferentes critérios, como preço, duração e paradas.

O desafio deste trabalho é implementar um sistema que processe um arquivo de entrada com uma lista de voos e consultas, gerando uma saída formatada com os resultados das buscas, obedecendo às restrições e especificações fornecidas.

## Proposta de Solução
O sistema será implementado na linguagem C, utilizando estruturas de dados como listas dinâmicas e árvores AVL para organizar e processar os dados de maneira eficiente. A solução inclui:
- Armazenar voos em memória a partir de um arquivo de entrada.
- Processar consultas com filtros baseados em expressões lógicas.
- Ordenar os resultados com base em critérios específicos.
- Gerar uma saída formatada com os resultados das consultas.

## Estrutura do Projeto
A estrutura do projeto segue o padrão especificado:

```plaintext
TP/
├── src/                   # Código-fonte
│   ├── main.c             # Ponto de entrada do programa.
│   ├── voo.c              # Implementação da TAD Voo.
│   ├── lista_voos.c       # Implementação da TAD Lista de Voos.
│   ├── consulta.c         # Implementação da TAD Consulta.
│   ├── arvore_avl.c       # Implementação da Árvore AVL.
│   ├── filtros.c          # Funções auxiliares (se necessário).
├── include/               # Cabeçalhos das TADs e funções.
│   ├── voo.h
│   ├── lista_voos.h
│   ├── consulta.h
│   ├── arvore_avl.h
│   ├── filtros.h
├── obj/                   # Arquivos objeto gerados na compilação.
├── bin/                   # Executável final.
│   ├── tp3.out            # Executável gerado pelo Makefile.
├── Makefile               # Script de compilação do projeto.
```

## Funcionalidades
1. **Carregamento de Dados**:
   - Lê uma lista de voos de um arquivo de entrada.
   - Armazena os dados em estruturas internas para processamento.

2. **Processamento de Consultas**:
   - Interpreta filtros definidos em expressões lógicas.
   - Aplica os filtros para selecionar voos que atendem aos critérios.
   - Ordena os resultados conforme os critérios especificados.

3. **Geração de Saída**:
   - Para cada consulta, imprime a consulta original e os voos que atendem aos critérios no formato correto.

## Como Executar
1. **Pré-requisitos**:
   - Sistema operacional Linux.
   - Compilador GCC.
   - Makefile configurado para compilar o projeto.

2. **Compilação**:
   Execute o comando a partir da raiz do projeto:
   ```bash
   make all
   ```

3. **Execução**:
   Após compilar, o executável estará disponível no diretório `bin/`. Para executar o programa:
   ```bash
   ./bin/tp3.out <arquivo_de_entrada>
   ```
   Substitua `<arquivo_de_entrada>` pelo nome do arquivo contendo a lista de voos e consultas.

4. **Exemplo de Entrada**:
   Um arquivo de entrada típico pode conter:
   ```
   2
   ORD LAX 198.6 1 2022-05-20T19:21:00 2022-05-20T23:50:00 0
   DFW LGA 108.6 5 2022-08-06T20:47:00 2022-08-07T00:15:00 0
   1 PDS (((org==ORD)&&(prc<200)))
   ```

5. **Exemplo de Saída**:
   O programa gerará a seguinte saída:
   ```
   1 PDS (((org==ORD)&&(prc<200)))
   ORD LAX 198.6 1 2022-05-20T19:21:00 2022-05-20T23:50:00 0
   ```

## Estruturas de Dados Utilizadas
- **TAD Voo**:
  Representa cada voo da lista.
- **Lista de Voos**:
  Armazena todos os voos em um vetor dinâmico.
- **Consulta**:
  Representa uma consulta, incluindo filtros e critérios de ordenação.
- **Árvore AVL**:
  Utilizada para criar índices de atributos como origem e destino, otimizando buscas.
- **Árvore Sintática**:
  Representa as expressões lógicas das consultas para aplicar os filtros.

## Autor
- Desenvolvido como parte do TP3 da disciplina de Estruturas de Dados e Algoritmos.

## Licença
Este projeto foi desenvolvido exclusivamente para fins acadêmicos e está sob as diretrizes da disciplina.
```