#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "arquivo.h"
#include "otsu.h"

void tonsDeCinza(RGB **matriz, int altura, int largura) {
	int i, j;

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			unsigned char media = (matriz[i][j].azul + matriz[i][j].verde + matriz[i][j].vermelho) / 3;

			matriz[i][j].vermelho = media;
			matriz[i][j].verde = media;
			matriz[i][j].azul = media;
		}
	}
}

double otsuPeso(int *histograma, double qtdPixeis, int inicio, int fim) {
	int i;
	double numerador = 0;

	for (i = inicio; i < fim; i++) {
		numerador += histograma[i];
	}

	return numerador / qtdPixeis;
}

double otsuVariancia(int *histograma, int inicio, int fim) {
	int i;
	double numeradorMean = 0, numeradorVariance = 0, denominador = 0;

	for (i = inicio; i < fim; i++) {
		numeradorMean += (i * histograma[i]);
		denominador += histograma[i];
	}

	double mean = (denominador > 0 ? numeradorMean / denominador : 0);

	for (i = inicio; i < fim; i++) {
		numeradorVariance += (pow(i - mean, 2) * histograma[i]);
	}

	return (denominador > 0 ? numeradorVariance / denominador : 0);
}

int *criarHistograma(int n) {
	int i;
	int *histograma = (int*) malloc(sizeof(int) * n);

	for (i = 0; i < n; i++) {
		histograma[i] = 0;
	}

	return histograma;
}

void popularHistogramaTonsCinza(int *histograma, RGB **matriz, int altura, int largura) {
	int i, j;

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			histograma[matriz[i][j].vermelho]++;
		}
	}
}

void binarizacao(RGB **matriz, int altura, int largura, RGB **matrizColorida) {
	int i, j, iVariancia = 0;
	int *histograma = criarHistograma(256);
	double *variancia = (double*) malloc(sizeof(double) * 256);
	double qtdPixeis = altura * largura;
	popularHistogramaTonsCinza(histograma, matriz, altura, largura);

	for (i = 0; i < 256; i++) {
		double bgW = otsuPeso(histograma, qtdPixeis, 0, i);
		double bgV = otsuVariancia(histograma, 0, i);
		double fgW = otsuPeso(histograma, qtdPixeis, i, 256);
		double fgV = otsuVariancia(histograma, i, 256);
		variancia[i] = ((bgW * bgV) + (fgW * fgV));

		if (variancia[i] < variancia[iVariancia]) {
			iVariancia = i;
		}
	}

	free(histograma);
	free(variancia);

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			if (matriz[i][j].vermelho >= iVariancia) {
				matriz[i][j].vermelho = 255;
				matriz[i][j].verde = 255;
				matriz[i][j].azul = 255;
			} else {
				matriz[i][j].vermelho = matrizColorida[i][j].vermelho;
				matriz[i][j].verde = matrizColorida[i][j].verde;
				matriz[i][j].azul = matrizColorida[i][j].azul;
			}
		}
	}
}