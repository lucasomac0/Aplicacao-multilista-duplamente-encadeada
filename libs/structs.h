#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef structs_h
#define structs_h

#define SUCESSO 1
#define FRACASSO 0;

typedef struct noPalavra{
    char palavra[70];
    int linha, coluna;
    struct noPalavra *ant;
    struct noPalavra *prox;
}noPalavra;
// a struct responsavel por cada palavra contida em uma linha, aponta para a anterior e para a proxima (pois é duplamente encadeada) e possui os campos linha e coluna, 
// juntamente do conteudo em si da palavra (string)

typedef struct noLinha{
    int numPalavras;
    noPalavra *palavras;
    struct noLinha *ant;
    struct noLinha *prox;
}noLinha;
//a struct responsavel por cada linha da lista. Possui um campo com o numero de palavras contidas na linha, um ponteiro
//para a primeira palavra da lista (do tipo noPalavra) e um ponteiro para a linha anterior e posterior.

typedef struct descLDE{
    noLinha *inicio;
    noLinha *fim;
    int numLinha;
}descLDE;
//o descritor da Lista Duplamente Encadeada, possui ponteiros para o inicio e o fim da lista
//juntamente com um campo indicando o numero de linhas que a lista possui

#endif

//funcoes principais
int inserePalavra(noLinha *linha, char *palavra, int coluna);
int abreArquivo(descLDE *lista, char *nomeArquivo);
noLinha* insereLinha(descLDE *lista, int numPalavras);
void exibeLista(descLDE *lista);
int buscaPalavra(descLDE *lista, char *chave);
void contaPalavras(descLDE *lista);
void removePalavra(descLDE *lista, char *chave);
void removePalavraPosicao(descLDE *lista, char *chave, int linha, int coluna);
int numeroOcorrencias(descLDE *lista, char *chave);
void edicaoPalavra(descLDE *lista, char *palavraNova, int indiceLinha, int indiceColuna);
void inserePalavraLista(descLDE *lista, char *palavra, int indiceLinha, int indiceColuna);
void substring(descLDE *lista, char *substring);
void atualizaArquivo(descLDE *lista, char *nomeArquivo);
//funcoes auxiliares
void lerString(char *string, int tamanho);
void limpaTela();
void pressEnter(); 
void limpaBuffer();

noLinha* insereLinha(descLDE *lista, int numPalavras) {
    // aloca memória para um novo nó de linha e inicializa os campos
    noLinha *novo = (noLinha*)malloc(sizeof(noLinha));
    novo->numPalavras = numPalavras;  // define o número de palavras na linha
    novo->palavras = NULL;  // inicializa a lista de palavras da linha como vazia
    novo->ant = lista->fim;  // define o ponteiro 'ant' para o fim atual da lista
    novo->prox = NULL;  // 'prox' é NULL, pois o novo nó será o último da lista

    // verifica se a lista está vazia
    if (lista->inicio == NULL) {
        // se a lista está vazia, define o novo nó como o primeiro da lista
        lista->inicio = novo;
    } else {
        // se a lista já contém linhas, ajusta o último nó para apontar para o novo
        lista->fim->prox = novo;
    }
    // atualiza o fim da lista para ser o novo nó
    lista->fim = novo;

    return novo;  // retorna o novo nó de linha criado
}


int inserePalavra(noLinha *linha, char *palavra, int coluna) {       
    // aloca memória para um novo nó de palavra e inicializa o conteúdo
    noPalavra *novo = (noPalavra*)malloc(sizeof(noPalavra));
    strcpy(novo->palavra, palavra);  // copia a palavra para o campo 'palavra' do nó
    novo->linha = linha->numPalavras;  // define a linha da palavra com base no contador de palavras da linha
    novo->coluna = coluna;  // define a coluna de início da palavra
    novo->ant = NULL;
    novo->prox = NULL;

    // verifica se a lista de palavras da linha está vazia
    if (linha->palavras == NULL) {
        // caso a lista esteja vazia, insere a nova palavra como o primeiro nó
        linha->palavras = novo;
        return SUCESSO;
    } else {
        // caso a lista já tenha palavras, percorre até o final
        noPalavra *aux = linha->palavras;
        while (aux->prox != NULL) {
            aux = aux->prox;  // avança para o próximo nó até o final da lista
        }
        // insere o novo nó no final da lista e ajusta os ponteiros
        aux->prox = novo;
        novo->ant = aux;
        return SUCESSO;
    }

    return FRACASSO;  // código de retorno caso a inserção falhe, porém não é alcançado neste fluxo
}


