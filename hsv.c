#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"
#include "hsv.h"

HSV **alocarMatrizHSV(int altura, int largura) {
	int i;
	HSV **matriz = (HSV**) malloc(sizeof(HSV*) * altura);

	for (i = 0; i < altura; i++) {
		matriz[i] = (HSV*) malloc(sizeof(HSV) * largura);
	}

	return matriz;
}

void liberarMatrizHSV(HSV **matriz, int altura) {
	int i;

	for (i = 0; i < altura; i++) {
		free(matriz[i]);
	}

	free(matriz);
}

HSV **rgb2hsv(RGB **matrizRGB, int altura, int largura) {
	int i, j;
	HSV **matrizHSV = alocarMatrizHSV(altura, largura);

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			float r = matrizRGB[i][j].vermelho / 255.0;
			float g = matrizRGB[i][j].verde / 255.0;
			float b = matrizRGB[i][j].azul / 255.0;
			float min = min_f(r, g, b);
			float max = max_f(r, g, b);
			float h;

			if (max == r && g >= b) {
				h = (max - min == 0 ? 0 : 60 * ((g - b) / (max - min)));
			} else if (max == r && g < b) {
				h = (max - min == 0 ? 0 : 60 * ((g - b) / (max - min)) + 360);
			} else if (max == g) {
				h = (max - min == 0 ? 0 : 60 * ((b - r) / (max - min)) + 120);
			} else if (max == b) {
				h = (max - min == 0 ? 0 : 60 * ((r - g) / (max - min)) + 240);
			}

			matrizHSV[i][j].hue = h;
			matrizHSV[i][j].saturation = (max == 0 ? 0 : ((max - min) / max) * 100.0);
			matrizHSV[i][j].value = max * 100.0;
		}
	}

	return matrizHSV;
}

RGB **hsv2rgb(HSV **matrizHSV, int altura, int largura) {
	int i, j;
	RGB **matrizRGB = alocarMatrizRGB(altura, largura);

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			float r = 0.0, g = 0.0, b = 0.0;
			float h = matrizHSV[i][j].hue / 60.0;
			float s = matrizHSV[i][j].saturation / 100;
			float v = matrizHSV[i][j].value / 100;
			float c = v * s;
			float m = v - c;
			float x = c * (1 - fabs(fmod(h, 2.0) - 1));

			if (h >= 0 && h < 1) {
				r = c;
				g = x;
			} else if (h >= 1 && h < 2) {
				r = x;
				g = c;
			} else if (h >= 2 && h < 3) {
				g = c;
				b = x;
			} else if (h >= 3 && h < 4) {
				g = x;
				b = c;
			} else if (h >= 4 && h < 5) {
				r = x;
				b = c;
			} else {
				r = c;
				b = x;
			}

			matrizRGB[i][j].vermelho = (int) ((r + m) * 255.0);
			matrizRGB[i][j].verde = (int) ((g + m) * 255.0);
			matrizRGB[i][j].azul = (int) ((b + m) * 255.0);
		}
	}

	return matrizRGB;
}

float obterTonalidade(RGB pixel) {
	float h;
	float r = pixel.vermelho / 255.0;
	float g = pixel.verde / 255.0;
	float b = pixel.azul / 255.0;
	float M = max_f(r, g, b);
	float m = min_f(r, g, b);
	float C = M - m;

	if (C == 0) {
		h = 0;
	} else if (M == r) {
		h = fmod((g - b) / C, 6);
	} else if (M == g) {
		h = (b - r) / C + 2;
	} else {
		h = (r - g) / C + 4;
	}

 	h *= 60;

 	if (h < 0) {
 		h += 360;
 	}

	return h;
}

void detectarDoenca(RGB **matriz, char *arquivo, int altura, int largura) {
	int i, j;
	float areaAfetada, areaTotal = 0.0, doenca = 0.0;

	for (i = 0; i < altura; i++) {
		for (j = 0; j < largura; j++) {
			if (matriz[i][j].vermelho != 255 || matriz[i][j].verde != 255 || matriz[i][j].azul != 255) {
				float hue = obterTonalidade(matriz[i][j]);

				if (hue >= 38.0 && hue <= 86.0) {
					matriz[i][j].vermelho = 0;
					matriz[i][j].verde = 255;
					matriz[i][j].azul = 0;
				} else {
					matriz[i][j].vermelho = 255;
					matriz[i][j].verde = 0;
					matriz[i][j].azul = 0;
					doenca++;
				}

				areaTotal++;
			}
		}
	}

	printf("Arquivo: \"%s\" - Área afetada: %5.2f%% (%.0f pixeis de %.0f)\n", arquivo, (doenca / areaTotal) * 100.0, doenca, areaTotal);
}