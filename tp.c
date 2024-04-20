#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp.h"

// Função utilizada para inverter o texto lido:

char *TextoInvertido(char *texto) {
    int i, j, tam;
    tam = strlen(texto);
    char *invertido = (char*) malloc((tam+1) * sizeof(char));

    j = tam - 1;
    for(i = 0; i < tam; i++) {
        invertido[j] = texto[i];
        j--;
    }
    invertido[i] = '\0';
    return invertido;
}

// Algoritmo de Força Bruta utilizado para a Primeira Estratégia:

int ForcaBruta(char* texto, char* padrao) {
    int n = strlen(texto); // tamanho do texto.
    int m = strlen(padrao); // tamanho do padrão.
    int i, j, k;
   
    // Se o tamanho do texto for maior ou igual ao do padrão, iremos procurar o padrão no texto:
    if(n >= m-1) {
        for(i = 1; i <= n; i++) {
            k = i;
            j = 1;
            while(texto[k-1] == padrao[j-1] && j <= m) {
                /* 
                    Se por acaso atingimos a última posição do texto e o padrão casar
                    até o momento, vamos para a próxima posição do padrão (como j <= m)
                    e retornamos para a primeira posição do texto:
                */ 
                if(k == n) {
                    j++;
                    k = 1;
                    continue;
                }
                j++;
                k++;
            }
            // Se todas as posições do padrão casaram, então j >= m:
            if(j >= m) {
                return i; // Retornamos em qual posição do texto inicia o casamento.
            }
        }
    }

    /* 
        Se o tamanho do texto for menor que o do padrão, o índice que caminha
        em cada posição do padrão, irá até <= ao tamanho do texto:
    */
    else {
        for(i = 1; i <= n; i++) {
            k = i;
            j = 1;
            while(texto[k-1] == padrao[j-1] && j <= n) {
                /* 
                    Se por acaso atingimos a última posição do texto e o padrão casava 
                    até o momento, vamos para a próxima posição do padrão (como j <= n)
                    e retornamos para a primeira posição do texto:
                */ 
                if(k == n) {
                    j++;
                    k = 1;
                    continue;
                }
                j++;
                k++;
            }
            // Se todas as posições do texto casaram, então j >= n:
            if(j >= n) {
                return i; // Retornamos em qual posição do padrão inicia o casamento.
            }
        }
    }
    // Se não houve casamento, a função retorna -1:
    return -1;
}

// Função utilizada para realizar o pré-processamento do padrão no Algoritmo BMH:

void PreProcessamento(int *d, int n, int m, char *texto, char *padrao) {
    // Inicialmente, toda a tabela de deslocamento é preenchida com o tamanho do padrão:
    for(int i = 1; i <= n; i++) {
        d[i-1] = m-1;
    }

    for(int i = 1; i <= n; i++) {
        int j = 1;
        // Verificamos se o caractere do texto se encontra no padrão:
        while(padrao[j-1] != texto[i-1] && j < m) {
            j++;
        }

        // Se esse caractere estiver no padrão então, j < m:
        if(j < m) {
            // Encontramos um k mínimo para esse caractere:
            for(int k = 1; k < m; k++) {
                if(padrao[m-k-2] == padrao[j-1]) {
                    d[i-1] = k; 
                    break;
                }
            }
        }
    }
}

// Algoritmo Boyer-Moore-Horspool utilizado para a Segunda Estratégia:

