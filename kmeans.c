#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "arquivo.h"
#include "kmeans.h"

int **alocarMatrizClusters(int altura, int largura) {
	int i;
	int **matriz = (int**) malloc(sizeof(int*) * altura);

	for (i = 0; i < altura; i++) {
		matriz[i] = (int*) malloc(sizeof(int) * largura);
	}

	return matriz;
}

void liberarMatrizClusters(int **matriz, int altura) {
	int i;

	for (i = 0; i < altura; i++) {
		free(matriz[i]);
	}

	free(matriz);
}

int **inicializarMatrizClusters(int altura, int largura) {
	int i, j;
	int **matriz = alocarMatrizClusters(altura, largura);

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			matriz[i][j] = 0;
		}
	}

	return matriz;
}

RGB *inicializarCentroides(RGB **matriz, int altura, int largura) {
	int i, j, k, l, iMenor, jMenor, iMaior, jMaior;
	float maiorDistancia = 0;
	RGB *centroides = (RGB*) malloc(sizeof(RGB) * KCLUSTERS);

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			if (matriz[i][j].vermelho != 255 || matriz[i][j].verde != 255 || matriz[i][j].azul != 255) {
				for (k = i; k < altura; k++) {
					for (l = j; l < largura; l++) {
						if (matriz[k][l].vermelho != 255 || matriz[k][l].verde != 255 || matriz[k][l].azul != 255) {
							float distancia = distanciaEuclidianaRGB(matriz[i][j], matriz[k][l]);

							if (distancia > maiorDistancia) {
								maiorDistancia = distancia;
								iMenor = i;
								jMenor = j;
								iMaior = k;
								jMaior = l;
							}
						}
					}
				}
			}
		}
	}

	centroides[0].vermelho = matriz[iMenor][jMenor].vermelho;
	centroides[0].verde = matriz[iMenor][jMenor].verde;
	centroides[0].azul = matriz[iMenor][jMenor].azul;
	centroides[1].vermelho = matriz[iMaior][jMaior].vermelho;
	centroides[1].verde = matriz[iMaior][jMaior].verde;
	centroides[1].azul = matriz[iMaior][jMaior].azul;

	return centroides;
}

int atualizarCluster(RGB *centroides, RGB pixel) {
	int i, cluster;
	float menorDistancia;

	for (i = 0; i < KCLUSTERS; i++) {
		float distancia = distanciaEuclidianaRGB(centroides[i], pixel);

		if (i == 0) {
			menorDistancia = distancia;
			cluster = i + 1;
		}

		if (distancia < menorDistancia) {
			menorDistancia = distancia;
			cluster = i + 1;
		}
	}

	return cluster;
}

void atualizarCentroides(RGB *centroides, int **matrizClusters, RGB **matriz, int altura, int largura) {
	int i, j;
	int *iCentroides = (int*) malloc(sizeof(int) * KCLUSTERS);
	int *somaR = (int*) malloc(sizeof(int) * KCLUSTERS);
	int *somaG = (int*) malloc(sizeof(int) * KCLUSTERS);
	int *somaB = (int*) malloc(sizeof(int) * KCLUSTERS);

	for (i = 0; i < KCLUSTERS; i++) {
		iCentroides[i] = 0;
		somaR[i] = 0;
		somaG[i] = 0;
		somaB[i] = 0;
	}

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			if (matrizClusters[i][j]) {
				iCentroides[matrizClusters[i][j] - 1]++;
				somaR[matrizClusters[i][j] - 1] += matriz[i][j].vermelho;
				somaG[matrizClusters[i][j] - 1] += matriz[i][j].verde;
				somaB[matrizClusters[i][j] - 1] += matriz[i][j].azul;
			}
		}
	}

	for (i = 0; i < KCLUSTERS; i++) {
		if (iCentroides[i]) {
			centroides[i].vermelho = (int) somaR[i] / iCentroides[i];
			centroides[i].verde = (int) somaG[i] / iCentroides[i];
			centroides[i].azul = (int) somaB[i] / iCentroides[i];
		}
	}

	free(iCentroides);
	free(somaR);
	free(somaG);
	free(somaB);
}

void colorirClusters(RGB **matriz, int **matrizClusters, RGB *centroides, int altura, int largura) {
	int i, j, iCluster;

	RGB preto;
	preto.vermelho = 0;
	preto.verde = 0;
	preto.azul = 0;

	float dCentroide1 = distanciaEuclidianaRGB(centroides[0], preto);
	float dCentroide2 = distanciaEuclidianaRGB(centroides[1], preto);

	if (dCentroide1 < dCentroide2) {
		iCluster = 1;
	} else {
		iCluster = 2;
	}

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			if (matrizClusters[i][j] > 0) {
				if (matrizClusters[i][j] == iCluster) {
					matriz[i][j].vermelho = 235;
					matriz[i][j].verde = 50;
					matriz[i][j].azul = 50;
				} else {
					matriz[i][j].vermelho = 150;
					matriz[i][j].verde = 235;
					matriz[i][j].azul = 50;
				}
			}
		}
	}

	liberarMatrizClusters(matrizClusters, altura);
}

void kmeans(RGB **matriz, int altura, int largura) {
	int i, j, recalcularCentroides;
	int **matrizClusters = inicializarMatrizClusters(altura, largura);
	RGB *centroides = inicializarCentroides(matriz, altura, largura);

	do {
		recalcularCentroides = 0;
		for (i = 0; i < altura; i++) {
			for (j = 0; j < largura; j++) {
				if (matriz[i][j].vermelho != 255 || matriz[i][j].verde != 255 || matriz[i][j].azul != 255) {
					int cluster = matrizClusters[i][j];
					matrizClusters[i][j] = atualizarCluster(centroides, matriz[i][j]);

					if (cluster != matrizClusters[i][j]) {
						recalcularCentroides = 1;
					}
				}
			}
		}

		if (recalcularCentroides) {
			atualizarCentroides(centroides, matrizClusters, matriz, altura, largura);
		}
	} while (recalcularCentroides);

	colorirClusters(matriz, matrizClusters, centroides, altura, largura);

	free(centroides);
}