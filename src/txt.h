#ifndef TXT_H
#define TXT_H

#include "pessoas.h"

/*  O módulo TXT é responsável por gerar um arquivo de saída com a extensão .txt contendo as informações textuais 
    solicitadas pelos comandos do arquivo .qry. Ele deve ser inicializado antes de processar o arquivo .qry e 
    finalizado ao final do programa para fechar o arquivo TXT.
*/

/*  Inicializa o módulo TXT criando o arquivo de saída no caminho indicado.
    Retorna 0 em sucesso ou -1 em erro.
*/
int txt_inicializar(const char* caminho);

/*  Finaliza o módulo TXT fechando o arquivo de saída. */
void txt_finalizar(void);

/*  Informa no TXT o CPF e nome de um morador que passou a ser sem-teto devido à remoção de sua quadra (comando rq). */
void txt_rq_morador(const char* cpf, const char* nome, const char* sobrenome);

/*  Reporta as estatísticas do censo (comando censo). */
void txt_censo(int* total, int* moradores, int* sem_teto, int* homens, int* mulheres, int* moradores_h, int* moradores_m, int* semteto_h,  int* semteto_m);

/*  Reporta todos os dados de um habitante (comando h?). Se for morador, reporta também o endereço. */
void txt_habitante(const Habitante* hab);

/*  Reporta os dados do habitante falecido (comando rip). Se for morador, reporta também o endereço. */
void txt_obito(const Habitante* hab);

/*  Reporta os dados do habitante despejado e o endereço do despejo (comando dspj). */
void txt_despejo(const Habitante* hab);

#endif