int abreArquivo(descLDE *lista, char *nomeArquivo) {
    FILE *arq = fopen(nomeArquivo, "r");  // abre o arquivo para leitura
    if (arq == NULL) {
        // se o arquivo não pôde ser aberto, exibe uma mensagem de erro e retorna FRACASSO
        printf("Erro ao abrir o arquivo");
        return FRACASSO;
    }
    
    char linha[256];  // buffer para armazenar cada linha do arquivo
    int numLinha = 1; // contador para o número da linha, começando em 1

    // lê o arquivo linha por linha
    while (fgets(linha, sizeof(linha), arq)) {
        // cria uma nova linha na lista para armazenar as palavras da linha atual
        noLinha *linhaAtual = (noLinha*)malloc(sizeof(noLinha));
        linhaAtual = insereLinha(lista, numLinha); // insere a nova linha na lista principal

        char *linhaOriginal = strdup(linha);  // cria uma cópia da linha original para preservar o conteúdo
        char *palavra = strtok(linha, " \t\n"); // divide a linha em palavras usando espaço, tab e nova linha como delimitadores

        // percorre cada palavra na linha
        while (palavra != NULL) {
            // encontra a posição da palavra na linha original
            char *pos = strstr(linhaOriginal, palavra); 
            int coluna = (pos - linhaOriginal) + 1;  // calcula a coluna da palavra (começando da coluna 1)

            inserePalavra(linhaAtual, palavra, coluna);  // insere a palavra e sua posição na linha

            // passa para a próxima palavra
            palavra = strtok(NULL, " \t\n");
        }

        free(linhaOriginal);  // libera a memória alocada para a cópia da linha
        numLinha++;  // incrementa o número da linha para a próxima iteração
    }
    
    fclose(arq);  // fecha o arquivo após a leitura completa
    return SUCESSO;  // retorna SUCESSO indicando que o arquivo foi processado corretamente
}


void exibeLista(descLDE *lista) {
    noLinha *linha = (noLinha*)malloc(sizeof(noLinha)); 
    linha = lista->inicio;
    while (linha != NULL) {
        printf("\n", linha->numPalavras);
        noPalavra *palavra = (noPalavra*)malloc(sizeof(noPalavra)); 
        palavra = linha->palavras;
        while (palavra != NULL) {
            printf("%s ", palavra->palavra, palavra->linha, palavra->coluna);
            palavra = palavra->prox;
        }
        linha = linha->prox;
    }
}


int buscaPalavra(descLDE *lista, char *chave) {
    // inicializa o ponteiro para a primeira linha da lista
    noLinha *linha = lista->inicio;
    int encontrada = 0;  // variável para marcar se a palavra foi encontrada

    // verifica se a lista está vazia
    if (lista == NULL || lista->inicio == NULL) {
        printf("\nLista vazia!\n");
        return 0;  // retorna 0 se a lista estiver vazia
    }

    // percorre cada linha da lista principal
    while (linha != NULL) {
        // inicializa o ponteiro para percorrer as palavras da linha atual
        noPalavra *palavra = linha->palavras;

        // percorre cada palavra na linha atual
        while (palavra != NULL) {
            // verifica se a palavra atual corresponde à chave de busca
            if (strcmp(palavra->palavra, chave) == 0) {
                // se a palavra for encontrada, imprime sua posição (linha e coluna)
                printf("\nPalavra encontrada na coordenada: Linha %i, Coluna %i\n", palavra->linha, palavra->coluna);
                encontrada = 1;  // marca que a palavra foi encontrada
            }
            palavra = palavra->prox;  // avança para a próxima palavra
        }

        linha = linha->prox;  // avança para a próxima linha na lista principal
    }

    // verifica se a palavra não foi encontrada em nenhuma linha
    if (encontrada == 0) {
        printf("\nPalavra nao encontrada!\n");
        return 0;  // retorna 0 para indicar que a palavra não foi encontrada
    }

    return 1;  // retorna 1 se a palavra foi encontrada pelo menos uma vez
}


