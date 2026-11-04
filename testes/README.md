# Casos de teste
Execute `main.exe` informando o caminho de cada TXT, a partir da raiz.
| Arquivo | Resultado esperado |
| --- | --- |
| ../pizzaria.txt | Rota 1 4 5 6 2 3; 7 minutos |
| inacessivel.txt | Nao existe rota |
| mao_unica.txt | Nao existe rota; nao inverter a rua 2 para 1 |
| origem.txt | Rota 1; 0 minutos |
| melhoria.txt | Rota 1 3 4; 6 minutos, substituindo estimativa 12 |
| duplicadas.txt | Rota 1 2; 3 minutos |
| tempo_grande.txt | Rota 1 2 3; 4000000000 minutos |
| empate.txt | Rota 1 2 4; 3 minutos |
| invalido.txt | Erro e encerramento com codigo diferente de zero |
| sem_terminador.txt | Erro e encerramento com codigo diferente de zero |
Validacao realizada: compilacao C11 com `-Wall -Wextra -Wpedantic -Werror`,
exemplo oficial, todos os casos desta pasta, caminho informado pelo teclado e
100 grafos aleatorios (semente 42) comparados com Bellman-Ford. Nesses grafos,
tambem foram conferidos a origem, o destino e a soma das ruas da rota impressa.
