#ifndef PESSOAS_H
#define PESSOAS_H

typedef struct Habitante Habitante;

/* inicializa o módulo abrindo ou criando o hashfile de habitantes no caminho indicado */
int pessoas_inicializar(const char *caminho_hf);

/* Finaliza o módulo fechando o hashfile */
void pessoas_finalizar(void);

/*  Insere um habitante sem endereço (sem-teto).
    Retorna 0 em sucesso ou -1 em erro.
*/
int pessoas_inserir(const char *cpf, const char *nome, const char *sobrenome, char sexo, const char *nasc);

/*  Define o endereço de um habitante existente, tornando-o morador.
    Retorna 0 em sucesso ou -1 se não encontrado.
*/
int pessoas_definir_endereco(const char *cpf, const char *cep, char face, int num, const char *compl);

/*  Busca o habitante identificado pelo cpf.
    Retorna ponteiro para Habitante em sucesso ou NULL se não encontrado.
    O ponteiro é válido até a próxima chamada de qualquer função deste módulo.
*/
const Habitante* pessoas_buscar(const char *cpf);

/*  Atualiza o endereço de um morador identificado pelo cpf.
    Retorna 0 em sucesso ou -1 se não encontrado ou se for sem-teto.
*/
int pessoas_atualizar_endereco(const char *cpf, const char *cep, char face, int num, const char *compl);

/*  Remove o endereço de um morador, tornando-o sem-teto (despejo).
    Retorna 0 em sucesso ou -1 se não encontrado.
*/
int pessoas_remover_endereco(const char *cpf);

/*  Remove o habitante identificado pelo cpf (óbito).
    Retorna 0 em sucesso ou -1 se não encontrado.
*/
int pessoas_remover(const char *cpf);

/*  Itera sobre todos os habitantes do hashfile. A função de callback é executada em cada registro
    e pode usar *ctx para armazenar resultados ou *ctx pode ser passado como NULL. O callback deve retornar 0 
    para continuar a iteração ou qualquer valor diferente de 0 para interrompê-la imediatamente. 
    Retorna 0 se iterou sobre todos os registros, o valor de parada do callback se a iteração foi interrompida, 
    ou -1 em erro.
*/
int pessoas_iterar(int (*callback)(const Habitante *hab, void *ctx), void *ctx);

// getters 
const char* habitante_get_cpf(const Habitante *h);
const char* habitante_get_nome(const Habitante *h);
const char* habitante_get_sobrenome(const Habitante *h);
char  habitante_get_sexo(const Habitante *h);
const char* habitante_get_nasc(const Habitante *h);
int habitante_get_morador(const Habitante *h);
const char* habitante_get_cep(const Habitante *h);
char habitante_get_face(const Habitante *h);
int habitante_get_num(const Habitante *h);
const char* habitante_get_compl(const Habitante *h);

#endif