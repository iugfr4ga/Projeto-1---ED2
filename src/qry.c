#include "qry.h"
#include "cidade.h"
#include "pessoas.h"
#include "svg.h"
#include "txt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define QRY_LINHA_TAM 256
#define CPF_TAM 32
#define CEP_TAM 32
#define COMPL_TAM 16

// contexto para contar moradores de uma quadra por face
typedef struct {
    const char* cep;
    int n, s, l, o;
} CtxContarFaces;

static int cb_contar_faces(const Habitante* hab, void* ctx) {
    CtxContarFaces* c = (CtxContarFaces*) ctx;
    if(!habitante_get_morador(hab))
        return 0;
    if(strcmp(habitante_get_cep(hab), c->cep) != 0)
        return 0;
    switch(habitante_get_face(hab)) {
        case 'N': c->n++; break;
        case 'S': c->s++; break;
        case 'L': c->l++; break;
        case 'O': c->o++; break;
    }
    return 0;
}

// contexto para despejar moradores de uma quadra removida
typedef struct {
    const char* cep;
} CtxDespejarQuadra;

static int cb_despejar_quadra(const Habitante* hab, void* ctx) {
    CtxDespejarQuadra* c = (CtxDespejarQuadra*) ctx;
    if(!habitante_get_morador(hab))
        return 0;
    if(strcmp(habitante_get_cep(hab), c->cep) != 0)
        return 0;
    txt_rq_morador(habitante_get_cpf(hab), habitante_get_nome(hab), habitante_get_sobrenome(hab));
    pessoas_remover_endereco(habitante_get_cpf(hab));
    return 0;
}

// contexto para o censo 
typedef struct {
    int total, moradores, sem_teto;
    int homens, mulheres;
    int moradores_h, moradores_m;
    int semteto_h, semteto_m;
} CtxCenso;

static int cb_censo(const Habitante* hab, void* ctx) {
    CtxCenso* c = (CtxCenso*) ctx;
    c->total++;
    char sexo = habitante_get_sexo(hab);
    int eh_morador = habitante_get_morador(hab);

    if(sexo == 'M') 
        c->homens++;
    else             
        c->mulheres++;

    if(eh_morador) {
        c->moradores++;
        if(sexo == 'M') 
            c->moradores_h++;
        else             
            c->moradores_m++;
    } 
    else {
        c->sem_teto++;
        if(sexo == 'M') 
            c->semteto_h++;
        else             
            c->semteto_m++;
    }
    return 0;
}

static void cmd_rq(const char* cep) {
    const Quadra* q = cidade_buscar_quadra(cep);
    if(q == NULL) 
        return;
        
    svg_marcar_remocao(quadra_get_x(q), quadra_get_y(q));

    // itera moradores da quadra, informa no TXT e os torna sem-teto
    CtxDespejarQuadra ctx = { .cep = cep };
    pessoas_iterar(cb_despejar_quadra, &ctx);

    cidade_remover_quadra(cep);
}

static void cmd_pq(const char* cep) {
    const Quadra* q = cidade_buscar_quadra(cep);
    if(q == NULL) 
        return;

    CtxContarFaces ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.cep = cep;
    pessoas_iterar(cb_contar_faces, &ctx);

    int total = ctx.n + ctx.s + ctx.l + ctx.o;
    svg_marcar_moradores(quadra_get_x(q), quadra_get_y(q), quadra_get_w(q), quadra_get_h(q), quadra_get_sw(q),
                ctx.n, ctx.s, ctx.l, ctx.o, total);
}

static void cmd_censo(void) {
    CtxCenso ctx;
    memset(&ctx, 0, sizeof(ctx));
    pessoas_iterar(cb_censo, &ctx);
    txt_censo(&ctx.total, &ctx.moradores, &ctx.sem_teto, &ctx.homens, &ctx.mulheres, &ctx.moradores_h, &ctx.moradores_m,
                &ctx.semteto_h, &ctx.semteto_m);
}

static void cmd_h(const char* cpf) {
    const Habitante* hab = pessoas_buscar(cpf);
    if(hab == NULL) 
        return;
    txt_habitante(hab);
}

