#ifndef PM_H
#define PM_H

/*  Uma pessoa é descrita pelos seguintes comandos do arquivo .pm:

    p <cpf> <nome> <sobrenome> <sexo> <nasc>
    - Insere uma pessoa com os dados fornecidos.

    m <cpf> <cep> <face> <num> <compl>
    - Define o endereço de uma pessoa. O endereço é composto por um CEP, uma face (N, S, L ou O), 
    um número e um complemento opcional. O endereço deve ser definido para uma pessoa já inserida.
*/

/*  Le e processa o arquivo .pm.
    O arquivo deve estar no caminho indicado por *caminho.
    Retorna 0 em sucesso ou -1 se o arquivo não puder ser aberto.
*/
int pm_processar(const char *caminho);

#endif