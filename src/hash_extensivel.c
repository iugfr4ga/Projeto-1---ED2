#include "hash_extensivel.h"
#include <stdio.h>
#include <stdlib.h>

#define HF_CHAVE_TAM 64 // tamanho em bytes das chaves

typedef struct {
    int prof_global;    // profundidade global atual do arquivo
    int bucket_cap;     // capacidade de slots por bucket
    int size_registro;  // tamanho dos registros
    int num_registros;  // numero total de registros vivos
    int num_buckets;    // numero total de buckets
    int num_expansoes;  // numero total de splits realizados
    long dir_offset;    // offset do diretorio no arquivo
    long prox_livre;    // proximo byte livre
} Cabecalho;

typedef struct {
    int prof_local;     // profundidade do bucket
    int num_registros;  // numero de registros vivos no bucket
} Bucket;

typedef struct {
    int ativo;                  // 1 se o slot estiver ocupado, 0 caso contrário
    char chave[HF_CHAVE_TAM];   // chave do registro
} Slot;

struct HashExtensivel {
    FILE *arquivo;      // ponteiro para o arquivo
    Cabecalho cab;      // cabeçalho do hashfile
    long *diretorio;    // vetor de offsets de cada bucket
};

static void cab_escrever(HashExtensivel *h) {
    rewind(h->arquivo);
    fwrite(&h->cab, sizeof(Cabecalho), 1, h->arquivo);
}

static void cab_ler(HashExtensivel *h) {
    rewind(h->arquivo);
    fread(&h->cab, sizeof(Cabecalho), 1, h->arquivo);
}

static void dir_escrever(HashExtensivel *h) {
    int tam = 1 << h->cab.prof_global;  // quantidade atual de diretorios = 2^prof_global
    fseek(h->arquivo, h->cab.dir_offset, SEEK_SET);
    fwrite(h->diretorio, sizeof(long), tam, h->arquivo);
    fflush(h->arquivo);
}

static void dir_ler(HashExtensivel *h) {
    int tam = 1 << h->cab.prof_global;
    fseek(h->arquivo, h->cab.dir_offset, SEEK_SET);
    fread(h->diretorio, sizeof(long), tam, h->arquivo);
}

static long bucket_alocar(HashExtensivel *h, int prof_local) {
    long offset = h->cab.prox_livre;

    Bucket b;
    b.prof_local = prof_local;
    b.num_registros = 0;
 
    fseek(h->arquivo, offset, SEEK_SET);
    fwrite(&b, sizeof(Bucket), 1, h->arquivo);
 
    // zera todos os slots do bucket
    int slot_tam = sizeof(Slot) + h->cab.size_registro;
    char *zero = calloc(1, slot_tam);
    for (int i = 0; i < h->cab.bucket_cap; i++)
        fwrite(zero, slot_tam, 1, h->arquivo);

    free(zero);
 
    h->cab.prox_livre += sizeof(Bucket) + h->cab.bucket_cap * slot_tam;
    h->cab.num_buckets++;
    return offset;
}

HashExtensivel *hash_criar(const char *caminho, size_t size_registro) {
    // falha se o arquivo já existir
    FILE *f = fopen(caminho, "rb");
    if (f != NULL) { 
        fclose(f); 
        return NULL; 
    }
 
    f = fopen(caminho, "w+b");
    if (f == NULL) 
        return NULL;
 
    HashExtensivel *h = calloc(1, sizeof(HashExtensivel));
    if (h == NULL) { 
        fclose(f); 
        return NULL; 
    }
    h->arquivo = f;
 
    // inicializa cabeçalho
    h->cab.prof_global = 1;
    h->cab.bucket_cap = 4;
    h->cab.size_registro = (int) size_registro;
    h->cab.num_registros = 0;
    h->cab.num_buckets = 0;
    h->cab.num_expansoes = 0;
    h->cab.dir_offset = sizeof(Cabecalho);
    h->cab.prox_livre = h->cab.dir_offset + 2 * sizeof(long);
 
    // aloca o diretório na memória 
    h->diretorio = calloc(2, sizeof(long));
    if (h->diretorio == NULL) { 
        fclose(f); 
        free(h); 
        return NULL; 
    }
 
    cab_escrever(h);
 
    // aloca 2 buckets iniciais e preenche o diretório
    for (int i = 0; i < 2; i++)
        h->diretorio[i] = bucket_alocar(h, 1);
 
    // reescreve cabeçalho e diretório com os offsets dos buckets
    cab_escrever(h);
    dir_escrever(h);
    return h;
}

HashExtensivel *hash_abrir(const char *caminho) {
    FILE *f = fopen(caminho, "r+b");
    if (f == NULL) 
        return NULL;
 
    HashExtensivel *h = calloc(1, sizeof(HashExtensivel));
    if (h == NULL) { 
        fclose(f); 
        return NULL; 
    }
    h->arquivo = f;
 
    cab_ler(h);
 
    // aloca e carrega o diretório
    int tam = 1 << h->cab.prof_global;
    h->diretorio = malloc(tam * sizeof(long));
    if (h->diretorio == NULL) { 
        fclose(f); 
        free(h); 
        return NULL; 
    }
    dir_ler(h);
 
    return h;
}
 

void hash_fechar(HashExtensivel *h) {
    if (h == NULL) 
        return;

    cab_escrever(h);
    dir_escrever(h);
    fclose(h->arquivo);
    free(h->diretorio);
    free(h);
}