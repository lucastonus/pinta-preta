#ifndef _MEDIANA_H_
#define _MEDIANA_H_

#define MATRIZ_MEDIANA 5
#define BRILHO 80
#define CONTRASTE 100

int truncate(int value);
int obterDimensaoMatrizMediana(int pos, int dimensao);
unsigned char mediana(unsigned char *vetor, int tamVetor);
void filtroMediana(RGB **matriz, int altura, int largura);
void brilho(RGB **matriz, int altura, int largura);
void contraste(RGB **matriz, int altura, int largura);

#endif