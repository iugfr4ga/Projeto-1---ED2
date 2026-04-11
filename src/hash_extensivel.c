#include "hash_extensivel.h"
#include <stdio.h>

typedef struct {
    int prof_global;    // profundidade global do arquivo
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
} BucketCab;

typedef struct {
    int ativo;          // 1 se o slot estiver ocupado, 0 caso contrário
    char chave[64];     // chave do registro
} Slot;

struct HashExtensivel {
    FILE *arquivo;      // ponteiro para o arquivo
    Cabecalho cab;      // cabeçalho do hashfile
    long *diretorio;    // vetor de offsets de tamanho 2^prof_global
};