int BMH(char *texto, char *padrao) {
    int n = strlen(texto);
    int m = strlen(padrao);
    int *d = (int*) malloc(n * sizeof(int));
    PreProcessamento(d, n, m, texto, padrao);
    int i, j, k;

    // Utilizando o BMH se o tamanho do texto for maior ou igual ao do padrão:
    if(n >= m-1) {
        i = m-1; 
        // Enquanto o índice i não ultrapassar o tamanho do texto:
        while(i <= n) {
            k = i;
            j = m-1;
            // Enquanto houver casamento entre o texto e padrão, subtraímos ambas os índices:
            while(texto[k-1] == padrao[j-1] && j > 0) {
                k--;
                j--;
            }

            // Se verificamos todo o padrão (j = 0), então houve casamento exato:
            if(j == 0) {
                return k;
            }
            
            /* 
                Se ainda não houve casamento, o valor do índice é incrementado com o valor 
                da tabela de deslocamento no qual houve colisão:
            */
            i += d[k-1];

            /*  Se o valor do índice i ultrapassou o tamanho do texto, trataremos uma 
                outra possível condição de haver casamento, já que o texto é circular:
            */ 
            if(i > n) {
                while(1) {
                    /*  A variável dif abaixo, representa o número de posições do padrão
                        que ainda estão alinhadas com o texto:
                    */
                    int dif = i-n;
                    dif = m-1-dif;
                    k = n; // O índice k é a última posição do texto. 
                    j = dif; // O índice j é a última posição ainda alinhada com o texto.

                    // Enquanto houver casamento exato, subtraímos ambas os índices:
                    while(texto[k-1] == padrao[j-1] && j > 0) {
                        k--;
                        j--;
                    }
                    
                    /*
                        Se j atingir o valor 0, significa que a parte do padrão que ainda
                        estava alinhada com o texto casou. Dessa maneira, iremos verificar 
                        se o restante do padrão que agora está alinhado com as primeiras 
                        posições do texto, casa com essas posição:
                    */
                    if(j == 0) {
                        int pos1 = 0; // índice que "moverá" sobre o texto.
                        int pos2 = dif; // índice que "moverá" sobre o padrão.
                        // Enquanto houver casamento exato, incrementamos ambos os índices:
                        while(texto[pos1] == padrao[pos2] && pos2 < m-1) {
                            pos1++;
                            pos2++;
                        }

                        // Se a variável pos2 for igual ao tamanho do padrão, houve casamento:
                        if(pos2 == m-1) {
                            return k;
                        }
                        // Caso contrário, não houve casamento:  
                        else {
                            return -1;
                        }
                    }
                    i++; // Se não houve casamento, o valor de i é incrementado 1 unidade.
                }
            }
        }
    }

    // Utilizando o BMH se o tamanho do padrão for maior que o do texto:
    else {
        i = n;
        // Enquanto o índice i não ultrapassar o tamanho do padrão:
        while(i < m) {
            k = n;
            j = i;
            // Enquanto houver casamento entre o texto e padrão, subtraímos ambas os índices:
            while(texto[k-1] == padrao[j-1] && j > 0) {
                k--;
                j--;
            }

            // Se verificamos todo o padrão (j = 0), então houve casamento exato:
            if(j == 0) {
                return k;
            }

            /* 
                Se ainda não houve casamento, o valor do índice é incrementado com o valor 
                da tabela de deslocamento no qual houve colisão:
            */
            i += d[k-1];

            /*  Se o valor do índice i ultrapassou o tamanho do padrão, trataremos uma 
                outra possível condição de haver casamento, já que o texto é circular:
            */ 
            if(i >= m) {
                while(1) {
                    /*  A variável dif abaixo, representa o número de posições do padrão
                        que ainda estão alinhadas com o texto:
                    */
                    int dif = i-n;
                    dif = m-1-dif;
                    k = n; // O índice k é a última posição do texto.
                    j = dif; // o índice j é a última posição ainda alinhada com o texto.

                    // Enquanto houver casamento exato, subtraímos ambas os índices:
                    while(texto[k-1] == padrao[j-1] && j > 0) {
                        k--;
                        j--;
                    }

                    /*
                        Se j atingir o valor 0, significa que a parte do padrão que ainda
                        estava alinhada com o texto casou. Dessa maneira, iremos verificar 
                        se o restante do padrão que agora está alinhado com as primeiras 
                        posições do texto, casa com essas posição:
                    */
                    if(j == 0) {
                        int pos1 = 0; // índice que "moverá" sobre o texto.
                        int pos2 = dif;  // índice que "moverá" sobre o padrão.
                        // Enquanto houver casamento exato, incrementamos ambos os índices:
                        while(texto[pos1] == padrao[pos2] && pos2 < n) {
                            pos1++;
                            pos2++;
                        }

                        // Se a variável pos2 for igual ao tamanho do texto, houve casamento:
                        if(pos2 == n) {
                            return k;
                        }
                        // Caso contrário, não houve casamento:
                        else {
                            return -1;
                        }
                    }

                    i++; // Se não houve casamento, o valor de i é incrementado 1 unidade.
                }
            }
        }
    }

    free(d);
    return -1;
}

