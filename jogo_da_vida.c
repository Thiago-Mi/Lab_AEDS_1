#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<stdbool.h>

int* criarListaAleatoria(int x) {

    int* lista = (int *) malloc(x * sizeof(int));
    // Preencher a lista com 1/3 de 1 e 2/3 de 0 aleatoriamente
    int vivos = x/3;
    srand(42);
    for (int i = 0; i < x; i++) {
        if (vivos > 0) {
            lista[i] = 1;
            vivos--;
        } else {
            lista[i] = 0;
        }
    }

    for (int i = x - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = lista[i];
        lista[i] = lista[j];
        lista[j] = temp;
    }

    return lista;
}

int** criarMatrizQuadrada(int x) {
    int **matriz = (int**)malloc(x * sizeof(int*));
    for (int i = 0; i < x; i++) {
        matriz[i] = (int*)malloc(x * sizeof(int));
    }

    return matriz;
}


void inserirMatrizEmArquivo(bool *matrizGeradora, int *geracao, int **matriz, int tamanho, FILE *arquivo) {
    if (*matrizGeradora){
        fprintf(arquivo, "Mg %d\n", *geracao);
    }else{
        fprintf(arquivo, "G%d \n", *geracao);
        *geracao++;
    }
    *matrizGeradora = !matrizGeradora;
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fprintf(arquivo, "%d ", matriz[i][j]); // Escreve cada elemento da matriz no arquivo
        }
        fprintf(arquivo, "\n"); // Nova linha para cada linha da matriz
    }

}

int main() {
    bool matrizGeradora = true;
    int geracao = 1;
    int tamMatriz;
    scanf("%d",&tamMatriz);
    int* lista = criarListaAleatoria(tamMatriz*tamMatriz);
    for ( int i =0; i<tamMatriz*tamMatriz; i++){
        printf(" %d ",lista[i]);
    }


     int **matriz = criarMatrizQuadrada(tamMatriz);
    int index = 0;
    printf("Digite os elementos da matriz:\n");
    for (int i = 0; i < tamMatriz; i++) {
        for (int j = 0; j < tamMatriz; j++) {
            matriz[i][j] = lista[index]; // Armazena os elementos na matriz
            index++;
            printf(" %d[%d][%d] ",matriz[i][j],i,j);
        }
        printf("\n");
    }
    FILE *arquivo = fopen("geracoes.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }
    inserirMatrizEmArquivo(&matrizGeradora, &geracao, matriz, tamMatriz, arquivo);
    inserirMatrizEmArquivo(&matrizGeradora, &geracao, matriz, tamMatriz, arquivo); //Teste para impressao da matriz da segunda geraçao 
    // printf("Matriz inserida no arquivo matriz.txt.\n");
    free(lista);
    fclose(arquivo);
    return 0;
}
