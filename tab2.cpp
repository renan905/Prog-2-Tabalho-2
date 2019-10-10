#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

#include <sstream> 

#include <algorithm>

using namespace std;

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
    char colunaCalculo[50];
    strcpy (colunaCalculo, argv[4]);


    // Abertura do arquivo
    ifstream arquivo(argv[1]);
    
    string linha;
    char chave[50];
    int posicaoChave = 0;
    int AUX01 = 0; //Auxiliar para contar a posicao da chave


    getline(arquivo, linha);
    int numeroColunas = count(linha.begin(), linha.end(), ',') + 1;

    // Volta para o inicio do arquivo
    arquivo.seekg (0, arquivo.beg);
   
    //  Le a linha ignorando o '\n'   
    while( getline(arquivo, linha)){
        // Le a linha novamente, agora separando os valores por ','
        stringstream streamLinha(linha);
        while ( getline(streamLinha, linha, ',') && ((AUX01)/numeroColunas) < maxLinhas) {

            strcpy(chave, linha.c_str());
            // Determinar qual a coluna da chave
            if (strcmp(chave, chaveAgregacao) == 0) posicaoChave = AUX01;

            cout << linha << "  " << AUX01 << endl;
            AUX01++;
        }
    }

    if (DEBUG_MODE == true){
        cout << "Posicao da Chave: "<< posicaoChave << endl;
        cout << "Numero de Colunas: "<< numeroColunas << endl;
    }

    // Fechamento do arquivo
    arquivo.close();
    return 0;
}