#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <math.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE  1
#define FALSE 0

// Definição do tipo string
typedef char * string;

#define MAX_STRING_LEN 20

typedef struct No
{
    string x;
    struct No *prox;
}Lista;

unsigned converter(string s) {
   unsigned h = 0;
   for (int i = 0; s[i] != '\0'; i++) 
      h = h * 256 + s[i];
   return h;
}

string* ler_strings(const char * arquivo, const int n)
{
    FILE* f = fopen(arquivo, "r");
    
    string* strings = (string *) malloc(sizeof(string) * n);

    for (int i = 0; !feof(f); i++) {
        strings[i] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
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

unsigned h_div(unsigned x, unsigned B)
{
    return x % B;
}

unsigned h_mul(unsigned x, unsigned B)
{
    const double A = 0.6180;
    return fmod(x * A, 1) * B;
}

int insere_mul(Lista **tabela_mul, string insercoes, int B)
{

    int ins = converter(insercoes);
    int pos =  h_mul(ins, B);
    
    if(tabela_mul[pos] == NULL)
    {
        tabela_mul[pos] = (Lista*)malloc(1*(sizeof(Lista)));
        tabela_mul[pos]->x = insercoes;
        tabela_mul[pos]->prox = NULL;
        return 0;
    }else
    {
        int i;
        Lista *p = tabela_mul[pos];
        for(i = 0; p != NULL; ++i)
        {
            if (!strcmp(p->x, insercoes))
            {
                return i;
            }
            p = p->prox;
        }

        Lista *Aux = malloc(1*(sizeof(Lista)));
        Aux->x = insercoes;
        Aux->prox = tabela_mul[pos];
        tabela_mul[pos] = Aux;
        return i;
    }
    
    return -1;
}

int insere_div(Lista **tabela_div, string insercoes, int B)
{

    int ins = converter(insercoes);
    int pos =  h_div(ins, B);
    
    if(tabela_div[pos] == NULL)
    {
        tabela_div[pos] = (Lista*)malloc(1*(sizeof(Lista)));
        tabela_div[pos]->x = insercoes;
        tabela_div[pos]->prox = NULL;
        return 0;
    }else
    {
        int i;
        Lista *p = tabela_div[pos];
        for(i = 0; p != NULL; ++i)
        {
            if (!strcmp(p->x, insercoes))
            {
                return i;
            }
            p = p->prox;
        }

        Lista *Aux = malloc(1*(sizeof(Lista)));
        Aux->x = insercoes;
        Aux->prox = tabela_div[pos];
        tabela_div[pos] = Aux;
        return i;
    }
    
    return -1;
}

int busca_mul(Lista **Tabela, string consultas, int B)
{
    int cons = converter(consultas);
    int pos = h_mul(cons, B);
    Lista *Aux = Tabela[pos];
        
    while(Aux != NULL)
    {
        if(!strcmp(Aux->x, consultas))
            return 1;
        Aux = Aux->prox;
    }

    return 0;
}

int busca_div(Lista **Tabela, string consultas, int B)
{
    int cons = converter(consultas);
    int pos = h_div(cons, B);
    Lista *Aux = Tabela[pos];
        
    while(Aux != NULL)
    {
        if(!strcmp(Aux->x, consultas))
            return 1;
        Aux = Aux->prox;
    }

    return 0;
}

void destroitabela(Lista **Tabela, int B)
{
    Lista * Aux;
    Lista * Aux2;
    for(int i = 0; i < B; i++)
        {
            Aux = Tabela[i];
            while(Aux != NULL)
            {
                Aux2 = Aux->prox;
                free(Aux);
                Aux = Aux2;
            }
        }

    free(Tabela);
    
    return;
}


int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string* insercoes = ler_strings("strings_entrada.txt", N);
    string* consultas = ler_strings("strings_busca.txt", M);
    
    int cont;
    // cria tabela hash com hash por divisão
    Lista ** tabela_div = (Lista**)malloc(B*(sizeof(Lista*)));
    for(int j = 0; j < B; j++)
        tabela_div[j] = NULL;
    // inserção dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        cont = insere_div(tabela_div, insercoes[i], B);
        if(cont != -1)
            colisoes_h_div += cont;
    }
    double tempo_insercao_h_div = finaliza_tempo();

    // busca dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        if(consultas[i] != NULL)
            encontrados_h_div += busca_div(tabela_div, consultas[i], B);
    }
    double tempo_busca_h_div = finaliza_tempo();

    // destroi tabela hash com hash por divisão
    destroitabela(tabela_div, B);



    // cria tabela hash com hash por multiplicação
    Lista ** tabela_mul = (Lista**)malloc(B*(sizeof(Lista*)));
    for(int j = 0; j < B; j++)
        tabela_mul[j] = NULL;
    // inserção dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
        cont = insere_mul(tabela_mul, insercoes[i], B);
        if(cont != -1)
            colisoes_h_mul += cont;
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
        if(consultas[i] != NULL)
            encontrados_h_mul += busca_mul(tabela_mul, consultas[i], B);
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // destroi tabela hash com hash por multiplicação
    destroitabela(tabela_mul, B);


    printf("Hash por Divisão\n");
    printf("Colisões na inserção: %d\n", colisoes_h_div);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_div);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados   : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicação\n");
    printf("Colisões na inserção: %d\n", colisoes_h_mul);
    printf("Tempo de inserção   : %fs\n", tempo_insercao_h_mul);
    printf("Tempo de busca      : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados   : %d\n", encontrados_h_mul);

    return 0;
}
