#include "pm.h"
#include "pessoas.h"
#include <stdio.h>
#include <string.h>

#define PM_LINHA_TAM 256
#define CEP_TAM 32
#define NOME_TAM 32
#define CPF_TAM 32
#define COMPL_TAM 16
#define DATA_TAM 12

int pm_processar(const char *caminho) {
    FILE *f = fopen(caminho, "r");
    if(f == NULL)
        return -1;

    char linha[PM_LINHA_TAM];
    while(fgets(linha, sizeof(linha), f)) {
        if(linha[0] == '\n')
            continue;

        char cmd[4];
        if(sscanf(linha, "%3s", cmd) != 1)
            continue;

        if(strcmp(cmd, "p") == 0) {
            char cpf[CPF_TAM], nome[NOME_TAM], sobrenome[NOME_TAM], nasc[DATA_TAM];
            char sexo;
            if (sscanf(linha, "%*s %31s %31s %31s %c %11s", cpf, nome, sobrenome, &sexo, nasc) == 5)
                pessoas_inserir(cpf, nome, sobrenome, sexo, nasc);

        } 
        else if(strcmp(cmd, "m") == 0) {
            char cpf[CPF_TAM], cep[CEP_TAM], compl[COMPL_TAM];
            char face;
            int num;
            if(sscanf(linha, "%*s %31s %31s %c %d %15s", cpf, cep, &face, &num, compl) == 5)
                pessoas_definir_endereco(cpf, cep, face, num, compl);
        }
    }
    fclose(f);
    return 0;
}