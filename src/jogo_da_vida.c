#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<stdbool.h>

//Copia a matrizB para a matrizA

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

int** criarMatrizQuadrada(int *x) {
    int **matriz = (int**)malloc(*x * sizeof(int*));
    for (int i = 0; i < *x; i++) {
        matriz[i] = (int*)malloc(*x * sizeof(int));
    }

    return matriz;
}

void copiarMatriz(int** matrizA, int** matrizB, int *tamMatriz){ 
    matrizA = criarMatrizQuadrada(tamMatriz);
    for (int i = 0; i < *tamMatriz; i++) {
            for (int j = 0; j < *tamMatriz; j++) {
               matrizA[i][j] =  matrizB[i][j]; 
            }
        }
}

int** criarMatrizAleatoria(int *tamMatriz){
    int* lista = criarListaAleatoria(*tamMatriz*(*tamMatriz));
    for ( int i = 0; i<*tamMatriz*(*tamMatriz); i++){
        printf(" %d ",lista[i]);
    }
    int **matriz = criarMatrizQuadrada(tamMatriz);
    int index = 0;
    for (int i = 0; i < *tamMatriz; i++) {
        for (int j = 0; j < *tamMatriz; j++) {
            matriz[i][j] = lista[index]; // Armazena os elementos na matriz
            index++;
            printf(" %d[%d][%d] ",matriz[i][j],i,j);
        }
        printf("\n");
    }
    free(lista);
    return matriz;
}

int validarPosicao(int i, int j, int *tamMatriz) {
    return (i >= 0 && i < *tamMatriz && j >= 0 && j < *tamMatriz);
}

// Função para checar as posições vizinhas e contar quantos 1s existem ao redor de uma posição [i][j]
void checarVizinhos(int **matrizA, int **matrizB, int i, int j, int *tamMatriz) {
    int count = 0; //Contador para quantidade de 1 envolta da posiçao
    
    // Array para as posições vizinhas
    int row[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int col[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    // Checa todas as posições vizinhas
    for (int k = 0; k < 8; k++) {
        if (validarPosicao(i + row[k], j + col[k], tamMatriz) && matrizB[i + row[k]][j + col[k]] == 1) {
            count++;
        }
    }
    if(count>3){
        matrizA[i][j] = 0;
    } else if (count<2){
        matrizA[i][j] = 0;
    }
    else if (count == 3)
    {
        matrizA[i][j] = 1;
    }
    else if (count == 2 && matrizB[i][j]==1){
        matrizA[i][j] = 1;
    }
    else{
        matrizA[i][j] = 0;
    }
    // printf("%d=",matrizA[i][j]);
}

// Função principal para percorrer a matriz e imprimir o número de 1s ao redor de cada posição
void checarTodosVizinhos(int **matrizA, int** matrizB, int *tamMatriz) {
    for (int i = 0; i < *tamMatriz; i++) {
        for (int j = 0; j < *tamMatriz; j++) {
            checarVizinhos(matrizA, matrizB, i, j, tamMatriz);
            printf("M[%d][%d]= %d -> %d  ", i, j, matrizB[i][j],matrizA[i][j]);
        }
        printf("\n");
    }
}



void lerMatrizGeracional(int **matriz, int geracaoAtual, int *tamMatriz){
    char letra;
    FILE *arquivo = fopen("../src/output/geracoes.mps", "r");
        if( arquivo == NULL){
            printf("Erro ao abrir o arquivo ao inserir na matriz");
            exit (0);
        }
    while (!(feof(arquivo))) {
        // Ler a letra
        fscanf(arquivo, "%c", &letra);
        // Se a letra for 'G', ler o número seguinte
        printf("%c",letra);
        if (letra == 'G') {
            int numero;
            fscanf(arquivo, "%d", &numero);
            // Se o número for igual a 0, ler a matriz
            if (numero == geracaoAtual) {
                for (int i=0;i<*tamMatriz;i++){
                    for (int j=0;j<*tamMatriz;j++){
                        fscanf(arquivo,"%d ",&matriz[i][j]);
                        printf("%d ", matriz[i][j]);
                }
                printf("\n");
                }
            }
        }
    }
    fclose(arquivo);
}

int** lerMatrizInput(char nomeArquivo[20], int *tamMatriz){
    FILE *arquivo = fopen(nomeArquivo, "r");
    if( arquivo == NULL){
        printf("Erro ao abrir o arquivo para leitura do input");
        exit (0);
    }

    int** matriz = criarMatrizQuadrada(tamMatriz);
    int i,j = 0;
    /*
    while(fscanf(arquivo,"%d",&aux)!= EOF){
        if (fgetc(arquivo == 'G')){
            return 1;
        }
    }*/
    int aux;
    fscanf(arquivo,"%d",&aux);
    for (i=0;i<*tamMatriz;i++){
        for (j=0;j<*tamMatriz;j++){
            fscanf(arquivo, "%d ", &matriz[i][j]);
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
    fclose(arquivo);
    return matriz;
}

void inserirMatrizEmArquivo( int geracoes, int **matriz, int *tamMatriz) {
    
    int **matrizAux = criarMatrizQuadrada(tamMatriz);
    
    for(int n =0; n<geracoes;n++){
        FILE *arquivo = fopen("../src/output/geracoes.mps", "a");
        if( arquivo == NULL){
            printf("Erro ao abrir o arquivo ao inserir na matriz");
            exit (0);
        }
        copiarMatriz(matrizAux, matriz, tamMatriz);
        fprintf(arquivo, "G%d\n", n+1);
        checarTodosVizinhos(matrizAux,matriz,tamMatriz);
        for (int i = 0; i < *tamMatriz; i++) {
            for (int j = 0; j < *tamMatriz; j++) {
                fprintf(arquivo, "%d ", matrizAux[i][j]); // Escreve cada elemento da matriz no arquivo
            }
            fprintf(arquivo, "\n"); // Nova linha para cada linha da matriz
        }
        fclose(arquivo);
        lerMatrizGeracional(matriz,n,tamMatriz);
    }
    free(matriz);
    free(matrizAux);
}

int main() {
    int geracoes;
    int n =5;
    int *tamMatriz = &n;
    FILE *arquivo; 
    arquivo = fopen("../datasets/input.mps", "r");
    if( arquivo == NULL){
        printf("Erro ao abrir o arquivo input");
        return 1;
    }
    fscanf(arquivo,"%d",tamMatriz);
    fclose(arquivo);
    // scanf("%d",&tamMatriz);
    printf("Digite número de gerações a ser avaliado: ");
    scanf("%d", &geracoes);
    arquivo = fopen("../src/output/geracoes.mps", "w+");
        if( arquivo == NULL){
            printf("Erro ao abrir o arquivo ao inserir na matriz");
            exit (0);
        }
    fclose(arquivo);
    //int** matrizAleatoria = criarListaAleatoria(tamMatriz);
    int** matrizG = lerMatrizInput("../datasets/input.mps",tamMatriz);    
    inserirMatrizEmArquivo(geracoes, matrizG, tamMatriz);
    
    return 0;
}
