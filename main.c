/*
 * Grupo: PREENCHER nomes completos e RAs antes da entrega.
 * Referencias: Projeto02.pdf, algoritmo Rota-mais-rapida(M), paginas 1-3;
 * 06-algorimo-guloso.pdf; 06.2-algorimos-gulosos.pdf (material da disciplina).
 * Auxilio de IA: PREENCHER o link de compartilhamento desta conversa.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#define INFINITO LLONG_MAX

/* Matriz linear: a rua de u para v fica na posicao u * lado + v.
 * Zero significa ausencia de rua, pois os tempos sao positivos.
 * Os identificadores usados pelo enunciado e exemplo sao de 1 a n.
 */
int ler_mapa(const char *caminho, int *n, int *destino, int **mapa)
{
    FILE *arquivo = fopen(caminho, "r");
    size_t lado, quantidade, i;
    int origem, chegada, minutos, fim = 0;
    if (arquivo == NULL) {
        fprintf(stderr, "Erro: nao foi possivel abrir o arquivo.\n");
        return 0;
    }
    if (fscanf(arquivo, "%d%d", destino, n) != 2 || *n < 1 ||
        *destino < 1 || *destino > *n) {
        fprintf(stderr, "Erro: destino ou numero de esquinas invalido.\n");
        fclose(arquivo);
        return 0;
    }
    lado = (size_t)*n + 1;
    if (lado > SIZE_MAX / lado || lado * lado > SIZE_MAX / sizeof(int)) {
        fprintf(stderr, "Erro: mapa grande demais.\n");
        fclose(arquivo);
        return 0;
    }
    quantidade = lado * lado;
    *mapa = calloc(quantidade, sizeof(int));
    if (*mapa == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente para o mapa.\n");
        fclose(arquivo);
        return 0;
    }
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
        i = (size_t)origem * lado + (size_t)chegada;
        /* Se houver ruas repetidas, conserva a mais rapida. */
        if ((*mapa)[i] == 0 || minutos < (*mapa)[i])
            (*mapa)[i] = minutos;
    }
    fclose(arquivo);
    if (!fim) {
        fprintf(stderr, "Erro: ruas invalidas ou terminador 0 ausente.\n");
        free(*mapa);
        *mapa = NULL;
        return 0;
    }
    return 1;
}

/* Segue o pseudocodigo do professor. O conjunto E e representado por
 * finalizada[e] == 0. Os resultados sao escritos nos vetores recebidos.
 * Tempo O(n^2); mapa O(n^2); vetores auxiliares O(n).
 */
void rota_mais_rapida(int n, const int mapa[], long long tempo[],
                      int anterior[], int finalizada[])
{
    int e, v, passo;
    size_t lado = (size_t)n + 1;
    for (e = 1; e <= n; e++) {
        tempo[e] = INFINITO;
        anterior[e] = -1;
        finalizada[e] = 0;
    }
    tempo[1] = 0;
    for (passo = 0; passo < n; passo++) {
        v = -1;
        for (e = 1; e <= n; e++)
            if (!finalizada[e] && (v == -1 || tempo[e] < tempo[v]))
                v = e;
        /* Nao ha caminho da origem para as esquinas restantes. */
        if (v == -1 || tempo[v] == INFINITO)
            break;
        finalizada[v] = 1;
        for (e = 1; e <= n; e++) {
            int minutos = mapa[(size_t)v * lado + (size_t)e];
            if (!finalizada[e] && minutos > 0 &&
                tempo[v] <= LLONG_MAX - minutos &&
                tempo[e] > tempo[v] + minutos) {
                tempo[e] = tempo[v] + minutos;
                anterior[e] = v;
            }
        }
    }
}

/* Escreve a rota da origem ao destino no vetor recebido. */
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
    int n, destino, *mapa = NULL, *anterior, *finalizada, *rota;
    int tamanho, i;
    long long *tempo;
    size_t lado;
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
    }
    if (!ler_mapa(argc == 2 ? argv[1] : caminho, &n, &destino, &mapa))
        return EXIT_FAILURE;
    lado = (size_t)n + 1;
    tempo = malloc(lado * sizeof *tempo);
    anterior = malloc(lado * sizeof *anterior);
    finalizada = malloc(lado * sizeof *finalizada);
    rota = malloc(lado * sizeof *rota);
    if (tempo == NULL || anterior == NULL || finalizada == NULL || rota == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        free(mapa); free(tempo); free(anterior); free(finalizada); free(rota);
        return EXIT_FAILURE;
    }
    rota_mais_rapida(n, mapa, tempo, anterior, finalizada);
    if (tempo[destino] == INFINITO) {
        printf("Nao existe rota da esquina #1 ate a esquina #%d.\n", destino);
    } else {
        reconstruir_rota(destino, anterior, rota, &tamanho);
        printf("Rota ate a esquina #%d:", destino);
        for (i = 0; i < tamanho; i++)
            printf(" %d", rota[i]);
        printf("\nTempo calculado para rota = %lld min.\n", tempo[destino]);
    }
    free(mapa); free(tempo); free(anterior); free(finalizada); free(rota);
    return EXIT_SUCCESS;
}