void contaPalavras(descLDE *lista) {
    int qtdPalavras = 0;  // contador para o número total de palavras
    noLinha *linha = lista->inicio;  // inicializa o ponteiro para percorrer as linhas da lista

    // percorre cada linha da lista principal
    while (linha != NULL) {
        noPalavra *palavra = linha->palavras;  // inicializa o ponteiro para a lista de palavras na linha atual

        // percorre cada palavra na linha atual
        while (palavra != NULL) {
            qtdPalavras++;  // incrementa o contador de palavras
            palavra = palavra->prox;  // avança para a próxima palavra
        }

        linha = linha->prox;  // avança para a próxima linha da lista principal
    }

    // imprime o número total de palavras contadas no texto
    printf("\nO numero total de palavras no texto eh: %i\n", qtdPalavras);
}


void removePalavra(descLDE *lista, char *chave) {
    // verifica se a lista ou o início da lista está vazio
    if (lista == NULL || lista->inicio == NULL) {
        printf("\nLista vazia!\n");
        return;
    }

    noLinha *linhaAtual = lista->inicio;  // inicializa o ponteiro para percorrer as linhas da lista
    int palavrasRemovidas = 0;  // contador para o número de palavras removidas

    // percorre cada linha na lista principal
    while (linhaAtual != NULL) {
        noPalavra *palavraAtual = linhaAtual->palavras;  // inicializa o ponteiro para a lista de palavras na linha atual
        noPalavra *anterior = NULL;  // inicializa o ponteiro para o nó anterior, para facilitar a remoção

        // percorre todas as palavras na linha atual
        while (palavraAtual != NULL) {
            noPalavra *proximaPalavra = palavraAtual->prox;  // armazena o próximo nó para continuar a iteração após a remoção

            // verifica se a palavra atual corresponde à chave
            if (strcmp(palavraAtual->palavra, chave) == 0) {
                // remove a palavra se for igual à chave fornecida
                if (anterior == NULL) {
                    // caso 1: palavra a ser removida é a primeira na lista de palavras da linha
                    linhaAtual->palavras = palavraAtual->prox;
                    if (palavraAtual->prox != NULL) {
                        palavraAtual->prox->ant = NULL;  // ajusta o ponteiro 'ant' do novo primeiro nó
                    }
                } else if (palavraAtual->prox == NULL) {
                    // caso 2: palavra a ser removida é a última na lista de palavras da linha
                    anterior->prox = NULL;  // ajusta o ponteiro 'prox' do nó anterior para NULL
                } else {
                    // caso 3: palavra a ser removida está no meio da lista
                    anterior->prox = palavraAtual->prox;  // conecta o nó anterior ao próximo nó
                    palavraAtual->prox->ant = anterior;  // conecta o próximo nó ao anterior
                }

                // libera a memória da palavra removida e atualiza o contador
                free(palavraAtual);
                linhaAtual->numPalavras--;  // decrementa o número de palavras na linha
                palavrasRemovidas++;  // incrementa o número total de palavras removidas
            } else {
                anterior = palavraAtual;  // atualiza 'anterior' apenas se a palavra não foi removida
            }

            palavraAtual = proximaPalavra;  // avança para a próxima palavra, usando o valor armazenado
        }

        // verifica se a linha ficou vazia após a remoção de palavras
        if (linhaAtual->numPalavras == 0) {
            noLinha *proximaLinha = linhaAtual->prox;  // armazena o próximo nó da linha

            // remove a linha vazia da lista principal
            if (linhaAtual->ant == NULL) {
                // caso 1: linha a ser removida é a primeira linha da lista
                lista->inicio = linhaAtual->prox;
                if (linhaAtual->prox != NULL) {
                    linhaAtual->prox->ant = NULL;  // ajusta o ponteiro 'ant' do novo primeiro nó
                }
            } else if (linhaAtual->prox == NULL) {
                // caso 2: linha a ser removida é a última linha da lista
                linhaAtual->ant->prox = NULL;
                lista->fim = linhaAtual->ant;  // atualiza o ponteiro 'fim' da lista
            } else {
                // caso 3: linha a ser removida está no meio da lista
                linhaAtual->ant->prox = linhaAtual->prox;  // conecta o nó anterior ao próximo
                linhaAtual->prox->ant = linhaAtual->ant;  // conecta o próximo nó ao anterior
            }

            // libera a memória da linha removida e atualiza o ponteiro da lista principal
            free(linhaAtual);
            lista->numLinha--;  // decrementa o número de linhas na lista
            linhaAtual = proximaLinha;  // avança para a próxima linha
        } else {
            linhaAtual = linhaAtual->prox;  // avança para a próxima linha se não houver remoção
        }
    }

    // imprime o número de ocorrências da palavra removida ou uma mensagem de ausência
    if (palavrasRemovidas > 0) {
        printf("\nForam removidas %d ocorrencia(s) da palavra \"%s\"\n", palavrasRemovidas, chave);
    } else {
        printf("\nPalavra \"%s\" nao encontrada no texto\n", chave);
    }
}


