#include <stdio.h>
#include <stdlib.h>
#include "structs.h"

int main() {

    descLDE lista;
    lista.inicio = NULL;
    lista.fim = NULL;
    int linha = 1;
    int coluna = 1;
    //linhas e coluna indexadas em 1, como estava no enunciadoe representacao grafica

    char *nomeArquivo = "arquivo.txt";
    abreArquivo(&lista, nomeArquivo);

    int opcao = 0;

    while(opcao != 10){

    printf("\nOla!\n");
    printf("Escolha uma das seguintes opcoes:\n");
    printf("1 - Leitura/Exibicao de texto\n");
    printf("2 - Busca de uma palavra\n");
    printf("3 - Remocao de palavra\n");
    printf("4 - Remocao de palavra na linha e coluna especificadas\n");
    printf("5 - Numero de ocorrencias de uma palavra\n");
    printf("6 - Numero total de palavras\n");
    printf("7 - Edicao de palavra\n");
    printf("8 - Insercao de palavra\n");
    printf("9 - Exibicao de linha que contem a substring\n");
    printf("10 - Sair\n");

    scanf("%i", &opcao);
    limpaTela();

        switch (opcao)
        {
        case 1:
            exibeLista(&lista);
            pressEnter();
            limpaTela();
            
            break;

        case 2:
            printf("Insira a palavra que deseja buscar:\n");
            char chaveBusca[30];
            lerString(chaveBusca, sizeof(chaveBusca));
            buscaPalavra(&lista, chaveBusca);
            pressEnter();
            limpaTela();
            
            break;

        case 3:
            printf("Insira a palavra que deseja remover:\n");
            char chaveRemocao[30];
            lerString(chaveRemocao, sizeof(chaveRemocao));
            removePalavra(&lista, chaveRemocao);
            atualizaArquivo(&lista, "arquivo.txt");
            pressEnter();
            limpaTela();
            
            break;

        case 4:
            printf("Insira a palavra que deseja remover:\n");
            char chaveRemocaoPosicao[30];
            lerString(chaveRemocaoPosicao, sizeof(chaveRemocaoPosicao));
            printf("\nAgora a linha e coluna:\n");
            int linhaRemocaoPosicao, colunaRemocaoPosicao;
            scanf("%i %i", &linhaRemocaoPosicao, &colunaRemocaoPosicao);
            removePalavraPosicao(&lista, chaveRemocaoPosicao, linhaRemocaoPosicao, colunaRemocaoPosicao);
            atualizaArquivo(&lista, "arquivo.txt");
            pressEnter();
            limpaTela();
            
            break;

        case 5:
            printf("Insira a palavra que deseja saber a ocorrencia:\n");
            char chaveOcorrencia[30];
            lerString(chaveOcorrencia, sizeof(chaveOcorrencia));
            numeroOcorrencias(&lista, chaveOcorrencia);
            pressEnter();
            limpaTela();
            
            break;

        case 6:
            contaPalavras(&lista);
            pressEnter();
            limpaTela();
            
            break;

        case 7:
            printf("Insira a palavra modificada:\n");
            char palavraNovaEdicao[30];
            lerString(palavraNovaEdicao, sizeof(palavraNovaEdicao));
            printf("\nInsira o indice da linha e da coluna da palavra que deseja editar:\n");
            int indiceLinhaEdicao, indiceColunaEdicao;
            scanf("%i %i", &indiceLinhaEdicao, &indiceColunaEdicao);
            edicaoPalavra(&lista, palavraNovaEdicao, indiceLinhaEdicao, indiceColunaEdicao);
            atualizaArquivo(&lista, "arquivo.txt");
            pressEnter();
            limpaTela();
            
            break;

        case 8:
            printf("Insira a palavra que deseja inserir na lista?\n");
            char palavraNovaInsercao[30];
            lerString(palavraNovaInsercao, sizeof(palavraNovaInsercao));
            printf("\nInsira o indice da linha e coluna onde a palavra deve ser inserida, respectivamente?\n");
            int indiceLinhaInsercao, indiceColunaInsercao;
            scanf("%i %i", &indiceLinhaInsercao, &indiceColunaInsercao);
            inserePalavraLista(&lista, palavraNovaInsercao, indiceLinhaInsercao, indiceColunaInsercao);
            atualizaArquivo(&lista, "arquivo.txt");
            pressEnter();
            limpaTela();
            
            break;

        case 9:
            printf("Insira a substring que deseja buscar no arquivo:\n");
            char substringUsuario[60];
            lerString(substringUsuario, sizeof(substringUsuario));
            substring(&lista, substringUsuario);
            pressEnter();
            limpaTela();
            
            break;
        
        case 10:
            limpaTela();
            printf("\nSaindo...\n");
            pressEnter();
            limpaTela();
            break;

        default:
            printf("\nErro! Opcao invalida.\n");
            pressEnter();
            break;
        }
    }

    return 0;
}
