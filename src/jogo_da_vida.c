/*
 * Este programa implementa o Jogo da Vida onde células vivas sobrevivem,
 * nascem ou morrem de acordo com um conjunto de regras simples.
 *
 * Regras:
 * 1. Qualquer célula viva com menos de dois vizinhos vivos morre (solidão).
 * 2. Qualquer célula viva com dois ou três vizinhos vivos permanece viva.
 * 3. Qualquer célula viva com mais de três vizinhos vivos morre (superpopulação).
 * 4. Qualquer célula morta com exatamente três vizinhos vivos se torna uma célula viva (reprodução).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/**
 * @brief Aloca uma matriz quadrada com base em um tamanho dado.
 *
 * @param x Tamanho da matriz.
 * @return Ponteiro para a matriz criada.
 */

int **criarMatrizQuadrada(int *x)
{
    int **matriz = (int **)malloc(*x * sizeof(int *));
    for (int i = 0; i < *x; i++)
    {
        matriz[i] = (int *)malloc(*x * sizeof(int));
    }

    return matriz;
}

/**
 * @brief Cria uma lista aleatória com um terço de 1s e dois terços de 0s.
 *
 * @param x Tamanho da lista.
 * @return Ponteiro para a lista criada.
 */
int *criarListaAleatoria(int x)
{
    int *lista = (int *)malloc(x * sizeof(int));
    int vivos = x / 3;
    srand(42);
    for (int i = 0; i < x; i++)
    {
        if (vivos > 0)
        {
            lista[i] = 1;
            vivos--;
        }
        else
        {
            lista[i] = 0;
        }
    }

    for (int i = x - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = lista[i];
        lista[i] = lista[j];
        lista[j] = temp;
    }

    return lista;
}

/**
 * @brief Cria uma matriz aleatória baseada em um tamanho dado.
 *
 * @param tamMatriz Tamanho da matriz.
 * @return Ponteiro para a matriz criada.
 */
int **criarMatrizAleatoria(int *tamMatriz)
{
    int *lista = criarListaAleatoria(*tamMatriz * (*tamMatriz));
    int **matriz = criarMatrizQuadrada(tamMatriz);
    int index = 0;
    for (int i = 0; i < *tamMatriz; i++)
    {
        for (int j = 0; j < *tamMatriz; j++)
        {
            matriz[i][j] = lista[index];
            index++;
        }
    }
    free(lista);
    return matriz;
}

/**
 * @brief Copia a matrizB para a matrizA.
 *
 * @param matrizA Matriz de destino.
 * @param matrizB Matriz de origem.
 * @param tamMatriz Tamanho das matrizes.
 */
void copiarMatriz(int **matrizA, int **matrizB, int *tamMatriz)
{
    matrizA = criarMatrizQuadrada(tamMatriz);
    for (int i = 0; i < *tamMatriz; i++)
    {
        for (int j = 0; j < *tamMatriz; j++)
        {
            matrizA[i][j] = matrizB[i][j];
        }
    }
}

/**
 * @brief Valida uma posição na matriz.
 *
 * @param i Linha.
 * @param j Coluna.
 * @param tamMatriz Tamanho da matriz.
 * @return true se a posição é válida, false caso contrário.
 */
int validarPosicao(int i, int j, int *tamMatriz)
{
    return (i >= 0 && i < *tamMatriz && j >= 0 && j < *tamMatriz);
}

/**
 * @brief Checa as posições vizinhas de uma célula, conta quantos 1s existem ao redor e altera a matriz de destino baseado na logica geracional.
 *
 * @param matrizA Matriz de destino.
 * @param matrizB Matriz de origem.
 * @param i Linha da célula.
 * @param j Coluna da célula.
 * @param tamMatriz Tamanho da matriz.
 */
