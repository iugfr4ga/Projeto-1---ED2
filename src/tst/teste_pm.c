#define UNITY_INCLUDE_DOUBLE
#include "unity/unity.h"
#include "../pm.h"
#include "../pessoas.h"
#include <stdio.h>
#include <string.h>

#define CAMINHO_HF "teste_pm_pessoas.hf"
#define CAMINHO_PM "teste.pm"

// para criar arquivos .pm temporarios
static void criar_pm(const char* conteudo) {
    FILE* f = fopen(CAMINHO_PM, "w");
    fprintf(f, "%s", conteudo);
    fclose(f);
}

void setUp(void) {
    remove(CAMINHO_HF);
    remove(CAMINHO_PM);
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, pessoas_inicializar(CAMINHO_HF), "pessoas_inicializar falhou.");
}

void tearDown(void) {
    pessoas_finalizar();
    remove(CAMINHO_HF);
    remove(CAMINHO_PM);
}

// pm_processar retorna 0 com arquivo valido 
void test_pm_processar_arquivo_valido(void) {
    criar_pm("p 111 Joao Silva M 01/01/1990\n");
    int ret = pm_processar(CAMINHO_PM);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

// pm_processar retorna -1 se o arquivo não existir 
void test_pm_processar_arquivo_inexistente(void) {
    int ret = pm_processar("nao_existe.pm");
    TEST_ASSERT_EQUAL_INT(-1, ret);
}

// habitante inserido via .pm pode ser encontrado 
void test_pm_insere_habitante(void) {
    criar_pm("p 111 Joao Silva M 01/01/1990\n");
    pm_processar(CAMINHO_PM);
    const Habitante* h = pessoas_buscar("111");
    TEST_ASSERT_NOT_NULL(h);
}

// dados do habitante sao lidos corretamente 
void test_pm_dados_habitante_corretos(void) {
    criar_pm("p 111 Joao Silva M 01/01/1990\n");
    pm_processar(CAMINHO_PM);
    const Habitante* h = pessoas_buscar("111");
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_EQUAL_STRING("111", habitante_get_cpf(h));
    TEST_ASSERT_EQUAL_STRING("Joao", habitante_get_nome(h));
    TEST_ASSERT_EQUAL_STRING("Silva", habitante_get_sobrenome(h));
    TEST_ASSERT_EQUAL_INT('M', habitante_get_sexo(h));
    TEST_ASSERT_EQUAL_STRING("01/01/1990", habitante_get_nasc(h));
}

// habitante inserido via .pm começa como sem-teto
void test_pm_habitante_comeca_sem_teto(void) {
    criar_pm("p 111 Joao Silva M 01/01/1990\n");
    pm_processar(CAMINHO_PM);
    const Habitante* h = pessoas_buscar("111");
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_EQUAL_INT(0, habitante_get_morador(h));
}

// comando m define endereço e torna habitante morador
void test_pm_define_endereco(void) {
    criar_pm(
        "p 111 Joao Silva M 01/01/1990\n"
        "m 111 cep01 S 45 apto1\n"
    );
    pm_processar(CAMINHO_PM);
    const Habitante* h = pessoas_buscar("111");
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_EQUAL_INT(1, habitante_get_morador(h));
    TEST_ASSERT_EQUAL_STRING("cep01", habitante_get_cep(h));
    TEST_ASSERT_EQUAL_INT('S', habitante_get_face(h));
    TEST_ASSERT_EQUAL_INT(45, habitante_get_num(h));
    TEST_ASSERT_EQUAL_STRING("apto1", habitante_get_compl(h));
}

// multiplos habitantes inseridos
void test_pm_multiplos_habitantes(void) {
    criar_pm(
        "p 111 Joao Silva M 01/01/1990\n"
        "p 222 Maria Santos F 15/06/1985\n"
        "p 333 Pedro Lima M 20/03/2000\n"
    );
    pm_processar(CAMINHO_PM);
    TEST_ASSERT_NOT_NULL(pessoas_buscar("111"));
    TEST_ASSERT_NOT_NULL(pessoas_buscar("222"));
    TEST_ASSERT_NOT_NULL(pessoas_buscar("333"));
}

// arquivo vazio nao crasha
void test_pm_arquivo_vazio(void) {
    criar_pm("");
    int ret = pm_processar(CAMINHO_PM);
    TEST_ASSERT_EQUAL_INT(0, ret);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_pm_processar_arquivo_valido);
    RUN_TEST(test_pm_processar_arquivo_inexistente);
    RUN_TEST(test_pm_insere_habitante);
    RUN_TEST(test_pm_dados_habitante_corretos);
    RUN_TEST(test_pm_habitante_comeca_sem_teto);
    RUN_TEST(test_pm_define_endereco);
    RUN_TEST(test_pm_multiplos_habitantes);
    RUN_TEST(test_pm_arquivo_vazio);

    return UNITY_END();
}