static void cmd_nasc(const char* cpf, const char* nome, const char* sobrenome, char sexo, const char* nasc) {
    pessoas_inserir(cpf, nome, sobrenome, sexo, nasc);
}

static void cmd_rip(const char* cpf) {
    const Habitante* hab = pessoas_buscar(cpf);
    if(hab == NULL) 
        return;

    txt_obito(hab);

    if(habitante_get_morador(hab)) {
        double px, py;
        if(cidade_coordenadas(habitante_get_cep(hab), habitante_get_face(hab), habitante_get_num(hab), quadra_get_sw(
        cidade_buscar_quadra(habitante_get_cep(hab))), &px, &py) == 0)
            svg_marcar_obito(px, py, habitante_get_face(hab));
    }
    pessoas_remover(cpf);
}

static void cmd_mud(const char* cpf, const char* cep, char face, int num, const char* compl) {
    const Habitante* hab = pessoas_buscar(cpf);
    if(hab == NULL) 
        return;
    if(!habitante_get_morador(hab)) 
        return;
    
    double px, py;
    if(cidade_coordenadas(cep, face, num, quadra_get_sw(cidade_buscar_quadra(cep)), &px, &py) == 0)
        svg_marcar_mudanca(px, py, cpf);

    pessoas_atualizar_endereco(cpf, cep, face, num, compl);
}

static void cmd_dspj(const char* cpf) {
    const Habitante* hab = pessoas_buscar(cpf);
    if(hab == NULL) 
        return;
    if(!habitante_get_morador(hab)) 
        return;

    txt_despejo(hab);

    double px, py;
    if(cidade_coordenadas(habitante_get_cep(hab), habitante_get_face(hab), habitante_get_num(hab), quadra_get_sw(
        cidade_buscar_quadra(habitante_get_cep(hab))), &px, &py) == 0)
        svg_marcar_despejo(px, py, habitante_get_face(hab));

    pessoas_remover_endereco(cpf);
}

int qry_processar(const char* caminho) {
    FILE* f = fopen(caminho, "r");
    if(f == NULL)
        return -1;

    char linha[QRY_LINHA_TAM];
    while(fgets(linha, sizeof(linha), f)) {
        if(linha[0] == '\n')
            continue;

        char cmd[8];
        if(sscanf(linha, "%7s", cmd) != 1)
            continue;

        if(strcmp(cmd, "rq") == 0) {
            char cep[CEP_TAM];
            if(sscanf(linha, "%*s %31s", cep) == 1)
                cmd_rq(cep);
        } 
        else if(strcmp(cmd, "pq") == 0) {
            char cep[CEP_TAM];
            if(sscanf(linha, "%*s %31s", cep) == 1)
                cmd_pq(cep);
        } 
        else if(strcmp(cmd, "censo") == 0) {
            cmd_censo();
        } 
        else if(strcmp(cmd, "h?") == 0) {
            char cpf[CPF_TAM];
            if(sscanf(linha, "%*s %31s", cpf) == 1)
                cmd_h(cpf);
        } 
        else if(strcmp(cmd, "nasc") == 0) {
            char cpf[CPF_TAM], nome[32], sobrenome[32], nasc[12];
            char sexo;
            if(sscanf(linha, "%*s %31s %31s %31s %c %11s", cpf, nome, sobrenome, &sexo, nasc) == 5)
                cmd_nasc(cpf, nome, sobrenome, sexo, nasc);
        } 
        else if(strcmp(cmd, "rip") == 0) {
            char cpf[CPF_TAM];
            if(sscanf(linha, "%*s %31s", cpf) == 1)
                cmd_rip(cpf);
        } 
        else if(strcmp(cmd, "mud") == 0) {
            char cpf[CPF_TAM], cep[CEP_TAM], compl[COMPL_TAM];
            char face;
            int num;
            if(sscanf(linha, "%*s %31s %31s %c %d %15s", cpf, cep, &face, &num, compl) == 5)
                cmd_mud(cpf, cep, face, num, compl);
        } 
        else if(strcmp(cmd, "dspj") == 0) {
            char cpf[CPF_TAM];
            if(sscanf(linha, "%*s %31s", cpf) == 1)
                cmd_dspj(cpf);
        }
    }
    fclose(f);
    return 0;
}