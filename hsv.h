#ifndef _HSV_H_
#define _HSV_H_

typedef struct {
	float hue;
	float saturation;
	float value;
} HSV;

#define min_f(a, b, c) (fminf(a, fminf(b, c)))
#define max_f(a, b, c) (fmaxf(a, fmaxf(b, c)))

HSV **rgb2hsv(RGB **matrizRGB, int altura, int largura);
RGB **hsv2rgb(HSV **matrizHSV, int altura, int largura);
HSV **alocarMatrizHSV(int altura, int largura);
void liberarMatrizHSV(HSV **matriz, int altura);
void detectarDoenca(RGB **matriz, char *arquivo, int altura, int largura);
float obterTonalidade(RGB pixel);

#endif