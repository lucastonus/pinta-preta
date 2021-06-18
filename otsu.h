#ifndef _OTSU_H_
#define _OTSU_H_

int *criarHistograma(int n);
double otsuPeso(int *histograma, double qtdPixeis, int inicio, int fim);
double otsuVariancia(int *histograma, int inicio, int fim);
void popularHistogramaTonsCinza(int *histograma, RGB **matriz, int altura, int largura);
void tonsDeCinza(RGB **matriz, int altura, int largura);
void binarizacao(RGB **matriz, int altura, int largura, RGB **matrizColorida);

#endif