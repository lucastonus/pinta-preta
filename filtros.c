#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "arquivo.h"
#include "filtros.h"

int obterDimensaoMatrizMediana(int pos, int dimensao) {
	int resultado = MATRIZ_MEDIANA;
	int metadeFiltro = floor(MATRIZ_MEDIANA / 2);

	if (pos - metadeFiltro < 0) {
		resultado += pos - metadeFiltro;
	}

	if ((dimensao - 1) - (pos + metadeFiltro) < 0) {
		resultado += (dimensao - 1) - (pos + metadeFiltro);
	}

	return resultado;
}

unsigned char mediana(unsigned char *vetor, int tamVetor) {
	int i, j;
	unsigned char aux;

	for (i = 0; i < tamVetor; i++) {
		for (j = i + 1; j < tamVetor; j++) {
			if (vetor[j] < vetor[i]) {
				aux = vetor[i];
				vetor[i] = vetor[j];
				vetor[j] = aux;
			}
		}
	}

	return vetor[(int) floor(tamVetor / 2)];
}

void filtroMediana(RGB **matriz, int altura, int largura) {
	if (MATRIZ_MEDIANA % 2 != 0) {
		int i, j, k, l;
		RGB **matrizCopia = copiarMatriz(matriz, altura, largura);

		for (i = 0; i < altura; i++) {
			for (j = 0; j < largura; j++) {
				int metadeFiltro = floor(MATRIZ_MEDIANA / 2);
				int alturaVetor = obterDimensaoMatrizMediana(i, altura);
				int larguraVetor = obterDimensaoMatrizMediana(j, largura);
				int tamVetor = alturaVetor * larguraVetor;
				unsigned char vetorR[tamVetor];
				unsigned char vetorG[tamVetor];
				unsigned char vetorB[tamVetor];

				int vetorPos = 0;
				for (k = -metadeFiltro; k <= metadeFiltro; k++) {
					for (l = -metadeFiltro; l <= metadeFiltro; l++) {
						if ((i + k) >= 0 && (i + k) < altura && (j + l) >= 0 && (j + l) < largura) {
							vetorR[vetorPos] = matrizCopia[i + k][j + l].vermelho;
							vetorG[vetorPos] = matrizCopia[i + k][j + l].verde;
							vetorB[vetorPos] = matrizCopia[i + k][j + l].azul;
							vetorPos++;
						}
					}
				}

				matriz[i][j].vermelho = mediana(vetorR, tamVetor);
				matriz[i][j].verde = mediana(vetorG, tamVetor);
				matriz[i][j].azul = mediana(vetorB, tamVetor);
			}
		}

		liberarMatrizRGB(matrizCopia, altura);
	} else {
		printf("Tamanho da matriz do filtro deve ser ímpar.\n");
	}
}

int truncate(int value) {
	int res = value;

	if (value < 0) {
		res = 0;
	}

	if (value > 255) {
		res = 255;
	}

    return res;
}

void brilho(RGB **matriz, int altura, int largura) {
	int i, j;

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			matriz[i][j].vermelho = truncate(matriz[i][j].vermelho + BRILHO) ;
			matriz[i][j].verde = truncate(matriz[i][j].verde + BRILHO);
			matriz[i][j].azul = truncate(matriz[i][j].azul + BRILHO);
		}
	}
}

void contraste(RGB **matriz, int altura, int largura) {
	int i, j;
	float fator = (259.0 * (CONTRASTE + 255.0)) / (255.0 * (259.0 - CONTRASTE));

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			matriz[i][j].vermelho = truncate((int) (fator * (matriz[i][j].vermelho - 128) + 128)) ;
			matriz[i][j].verde = truncate((int) (fator * (matriz[i][j].verde - 128) + 128));
			matriz[i][j].azul = truncate((int) (fator * (matriz[i][j].azul - 128) + 128));
		}
	}
}