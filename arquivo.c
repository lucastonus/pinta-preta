#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivo.h"

int contarLinhasArquivo(char *nomeArquivo) {
	int count = 0;
	FILE *arquivo = fopen(nomeArquivo, "r");
	char lixo[TAM_CAMINHO_IMG];

	if (arquivo == NULL) {
		printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
		exit(0);
	}

	while (fscanf(arquivo, "%s", lixo) == 1) {
		count++;
	}

	fclose(arquivo);

	return count;
}

void lerArquivo(char *nomeArquivo, char **listaImagens, int nImagens) {
	int i;

	FILE *arquivo = fopen(nomeArquivo, "r");

	if (arquivo == NULL) {
		printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
		exit(0);
	} else {
		for (i = 0; i < nImagens; i++) {
			fscanf(arquivo, "%s", listaImagens[i]);
		}
	}

	fclose(arquivo);
}

char **alocarListaImagens(char *nomeArquivo, int nImagens) {
	char **listaImagens = NULL;

	FILE *arquivo = fopen(nomeArquivo, "r");

	if (arquivo == NULL) {
		printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
		exit(0);
	} else {
		int i;

		if (nImagens > 0) {
			listaImagens = (char**) malloc(sizeof(char*) * nImagens);

			for (i = 0; i < nImagens; i++) {
				listaImagens[i] = (char*) malloc(sizeof(char) * TAM_CAMINHO_IMG);
			}
		} else {
			printf("O arquivo %s não possui nenhum nome de imagem.\n", nomeArquivo);
			exit(0);
		}
	}

	fclose(arquivo);

	return listaImagens;
}

void mostrarListaImagens(char **listaImagens, int nImagens) {
	int i;

	for (i = 0; i < nImagens; i++) {
		printf("%s\n", listaImagens[i]);
	}
}

void liberarListaImagens(char **listaImagens, int nImagens) {
	int i;

	for (i = 0; i < nImagens; i++) {
		free(listaImagens[i]);
	}

	free(listaImagens);
}

void obterCaminhoArquivo(char *src, char *dest, int tipo) {
	char* imagem;
	char cpSrc[TAM_CAMINHO_IMG];

	memcpy(cpSrc, src, strlen(src) + 1);

	imagem = strtok(cpSrc, "/");
	imagem = strtok(NULL, "/");

	switch (tipo) {
		case 0:
			sprintf(dest, "saida/1.mediana/%s", imagem);
			break;
		case 1:
			sprintf(dest, "saida/2.brilho/%s", imagem);
			break;
		case 2:
			sprintf(dest, "saida/3.contraste/%s", imagem);
			break;
		case 3:
			sprintf(dest, "saida/4.cinza/%s", imagem);
			break;
		case 4:
			sprintf(dest, "saida/5.otsu/%s", imagem);
			break;
		case 5:
			sprintf(dest, "saida/6.kmeans/%s", imagem);
			break;
		case 6:
			sprintf(dest, "saida/7.hsv/%s", imagem);
			break;
		default:
			sprintf(dest, "saida.bmp");
			break;
	}
}