#include <iostream>
#include <cstring>
#include <fstream>

#include <string>

#include <sstream> 
#include <algorithm>

using namespace std;

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


    // Abertura do arquivo
    ifstream arquivo(argv[1]);
    
    string linha;
    char chave[50];
    int posicaoChave = 0;
    int posicaoValor = 0;
    int AUX01 = 0; //Auxiliar para contar a posicao da chave
    int contadorLinhas = 0;
    bool cabecalhoLido = false;

    getline(arquivo, linha);
    int numeroColunas = count(linha.begin(), linha.end(), ',') + 1;

    structLinhas Linhas[maxLinhas];
    // Volta para o inicio do arquivo
    arquivo.seekg (0, arquivo.beg);

    ofstream saida("notasNormalizadas.txt");
    //  Le a linha ignorando o '\n'   
    while( getline(arquivo, linha)){
        // Le a linha novamente, agora separando os valores por ','

        stringstream streamLinha(linha);
        int contadorParametros = 0;
        while ( getline(streamLinha, linha, ',') && contadorLinhas < maxLinhas) {
            strcpy(chave, linha.c_str());
            // Determinar qual a coluna da chave
            if (strcmp(chave, chaveAgregacao) == 0) posicaoChave = AUX01;
            if (strcmp(chave, colunaCalculo) == 0) posicaoValor = AUX01;

            if (cabecalhoLido){
                if (contadorParametros == posicaoChave){
                    strcpy(Linhas[contadorLinhas].colunaChave, chave);
                    saida << Linhas[contadorLinhas].colunaChave << ',';
                }
                if (contadorParametros == posicaoValor){
                    Linhas[contadorLinhas].valor = atoi(chave);
                    saida << Linhas[contadorLinhas].valor << endl;
                }
            }
            AUX01++;
            contadorParametros++;
            // cout << "PATAMETRO" << contadorParametros<< endl;
        }
        cabecalhoLido = true;
        contadorLinhas++;
    }

    if (DEBUG_MODE == true){
        cout << endl << "--- DEBUG INFO ----" << endl;
        cout << "Posicao da Chave: "<< posicaoChave << endl;
        cout << "Posicao da Valor: "<< posicaoValor << endl;
        cout << "Numero de Colunas: "<< numeroColunas << endl;
        cout << "Numero de Linhas: "<< maxLinhas << endl;
    }

    // Fechamento do arquivo
    arquivo.close();
    saida.close();

    return 0;
}