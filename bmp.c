#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

RGB **alocarMatrizRGB(int altura, int largura) {
	int i;
	RGB **matriz = (RGB**) malloc(sizeof(RGB*) * altura);

	for (i = 0; i < altura; i++) {
		matriz[i] = (RGB*) malloc(sizeof(RGB) * largura);
	}

	return matriz;
}

RGB **popularMatrizImagem(char *arquivo, int altura, int largura) {
	int i, j;
	RGB **matriz = alocarMatrizRGB(altura, largura);

	FILE *fileIn = fopen(arquivo, "rb");
	fseek(fileIn, (sizeof(CABECALHO_ARQUIVO) + sizeof(CABECALHO_IMAGEM)), SEEK_SET);

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			fread(&matriz[i][j], sizeof(RGB), 1, fileIn);
		}

		if ((largura * 3) % 4 != 0) {
			int k, count = 4 - (largura * 3) % 4;
			unsigned char aux;

			for (k = 0; k < count; k++) {
				fread(&aux, sizeof(unsigned char), 1, fileIn);
			}
		}
	}

	fclose(fileIn);

	return matriz;
}

RGB **copiarMatriz(RGB **matriz, int altura, int largura) {
	int i, j;
	RGB **matrizCopia = alocarMatrizRGB(altura, largura);

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			matrizCopia[i][j] = matriz[i][j];
		}
	}

	return matrizCopia;
}

void liberarMatrizRGB(RGB **matriz, int altura) {
	int i;

	for (i = 0; i < altura; i++) {
		free(matriz[i]);
	}

	free(matriz);
}

void gravarImagem(char *arquivo, CABECALHO_ARQUIVO cabecalho_arquivo, CABECALHO_IMAGEM cabecalho_imagem, RGB **matriz) {
	int i, j;
	FILE *fileOut = fopen(arquivo, "wb");
	fwrite(&cabecalho_arquivo, sizeof(CABECALHO_ARQUIVO), 1, fileOut);
	fwrite(&cabecalho_imagem, sizeof(CABECALHO_IMAGEM), 1, fileOut);

	for (i = 0; i < cabecalho_imagem.altura; i++) {
		for (j = 0; j < cabecalho_imagem.largura; j++) {
			fwrite(&matriz[i][j], sizeof(RGB), 1, fileOut);
		}

		if ((cabecalho_imagem.largura * 3) % 4 != 0) {
			int k, count = 4 - (cabecalho_imagem.largura * 3) % 4;
			unsigned char aux = 0;

			for (k = 0; k < count; k++) {
				fwrite(&aux, sizeof(aux), 1, fileOut);
			}
		}
	}

	fclose(fileOut);
}

float distanciaEuclidianaRGB(RGB pixelX, RGB pixelY) {
	return (float) sqrt(pow(pixelX.vermelho - pixelY.vermelho, 2) + pow(pixelX.verde - pixelY.verde, 2) + pow(pixelX.azul - pixelY.azul, 2));
}