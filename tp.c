#include <stdio.h>
#include <stdlib.h>
#include "tp.h"

/* 
    Função utilizada para alocar o grid S dinamicamente com R linhas e C colunas
    para cada cada teste:
*/

void AlocaGrid(int ***s, int *r, int *c) {
    *s = (int**) malloc(((*r)+1) * sizeof(int*));
        for(int i = 1; i <= *r; i++) {
            (*s)[i] = (int*) malloc(((*c)+1) * sizeof(int));
        }
    
    // Verificando se a memória foi alocada corretamente:
    if(s == NULL) {
        printf("A memoria nao foi alocada corretamente!\n");
        exit(1);
    }
}

/*
    Função utilizada para caminhar para baixo ou para direita. Essa função
    seleciona o caminho no qual há o maior ganho de energia ou a menor perda 
    de energia:
*/

void Seleciona(int **s, int **energia, int *r, int *c, int **i, int **j) {
    // Verifica se é possível caminhar tanto para baixo quanto para a direita:
    if(**i < *r && **j < *c) {
        /* 
            Escolhe a posição em que há o maior ganho de energia ou a menor perda.
            A primeira condição verifica se caminhar para baixo é mais vantajoso
            do que caminhar para a direita:
        */  
        if(s[(**i)+1][**j] > s[**i][(**j)+1]) { 
            **energia += s[(**i)+1][**j]; // Atualiza a energia de Harry.
            (**i)++; // Atualiza a posição no Grid S[i][j] para S[i+1][j].
        }
        
        // Verificando se caminhar para a direita será mais vantajoso:
        else if(s[(**i)+1][**j] < s[**i][(**j)+1]) {
            **energia += s[**i][(**j)+1]; // Atualiza a energia de Harry.
            (**j)++; // Atualiza a posição no Grid S[i][j] para S[i][j+1].
        }

        // Se em ambas as posições os valores forem iguais, então:
        else {
            /* 
                Verificamos se, após caminharmos para baixo, se o próximo
                valor da direita é mais vantajoso do que o valor da direita
                se tivessemos caminhado para o lado anteriormente, ou se
                são iguais novamente (neste caso, vamos para baixo mesmo):
            */
            if(s[(**i)+1][(**j)+1] >= s[(**i)][(**j)+2]) {
                **energia += s[(**i)+1][**j]; // Atualiza a energia de Harry.
                (**i)++; // Atualiza a posição no Grid S[i][j] para S[i+1][j]. 
            }

            // Se a condição anterior for falsa, caminhamos para a direita:
            else {
                **energia += s[**i][(**j)+1]; // Atualiza a energia de Harry. 
                (**j)++; // Atualiza a posição no Grid S[i][j] para S[i][j+1].
            }
        }
    }

    /* 
        As duas condições abaixo serão realizadas caso nos encontrarmos em uma posição
        no grid na qual não seja possível caminhar para os dois lados, ou seja,
        ou atingimos a última linha ou a última coluna. Entraremos na primeira
        condição se caso estivermos na última linha do grid S. Assim, não temos 
        outra opção a não ser caminhar para a direita:
    */

    else if(**i == *r) { //
        **energia += s[**i][(**j)+1]; // Atualiza a energia de Harry.
        (**j)++; // Atualiza a posição no Grid S[i][j] para S[i][j+1].
    }

    // A segunda condição será realizada se caso estivermos na última coluna:
    else {
        **energia += s[(**i)+1][**j]; // Atualiza a energia de Harry.
        (**i)++; // Atualiza a posição no Grid S[i][j] para S[i+1][j].
    }
}

/*
    A primeira estratégia utilizada é uma abordagem gulosa e utiliza duas funções:
    a função Estrategia1, mostrada a seguir, e a função Seleciona:
*/

