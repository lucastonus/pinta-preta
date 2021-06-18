#ifndef _KMEANS_H_
#define _KMEANS_H_

#define KCLUSTERS 2

RGB *inicializarCentroides(RGB **matriz, int altura, int largura);
int **alocarMatrizClusters(int altura, int largura);
int **inicializarMatrizClusters(int altura, int largura);
int atualizarCluster(RGB *centroides, RGB pixel);
void liberarMatrizClusters(int **matriz, int altura);
void atualizarCentroides(RGB *centroides, int **matrizClusters, RGB **matriz, int altura, int largura);
void colorirClusters(RGB **matriz, int **matrizClusters, RGB *centroides, int altura, int largura);
void kmeans(RGB **matriz, int altura, int largura);

#endif