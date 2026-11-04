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

/* Matriz: mapa[u][v] guarda o tempo da rua de u para v.
 * Zero significa ausencia de rua, pois os tempos sao positivos.
 * Os identificadores usados pelo enunciado e exemplo sao de 1 a n.
 */
int ler_mapa(const char *caminho, int *n, int *destino, int mapa[][MAX_ESQUINAS + 1])
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
    for (u = 1; u <= *n; u++)
        for (v = 1; v <= *n; v++)
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
        /* Se houver ruas repetidas, conserva a mais rapida. */
        if (mapa[origem][chegada] == 0 || minutos < mapa[origem][chegada])
            mapa[origem][chegada] = minutos;
    }
    fclose(arquivo);
    if (!fim) {
        fprintf(stderr, "Erro: ruas invalidas ou terminador 0 ausente.\n");
        return 0;
    }
    return 1;
}

/* Segue o pseudocodigo do professor. O conjunto E e representado por
 * finalizada[e] == 0. Os resultados sao escritos nos vetores recebidos.
 * Tempo O(n^2). Memoria fixa: O(MAX_ESQUINAS^2).
 */
void rota_mais_rapida(int n, int mapa[][MAX_ESQUINAS + 1], long long tempo[],
                      int anterior[], int finalizada[])
{
    int e, v, passo;
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
    /* static evita colocar a matriz grande na pilha de execucao. */
    static int mapa[MAX_ESQUINAS + 1][MAX_ESQUINAS + 1];
    static long long tempo[MAX_ESQUINAS + 1];
    static int anterior[MAX_ESQUINAS + 1];
    static int finalizada[MAX_ESQUINAS + 1];
    static int rota[MAX_ESQUINAS + 1];
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
    }
    if (!ler_mapa(argc == 2 ? argv[1] : caminho, &n, &destino, mapa))
        return EXIT_FAILURE;
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
    return EXIT_SUCCESS;
}