void removePalavraPosicao(descLDE *lista, char *chave, int linha, int coluna) {
    if (lista == NULL || lista->inicio == NULL) {
        // verifica se a lista está vazia; se estiver, exibe uma mensagem e termina a função
        printf("\nLista vazia!\n");
        return;
    }

    noLinha *linhaAtual = lista->inicio;
    int numLinhaAtual = 1;

    // procura a linha específica até encontrar o número da linha desejado ou até o final
    while (linhaAtual != NULL && numLinhaAtual != linha) {
        linhaAtual = linhaAtual->prox;
        numLinhaAtual++;
    }

    // verifica se a linha foi encontrada; se não foi, exibe uma mensagem e termina a função
    if (linhaAtual == NULL) {
        printf("\nLinha %d nao encontrada!\n", linha);
        return;
    }

    noPalavra *palavraAtual = linhaAtual->palavras;
    noPalavra *anterior = palavraAtual->ant; // define o ponteiro para o nó anterior da palavra
    int palavraRemovida = 0;

    // percorre todas as palavras na linha específica
    while (palavraAtual != NULL) {
        // verifica se a palavra e a coluna correspondem às especificadas na chamada da função
        if (strcmp(palavraAtual->palavra, chave) == 0) {
            // remove a palavra se encontrar a correspondência na posição especificada

            if (anterior == NULL) {
                // caso 1: a palavra é a primeira da lista de palavras da linha
                linhaAtual->palavras = palavraAtual->prox; // atualiza o início da lista de palavras
                palavraAtual->prox = NULL;
                if (palavraAtual->prox != NULL) {
                    palavraAtual->prox->ant = NULL; // ajusta o ponteiro 'ant' do novo primeiro nó
                }
            } else if (palavraAtual->prox == NULL) {
                // caso 2: a palavra é a última da lista de palavras da linha
                palavraAtual->ant->prox = NULL;  // ajusta o ponteiro 'prox' do nó anterior
                palavraAtual->ant = NULL;
            } else {
                // caso 3: a palavra está no meio da lista de palavras da linha
                anterior->prox = palavraAtual->prox; // conecta o nó anterior ao próximo nó
                palavraAtual->prox->ant = anterior; // conecta o próximo nó ao anterior
                palavraAtual->ant = NULL;
            }

            // libera a memória alocada para a palavra removida e ajusta o contador
            free(palavraAtual);
            linhaAtual->numPalavras--;  // decrementa o número de palavras na linha
            palavraRemovida = 1; // marca que a palavra foi removida
            break; // sai do loop, pois a palavra foi removida
        }

        // atualiza os ponteiros para percorrer a lista de palavras
        anterior = palavraAtual;
        palavraAtual = palavraAtual->prox;
    }

    // verifica se a palavra foi removida e informa a remoção ou a ausência da palavra
    if (palavraRemovida) {
        printf("\nPalavra \"%s\" removida da linha %d, coluna %d\n", chave, linha, coluna);
    } else {
        printf("\nPalavra \"%s\" nao encontrada na posicao especificada (linha %d, coluna %d)\n",
               chave, linha, coluna);
        return; // se a palavra não foi encontrada, retorna
    }


    // Se a linha ficou realmente vazia após a remoção
    // if (linhaAtual->numPalavras == 0) {
    //     if (linhaAtual->ant == NULL) {
    //         // Removendo a primeira linha da lista de linhas
    //         lista->inicio = linhaAtual->prox;
    //         if (linhaAtual->prox != NULL) {
    //             linhaAtual->prox->ant = NULL;
    //         } else {
    //             lista->fim = NULL;  // A lista está agora completamente vazia
    //         }
    //     } else if (linhaAtual->prox == NULL) {
    //         // Removendo a última linha da lista de linhas
    //         linhaAtual->ant->prox = NULL;
    //         lista->fim = linhaAtual->ant;
    //     } else {
    //         // Removendo uma linha no meio da lista de linhas
    //         linhaAtual->ant->prox = linhaAtual->prox;
    //         linhaAtual->prox->ant = linhaAtual->ant;
    //     }

    //     free(linhaAtual);
    //     lista->numLinha--;
    // }

    //essa parte comentada do código é referente a tentativa de remover a linha
    //caso uma linha que continha uma unica palavra tivesse sua unica palavra removida.
    //como estava causando muitos erros no código (excluindo linhas quando nao deveria)
    //optei por tirar esta parte do código, por enquanto
}

