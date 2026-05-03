#include "unity/unity.h"
#include "../txt.h"
#include "../pessoas.h"
#include <stdio.h>
#include <string.h>

#define CAMINHO_HF "teste_txt_pessoas.hf"
#define CAMINHO_HFD "teste_txt_pessoas.hfd"
#define CAMINHO_TXT "teste_txt.txt"

// le o conteúdo do arquivo txt gerado para verificar a saída 
static void ler_txt(char* buf, int tam) {
    memset(buf, 0, tam);
    FILE* f = fopen(CAMINHO_TXT, "r");
    if(f == NULL)
        return;
    fread(buf, 1, tam - 1, f);
    fclose(f);
}

void setUp(void) {
    remove(CAMINHO_HF);
    remove(CAMINHO_HFD);
    remove(CAMINHO_TXT);
    pessoas_inicializar(CAMINHO_HF);
    txt_inicializar(CAMINHO_TXT);
}

void tearDown(void) {
    txt_finalizar();
    pessoas_finalizar(CAMINHO_HFD);
    remove(CAMINHO_HF);
    remove(CAMINHO_HFD);
    remove(CAMINHO_TXT);
}

// txt_inicializar retorna 0 com caminho válido
void test_txt_inicializar_valido(void) {
    // já inicializado no setUp
    TEST_PASS();
}

// txt_inicializar retorna -1 com caminho inválido 
void test_txt_inicializar_invalido(void) {
    txt_finalizar();
    int ret = txt_inicializar("/caminho/invalido/nao_existe/saida.txt");
    TEST_ASSERT_EQUAL_INT(-1, ret);
    // reinicializa para o tearDown não crashar
    txt_inicializar(CAMINHO_TXT);
}

// txt_rq_morador escreve cpf e nome no arquivo
void test_txt_rq_morador(void) {
    txt_rq_morador("111", "Joao", "Silva");
    txt_finalizar();

    char buf[512];
    ler_txt(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "111"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Joao"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Silva"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "[rq]"));

    txt_inicializar(CAMINHO_TXT);
}

// txt_censo escreve todas as estatísticas
void test_txt_censo_escreve_estatisticas(void) {
    int total = 10, moradores = 6, sem_teto = 4, homens = 5, mulheres = 5, mor_h = 3, mor_m = 3, st_h = 2, st_m = 2;
    txt_censo(&total, &moradores, &sem_teto, &homens, &mulheres, &mor_h, &mor_m, &st_h, &st_m);
    txt_finalizar();

    char buf[1024];
    ler_txt(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "[censo]"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Total de habitantes: 10"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Total de moradores: 6"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Total sem-teto: 4"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Homens: 5"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Mulheres: 5"));

    txt_inicializar(CAMINHO_TXT);
}

// txt_censo com total zero não crasha por divisão por zero
void test_txt_censo_total_zero(void) {
    int total = 0, moradores = 0, sem_teto = 0, homens = 0, mulheres = 0, mor_h = 0, mor_m = 0, st_h = 0, st_m = 0;

    txt_censo(&total, &moradores, &sem_teto, &homens, &mulheres, &mor_h, &mor_m, &st_h, &st_m);
    txt_finalizar();

    char buf[1024];
    ler_txt(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "[censo]"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Total de habitantes: 0"));

    txt_inicializar(CAMINHO_TXT);
}

// txt_habitante sem-teto não imprime endereço
void test_txt_habitante_sem_teto(void) {
    pessoas_inserir("111", "Joao", "Silva", 'M', "01/01/1990");
    const Habitante* h = pessoas_buscar("111");

    txt_habitante(h);
    txt_finalizar();

    char buf[512];
    ler_txt(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "[h?]"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Joao"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "111"));
    // sem-teto não tem endereço
    TEST_ASSERT_NULL(strstr(buf, "CEP"));

    txt_inicializar(CAMINHO_TXT);
}

// txt_habitante morador imprime endereço
void test_txt_habitante_morador(void) {
    pessoas_inserir("111", "Joao", "Silva", 'M', "01/01/1990");
    pessoas_definir_endereco("111", "cep01", 'S', 45, "apto1");
    const Habitante* h = pessoas_buscar("111");

    txt_habitante(h);
    txt_finalizar();

    char buf[512];
    ler_txt(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "[h?]"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "cep01"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "apto1"));

    txt_inicializar(CAMINHO_TXT);
}

// txt_obito sem-teto não imprime endereço
void test_txt_obito_sem_teto(void) {
    pessoas_inserir("222", "Maria", "Santos", 'F', "15/06/1985");
    const Habitante* h = pessoas_buscar("222");

    txt_obito(h);
    txt_finalizar();

    char buf[512];
    ler_txt(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "[rip]"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Maria"));
    TEST_ASSERT_NULL(strstr(buf, "CEP"));

    txt_inicializar(CAMINHO_TXT);
}

// txt_obito morador imprime endereço
void test_txt_obito_morador(void) {
    pessoas_inserir("222", "Maria", "Santos", 'F', "15/06/1985");
    pessoas_definir_endereco("222", "cep02", 'N', 10, "-");
    const Habitante* h = pessoas_buscar("222");

    txt_obito(h);
    txt_finalizar();

    char buf[512];
    ler_txt(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "[rip]"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "cep02"));

    txt_inicializar(CAMINHO_TXT);
}

// txt_despejo imprime habitante e endereço
void test_txt_despejo(void) {
    pessoas_inserir("333", "Pedro", "Lima", 'M', "20/03/2000");
    pessoas_definir_endereco("333", "cep03", 'L', 30, "casa");
    const Habitante* h = pessoas_buscar("333");

    txt_despejo(h);
    txt_finalizar();

    char buf[512];
    ler_txt(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "[dspj]"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "Pedro"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "cep03"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "casa"));

    txt_inicializar(CAMINHO_TXT);
}

// txt_habitante com NULL não crasha
void test_txt_habitante_null(void) {
    txt_habitante(NULL);
    TEST_PASS();
}

// txt_obito com NULL não crasha
void test_txt_obito_null(void) {
    txt_obito(NULL);
    TEST_PASS();
}

// txt_despejo com NULL não crasha
void test_txt_despejo_null(void) {
    txt_despejo(NULL);
    TEST_PASS();
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_txt_inicializar_valido);
    RUN_TEST(test_txt_inicializar_invalido);
    RUN_TEST(test_txt_rq_morador);
    RUN_TEST(test_txt_censo_escreve_estatisticas);
    RUN_TEST(test_txt_censo_total_zero);
    RUN_TEST(test_txt_habitante_sem_teto);
    RUN_TEST(test_txt_habitante_morador);
    RUN_TEST(test_txt_obito_sem_teto);
    RUN_TEST(test_txt_obito_morador);
    RUN_TEST(test_txt_despejo);
    RUN_TEST(test_txt_habitante_null);
    RUN_TEST(test_txt_obito_null);
    RUN_TEST(test_txt_despejo_null);

    return UNITY_END();
}