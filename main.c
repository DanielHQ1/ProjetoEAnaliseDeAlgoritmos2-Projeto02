/*
 * Grupo: PREENCHER nomes completos e RAs antes da entrega.
 * Referencias: Projeto02.pdf, algoritmo Rota-mais-rapida(M), paginas 1-3;
 * 06-algorimo-guloso.pdf; 06.2-algorimos-gulosos.pdf (material da disciplina).
 * Auxilio de IA: PREENCHER o link de compartilhamento desta conversa.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define INFINITO LLONG_MAX
#define MAX_ESQUINAS 1000

static int mapa[MAX_ESQUINAS][MAX_ESQUINAS];

int ler_mapa(const char *caminho, int *n, int *destino, int mapa[][MAX_ESQUINAS])
{
    FILE *arquivo = fopen(caminho, "r");
    int u, v;
    int origem, chegada, minutos, fim = 0;
    if (arquivo == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo.\n");
        return 0;
    }
    if (fscanf(arquivo, "%d%d", destino, n) != 2 || *n < 1 || *n > MAX_ESQUINAS ||
        *destino < 1 || *destino > *n) {
        fprintf(stderr, "Erro: destino invalido ou numero de esquinas fora do limite de 1 a %d.\n", MAX_ESQUINAS);
        fclose(arquivo);
        return 0;
    }
    for (u = 0; u < *n; u++)
        for (v = 0; v < *n; v++)
            mapa[u][v] = 0;
    while (fscanf(arquivo, "%d", &origem) == 1) {
        if (origem == 0) {
            char extra;
            fim = fscanf(arquivo, " %c", &extra) == EOF;
            break;
        }
        if (fscanf(arquivo, "%d%d", &chegada, &minutos) != 2 ||
            origem < 1 || origem > *n || chegada < 1 || chegada > *n ||
            minutos <= 0)
            break;

        origem--;
        chegada--;
        if (mapa[origem][chegada] == 0 || minutos < mapa[origem][chegada])
            mapa[origem][chegada] = minutos;
    }
    fclose(arquivo);
    if (!fim) {
        fprintf(stderr, "Erro: ruas invalidas ou terminador 0 ausente.\n");
        return 0;
    }
    (*destino)--;
    return 1;
}

void rota_mais_rapida(int n, int mapa[][MAX_ESQUINAS], long long tempo[],
                      int anterior[], int finalizada[])
{
    int e, v, passo;
    for (e = 0; e < n; e++) {
        tempo[e] = INFINITO;
        anterior[e] = -1;
        finalizada[e] = 0;
    }
    tempo[0] = 0;
    for (passo = 0; passo < n; passo++) {
        v = -1;
        for (e = 0; e < n; e++)
            if (!finalizada[e] && (v == -1 || tempo[e] < tempo[v]))
                v = e;

        if (v == -1 || tempo[v] == INFINITO)
            break;
        finalizada[v] = 1;
        for (e = 0; e < n; e++) {
            int minutos = mapa[v][e];
            if (!finalizada[e] && minutos > 0 &&
                tempo[v] <= LLONG_MAX - minutos &&
                tempo[e] > tempo[v] + minutos) {
                tempo[e] = tempo[v] + minutos;
                anterior[e] = v;
            }
        }
    }
}

void reconstruir_rota(int destino, const int anterior[], int rota[],
                     int *tamanho)
{
    int atual, i;
    *tamanho = 0;
    for (atual = destino; atual != -1; atual = anterior[atual])
        rota[(*tamanho)++] = atual;
    for (i = 0; i < *tamanho / 2; i++) {
        int aux = rota[i];
        rota[i] = rota[*tamanho - i - 1];
        rota[*tamanho - i - 1] = aux;
    }
}

int main(int argc, char *argv[])
{
    char caminho[4096];
    const char *caminho_arquivo;

    long long tempo[MAX_ESQUINAS];
    int anterior[MAX_ESQUINAS];
    int finalizada[MAX_ESQUINAS];
    int rota[MAX_ESQUINAS];
    int n, destino;
    int tamanho, i;
    if (argc > 2) {
        fprintf(stderr, "Uso: %s [caminho-do-arquivo.txt]\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (argc == 1) {
        printf("Informe o caminho do arquivo TXT: ");
        fflush(stdout);
        if (fgets(caminho, sizeof caminho, stdin) == NULL)
            return EXIT_FAILURE;
        caminho[strcspn(caminho, "\r\n")] = '\0';
        caminho_arquivo = caminho;
    } else {
        caminho_arquivo = argv[1];
    }
    if (!ler_mapa(caminho_arquivo, &n, &destino, mapa))
        return EXIT_FAILURE;
    rota_mais_rapida(n, mapa, tempo, anterior, finalizada);
    if (tempo[destino] == INFINITO) {
        printf("Nao existe rota da esquina #1 ate a esquina #%d.\n", destino + 1);
    } else {
        reconstruir_rota(destino, anterior, rota, &tamanho);
        printf("Rota ate a esquina #%d:", destino + 1);
        for (i = 0; i < tamanho; i++)
            printf(" %d", rota[i] + 1);
        printf("\nTempo calculado para rota = %lld min.\n", tempo[destino]);
    }
    return EXIT_SUCCESS;
}
