#include "txt.h"
#include "pessoas.h"
#include <stdio.h>

static FILE* txt_arquivo = NULL;

static void imprimir_habitante(const Habitante* hab) {
    fprintf(txt_arquivo, "CPF: %s | Nome: %s %s | Sexo: %c | Nascimento: %s\n",
            habitante_get_cpf(hab), habitante_get_nome(hab), habitante_get_sobrenome(hab), habitante_get_sexo(hab),
            habitante_get_nasc(hab));
}

static void imprimir_endereco(const Habitante* hab) {
    fprintf(txt_arquivo, "CEP: %s | Face: %c | Número: %d | Complemento: %s\n",
            habitante_get_cep(hab), habitante_get_face(hab), habitante_get_num(hab), habitante_get_compl(hab));
}

int txt_inicializar(const char* caminho) {
    txt_arquivo = fopen(caminho, "w");
    return txt_arquivo != NULL ? 0 : -1;
}

void txt_finalizar(void) {
    if(txt_arquivo == NULL)
        return;
    fclose(txt_arquivo);
    txt_arquivo = NULL;
}

void txt_rq_morador(const char* cpf, const char* nome, const char* sobrenome) {
    if(txt_arquivo == NULL)
        return;
    fprintf(txt_arquivo, "[rq]\nMorador sem-teto: CPF: %s | Nome: %s %s\n", cpf, nome, sobrenome);
}

void txt_censo(int* total, int* moradores, int* sem_teto, int* homens, int* mulheres, int* moradores_h, int* moradores_m, int* semteto_h,  int* semteto_m) {
    if(txt_arquivo == NULL)
        return;

    double prop_mor = (*total > 0) ? (100.0 * (*moradores) / (*total)) : 0.0;
    double pct_h = (*total > 0) ? (100.0 * (*homens) / (*total)) : 0.0;
    double pct_m = (*total > 0) ? (100.0 * (*mulheres) / (*total)) : 0.0;
    double pct_mor_h = (*moradores > 0) ? (100.0 * (*moradores_h) / (*moradores)) : 0.0;
    double pct_mor_m = (*moradores > 0) ? (100.0 * (*moradores_m) / (*moradores)) : 0.0;
    double pct_st_h = (*sem_teto  > 0) ? (100.0 * (*semteto_h) / (*sem_teto)) : 0.0;
    double pct_st_m = (*sem_teto  > 0) ? (100.0 * (*semteto_m) / (*sem_teto)) : 0.0;

    fprintf(txt_arquivo, "[censo]\n");
    fprintf(txt_arquivo, "Total de habitantes: %d\n", *total);
    fprintf(txt_arquivo, "Total de moradores: %d\n", *moradores);
    fprintf(txt_arquivo, "Proporcao moradores/habitantes: %.2f%%\n", prop_mor);
    fprintf(txt_arquivo, "Homens: %d | Mulheres: %d\n", *homens, *mulheres);
    fprintf(txt_arquivo, "%% habitantes homens: %.2f%%\n", pct_h);
    fprintf(txt_arquivo, "%% habitantes mulheres: %.2f%%\n", pct_m);
    fprintf(txt_arquivo, "%% moradores homens: %.2f%%\n", pct_mor_h);
    fprintf(txt_arquivo, "%% moradores mulheres: %.2f%%\n", pct_mor_m);
    fprintf(txt_arquivo, "Total sem-teto: %d\n", *sem_teto);
    fprintf(txt_arquivo, "%% sem-teto homens: %.2f%%\n", pct_st_h);
    fprintf(txt_arquivo, "%% sem-teto mulheres: %.2f%%\n", pct_st_m);
}

void txt_habitante(const Habitante* hab) {
    if(txt_arquivo == NULL || hab == NULL)
        return;
    fprintf(txt_arquivo, "[h?]\n"); 
    imprimir_habitante(hab);
    if(habitante_get_morador(hab))
        imprimir_endereco(hab);
}

void txt_obito(const Habitante* hab) {
    if(txt_arquivo == NULL || hab == NULL)
        return;
    fprintf(txt_arquivo, "[rip]\n");
    imprimir_habitante(hab);
    if(habitante_get_morador(hab))
        imprimir_endereco(hab);
}

void txt_despejo(const Habitante* hab) {
    if(txt_arquivo == NULL || hab == NULL)
        return;
    fprintf(txt_arquivo, "[dspj]\n");
    imprimir_habitante(hab);
    imprimir_endereco(hab);
}