// Algoritmo Shift-And utilizado para a Terceira Estratégia:

int ShiftAnd(char *texto, char *padrao) {
    int n = strlen(texto);
    int m = strlen(padrao);
    long M[256], i, R;

    // Inicialmente, todos as posições da tabela M que armazena uma máscara de bits recebem 0:
    for(i = 0; i < 256; i++) {
        M[i] = 0;
    }

    // Se o tamanho do texto for maior ou igual que o do padrão:
    if(n >= m-1) {

        /* 
            Realizamos o pré-processamento de cada caractere do padrão, isto é, obtemos a máscara de bits para cada caractere do padrão:
        */
        for(i = 1; i < m; i++) {
            M[padrao[i-1] + 127] |= 1 << (m - 1 - i);
        }

        R = 0; // O valor de R inicia em 0.

        // Realizamos a pesquisa sobre o texto: 
        for(i = 0; i < n; i++) {
            /* 
                Para cada novo caractere lido do texto, o valor de R é atualizado para ((R >> 1) | 10^m-1) & M[T[i]]:
            */
            R = ((((unsigned long)R) >> 1) | (1 << (m - 2))) & M[texto[i] + 127];

            // Se R and 1 for diferente de 0, significa que ocorreu um casamento exato:
            if((R & 1) != 0) {
                return i-m+3;
            }
        } 

        return -1; // Se não houve casamento, a função retorna -1.
    }

    // Se o tamanho do padrão for maior que o do texto:
    else {
        /* 
            Realizamos o pré-processamento de cada caractere do padrão, isto é, obtemos a máscara de bits para cada caractere do padrão:
        */
        for(i = 1; i <= n; i++) {
            M[padrao[i-1] + 127] |= 1 << (n - i);
        }

        R = 0; // O valor de R inicia em 0.

        // Realizamos a pesquisa até o tamanho máximo do padrão: 
        for(i = 0; i < m-1; i++) {
            /* 
                Para cada novo caractere lido do texto, o valor de R é atualizado para ((R >> 1) | 10^m-1) & M[T[i]].
            */
            R = ((((unsigned long)R) >> 1) | (1 << (n - 1))) & M[texto[i] + 127];

            // Se R and 1 for diferente de 0, significa que ocorreu um casamento exato:
            if((R & 1) != 0) {
                return i-n+2;
            }
        }

        return -1; // Se não houve casamento, a função retorna -1.
    }
}

/*
    Função utilizada para salvar o arquivo de saída com os dados obtidos. 
    O arquivo de saída tem o mesmo nome do arquivo de entrada porém com a 
    extensão out:
*/

void SalvarOutput(char *input, int *resultados, int t) {
    FILE *saida;
    int tam = strlen(input);
    char *output = (char*) malloc((tam+3) * sizeof(char));
    char *extensao = "out";
    int i = 0, j = 0;
    
    for(i = 0; i < tam-4; i++) {
        output[i] = input[i];
    }

    for(i = tam-4; j < 3; i++, j++) {
        output[i] = extensao[j];
    }

    for(i = tam-1, j = tam-4; i < tam+3; i++, j++) {
        output[i] = input[j];
    }

    saida = fopen(output, "w");

    if(saida == NULL) {
        printf("Houve um erro ao criar o arquivo de saída!\n");
        exit(1);
    }
    
    for(int i = 0; i < t; i++) {
        if(resultados[i] == -1) {
            fprintf(saida, "N\n");
        }
        else {
            fprintf(saida, "S %d\n", resultados[i]);
        }
    }

    fclose(saida);
    free(output);
}

