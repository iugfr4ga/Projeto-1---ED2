#ifndef SVG_H
#define SVG_H

#define SVG_LARGURA 50000
#define SVG_ALTURA 50000

/*  O módulo SVG é responsável por gerar um arquivo SVG de saída com a representação gráfica da cidade e seus habitantes. 
    Ele deve ser inicializado antes de processar os arquivos .geo e .qry e finalizado ao final do programa para 
    fechar o arquivo SVG.

    O arquivo SVG gerado tem as seguintes características:

    - A origem (0,0) é o canto superior esquerdo da cidade.

    - O eixo x cresce para a direita e o eixo y cresce para baixo.

    - As coordenadas N, S, L, O são invertidas.

    - A área do SVG é definida por SVG_LARGURA e SVG_ALTURA e pode ser aumentada caso a quadra não esteja dentro desses
    limites.
*/

/*  Inicializa o módulo SVG criando o arquivo de saída no caminho indicado.
    Retorna 0 em sucesso ou -1 em erro.
*/
int svg_inicializar(const char* caminho);

/*  Finaliza o SVG fechando a tag </svg> e o arquivo.  */
void svg_finalizar(void);

/*  Desenha uma quadra como retângulo com preenchimento, borda e espessura dados.
    O ponto (x, y) é o canto superior esquerdo no sistema SVG (y cresce para baixo).
*/
void svg_desenhar_quadra(const char* cep, double x, double y, double w, double h, const char* cfill, const char* cstrk, const char* sw);

/*  Marca a âncora (canto superior esquerdo) da quadra removida com um X vermelho.  */
void svg_marcar_remocao(double x, double y);

/*  Escreve o número de moradores de cada face próximo ao limite da face e o total no centro da quadra. */
void svg_marcar_moradores(double x, double y, double w, double h, const char* sw, int n, int s, int l, int o, int total);

/*  Marca o endereço de um óbito com uma cruz vermelha. */
void svg_marcar_obito(double px, double py, char face);

/*  Marca o endereço de destino de uma mudança com um quadrado vermelho contendo o cpf em fonte minúscula. */
void svg_marcar_mudanca(double px, double py, const char* cpf);

/*  Marca o local de um despejo com um círculo preto. */
void svg_marcar_despejo(double px, double py, char face);

#endif