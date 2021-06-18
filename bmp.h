#ifndef _BMP_H_
#define _BMP_H_

#pragma pack(push, 1)

typedef struct {
	unsigned short int tipo;
	unsigned int tamanho;
	unsigned short int reservado1;
	unsigned short int reservado2;
	unsigned int offset;
} CABECALHO_ARQUIVO;

typedef struct {
	unsigned int tamanho;
	int largura;
	int altura;
	unsigned short int planos;
	unsigned short int bits_pixel;
	unsigned int compressao;
	unsigned int tamanho_imagem;
	int wresolution;
	int hresolution;
	unsigned int numero_cores;
	unsigned int cores_significantes;
} CABECALHO_IMAGEM;

typedef struct {
	unsigned char azul;
	unsigned char verde;
	unsigned char vermelho;
} RGB;

RGB **alocarMatrizRGB(int altura, int largura);
RGB **popularMatrizImagem(char *arquivo, int altura, int largura);
RGB **copiarMatriz(RGB **matriz, int altura, int largura);
void liberarMatrizRGB(RGB **matriz, int altura);
void gravarImagem(char *arquivo, CABECALHO_ARQUIVO cabecalho_arquivo, CABECALHO_IMAGEM cabecalho_imagem, RGB **matriz);
float distanciaEuclidianaRGB(RGB pixelX, RGB pixelY);

#endif