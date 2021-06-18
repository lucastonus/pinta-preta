#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "arquivo.h"
#include "bmp.h"
#include "filtros.h"
#include "otsu.h"
#include "hsv.h"
#include "kmeans.h"

void processarImagens(char **listaImagens, int nImagens) {
	int i, j, k, l;

	for (i = 0; i < nImagens; i++) {
		CABECALHO_ARQUIVO cabecalho_arquivo;
		CABECALHO_IMAGEM cabecalho_imagem;
		char fileOut[TAM_CAMINHO_IMG];

		FILE *fileIn = fopen(listaImagens[i], "rb");

		if (fileIn == NULL) {
			printf("Erro ao abrir o arquivo %s.\n", listaImagens[i]);
			exit(0);
		}

		fread(&cabecalho_arquivo, sizeof(CABECALHO_ARQUIVO), 1, fileIn);
		fread(&cabecalho_imagem, sizeof(CABECALHO_IMAGEM), 1, fileIn);
		fclose(fileIn);

		RGB **matrizRGB = popularMatrizImagem(listaImagens[i], cabecalho_imagem.altura, cabecalho_imagem.largura);

		filtroMediana(matrizRGB, cabecalho_imagem.altura, cabecalho_imagem.largura);
		obterCaminhoArquivo(listaImagens[i], fileOut, 0);
		gravarImagem(fileOut, cabecalho_arquivo, cabecalho_imagem, matrizRGB);

		// brilho(matrizRGB, cabecalho_imagem.altura, cabecalho_imagem.largura);
		// obterCaminhoArquivo(listaImagens[i], fileOut, 1);
		// gravarImagem(fileOut, cabecalho_arquivo, cabecalho_imagem, matrizRGB);

		// contraste(matrizRGB, cabecalho_imagem.altura, cabecalho_imagem.largura);
		// obterCaminhoArquivo(listaImagens[i], fileOut, 2);
		// gravarImagem(fileOut, cabecalho_arquivo, cabecalho_imagem, matrizRGB);

		RGB **matrizColorida = copiarMatriz(matrizRGB, cabecalho_imagem.altura, cabecalho_imagem.largura);

		tonsDeCinza(matrizRGB, cabecalho_imagem.altura, cabecalho_imagem.largura);
		obterCaminhoArquivo(listaImagens[i], fileOut, 3);
		gravarImagem(fileOut, cabecalho_arquivo, cabecalho_imagem, matrizRGB);

		binarizacao(matrizRGB, cabecalho_imagem.altura, cabecalho_imagem.largura, matrizColorida);
		liberarMatrizRGB(matrizColorida, cabecalho_imagem.altura);
		obterCaminhoArquivo(listaImagens[i], fileOut, 4);
		gravarImagem(fileOut, cabecalho_arquivo, cabecalho_imagem, matrizRGB);

		// kmeans(matrizRGB, cabecalho_imagem.altura, cabecalho_imagem.largura);
		// obterCaminhoArquivo(listaImagens[i], fileOut, 5);
		// gravarImagem(fileOut, cabecalho_arquivo, cabecalho_imagem, matrizRGB);

		// HSV **matrizHSV = rgb2hsv(matrizRGB, cabecalho_imagem.altura, cabecalho_imagem.largura);
		detectarDoenca(matrizRGB, listaImagens[i], cabecalho_imagem.altura, cabecalho_imagem.largura);
		// RGB **matrizDoenca = hsv2rgb(matrizHSV, cabecalho_imagem.altura, cabecalho_imagem.largura);
		obterCaminhoArquivo(listaImagens[i], fileOut, 6);
		gravarImagem(fileOut, cabecalho_arquivo, cabecalho_imagem, matrizRGB);

		// liberarMatrizHSV(matrizHSV, cabecalho_imagem.altura);
		// liberarMatrizRGB(matrizDoenca, cabecalho_imagem.altura);
		liberarMatrizRGB(matrizRGB, cabecalho_imagem.altura);
	}
}

int main(int argc, char *argv[]) {
	if (argc == 2) {
		char **listaImagens = NULL;
		int nImagens = contarLinhasArquivo(argv[1]);
		listaImagens = alocarListaImagens(argv[1], nImagens);
		lerArquivo(argv[1], listaImagens, nImagens);
		// mostrarListaImagens(listaImagens, nImagens);
		processarImagens(listaImagens, nImagens);
		liberarListaImagens(listaImagens, nImagens);
	} else {
		printf("Informe o nome do arquivo que possui os caminhos das imagens.\n");
	}

	return 1;
}