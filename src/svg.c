#include "svg.h"
#include <stdio.h>
#include <stdlib.h>

static FILE* svg_arquivo = NULL;

int svg_inicializar(const char* caminho) {
    svg_arquivo = fopen(caminho, "w");
    if(svg_arquivo == NULL)
        return -1;

    fprintf(svg_arquivo,"<svg xmlns=\"http://www.w3.org/2000/svg\""
            " width=\"%d\" height=\"%d\">\n",
            SVG_LARGURA, SVG_ALTURA);
    return 0;
}

void svg_finalizar(void) {
    if(svg_arquivo == NULL)
        return;
    fprintf(svg_arquivo, "</svg>\n");
    fclose(svg_arquivo);
    svg_arquivo = NULL;
}

void svg_desenhar_quadra(const char* cep, double x, double y, double w, double h, const char* cfill, const char* cstrk, const char* sw) {
    if(svg_arquivo == NULL)
        return;

    fprintf(svg_arquivo,
            "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\""
            " fill=\"%s\" stroke=\"%s\" stroke-width=\"%s\"/>\n",
            x, y, w, h, cfill, cstrk, sw);

    // CEP no centro da quadra
    fprintf(svg_arquivo,
            "<text x=\"%.2f\" y=\"%.2f\""
            " text-anchor=\"start\" dominant-baseline=\"hanging\""
            " font-size=\"10\" font-weight=\"bold\" fill=\"black\">%s</text>\n",
            x, y, cep);
}

void svg_marcar_remocao(double x, double y) {
    if (svg_arquivo == NULL)
        return;

    // X vermelho
    double d = 2.5;
    fprintf(svg_arquivo,
            "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\""
            " stroke=\"red\" stroke-width=\"1\"/>\n",
            x - d, y - d, x + d, y + d);
    fprintf(svg_arquivo,
            "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\""
            " stroke=\"red\" stroke-width=\"1\"/>\n",
            x + d, y - d, x - d, y + d);
}

void svg_marcar_moradores(double x, double y, double w, double h, const char* sw, int n, int s, int l, int o, int total) {
    if (svg_arquivo == NULL)
        return;

    double cx = x + w / 2.0;
    double cy = y + h / 2.0;
    double offset = 10 * atof(sw);   // distancia do limite da face para o texto

    // face S — topo da quadra
    fprintf(svg_arquivo,
            "<text x=\"%.2f\" y=\"%.2f\""
            " text-anchor=\"middle\" font-size=\"9\" fill=\"black\">%d</text>\n",
            cx, y + offset, s);

    // face N — base da quadra
    fprintf(svg_arquivo,
            "<text x=\"%.2f\" y=\"%.2f\""
            " text-anchor=\"middle\" font-size=\"9\" fill=\"black\">%d</text>\n",
            cx, y + h - offset, n);

    // face O — lado direito
    fprintf(svg_arquivo,
            "<text x=\"%.2f\" y=\"%.2f\""
            " text-anchor=\"start\" font-size=\"9\" fill=\"black\">%d</text>\n",
            x + w - offset, cy, o);

    // face L — lado esquerdo
    fprintf(svg_arquivo,
            "<text x=\"%.2f\" y=\"%.2f\""
            " text-anchor=\"end\" font-size=\"9\" fill=\"black\">%d</text>\n",
            x + offset, cy, l);

    // total no centro
    fprintf(svg_arquivo,
            "<text x=\"%.2f\" y=\"%.2f\""
            " text-anchor=\"middle\" font-size=\"9\" font-weight=\"bold\" fill=\"black\">%d</text>\n",
            cx, cy, total);
}

void svg_marcar_obito(double px, double py, char face) {
    if(svg_arquivo == NULL)
        return;

    double d = 2.5;

    switch(face) {
        case 'S': py = py + d; break;
        case 'N': py = py - d; break;
        case 'O': px = px - d; break;
        case 'L': px = px + d; break;
    }

    // cruz vermelha
    fprintf(svg_arquivo,
            "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\""
            " stroke=\"red\" stroke-width=\"1\"/>\n",
            px - d, py - 0.5, px + d, py - 0.5);

    fprintf(svg_arquivo,
            "<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\""
            " stroke=\"red\" stroke-width=\"1\"/>\n",
            px, py - d, px, py + d);
}

void svg_marcar_mudanca(double px, double py, const char *cpf) {
    if(svg_arquivo == NULL)
        return;

    double lado = 14.0;

    // quadrado vermelho
    fprintf(svg_arquivo,
            "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\""
            " fill=\"red\" fill-opacity=\"0.4\"/>\n",
            px, py, lado, lado);

    // CPF em fonte minuscula dentro do quadrado
    fprintf(svg_arquivo,
            "<text x=\"%.2f\" y=\"%.2f\""
            " text-anchor=\"start\" dominant-baseline=\"central\""
            " font-size=\"2\" fill=\"black\">%s</text>\n",
            px, py + 7, cpf);
}

void svg_marcar_despejo(double px, double py, char face) {
    if(svg_arquivo == NULL)
        return;

    double d = 3.33;

    switch(face) {
        case 'S': py = py + d; break;
        case 'N': py = py - d; break;
        case 'O': px = px - d; break;
        case 'L': px = px + d; break;
    }

    // circulo preto
    fprintf(svg_arquivo,
            "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\""
            " fill=\"black\" fill-opacity=\"0.7\"/>\n", 
            px, py, d);
}