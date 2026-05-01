#include "pessoas.h"
#include "hash_extensivel.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CEP_TAM 32
#define NOME_TAM 32
#define CPF_TAM 32
#define COMPL_TAM 16
#define DATA_TAM 12

typedef struct {
    char cep[CEP_TAM];
    char face;
    int num;
    char compl[COMPL_TAM];
} Endereco;

struct Habitante {
    char cpf[CPF_TAM];
    char nome[NOME_TAM];
    char sobrenome[NOME_TAM];
    char sexo;
    char nasc[DATA_TAM];
    int morador;        // se morador = 0, é sem-teto 
    Endereco endereco;
};

static HashExtensivel *hf_habitantes = NULL;
static Habitante buf_habitante;

int pessoas_inicializar(const char *caminho_hf) {
    hf_habitantes = hash_abrir(caminho_hf);
    if(hf_habitantes == NULL)
        hf_habitantes = hash_criar(caminho_hf, sizeof(Habitante));
    return hf_habitantes != NULL ? 0 : -1;
}

void pessoas_finalizar(const char* caminho_hfd) {
    if(hf_habitantes != NULL) {
        hash_dump(hf_habitantes, caminho_hfd);
        hash_fechar(hf_habitantes);
        hf_habitantes = NULL;
    }
}

int pessoas_inserir(const char *cpf, const char *nome, const char *sobrenome, char sexo, const char *nasc) {
    if (cpf == NULL || nome == NULL || sobrenome == NULL || nasc == NULL)
        return -1;

    Habitante h;
    memset(&h, 0, sizeof(Habitante));
    strncpy(h.cpf, cpf, CPF_TAM - 1);
    h.cpf[CPF_TAM - 1] = '\0';
    strncpy(h.nome, nome, NOME_TAM - 1);
    h.nome[NOME_TAM - 1] = '\0';
    strncpy(h.sobrenome, sobrenome, NOME_TAM - 1);
    h.sobrenome[NOME_TAM - 1] = '\0';
    h.sexo = sexo;
    strncpy(h.nasc, nasc, DATA_TAM - 1);
    h.nasc[DATA_TAM - 1] = '\0';
    h.morador = 0;

    return hash_inserir(hf_habitantes, cpf, &h);
}

int pessoas_definir_endereco(const char *cpf, const char *cep, char face, int num, const char *compl) {
    if (cpf == NULL || cep == NULL) 
        return -1;

    if (hash_buscar(hf_habitantes, cpf, &buf_habitante) != 0)
        return -1;

    strncpy(buf_habitante.endereco.cep, cep, CEP_TAM  - 1);
    buf_habitante.endereco.cep[CEP_TAM - 1] = '\0';
    if (compl != NULL) {
        strncpy(buf_habitante.endereco.compl, compl, COMPL_TAM - 1);
        buf_habitante.endereco.compl[COMPL_TAM - 1] = '\0';
    } else {
        buf_habitante.endereco.compl[0] = '\0';
    }
    buf_habitante.endereco.face = face;
    buf_habitante.endereco.num = num;
    buf_habitante.morador = 1;

    return hash_atualizar(hf_habitantes, cpf, &buf_habitante);
}

const Habitante* pessoas_buscar(const char *cpf) {
    if (cpf == NULL) return NULL;
    if (hash_buscar(hf_habitantes, cpf, &buf_habitante) != 0)
        return NULL;
    return &buf_habitante;
}

int pessoas_atualizar_endereco(const char *cpf, const char *cep, char face, int num, const char *compl) {
    if (cpf == NULL || cep == NULL) 
        return -1;

    if (hash_buscar(hf_habitantes, cpf, &buf_habitante) != 0)
        return -1;

    // se morador for sem-teto, não pode atualizar endereço(deve usar pessoas_definir_endereco antes)
    if (buf_habitante.morador == 0)
        return -1;

    strncpy(buf_habitante.endereco.cep, cep, CEP_TAM - 1);
    buf_habitante.endereco.cep[CEP_TAM - 1] = '\0';
    if (compl != NULL) {
        strncpy(buf_habitante.endereco.compl, compl, COMPL_TAM - 1);
        buf_habitante.endereco.compl[COMPL_TAM - 1] = '\0';
    } else {
        buf_habitante.endereco.compl[0] = '\0';
    }
    buf_habitante.endereco.face = face;
    buf_habitante.endereco.num = num;

    return hash_atualizar(hf_habitantes, cpf, &buf_habitante);
}

int pessoas_remover_endereco(const char *cpf) {
    if (cpf == NULL) 
        return -1;

    if (hash_buscar(hf_habitantes, cpf, &buf_habitante) != 0)
        return -1;

    if (buf_habitante.morador == 0)
        return -1;

    memset(&buf_habitante.endereco, 0, sizeof(Endereco));
    buf_habitante.morador = 0;

    return hash_atualizar(hf_habitantes, cpf, &buf_habitante);
}

int pessoas_remover(const char *cpf) {
    if (cpf == NULL) 
        return -1;
    return hash_remover(hf_habitantes, cpf);
}

// callback intermediário para pessoas_iterar 
typedef struct {
    int (*cb)(const Habitante *hab, void *ctx);
    void *ctx;
} CtxIterar;

static int cb_intermediario(const char *chave, const void *registro, void *ctx) {
    (void) chave;
    CtxIterar *c = (CtxIterar*) ctx;
    return c->cb((const Habitante*) registro, c->ctx);
}

int pessoas_iterar(int (*callback)(const Habitante *hab, void *ctx), void *ctx) {
    if (callback == NULL) 
        return -1;
    CtxIterar c = { .cb = callback, .ctx = ctx };
    return hash_iterar(hf_habitantes, cb_intermediario, &c);
}

// getters 
const char* habitante_get_cpf(const Habitante *h) { 
    return h->cpf;            
}

const char* habitante_get_nome(const Habitante *h) { 
    return h->nome;           
}

const char* habitante_get_sobrenome(const Habitante *h) { 
    return h->sobrenome;      
}

char habitante_get_sexo(const Habitante *h) { 
    return h->sexo;           
}

const char* habitante_get_nasc(const Habitante *h) { 
    return h->nasc;           
}

int habitante_get_morador(const Habitante *h) { 
    return h->morador;        
}

const char* habitante_get_cep(const Habitante *h) { 
    return h->endereco.cep;   
}

char habitante_get_face(const Habitante *h) { 
    return h->endereco.face;  
}

int habitante_get_num(const Habitante *h) { 
    return h->endereco.num;   
}

const char* habitante_get_compl(const Habitante *h) { 
    return h->endereco.compl; 
}