int numeroOcorrencias(descLDE *lista, char *chave) {
    noLinha *linha = lista->inicio;  // inicializa o ponteiro para percorrer as linhas da lista
    int ocorrencias = 0;  // contador para armazenar o número de ocorrências da palavra

    // verifica se a lista ou o início da lista está vazio
    if (lista == NULL || lista->inicio == NULL) {
        printf("\nLista vazia!\n");
        return 0;  // retorna 0 se a lista estiver vazia
    }

    // percorre todas as linhas da lista principal
    while (linha != NULL) {
        noPalavra *palavra = linha->palavras;  // inicializa o ponteiro para a lista de palavras na linha atual

        // percorre todas as palavras da linha atual
        while (palavra != NULL) {
            // compara a palavra atual com a palavra chave
            if (strcmp(palavra->palavra, chave) == 0) {
                ocorrencias++;  // incrementa o contador se a palavra for encontrada
            }
            palavra = palavra->prox;  // avança para a próxima palavra
        }

        linha = linha->prox;  // avança para a próxima linha na lista principal
    }

    // imprime o número total de ocorrências da palavra no arquivo
    printf("\nA palavra '%s' aparece %i vezes no arquivo!\n", chave, ocorrencias);

    return ocorrencias;  // retorna o total de ocorrências encontradas
}


void edicaoPalavra(descLDE *lista, char *palavraNova, int indiceLinha, int indiceColuna) {
    if (lista == NULL || lista->inicio == NULL) {
        printf("\nA lista esta vazia, nao ha palavras para editar!\n");
        return;
    }

    noLinha *linha = lista->inicio;
    // chega até a linha correta
    for (int i = 1; i < indiceLinha && linha != NULL; i++) {
        linha = linha->prox;
    }

    if (linha == NULL) {
        printf("\nLinha %d nao encontrada!\n", indiceLinha);
        return;
    }

    noPalavra *palavraAtual = linha->palavras;
    int colunaAtual = 1;

    // percorre as palavras até encontrar aquela que contém a coluna desejada
    while (palavraAtual != NULL) {
        int tamanhoPalavra = strlen(palavraAtual->palavra);
        
        // verifica se a coluna está dentro desta palavra
        if (colunaAtual <= indiceColuna && indiceColuna < colunaAtual + tamanhoPalavra) {
            strcpy(palavraAtual->palavra, palavraNova);
            printf("\nPalavra editada com sucesso!\n");
            return;
        }
        
        // avança para a próxima palavra
        colunaAtual += tamanhoPalavra + 1; // +1 para contar o espaço após a palavra
        palavraAtual = palavraAtual->prox;
    }

    printf("\nColuna %d nao encontrada na linha %d!\n", indiceColuna, indiceLinha);
}

