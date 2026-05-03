#include "unity/unity.h"
#include "../pessoas.h"
#include <string.h>
#include <stdio.h>

#define CAMINHO_HF_PESSOAS "teste_pessoas.hf"
#define CAMINHO_HFD_PESSOAS "teste_pessoas.hfd"

void setUp(void) {
    remove(CAMINHO_HF_PESSOAS);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, pessoas_inicializar(CAMINHO_HF_PESSOAS), "pessoas_inicializar falhou.");
}

void tearDown(void) {
    pessoas_finalizar(CAMINHO_HFD_PESSOAS);
    remove(CAMINHO_HF_PESSOAS);
    remove(CAMINHO_HFD_PESSOAS);
}

void test_pessoas_inicializar(void) {
    TEST_PASS();
}

void test_pessoas_inserir_habitante(void) {
    int ret = pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_pessoas_inserir_duplicado_retorna_erro(void) {
    pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    int ret = pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    TEST_ASSERT_EQUAL_INT(-1, ret);
}

void test_pessoas_buscar_existente(void) {
    pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    const Habitante *h = pessoas_buscar("111.111.111-11");
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_EQUAL_STRING("Joao", habitante_get_nome(h));
    TEST_ASSERT_EQUAL_STRING("Silva", habitante_get_sobrenome(h));
    TEST_ASSERT_EQUAL_INT('M', habitante_get_sexo(h));
    TEST_ASSERT_EQUAL_STRING("01/01/1990", habitante_get_nasc(h));
    TEST_ASSERT_EQUAL_INT(0, habitante_get_morador(h));
}

void test_pessoas_buscar_inexistente_retorna_null(void) {
    const Habitante *h = pessoas_buscar("000.000.000-00");
    TEST_ASSERT_NULL(h);
}

void test_pessoas_inserir_sem_teto_nao_e_morador(void) {
    pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    const Habitante *h = pessoas_buscar("111.111.111-11");
    TEST_ASSERT_EQUAL_INT(0, habitante_get_morador(h));
}

void test_pessoas_definir_endereco(void) {
    pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    int ret = pessoas_definir_endereco("111.111.111-11", "cep01", 'S', 10, "apto 1");
    TEST_ASSERT_EQUAL_INT(0, ret);

    const Habitante *h = pessoas_buscar("111.111.111-11");
    TEST_ASSERT_EQUAL_INT(1, habitante_get_morador(h));
    TEST_ASSERT_EQUAL_STRING("cep01",  habitante_get_cep(h));
    TEST_ASSERT_EQUAL_INT('S', habitante_get_face(h));
    TEST_ASSERT_EQUAL_INT(10, habitante_get_num(h));
    TEST_ASSERT_EQUAL_STRING("apto 1", habitante_get_compl(h));
}

void test_pessoas_definir_endereco_inexistente_retorna_erro(void) {
    int ret = pessoas_definir_endereco("000.000.000-00", "cep01", 'S', 10, NULL);
    TEST_ASSERT_EQUAL_INT(-1, ret);
}

void test_pessoas_atualizar_endereco(void) {
    pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    pessoas_definir_endereco("111.111.111-11", "cep01", 'S', 10, NULL);
    int ret = pessoas_atualizar_endereco("111.111.111-11", "cep02", 'N', 20, NULL);
    TEST_ASSERT_EQUAL_INT(0, ret);

    const Habitante *h = pessoas_buscar("111.111.111-11");
    TEST_ASSERT_EQUAL_STRING("cep02", habitante_get_cep(h));
    TEST_ASSERT_EQUAL_INT('N', habitante_get_face(h));
    TEST_ASSERT_EQUAL_INT(20, habitante_get_num(h));
}

void test_pessoas_atualizar_endereco_sem_teto_retorna_erro(void) {
    pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    int ret = pessoas_atualizar_endereco("111.111.111-11", "cep01", 'S', 10, NULL);
    TEST_ASSERT_EQUAL_INT(-1, ret);
}

void test_pessoas_remover_endereco(void) {
    pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    pessoas_definir_endereco("111.111.111-11", "cep01", 'S', 10, NULL);
    int ret = pessoas_remover_endereco("111.111.111-11");
    TEST_ASSERT_EQUAL_INT(0, ret);

    const Habitante *h = pessoas_buscar("111.111.111-11");
    TEST_ASSERT_EQUAL_INT(0, habitante_get_morador(h));
}

void test_pessoas_remover_endereco_sem_teto_retorna_erro(void) {
    pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    int ret = pessoas_remover_endereco("111.111.111-11");
    TEST_ASSERT_EQUAL_INT(-1, ret);
}

void test_pessoas_remover_habitante(void) {
    pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    int ret = pessoas_remover("111.111.111-11");
    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_NULL(pessoas_buscar("111.111.111-11"));
}

void test_pessoas_remover_inexistente_retorna_erro(void) {
    int ret = pessoas_remover("000.000.000-00");
    TEST_ASSERT_EQUAL_INT(-1, ret);
}

void test_pessoas_persistencia_apos_reabrir(void) {
    pessoas_inserir("111.111.111-11", "Joao", "Silva", 'M', "01/01/1990");
    pessoas_definir_endereco("111.111.111-11", "cep01", 'S', 10, NULL);
    pessoas_finalizar(CAMINHO_HFD_PESSOAS);
    TEST_ASSERT_EQUAL_INT(0, pessoas_inicializar(CAMINHO_HF_PESSOAS));

    const Habitante *h = pessoas_buscar("111.111.111-11");
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_EQUAL_STRING("Joao", habitante_get_nome(h));
    TEST_ASSERT_EQUAL_INT(1, habitante_get_morador(h));
    TEST_ASSERT_EQUAL_STRING("cep01", habitante_get_cep(h));
}

typedef struct {
    int total;
    int moradores;
    int sem_teto;
} CtxCenso;

static int cb_censo(const Habitante *hab, void *ctx) {
    CtxCenso *c = (CtxCenso*) ctx;
    c->total++;
    if (habitante_get_morador(hab)) 
        c->moradores++;
    else 
        c->sem_teto++;
    return 0;
}

void test_pessoas_iterar_conta_habitantes(void) {
    pessoas_inserir("111.111.111-11", "Joao",  "Silva", 'M', "01/01/1990");
    pessoas_inserir("222.222.222-22", "Maria", "Lima",  'F', "02/02/1992");
    pessoas_inserir("333.333.333-33", "Pedro", "Souza", 'M', "03/03/1995");
    pessoas_definir_endereco("111.111.111-11", "cep01", 'S', 10, NULL);

    CtxCenso ctx = {0};
    TEST_ASSERT_EQUAL_INT(0, pessoas_iterar(cb_censo, &ctx));
    TEST_ASSERT_EQUAL_INT(3, ctx.total);
    TEST_ASSERT_EQUAL_INT(1, ctx.moradores);
    TEST_ASSERT_EQUAL_INT(2, ctx.sem_teto);
}

void test_pessoas_iterar_nao_visita_removidos(void) {
    pessoas_inserir("111.111.111-11", "Joao",  "Silva", 'M', "01/01/1990");
    pessoas_inserir("222.222.222-22", "Maria", "Lima",  'F', "02/02/1992");
    pessoas_remover("111.111.111-11");

    CtxCenso ctx = {0};
    pessoas_iterar(cb_censo, &ctx);
    TEST_ASSERT_EQUAL_INT(1, ctx.total);
}

void test_pessoas_iterar_vazio(void) {
    CtxCenso ctx = {0};
    TEST_ASSERT_EQUAL_INT(0, pessoas_iterar(cb_censo, &ctx));
    TEST_ASSERT_EQUAL_INT(0, ctx.total);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_pessoas_inicializar);
    RUN_TEST(test_pessoas_inserir_habitante);
    RUN_TEST(test_pessoas_inserir_duplicado_retorna_erro);
    RUN_TEST(test_pessoas_buscar_existente);
    RUN_TEST(test_pessoas_buscar_inexistente_retorna_null);
    RUN_TEST(test_pessoas_inserir_sem_teto_nao_e_morador);
    RUN_TEST(test_pessoas_definir_endereco);
    RUN_TEST(test_pessoas_definir_endereco_inexistente_retorna_erro);
    RUN_TEST(test_pessoas_atualizar_endereco);
    RUN_TEST(test_pessoas_atualizar_endereco_sem_teto_retorna_erro);
    RUN_TEST(test_pessoas_remover_endereco);
    RUN_TEST(test_pessoas_remover_endereco_sem_teto_retorna_erro);
    RUN_TEST(test_pessoas_remover_habitante);
    RUN_TEST(test_pessoas_remover_inexistente_retorna_erro);
    RUN_TEST(test_pessoas_persistencia_apos_reabrir);
    RUN_TEST(test_pessoas_iterar_conta_habitantes);
    RUN_TEST(test_pessoas_iterar_nao_visita_removidos);
    RUN_TEST(test_pessoas_iterar_vazio);

    return UNITY_END();
}