int Estrategia1(int **s, int *r, int *c) {
    int min = 1; // O mínimo de energia sempre começa com 1.
    int *energia = (int*) malloc(sizeof(int));
    int *i = (int*) malloc(sizeof(int));
    int *j = (int*) malloc(sizeof(int));

    // Verificando se a memória foi alocada corretamente:
    if(energia == NULL || i == NULL || j == NULL) {
        printf("A memoria nao foi alocada corretamente!\n");
        exit(1);
    }

    *energia = 1; // A energia atual também irá iniciar com o valor 1.
    *i = 1;
    *j = 1;

    // O laço a seguir será feito enquanto não alcançarmos a posição S[R][C]:
    do {
        // Caminha para baixo ou para a direita de acordo com o critério 
        // especificado na função Seleciona:
        Seleciona(s, &energia, r, c, &i, &j);
        /*
            Se a energia atual de Harry, modificada pela função Seleciona, for menor
            ou igual ao valor 0, a energia mínima necessária na posição S[1][1] será
            incrementada em 1 unidade. A energia atual de Harry no início do percurso
            será igual à energia mínima e voltaremos para posição 1x1:
        */ 
        if(*energia <= 0) {
            min++; 
            *energia = min;
            *i = 1;
            *j = 1;
        }
    }while(*i != *r || *j != *c);

    // Libera a memória alocada dinamicamente:
    free(energia);
    free(i);
    free(j);

    printf("%d\n", min);
    return min;
}

/* 
    Função utilizada para liberar o grid S alocado previamente com, R linhas e C colunas,
    para cada caso de teste:
*/

void LiberaGrid(int **s, int *r) {
    for(int i = 1; i <= *r; i++) {
            free(s[i]); // Libera cada uma das linhas do grid S
    }
    free(s); // Libera o array que guardava o endereço das linhas
}

/*
    A segunda estratégia é uma implementação utilizando Programação Dinâmica 
    e descrita na função Estrategia2:
*/

int Estrategia2(int **s, int *r, int *c) {
    // Criamos uma matriz auxiliar mat[R][C] que irá armazenar o mínimo de energia
    // necessária que Harry precisa ter para estar em cada posição do Grid S:
    int **mat; 
    
    // Utilizando a função AlocaGrid para alocar também uma matriz auxiliar de dimensões RxC:
    AlocaGrid(&mat, r, c);
    int cima, esquerda; // Variáveis utilizadas para armazenar o valor de mat[i-1][j] e mat[i][j-1]
 
    // O laço a seguir será feito partindo da posição mat[1][1] até a posição mat[R][C]
    for(int i = 1; i <= *r; i++) {
        for(int j = 1; j <= *c; j++) {
            // Se estivermos na primeira posição, então o mínimo de energia que Harry precisa é 1:
            if(i == 1 && j == 1) {
                mat[i][j] = 1;
                continue; // Iremos para próxima iteração, isto é, mat[i][j+1]
            }

            // Condição realizada se caso não estivermos nem na primeira linha e nem na 
            // primeira coluna. Desse modo, será possível comparar a posição atual na matriz
            // auxiliar com as posições tanto de cima quanto da esquerda da matriz:
            if(i > 1 && j > 1) { 
                cima = mat[i-1][j];
                esquerda = mat[i][j-1];
                // Se o mínimo de energia de cima for menor ou igual ao mínimo de energia
                // da esquerda:
                if(cima <= esquerda) {
                    // Se há uma poção na posição S[i][j] do Grid então, não é necessário
                    // aumentar a energia mínima em mat[i][j]:
                    if(s[i][j] > 0) {
                        mat[i][j] = cima;
                    }
                    // Por outro lado, se há um monstro na célula S[i][j] do Grid então,
                    // é preciso aumentar a energia mínima em mat[i][j]:  
                    else { 
                        mat[i][j] = s[i][j] + cima;
                    }
                }
                
                // Condição realizada se caso o mínimo de energia da esquerda for menor
                // que o mínimo de energia de cima:
                else {
                    // Se há uma poção na posição S[i][j] do Grid então, não é necessário
                    // aumentar a energia mínima em mat[i][j]:
                    if(s[i][j] > 0) {
                        mat[i][j] = esquerda;
                    }
                    // Por outro lado, se há um monstro na célula S[i][j] do Grid então,
                    // é preciso aumentar a energia mínima em mat[i][j]:
                    else {
                        mat[i][j] = s[i][j] + esquerda;
                    }
                }
            }

            // Condição realizada se caso estivermos na primeira linha e em uma coluna diferente
            // da primeira. Nesse caso, só podemos comparar a posição atual na matriz
            // auxiliar com a posição da esquerda da matriz: 
            else if(i == 1) { 
                esquerda = mat[i][j-1];
                // Se há uma poção na posição S[i][j] do Grid então, não é necessário
                // aumentar a energia mínima em mat[i][j]:
                if(s[i][j] > 0) {
                    mat[i][j] = esquerda;
                }
                // Por outro lado, se há um monstro na célula S[i][j] do Grid então,
                // é preciso aumentar a energia mínima em mat[i][j]:
                else {
                    mat[i][j] = s[i][j] + esquerda;
                }
            }

            // Condição realizada se caso estivermos na primeira coluna e em uma linha diferente
            // da primeira. Nesse caso, só podemos comparar a posição atual na matriz
            // auxiliar com a posição de cima da matriz: 
            else {
                cima = mat[i-1][j];
                // Se há uma poção na posição S[i][j] do Grid então, não é necessário
                // aumentar a energia mínima em mat[i][j]:
                if(s[i][j] > 0) {
                    mat[i][j] = cima;
                }
                // Por outro lado, se há um monstro na célula S[i][j] do Grid então,
                // é preciso aumentar a energia mínima em mat[i][j]:
                else {
                    mat[i][j] = s[i][j] + cima;
                }
            }

            // Se a energia mínima em mat[i][j] for menor ou igual 0, significa que a 
            // energia mínima na verdade é  ( mat[i][j] x -1 ) + 1, isto é, o mínimo
            // para que Harry não morra:
            if(mat[i][j] <= 0) {
                mat[i][j] = abs(mat[i][j]) + 1;
            }
        }
    }

    // O valor final estará guardado na célular mat[R][C]:
    int min = mat[*r][*c];
    printf("%d\n", min);

    // Utilizando a função LiberaGrid para liberar a matriz alocada dinamicamente:
    LiberaGrid(mat, r);

    return min;
}