void limpaTela() {
    #ifdef _WIN32
        system("cls");  // Comando para Windows
    #else
        system("clear");  // Comando para Unix/Linux/MacOS
    #endif
}

void inserePalavraLista(descLDE *lista, char *palavra, int indiceLinha, int indiceColuna){
    if(lista == NULL){
        printf("\nLista nao instanciada!\n");
        return;
    }

    // se a lista está vazia e queremos inserir na primeira linha
    if(lista->inicio == NULL && indiceLinha == 1){
        noLinha *novaLinha = (noLinha*)malloc(sizeof(noLinha));
        if(novaLinha == NULL){
            printf("\nErro na alocacao de memoria!\n");
            return;
        }
        
        novaLinha->numPalavras = 0;
        novaLinha->ant = NULL;
        novaLinha->prox = NULL;
        novaLinha->palavras = NULL;
        lista->inicio = novaLinha;
        lista->fim = novaLinha;
        lista->numLinha = 1;
        
        noPalavra *novaPalavra = (noPalavra*)malloc(sizeof(noPalavra));
        if(novaPalavra == NULL){
            printf("\nErro na alocacao de memoria!\n");
            return;
        }
        
        strcpy(novaPalavra->palavra, palavra);
        novaPalavra->coluna = indiceColuna;
        novaPalavra->ant = NULL;
        novaPalavra->prox = NULL;
        novaLinha->palavras = novaPalavra;
        novaLinha->numPalavras = 1;
        printf("\nPalavra inserida com sucesso na nova linha!\n");
        return;
    }

    // encontra a linha onde a palavra será inserida
    noLinha *linhaAtual = lista->inicio;
    int numLinhaAtual = 1;

    while(linhaAtual != NULL && numLinhaAtual < indiceLinha){
        linhaAtual = linhaAtual->prox;
        numLinhaAtual++;
    }

    // se a linha não existe e não é a próxima após a última
    if(linhaAtual == NULL && numLinhaAtual < indiceLinha){
        printf("\nLinha invalida! So eh possivel inserir ate a linha %d\n", numLinhaAtual);
        return;
    }

    // se precisamos criar uma nova linha no final
    if(linhaAtual == NULL && numLinhaAtual == indiceLinha){
        noLinha *novaLinha = (noLinha*)malloc(sizeof(noLinha));
        if(novaLinha == NULL){
            printf("\nErro na alocacao de memoria!\n");
            return;
        }
        
        novaLinha->numPalavras = 0;
        novaLinha->ant = lista->fim;
        novaLinha->prox = NULL;
        novaLinha->palavras = NULL;
        lista->fim->prox = novaLinha;
        lista->fim = novaLinha;
        lista->numLinha++;
        linhaAtual = novaLinha;
    }

    // cria nova palavra
    noPalavra *novaPalavra = (noPalavra*)malloc(sizeof(noPalavra));
    if(novaPalavra == NULL){
        printf("\nErro na alocacao de memoria!\n");
        return;
    }
    
    strcpy(novaPalavra->palavra, palavra);
    novaPalavra->coluna = indiceColuna;
    
    // se é a primeira palavra da linha
    if(linhaAtual->palavras == NULL){
        novaPalavra->ant = NULL;
        novaPalavra->prox = NULL;
        linhaAtual->palavras = novaPalavra;
    }else{
        // encontra a posição correta para inserir a palavra
        noPalavra *palavraAtual = linhaAtual->palavras;
        noPalavra *anterior = NULL;
        
        while(palavraAtual != NULL && palavraAtual->coluna < indiceColuna){
            anterior = palavraAtual;
            palavraAtual = palavraAtual->prox;
        }
        
        // insere no início da linha
        if(anterior == NULL){
            novaPalavra->prox = linhaAtual->palavras;
            novaPalavra->ant = NULL;
            linhaAtual->palavras->ant = novaPalavra;
            linhaAtual->palavras = novaPalavra;
        }
        // insere no final da linha
        else if(palavraAtual == NULL){
            novaPalavra->prox = NULL;
            novaPalavra->ant = anterior;
            anterior->prox = novaPalavra;
        }
        // insere no meio da linha
        else{
            novaPalavra->prox = palavraAtual;
            novaPalavra->ant = anterior;
            anterior->prox = novaPalavra;
            palavraAtual->ant = novaPalavra;
        }
    }
    
    linhaAtual->numPalavras++;
    printf("\nPalavra inserida com sucesso!\n");
}

