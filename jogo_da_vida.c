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

int** criarMatrizAleatoria(int tamMatriz){
    int* lista = criarListaAleatoria(tamMatriz*tamMatriz);
    for ( int i = 0; i<tamMatriz*tamMatriz; i++){
        printf(" %d ",lista[i]);
    }
    int **matriz = criarMatrizQuadrada(tamMatriz);
    int index = 0;
    for (int i = 0; i < tamMatriz; i++) {
        for (int j = 0; j < tamMatriz; j++) {
            matriz[i][j] = lista[index]; // Armazena os elementos na matriz
            index++;
            printf(" %d[%d][%d] ",matriz[i][j],i,j);
        }
        printf("\n");
    }
    free(lista);
    return matriz;
}

void inserirMatrizEmArquivo(bool *matrizGeradora, int *geracao, int **matriz, int tamanho, FILE *arquivo) {
    if (*matrizGeradora){
        fprintf(arquivo, "M%d\n", *geracao--);
    }else{
        fprintf(arquivo, "G%d\n",*geracao);
        *geracao++;
    }
    *matrizGeradora = !matrizGeradora;
    //---> implementar função do jogo para modificar a matriz aqui <---
    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            fprintf(arquivo, "%d ", matriz[i][j]); // Escreve cada elemento da matriz no arquivo
        }
        fprintf(arquivo, "\n"); // Nova linha para cada linha da matriz
    }
}

int** lerMatrizArquivo(char nomeArquivo[10],bool lerGeradora, int tamMatriz){
    FILE *arquivo = fopen(nomeArquivo, "r");
    int** matriz = criarMatrizQuadrada(tamMatriz);
    int i,j = 0;
    char linha[3];
    if( arquivo == NULL){
        printf("Erro ao abrir o arquivo");
        return 0;
    }
    /*
    while(fscanf(arquivo,"%d",&aux)!= EOF){
        if (fgetc(arquivo == 'G')){
            return 1;
        }
    }*/
    fgets(linha, 3, arquivo);
    for (i=0;i<tamMatriz;i++){
        for (j=0;j<tamMatriz;j++){
            fscanf(arquivo, "%d ", &matriz[i][j]);
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
    fclose(arquivo);
    return matriz;
}

int main() {
    bool matrizGeradora = true;
    int geracoes = 2;
    int tamMatriz = 5;
    // scanf("%d",&tamMatriz);
    scanf("Digite quantas gerações serão feitas: %d", &geracoes);
    FILE *geracoesArq = fopen("geracoes.txt", "wr");
    //int** matrizAleatoria = criarListaAleatoria(tamMatriz);
    int** matrizG = lerMatrizArquivo("input.txt", true,tamMatriz);    
    inserirMatrizEmArquivo(&matrizGeradora, &geracoes, matrizG, tamMatriz,geracoesArq);
    inserirMatrizEmArquivo(&matrizGeradora, &geracoes, matrizG, tamMatriz,geracoesArq); //Teste para impressao da matriz da segunda geraçao 
    inserirMatrizEmArquivo(&matrizGeradora, &geracoes, matrizG, tamMatriz,geracoesArq);
    fclose(geracoesArq);
    return 0;
}