// Função utilizada para a leitura do arquivo de entrada fornecido:

void LerArquivo(char *input, int opt) {
    FILE *entrada;
    entrada = fopen(input, "r");

    // Verificando se o arquivo de entrada fornecido foi encontrado:
    if(entrada == NULL) {
        printf("Arquivo de entrada não encontrado!\n");
        exit(1); 
    }

    // Leitura do número de casos de testes:
    int t;
    fscanf(entrada, "%d\n", &t);
    printf("%d\n", t);

    int pos = 0;
    int *resultados = (int*) malloc(t * sizeof(int));

    char c; // Variável utilizada para ler cada caractere do arquivo de entrada.
    
    // Inicialmente, tanto o tamanho do padrão quanto o tamanho do texto é 1:
    int tampadrao = 1;
    int tamtexto = 1;

    /* 
        Alocando duas strings dinamicamente, uma para o padrão e outra para o texto.
        Inicialmente, ambas possuem tamanho 1:
    */

    char *padrao = (char*) malloc(tampadrao * sizeof(char));
    char *texto = (char*) malloc(tamtexto * sizeof(char));
    char *texto_invertido;
    int normal, invertido;

    // Verificando se a memória foi alocada corretamente:
    if(padrao == NULL || texto == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    // Laço utilizado para cada caso de teste:
    for(int i = 1; i <= t; i++) {
        // A variável c receberá cada caractere do arquivo de entrada até EOF (fim do arquivo):
        while((c = getc(entrada)) != EOF) {
            padrao[tampadrao-1] = c; // O primeiro caractere é do padrão.
            tampadrao++; // O tamanho do padrão é incrementado.
            /* 
                Fazemos um realloc na string padrão pois, pode ser que o próximo caractere
                também seja do padrão:
            */
            padrao = (char*) realloc(padrao, tampadrao * sizeof(char));

            // Se o caractere for um espaço em branco, então começaremos a ler o texto:
            if(c == ' ') {
                padrao[tampadrao-1] = '\0';
                // Leremos cada caractere individualmente do texto:
                while(((c = getc(entrada)) != '\n') && (c != EOF)) {
                    texto[tamtexto-1] = c;
                    tamtexto++; // O tamanho do texto é incrementado.
                    /* 
                        Fazemos um realloc na string texto pois, precisaremos de um tamanho maior para o texto:
                    */
                    texto = (char*) realloc(texto, tamtexto * sizeof(char));
                }
                    
                /* 
                    Ao fim do while mais interno, terminamos de ler o padrão e o texto
                    para o caso de texto i.
                */
                texto[tamtexto-1] = '\0';

                // Se a estratégia 1 foi selecionada:
                if(opt == 1) {
                    normal = ForcaBruta(texto, padrao);
                    texto_invertido = TextoInvertido(texto);
                    invertido = ForcaBruta(texto_invertido, padrao);

                    // Se houve casamento tanto no texto normal quanto no invertido:
                    if(normal != -1 && invertido != -1) {
                        // Se o casamento no texto normal ocorreu antes ou na mesma posição:
                        if(normal <= tamtexto - invertido) {
                            resultados[pos] = normal;
                            pos++;
                            printf("S %d\n", normal);
                        }
                        // Se o casamento no texto invertido ocorreu antes:
                        else {
                            resultados[pos] = tamtexto - invertido;
                            pos++;
                            printf("S %d\n", tamtexto - invertido);
                        }
                    }
                    // Se houve casamento apenas no texto normal:
                    else if (normal != -1) {
                            resultados[pos] = normal;
                            pos++;
                            printf("S %d\n", normal);
                    }
                    // Se houve casamento apenas no texto invertido:
                    else if(invertido != -1) {
                        resultados[pos] = tamtexto - invertido;
                        pos++;
                        printf("S %d\n", tamtexto - invertido);
                    }
                    // Se não houve casamento em nenhum dos dois casos:
                    else {
                        resultados[pos] = -1;
                        pos++;
                        printf("N\n");
                    }

                    free(texto_invertido);
                }

                // Se a estratégia 2 foi selecionada:
                else if(opt == 2) {
                    normal = BMH(texto, padrao);
                    texto_invertido = TextoInvertido(texto);
                    invertido = BMH(texto_invertido, padrao);

                    // Se houve casamento tanto no texto normal quanto no invertido:
                    if(normal != -1 && invertido != -1) {
                        // Se o casamento no texto normal ocorreu antes ou na mesma posição:
                        if(normal+1 <= tamtexto-invertido-1) {
                            resultados[pos] = normal+1;
                            pos++;
                            printf("S %d\n", normal+1);
                        }
                        // Se o casamento no texto invertido ocorreu antes:
                        else {
                            resultados[pos] = tamtexto-invertido-1;
                            pos++;
                            printf("S %d\n", tamtexto-invertido-1);
                        }
                    }
                    // Se houve casamento apenas no texto normal:
                    else if(normal != -1) {
                        resultados[pos] = normal+1;
                        pos++;
                        printf("S %d\n", normal+1);
                    }
                    // Se houve casamento apenas no texto invertido:
                    else if(invertido != -1) {
                        resultados[pos] = tamtexto-invertido-1;
                        pos++;
                        printf("S %d\n", tamtexto-invertido-1);
                    }
                    // Se não houve casamento em nenhum dos dois casos:
                    else {
                        resultados[pos] = -1;
                        pos++;
                        printf("N\n");
                    }

                    free(texto_invertido);
                }
                
                // Se a estratégia 3 foi selecionada:
                else {
                    normal = ShiftAnd(texto, padrao);
                    texto_invertido = TextoInvertido(texto);
                    invertido = ShiftAnd(texto_invertido, padrao);

                    // Se houve casamento tanto no texto normal quanto no invertido:
                    if(normal != -1 && invertido != -1) {
                        // Se o casamento no texto normal ocorreu antes ou na mesma posição:
                        if(normal <= tamtexto-invertido) {
                            resultados[pos] = normal;
                            pos++;
                            printf("S %d\n", normal);
                        }
                        // Se o casamento no texto invertido ocorreu antes:
                        else {
                            resultados[pos] = tamtexto - invertido;
                            pos++;
                            printf("S %d\n", tamtexto - invertido);
                        }
                    }
                    // Se houve casamento apenas no texto normal:
                    else if(normal != -1) {
                        resultados[pos] = normal;
                        pos++;
                        printf("S %d\n", normal);
                    }
                    // Se houve casamento apenas no texto invertido:
                    else if(invertido != -1) {
                        resultados[pos] = tamtexto - invertido;
                        pos++;
                        printf("S %d\n", tamtexto - invertido);
                    }
                    // Se não houve casamento em nenhum dos dois casos:
                    else {
                        resultados[pos] = -1;
                        pos++;
                        printf("N\n");
                    }
                    
                    free(texto_invertido);
                }

                tampadrao = 1; // O tamanho do padrão para o próximo caso de teste é 1.
                tamtexto = 1; // O tamanho do texto para o próximo caso de teste é 1.

                // Realocamos as strings novamente para que elas retornem ao tamanho inicial:
                padrao = (char*) realloc(padrao, tampadrao * sizeof(char));
                texto = (char*) realloc(texto, tamtexto * sizeof(char));    
            }
        }
    }
    
    // Liberando a memória que foi alocada dinamicamente:
    free(padrao);
    free(texto);
    
    // Criando o arquivo de saída e liberando a variável utilizada para armazenar os resultados:
    SalvarOutput(input, resultados, t);
    free(resultados);

    // Fechando o arquivo de entrada:
    fclose(entrada);
}