void pressEnter() {
    printf("\nPressione ENTER para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF); 
    getchar(); 
}

void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lerString(char *string, int tamanho) {
    limpaBuffer(); 
    printf("Digite a string: ");
    fgets(string, tamanho, stdin);
    if (string[strlen(string)-1] == '\n') {
        string[strlen(string)-1] = '\0';  
    }
}

void substring(descLDE *lista, char *substring) {
    // flag para controlar se a substring foi encontrada em alguma linha
    int encontrada = 0;
    noLinha *linha = lista->inicio;

    while (linha != NULL) {
        noPalavra *palavra = linha->palavras;
        // cria uma string vazia para armazenar todas as palavras da linha atual
        char stringLinha[1024] = "";

        // concatena todas as palavras da linha em uma única string
        while (palavra != NULL) {
            strcat(stringLinha, palavra->palavra);
            strcat(stringLinha, " "); // adiciona espaço entre as palavras
            palavra = palavra->prox;
        }

        // procura todas as ocorrências da substring na linha atual
        char *pos = strstr(stringLinha, substring);
        while (pos != NULL) {
            printf("\nSubstring '*%s*' encontrada na linha *%i*!\n", substring,  linha->numPalavras);
            encontrada = 1;
            // continua procurando a partir da posição seguinte à última ocorrência
            pos = strstr(pos + 1, substring);
        }

        // move para a próxima linha
        linha = linha->prox;
    }

    // se não encontrou em nenhuma linha, avisa o usuário
    if (encontrada == 0) {
        printf("\nSubstring nao encontrada no texto!\n");
    }
}

void atualizaArquivo(descLDE *lista, char *nomeArquivo) {
    // verifica se a lista está vazia antes de tentar salvar
    if (lista == NULL || lista->inicio == NULL) {
        printf("A lista está vazia, nada a ser salvo.\n");
        return;
    }

    // cria um arquivo temporário para escrita
    FILE *arquivoTemp = fopen("arqTemp.txt", "w");
    if (arquivoTemp == NULL) {
        printf("Erro ao abrir o arquivo temporário.\n");
        return;
    }

    // percorre cada linha da lista
    noLinha *linhaAtual = lista->inicio;
    while (linhaAtual != NULL) {
        // para cada linha, percorre todas as palavras
        noPalavra *palavraAtual = linhaAtual->palavras;
        while (palavraAtual != NULL) {
            // escreve cada palavra no arquivo seguida de um espaço
            fprintf(arquivoTemp, "*%s* ", palavraAtual->palavra);
            palavraAtual = palavraAtual->prox;
        }
        // após cada linha, insere uma quebra de linha no arquivo
        fprintf(arquivoTemp, "\n");
        linhaAtual = linhaAtual->prox;
    }

    fclose(arquivoTemp);

    // renomeia o arquivo original para backup
    if (rename(nomeArquivo, "arquivoOLD.txt") != 0) {
        printf("\nErro ao renomear o arquivo original.\n");
        return;
    }

    // renomeia o arquivo temporário para o nome original
    if (rename("arqTemp.txt", nomeArquivo) != 0) {
        printf("\nErro ao renomear o arquivo temporário.\n");
        return;
    }

    printf("\nArquivo atualizado com sucesso.\n");
}
