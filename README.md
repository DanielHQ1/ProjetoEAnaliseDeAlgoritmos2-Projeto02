# Fast Pizzaria

Implementacao em C do algoritmo `Rota-mais-rapida(M)` do Projeto02.
A escolha gulosa seleciona a esquina ainda nao finalizada com menor tempo
acumulado desde a origem. Ao melhorar um tempo, guarda tambem o predecessor.
Os vetores sao parametros de saida: a funcao escreve neles, sem retornar vetores.

## Compilar e executar

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic -o main.exe main.c
.\main.exe
```

Informe o caminho do TXT quando solicitado, sem aspas. Caminhos com espacos
sao aceitos. Tambem e possivel fornecer o caminho como argumento:

```powershell
.\main.exe "pizzaria.txt"
```

O TXT contem: destino, numero de esquinas, triplas `origem destino minutos`
e um `0` final. As ruas sao direcionadas e os tempos devem ser positivos.
Em ruas repetidas, conserva-se o menor tempo. Destinos inacessiveis produzem
uma mensagem, sem tentar reconstruir uma rota inexistente.

Resultado do exemplo:

```text
Rota ate a esquina #3: 1 4 5 6 2 3
Tempo calculado para rota = 7 min.
```

## Decisoes

- Origem 1 e esquinas de 1 a n, conforme pseudocodigo, entrada e saida.
  A mencao a origem 0 no texto introdutorio e inconsistente com esses trechos.
- Matriz de adjacencia; zero representa ausencia de rua.
- Selecao por varredura e relaxamento conforme o algoritmo fornecido.
- Empates conservam o primeiro predecessor encontrado; a selecao de esquinas
  desempata pelo menor numero. Uma das rotas otimas e apresentada.
- Arrays estaticos, sem alocacao dinamica. Limite de 1000 esquinas definido
  por MAX_ESQUINAS em main.c. Entradas maiores sao rejeitadas.
- Tempos em long long. Tempo O(n^2); memoria fixa O(MAX_ESQUINAS^2).

## Antes da entrega

Preencher os nomes completos e RAs do grupo no inicio de `main.c`, e o link
de compartilhamento da conversa com a IA nas referencias desse arquivo.
Compactar apenas os fontes e arquivos TXT de teste, sem executaveis ou `.git`.
O README e apenas documentacao do repositorio.
