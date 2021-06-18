#ifndef _ARQUIVO_H_
#define _ARQUIVO_H_

#define TAM_CAMINHO_IMG 100

int contarLinhasArquivo(char *nomeArquivo);
char **alocarListaImagens(char *nomeArquivo, int nImagens);
void lerArquivo(char *nomeArquivo, char **listaImagens, int nImagens);
void mostrarListaImagens(char **listaImagens, int nImagens);
void liberarListaImagens(char **listaImagens, int nImagens);
void obterCaminhoArquivo(char *src, char *dest, int tipo);

#endif