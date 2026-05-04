#ifndef QRY_H
#define QRY_H

/*  Um arquivo .qry executa comandos sobre a cidade e suas pessoas e gera saídas em TXT e SVG. 
    A cidade e as pessoas devem ser inicializadas antes de processar o arquivo .qry.
    Os comandos do arquivo .qry são:

    rq <cep>
    - Remove quadra identificada pelo cep. Moradores da quadra passam a ser sem-tetos.
    TXT: Reporta os moradores despejados (cpf, nome e sobrenome),
    SVG: Marca a quadra removida com um pequeno X vermelho sobre sua ancora.

    pq <cep>
    - Calcula o número de moradores da quadra (por face e total). 
    SVG: Coloca o número de moradores em cada face da quadra e o total no centro.

    censo
    - Realiza um censo populacional, contando o número total de habitantes, número total de moradores, 
    proporção moradores/habitantes, número de homens, número de mulheres, % de habitantes homens, % habitantes 
    mulheres, % de moradores homens, % de moradores mulheres, número total de sem-tetos, % sem-tetos homens, 
    %sem-tetos mulheres.
    TXT: Reporta os resultados.

    h? <cpf>
    - Exibe as informações do habitante identificado pelo cpf.
    TXT: Reporta os dados do habitante. Se for morador, reporta também o endereço.

    nasc <cpf> <nome> <sobrenome> <sexo> <data_nasc>
    - Nasce um novo habitante com os dados fornecidos. O habitante é adicionado ao sistema sem endereço (sem-teto).

    rip <cpf>
    - Habitante identificado pelo cpf falece e é removido do sistema.
    TXT: Reporta os dados da pessoa falecida. Se for morador, reporta também o endereço. 
    SVG: Se o habitante era morador, seu endereço é marcado com uma pequena cruz vermelha.

    mud <cpf> <cep> <face> <num> <compl>
    - Morador identificado por cpf muda-se para novo endereço fornecido.
    SVG: Marca o endereço de destino com um pequeno quadrado vermelho e escreve o cpf do morador
    dentro do quadrado.

    dspj <cpf>
    - O habitante identificado pelo cpf perde seu endereço (torna-se sem-teto). 
    TXT: Registra os dados do morador despejado e antigo endereço. 
    SVG: Marca com umpequeno círculo preto o local do despejo.
*/


/*  Le e processa o arquivo .qry.
    O arquivo deve estar no caminho indicado por *caminho.
    Retorna 0 em sucesso ou -1 se o arquivo não puder ser aberto.
*/
int qry_processar(const char* caminho);

#endif