void checarVizinhos(int **matrizA, int **matrizB, int i, int j, int *tamMatriz)
{
    int count = 0;

    int linha[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int colun[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int k = 0; k < 8; k++)
    {
        if (validarPosicao(i + linha[k], j + colun[k], tamMatriz) && matrizB[i + linha[k]][j + colun[k]] == 1)
        {
            count++;
        }
    }
    if (count > 3) // Implementação da regra de superpopulação
    {
        matrizA[i][j] = 0;
    }
    else if (count < 2) // Implementação da regra de solidão
    {
        matrizA[i][j] = 0;
    }
    else if (count == 3) // Implementação da regra de geração
    {
        matrizA[i][j] = 1;
    }
    else if (count == 2 && matrizB[i][j] == 1) // Implementação da regra de sobrevivencia
    {
        matrizA[i][j] = 1;
    }
    else
    {
        matrizA[i][j] = 0;
    }
}

/**
 * @brief Checa todas as células da matriz e atualiza seu estado.
 *
 * @param matrizA Matriz de destino.
 * @param matrizB Matriz de origem.
 * @param tamMatriz Tamanho da matriz.
 */
void checarTodosVizinhos(int **matrizA, int **matrizB, int *tamMatriz)
{
    for (int i = 0; i < *tamMatriz; i++)
    {
        for (int j = 0; j < *tamMatriz; j++)
        {
            checarVizinhos(matrizA, matrizB, i, j, tamMatriz);
        }
    }
}

/**
 * @brief Lê a ultima matriz da geração de um arquivo contendo várias gerações.
 *
 * @param matriz Matriz para armazenar a geração atual.
 * @param geracaoAtual Número da geração a ser lida.
 * @param tamMatriz Tamanho da matriz.
 */
void lerMatrizGeracional(int **matriz, int geracaoAtual, int *tamMatriz)
{
    char letra;
    FILE *arquivo = fopen("../src/output/geracoes.mps", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo ao inserir na matriz");
        exit(0);
    }
    while (!(feof(arquivo)))
    {
        fscanf(arquivo, "%c", &letra);
        if (letra == 'G')
        {
            int numero;
            fscanf(arquivo, "%d", &numero); 
            if (numero == geracaoAtual + 1) //Identificaçãao da geração atual escrita no arquivo
            {
                for (int i = 0; i < *tamMatriz; i++)
                {
                    for (int j = 0; j < *tamMatriz; j++)
                    {
                        fscanf(arquivo, "%d ", &matriz[i][j]);
                    }
                }
                fclose(arquivo);
                return;
            }
        }
    }
}

/**
 * @brief Lê a matriz do arquivo input.
 *
 * @param nomeArquivo Nome do arquivo.
 * @param tamMatriz Tamanho da matriz.
 * @return Ponteiro para a matriz lida.
 */
int **lerMatrizInput(char nomeArquivo[20], int *tamMatriz)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo para leitura do input");
        exit(0);
    }

    int **matriz = criarMatrizQuadrada(tamMatriz);
    int i, j = 0;
    int aux;
    fscanf(arquivo, "%d", &aux);
    for (i = 0; i < *tamMatriz; i++)
    {
        for (j = 0; j < *tamMatriz; j++)
        {
            fscanf(arquivo, "%d ", &matriz[i][j]);
        }
    }
    fclose(arquivo);
    return matriz;
}

/**
 * @brief Insere a matriz em um arquivo contendo várias gerações.
 *
 * @param geracoes Número total de gerações.
 * @param matriz Matriz a ser inserida.
 * @param tamMatriz Tamanho da matriz.
 */
void inserirMatrizEmArquivo(int geracoes, int **matrizInput, int *tamMatriz)
{
    for (int n = 0; n < geracoes; n++) // Itera sobre as gerações e grava as matrizes resultantes em um arquivo
    {
        FILE *arquivo = fopen("../src/output/geracoes.mps", "a");
        int **matrizAux = criarMatrizQuadrada(tamMatriz);
        if (arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo geracoes.mps");
            exit(0);
        }
        fprintf(arquivo, "G%d\n", n + 1);
        checarTodosVizinhos(matrizAux, matrizInput, tamMatriz);
        for (int i = 0; i < *tamMatriz; i++)
        {
            for (int j = 0; j < *tamMatriz; j++)
            {
                fprintf(arquivo, "%d ", matrizAux[i][j]);
            }
            fprintf(arquivo, "\n");
        }
        fclose(arquivo);
        lerMatrizGeracional(matrizInput, n, tamMatriz);
        free(matrizAux);
    }
}

/**
 * @brief Função principal.
 *
 * @return 0 se bem-sucedido.
 */
int main()
{
    int geracoes;
    int tamMatrizValue;
    int *tamMatriz = &tamMatrizValue;
    FILE *arquivo;
    arquivo = fopen("../datasets/input.mps", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo input");
        return 1;
    }
    fscanf(arquivo, "%d", tamMatriz); // Leitura do tamanho da matriz pelo arquivo input.mps
    fclose(arquivo);
    printf("Digite número de gerações a ser avaliado: ");
    scanf("%d", &geracoes);
    arquivo = fopen("../src/output/geracoes.mps", "w+");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo ao inserir na matriz");
        exit(0);
    }
    fclose(arquivo);
    int **matrizInput = lerMatrizInput("../datasets/input.mps", tamMatriz);
    inserirMatrizEmArquivo(geracoes, matrizInput, tamMatriz);
    free(matrizInput);
    return 0;
}
