#ifndef CIDADE_H
#define CIDADE_H

#define FACE_S 'S'
#define FACE_N 'N'
#define FACE_O 'O'
#define FACE_L 'L'

typedef struct Quadra Quadra;

/* Inicializa a cidade abrindo ou criando o hashfile de quadras no caminho indicado */
int cidade_inicializar(const char *caminho_hf);

/* Finaliza a cidade fechando o hashfile */
void cidade_finalizar(void);

/*  Insere uma quadra com os dados fornecidos.
    Retorna 0 em sucesso ou -1 em erro.
*/
int cidade_inserir_quadra(const char *cep, double x, double y, double w, double h, const char *cfill, const char *cstrk, float sw);

/*  Remove a quadra identificada pelo cep.
    Retorna 0 em sucesso ou -1 se não encontrada.
*/
int cidade_remover_quadra(const char *cep);

/*  Busca a quadra identificada pelo cep.
    Retorna ponteiro para Quadra em sucesso ou NULL se não encontrada.
    O ponteiro é válido até a próxima chamada de qualquer função deste módulo.
*/
const Quadra* cidade_buscar_quadra(const char *cep);

/*  Dado um endereço CEP/face/número, calcula as coordenadas SVG do ponto
    e armazena em *px e *py.
    Retorna 0 em sucesso ou -1 se a quadra não for encontrada.
*/
int cidade_coordenadas(const char *cep, char face, int num, double *px, double *py);

// getters
const char* quadra_get_cep(const Quadra *q);
double quadra_get_x(const Quadra *q);
double quadra_get_y(const Quadra *q);
double  quadra_get_w(const Quadra *q);
double quadra_get_h(const Quadra *q);
const char* quadra_get_cfill(const Quadra *q);
const char* quadra_get_cstrk(const Quadra *q);
float quadra_get_sw(const Quadra *q);

#endif