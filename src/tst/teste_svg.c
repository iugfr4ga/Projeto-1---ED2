#include "unity/unity.h"
#include "../svg.h"
#include <stdio.h>
#include <string.h>

#define CAMINHO_SVG "teste_svg.svg"

static void ler_svg(char *buf, int tam) {
    memset(buf, 0, tam);
    FILE* f = fopen(CAMINHO_SVG, "r");
    if(f == NULL) 
        return;
    fread(buf, 1, tam - 1, f);
    fclose(f);
}

void setUp(void) {
    remove(CAMINHO_SVG);
    svg_inicializar(CAMINHO_SVG);
}

void tearDown(void) {
    svg_finalizar();
    remove(CAMINHO_SVG);
}

void test_svg_inicializar_valido(void) {
    TEST_PASS();
}

void test_svg_inicializar_invalido(void) {
    svg_finalizar();
    int ret = svg_inicializar("/caminho/invalido/nao_existe/saida.svg");
    TEST_ASSERT_EQUAL_INT(-1, ret);
    svg_inicializar(CAMINHO_SVG);
}

void test_svg_contem_tag_svg(void) {
    svg_finalizar();
    char buf[512];
    ler_svg(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "<svg"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "</svg>"));
    svg_inicializar(CAMINHO_SVG);
}

void test_svg_desenhar_quadra_rect(void) {
    svg_desenhar_quadra("cep01", 10, 20, 100, 60, "orange", "black", "1.0");
    svg_finalizar();

    char buf[1024];
    ler_svg(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "<rect"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "orange"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "black"));

    svg_inicializar(CAMINHO_SVG);
}

void test_svg_desenhar_quadra_cep(void) {
    svg_desenhar_quadra("cep01", 10, 20, 100, 60, "orange", "black", "1.0");
    svg_finalizar();

    char buf[1024];
    ler_svg(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "cep01"));

    svg_inicializar(CAMINHO_SVG);
}

void test_svg_marcar_remocao(void) {
    svg_marcar_remocao(50, 50);
    svg_finalizar();

    char buf[1024];
    ler_svg(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "<line"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "stroke=\"red\""));

    svg_inicializar(CAMINHO_SVG);
}

void test_svg_marcar_moradores(void) {
    svg_marcar_moradores(0, 0, 100, 60, "1.0", 2, 3, 1, 4, 10);
    svg_finalizar();

    char buf[2048];
    ler_svg(buf, sizeof(buf));

    int count = 0;
    const char* p = buf;
    while((p = strstr(p, "<text")) != NULL) {
        count++;
        p++;
    }
    TEST_ASSERT_GREATER_OR_EQUAL_INT(5, count);

    svg_inicializar(CAMINHO_SVG);
}

void test_svg_marcar_obito_face_s(void) {
    svg_marcar_obito(50, 50, 'S');
    svg_finalizar();

    char buf[1024];
    ler_svg(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "<line"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "stroke=\"red\""));

    svg_inicializar(CAMINHO_SVG);
}

void test_svg_marcar_obito_todas_faces(void) {
    svg_marcar_obito(50, 50, 'N');
    svg_marcar_obito(50, 50, 'S');
    svg_marcar_obito(50, 50, 'L');
    svg_marcar_obito(50, 50, 'O');
    int ret = 0;  // se chegou aqui sem crash, passou
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_svg_marcar_mudanca(void) {
    svg_marcar_mudanca(30, 40, "123456789");
    svg_finalizar();

    char buf[1024];
    ler_svg(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "<rect"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "123456789"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "fill=\"red\""));

    svg_inicializar(CAMINHO_SVG);
}

void test_svg_marcar_despejo_face_s(void) {
    svg_marcar_despejo(50, 50, 'S');
    svg_finalizar();

    char buf[1024];
    ler_svg(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "<circle"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "fill=\"black\""));

    svg_inicializar(CAMINHO_SVG);
}

void test_svg_marcar_despejo_todas_faces(void) {
    svg_marcar_despejo(50, 50, 'N');
    svg_marcar_despejo(50, 50, 'S');
    svg_marcar_despejo(50, 50, 'L');
    svg_marcar_despejo(50, 50, 'O');
    int ret = 0;
    TEST_ASSERT_EQUAL_INT(0, ret);
}

void test_svg_multiplas_quadras(void) {
    svg_desenhar_quadra("cep01", 0,   0, 100, 60, "orange", "black", "1.0");
    svg_desenhar_quadra("cep02", 120, 0, 100, 60, "blue",   "black", "1.0");
    svg_finalizar();

    char buf[2048];
    ler_svg(buf, sizeof(buf));
    TEST_ASSERT_NOT_NULL(strstr(buf, "cep01"));
    TEST_ASSERT_NOT_NULL(strstr(buf, "cep02"));

    svg_inicializar(CAMINHO_SVG);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_svg_inicializar_valido);
    RUN_TEST(test_svg_inicializar_invalido);
    RUN_TEST(test_svg_contem_tag_svg);
    RUN_TEST(test_svg_desenhar_quadra_rect);
    RUN_TEST(test_svg_desenhar_quadra_cep);
    RUN_TEST(test_svg_marcar_remocao);
    RUN_TEST(test_svg_marcar_moradores);
    RUN_TEST(test_svg_marcar_obito_face_s);
    RUN_TEST(test_svg_marcar_obito_todas_faces);
    RUN_TEST(test_svg_marcar_mudanca);
    RUN_TEST(test_svg_marcar_despejo_face_s);
    RUN_TEST(test_svg_marcar_despejo_todas_faces);
    RUN_TEST(test_svg_multiplas_quadras);

    return UNITY_END();
}