/*
    Função utilizada para salvar o arquivo de saída saida.txt com o resultado de 
    cada caso de teste:
*/

void SalvarOutput(int *resultados, int *t) {
    FILE *saida;
    saida = fopen("saida.txt", "w");

    if(saida == NULL) {
        printf("Houve um erro ao criar o arquivo de saída!\n");
        exit(1);
    }
    
    for(int i = 0; i < *t; i++) {
        fprintf(saida, "%d\n", resultados[i]);
    }
    fclose(saida);
}

/*
    Função utilizada para realizar a leitura do arquivo de entrada que contém
    todos os dados necessários para a execução correta do programa:
*/

void LerArquivo(char *input, int opt) {
    FILE *entrada;
    entrada = fopen(input, "r");

    if(entrada == NULL) {
        printf("Arquivo de entrada não encontrado!\n");
        exit(1);
    }

    // Leitura do número de casos de testes:
    int *t = (int*) malloc(sizeof(int));

    // Verificando se a memória foi alocada corretamente:
    if(t == NULL) {
        printf("A memoria nao foi alocada corretamente!\n");
        exit(1);
    }
    fscanf(entrada, "%d", &(*t));

    // Variável para armazenar o número de linhas em cada teste.
    int *r = (int*) malloc(sizeof(int));
    // Variável para armazenar o número de colunas em cada teste.
    int *c = (int*) malloc(sizeof(int)); 
    int **s;

    // Verificando se a memória foi alocada corretamente:
    if(r == NULL || c == NULL) {
        printf("A memoria nao foi alocada corretamente!\n");
        exit(1);
    }

    int *resultados = (int*) malloc((*t) * sizeof(int));
    for(int cont = 0; cont < *t; cont++) {
        // Lendo a quantidade de linhas e colunas que o teste na posição cont possui:
        fscanf(entrada, "%d %d", &(*r), &(*c));

        AlocaGrid(&s, r, c);
        for(int i = 1; i <= *r; i++) {
            for(int j = 1; j <= *c; j++) {
                fscanf(entrada, "%d", &s[i][j]);
            }
        }

        // Se a estratégia 1 foi selecionada:
        if(opt == 1) {
            resultados[cont] = Estrategia1(s, r, c);
        }

        // Se a estratégia 1 não foi selecionada então, será utilizada a estratégia 2:
        else {
            resultados[cont] = Estrategia2(s, r, c);
        }

        // Liberando o Grid S alocado dinamicamente:
        LiberaGrid(s, r);
    }
    SalvarOutput(resultados, t);

    // Liberando as variáveis t, r, c e resultados que foram previamente alocadas:
    free(t);
    free(r);
    free(c);
    free(resultados);
    // Fechando o arquivo de entrada:
    fclose(entrada);
}
