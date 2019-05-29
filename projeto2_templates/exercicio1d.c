#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

void merge_sort_recursivo(int *l, int ini, int fim) {
    int l1, l2, lc;
    int meio;
    int aux[fim+1];
    if (ini == fim) // criterio de parada
        return; // vetor com um soh elemento
    // ordena metades inferior e superior
    meio = ini + (fim-ini)/2;
    merge_sort_recursivo(l,ini,meio); // metade inferior
    merge_sort_recursivo(l,meio+1,fim); // metade superior
    // faz o merge das listas
    l1 = lc = ini;
    l2 = meio+1;
    while ( l1 <= meio && l2 <= fim)
        aux[lc++] = (l[l1] < l[l2]) ? l[l1++] : l[l2++];
        /*{
            if (l->vet[l1] < l->vet[l2]) {
                l->copia[lc] = l->vet[l1];
                l1++;
            } else {
                l->copia[lc] = l->vet[l2];
                l2++;
            }
            lc++;
        }*/
    for (; l1 <= meio; l1++, lc++)
        aux[lc] = l[l1];
    for (; l2 <= fim; l2++, lc++)
        aux[lc] = l[l2];
    // passa a copia ordenada sobre o vetor original
    for (lc=ini; lc<=fim; lc++) {
        l[lc] = aux[lc];
    }
    return;
}

int busca(int *vet, int *table, int x, int y, int n)
{
    for(int i = 0; i<y; i++)
        if(x < vet[table[i]])
            for(int j = vet[table[i-1]] ; j < vet[table[i]]; j++)
                if (vet[j] == x)
                    return 1;
    
    for (int i = vet[table[y-1]]; i < n; i++)
    {
        if(x == vet[i])
            return 1;
    }
    return 0;
}

int* ler_inteiros(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");

    int* inteiros = (int *) malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
}

void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo()
{
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int index_size = 10000;
    unsigned encontrados = 0;

    int* entradas = ler_inteiros("inteiros_entrada.txt", N);
    int* consultas = ler_inteiros("inteiros_busca.txt", N);
    
    // ordenar entrada
    merge_sort_recursivo(entradas, 0, N-1);
    // criar tabela de indice
    int table[N/index_size];
    int j = 0;

    for(j = 0; j<(N/index_size); j++)
        table[j] = (j)*(index_size);
    // realizar consultas na tabela de indices 
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // buscar o elemento consultas[i] na entrada
        encontrados += busca(entradas, table, consultas[i], (N/index_size), N);
    }
    double tempo_busca = finaliza_tempo();

    printf("Tempo de busca    :\t%fs\n", tempo_busca);
    printf("Itens encontrados :\t%d\n", encontrados);

    return 0;
}
