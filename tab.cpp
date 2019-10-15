#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

#include <string>


// #include <sstream> 
using namespace std;

// Quicksort
int particiona(int *v, int beg, int end, int pivo);
void quickSort2(int *v, int beg, int end);
void quickSort(int *v, int n);


struct structLinhas{
    char colunaChave[50];
    double valor = 0;
};

int main(int argc, char *argv[]){
    bool DEBUG_MODE = true;

    // Checar se o numero de argumento esta correto
    if (argc < 4){
        cout << "Argumentos Insuficientes!\n";
        return -1;
    }
    if (argc > 5){
        cout << "Argumentos em Excesso!\n";
        return -1;
    }

    // Leituras da linha de comando
    int maxLinhas = atoi(argv[2]);
    char chaveAgregacao[50];
    strcpy (chaveAgregacao, argv[3]);
    int tamanhoChave = strlen(argv[3]);
    char colunaCalculo[50];
    strcpy (colunaCalculo, argv[4]);


    int posicaoChave = 0, posicaoValor = 0; //Guarda a posicao do chave e da coluna de calculo
    int contadorLinhas = 0, contadorParametros = 0, numIteracoes = 0; // Contadores
    int nArquivoSaida = 1; //Numero do arquivo de saida

    bool primeira = false;

    // Variavel de leitura
    string linha;

    // Abertura do arquivo
    ifstream arquivo(argv[1]);
    
    // Verifica chave e quantidade de colunas

    char chave[50];
    // char *chave = new char[linha.length()+1];
    getline(arquivo, linha, '\n');
    strcpy(chave, linha.c_str());

    // Contagem do numero de colunas
    int numeroColunas = 1;
    for (int i = 0; chave[i] != '\0'; i++){
        if (chave[i] == ',') numeroColunas++;
    }

    // Volta para o inicio do arquivo apos ler o numero de colunas
    arquivo.seekg (0, arquivo.beg);

    while(numIteracoes < numeroColunas){
        getline(arquivo, linha, ',');
        strcpy(chave, linha.c_str());
        // Determinar qual a coluna da chave
        if (strcmp(chave, chaveAgregacao) == 0) posicaoChave = numIteracoes;
        if (strcmp(chave, colunaCalculo) == 0) posicaoValor = numIteracoes;
        numIteracoes++;
    }
    // Volta para o inicio do arquivo apos ler o numero de colunas
    arquivo.seekg (0, arquivo.beg);
    numIteracoes = 0;

    char arqSaida[20];

    structLinhas Linhas[maxLinhas+1];

    while (arquivo.peek() != EOF){
        
        sprintf(arqSaida, "%d.txt", nArquivoSaida);
        
        ofstream saida(arqSaida);

        if (DEBUG_MODE == true){
            printf("Arquivo: %s gerado com sucesso\n", arqSaida );
        }

        while(contadorLinhas-1 < maxLinhas && arquivo.peek() != EOF){
            // Se for a ultima coluna le o ignorando '\n' se nao le ignorando ','
            if ((numeroColunas-1) != contadorParametros){
                getline(arquivo, linha, ',');
            }
            else{
                getline(arquivo, linha, '\n');
            }
            // Converte String para vetor de char
            strcpy(chave, linha.c_str());

            if (contadorLinhas > 0){
                if (contadorParametros == posicaoChave){
                    strcpy(Linhas[contadorLinhas].colunaChave, chave);
                    saida << Linhas[contadorLinhas].colunaChave << ',';
                }
                if (contadorParametros == posicaoValor){
                    Linhas[contadorLinhas].valor = stod(chave);
                    saida << Linhas[contadorLinhas].valor << endl;
                }
            }
            numIteracoes++;
            contadorParametros++;

            // Contador de Linhas
            if (numeroColunas == contadorParametros){
                contadorLinhas++;
                contadorParametros = 0;
            }            
        }

        saida.close();
        nArquivoSaida++;
        contadorLinhas = 0;
    }

    // Fechamento do arquivo
    arquivo.close();

    if (DEBUG_MODE == true){
        cout << endl << "--- DEBUG INFO ----" << endl;
        cout << "Numero de Colunas: "<< numeroColunas << endl;
        cout << "Numero de Linhas: "<< maxLinhas << endl;
        cout << "Total de Arquivos Criados: "<< nArquivoSaida-1 << endl;
        cout << "Chave de Agregacao: "<< chaveAgregacao << " - " << posicaoChave << endl;
        cout << "Coluna da Calculo: "<< colunaCalculo << " - " << posicaoValor<< endl;
    }

    return 0;
}

// particiona o subvetor v[beg, ..., end - 1]
int particiona(int *v, int beg, int end, int pivo) {
    int valorPivo = v[pivo];
    //colocamos o pivo temporariamente na ultima posição
    swap(v[pivo], v[end-1]);
    // ao acharmos um elemento menor do que o pivo, vamos coloca-lo
    // na posicao "pos"
    int pos = beg;
    for(int i = beg; i < end-1; i++) {
        if (v[i] < valorPivo) {
            swap(v[pos], v[i]);
            pos++;
        }
    }
    //coloque o pivo depois do ultimo elemento menor que ele
    swap(v[pos],v[end-1]);
    return pos;
}

void quickSort2(int *v, int beg, int end) {
    if(beg == end) return;
    int pos = particiona(v, beg, end, beg);
    quickSort2(v, beg, pos);
    quickSort2(v, pos + 1, end);
}

void quickSort(int *v, int n) {
    quickSort2(v, 0, n);
}