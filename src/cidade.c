#include "cidade.h"
#include "hash_extensivel.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CEP_TAM 32
#define COR_TAM 16

struct Quadra {
    char cep[CEP_TAM];
    double x, y;
    double w, h;
    char cfill[COR_TAM];
    char cstrk[COR_TAM];
    float sw;
};

static HashExtensivel *hf_quadras = NULL;
static Quadra buf_quadra;  // buffer para retorno de cidade_buscar_quadra

int cidade_inicializar(const char *caminho_hf) {
    hf_quadras = hash_abrir(caminho_hf);
    if (hf_quadras == NULL)
        hf_quadras = hash_criar(caminho_hf, sizeof(Quadra));
    return hf_quadras != NULL ? 0 : -1;
}

void cidade_finalizar(void) {
    if (hf_quadras != NULL) {
        hash_fechar(hf_quadras);
        hf_quadras = NULL;
    }
}

int cidade_inserir_quadra(const char *cep, double x, double y, double w, double h, const char *cfill, const char *cstrk, float sw) {
    if (cep == NULL) 
        return -1;

    Quadra q;
    strncpy(q.cep, cep, CEP_TAM - 1);  
    q.cep[CEP_TAM - 1] = '\0';
    strncpy(q.cfill, cfill, COR_TAM - 1);  
    q.cfill[COR_TAM - 1] = '\0';
    strncpy(q.cstrk, cstrk, COR_TAM - 1);  
    q.cstrk[COR_TAM - 1] = '\0';

    q.x = x;
    q.y = y;
    q.w = w;
    q.h = h;
    q.sw = sw;

    return hash_inserir(hf_quadras, cep, &q);
}

int cidade_remover_quadra(const char *cep) {
    if (cep == NULL) 
        return -1;
    return hash_remover(hf_quadras, cep);
}

const Quadra* cidade_buscar_quadra(const char *cep) {
    if (cep == NULL) 
        return NULL;
    if (hash_buscar(hf_quadras, cep, &buf_quadra) != 0)
        return NULL;
    return &buf_quadra;
}

int cidade_coordenadas(const char *cep, char face, int num, double *px, double *py) {
    if (px == NULL || py == NULL) 
        return -1;

    const Quadra *q = cidade_buscar_quadra(cep);
    if (q == NULL) 
        return -1;
    
    switch (face) {
        case 'S': *px = q->x + q->w - num; *py = q->y; break;
        case 'N': *px = q->x + num; *py = q->y + q->h; break;
        case 'O': *px = q->x; *py = q->y + num; break;
        case 'L': *px = q->x + q->w; *py = q->y + q->h - num; break;
        default: return -1;
    }
    return 0;
}


const char* quadra_get_cep(const Quadra *q) { 
    return q->cep;   
}

double quadra_get_x(const Quadra *q) { 
    return q->x;     
}

double quadra_get_y(const Quadra *q) { 
    return q->y;     
}

double quadra_get_w(const Quadra *q) { 
    return q->w;     
}

double quadra_get_h(const Quadra *q) { 
    return q->h;     
}

const char* quadra_get_cfill(const Quadra *q) { 
    return q->cfill; 
}

const char* quadra_get_cstrk(const Quadra *q) { 
    return q->cstrk; 
}

float quadra_get_sw(const Quadra *q) { 
    return q->sw;    
}