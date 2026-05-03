#include "unity/unity.h"
#include "../qry.h"
#include "../cidade.h"
#include "../pessoas.h"
#include "../svg.h"
#include "../txt.h"
#include <stdio.h>
#include <string.h>

#define CAMINHO_HF_QUADRAS "teste_qry_quadras.hf"
#define CAMINHO_HFD_QUADRAS "teste_qry_quadras.hfd"
#define CAMINHO_HF_PESSOAS "teste_qry_pessoas.hf"
#define CAMINHO_HFD_PESSOAS "teste_qry_pessoas.hfd"
#define CAMINHO_QRY "teste.qry"
#define CAMINHO_SVG "teste_qry.svg"
#define CAMINHO_TXT "teste_qry.txt"

static void criar_qry(const char* conteudo) {
    FILE* f = fopen(CAMINHO_QRY, "w");
    fprintf(f, "%s", conteudo);
    fclose(f);
}

void setUp(void) {
    remove(CAMINHO_HF_QUADRAS);
    remove(CAMINHO_HF_PESSOAS);
    remove(CAMINHO_HFD_QUADRAS);
    remove(CAMINHO_HFD_PESSOAS);
    remove(CAMINHO_QRY);
    remove(CAMINHO_SVG);
    remove(CAMINHO_TXT);

    TEST_ASSERT_EQUAL_INT_MESSAGE(0, cidade_inicializar(CAMINHO_HF_QUADRAS), "cidade_inicializar falhou.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, pessoas_inicializar(CAMINHO_HF_PESSOAS), "pessoas_inicializar falhou.");
    svg_inicializar(CAMINHO_SVG);
    txt_inicializar(CAMINHO_TXT);

    // quadra e habitantes base para os testes
    cidade_inserir_quadra("cep01", 0, 0, 100, 60, "orange", "black", "1.0");
    pessoas_inserir("111", "Joao", "Silva", 'M', "01/01/1990");
    pessoas_inserir("222", "Maria", "Santos", 'F', "15/06/1985");
    pessoas_definir_endereco("111", "cep01", 'S', 10, "-");
    pessoas_definir_endereco("222", "cep01", 'N', 20, "-");
}

void tearDown(void) {
    txt_finalizar();
    svg_finalizar();
    cidade_finalizar(CAMINHO_HFD_QUADRAS);
    pessoas_finalizar(CAMINHO_HFD_PESSOAS);
    remove(CAMINHO_HF_QUADRAS);
    remove(CAMINHO_HF_PESSOAS);
    remove(CAMINHO_HFD_QUADRAS);
    remove(CAMINHO_HFD_PESSOAS);
    remove(CAMINHO_QRY);
    remove(CAMINHO_SVG);
    remove(CAMINHO_TXT);
}

// qry_processar retorna 0 com arquivo valido 
void test_qry_processar_arquivo_valido(void) {
    criar_qry("censo\n");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

// qry_processar retorna -1 se o arquivo não existir
void test_qry_processar_arquivo_inexistente(void) {
    int ret = qry_processar("nao_existe.qry");
    TEST_ASSERT_EQUAL_INT(-1, ret);
}

// arquivo vazio não crasha
void test_qry_arquivo_vazio(void) {
    criar_qry("");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

// rq remove a quadra
void test_qry_rq_remove_quadra(void) {
    criar_qry("rq cep01\n");
    qry_processar(CAMINHO_QRY);
    TEST_ASSERT_NULL(cidade_buscar_quadra("cep01"));
}

// rq torna moradores da quadra sem-teto
void test_qry_rq_moradores_viram_sem_teto(void) {
    criar_qry("rq cep01\n");
    qry_processar(CAMINHO_QRY);
    const Habitante* h1 = pessoas_buscar("111");
    TEST_ASSERT_NOT_NULL(h1);
    TEST_ASSERT_EQUAL_INT(0, habitante_get_morador(h1));
    const Habitante* h2 = pessoas_buscar("222");
    //TEST_ASSERT_NOT_NULL(h1);
    TEST_ASSERT_NOT_NULL(h2);
    //TEST_ASSERT_EQUAL_INT(0, habitante_get_morador(h1));
    TEST_ASSERT_EQUAL_INT(0, habitante_get_morador(h2));
}

// rq com quadra inexistente não crasha
void test_qry_rq_quadra_inexistente(void) {
    criar_qry("rq nao_existe\n");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

// pq com quadra existente não crasha
void test_qry_pq_nao_crasha(void) {
    criar_qry("pq cep01\n");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

// pq com quadra inexistente não crasha
void test_qry_pq_quadra_inexistente(void) {
    criar_qry("pq nao_existe\n");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

// censo não crasha
void test_qry_censo_nao_crasha(void) {
    criar_qry("censo\n");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

// h? encontra habitante existente sem crashar
void test_qry_h_habitante_existente(void) {
    criar_qry("h? 111\n");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

// h? com cpf inexistente não crasha
void test_qry_h_habitante_inexistente(void) {
    criar_qry("h? 999\n");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

// nasc insere novo habitante corretamente
void test_qry_nasc_insere_habitante(void) {
    criar_qry("nasc 333 Pedro João M 20/03/2000\n");
    qry_processar(CAMINHO_QRY);
    const Habitante* h = pessoas_buscar("333");
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_EQUAL_STRING("Pedro", habitante_get_nome(h));
    TEST_ASSERT_EQUAL_STRING("João", habitante_get_sobrenome(h));
    TEST_ASSERT_EQUAL_INT('M', habitante_get_sexo(h));
}

// rip remove o habitante
void test_qry_rip_remove_habitante(void) {
    criar_qry("rip 111\n");
    qry_processar(CAMINHO_QRY);
    TEST_ASSERT_NULL(pessoas_buscar("111"));
}

// rip com cpf inexistente não crasha
void test_qry_rip_habitante_inexistente(void) {
    criar_qry("rip 999\n");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

// mud atualiza endereço do morador
void test_qry_mud_atualiza_endereco(void) {
    cidade_inserir_quadra("cep02", 200, 0, 100, 60, "orange", "black", "1.0");
    criar_qry("mud 111 cep02 N 15 apto2\n");
    qry_processar(CAMINHO_QRY);
    const Habitante* h = pessoas_buscar("111");
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_EQUAL_STRING("cep02", habitante_get_cep(h));
    TEST_ASSERT_EQUAL_INT('N', habitante_get_face(h));
    TEST_ASSERT_EQUAL_INT(15, habitante_get_num(h));
}

// mud com sem-teto não crasha
void test_qry_mud_sem_teto_ignorado(void) {
    pessoas_inserir("555", "Carlos", "Souza", 'M', "05/05/1980");
    criar_qry("mud 555 cep01 S 10\n");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
    // endereço não deve ter sido definido
    const Habitante* h = pessoas_buscar("555");
    TEST_ASSERT_EQUAL_INT(0, habitante_get_morador(h));
}

// dspj torna morador sem-teto
void test_qry_dspj_vira_sem_teto(void) {
    criar_qry("dspj 111\n");
    qry_processar(CAMINHO_QRY);
    const Habitante* h = pessoas_buscar("111");
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_EQUAL_INT(0, habitante_get_morador(h));
}

// dspj com sem-teto não crasha
void test_qry_dspj_sem_teto_ignorado(void) {
    pessoas_inserir("555", "Carlos", "Silva", 'M', "05/05/1980");
    criar_qry("dspj 555\n");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

// dspj com cpf inexistente não crasha
void test_qry_dspj_inexistente(void) {
    criar_qry("dspj 999\n");
    int ret = qry_processar(CAMINHO_QRY);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_qry_processar_arquivo_valido);
    RUN_TEST(test_qry_processar_arquivo_inexistente);
    RUN_TEST(test_qry_arquivo_vazio);
    RUN_TEST(test_qry_rq_remove_quadra);
    RUN_TEST(test_qry_rq_moradores_viram_sem_teto);
    RUN_TEST(test_qry_rq_quadra_inexistente);
    RUN_TEST(test_qry_pq_nao_crasha);
    RUN_TEST(test_qry_pq_quadra_inexistente);
    RUN_TEST(test_qry_censo_nao_crasha);
    RUN_TEST(test_qry_h_habitante_existente);
    RUN_TEST(test_qry_h_habitante_inexistente);
    RUN_TEST(test_qry_nasc_insere_habitante);
    RUN_TEST(test_qry_rip_remove_habitante);
    RUN_TEST(test_qry_rip_habitante_inexistente);
    RUN_TEST(test_qry_mud_atualiza_endereco);
    RUN_TEST(test_qry_mud_sem_teto_ignorado);
    RUN_TEST(test_qry_dspj_vira_sem_teto);
    RUN_TEST(test_qry_dspj_sem_teto_ignorado);
    RUN_TEST(test_qry_dspj_inexistente);

    return UNITY_END();
}