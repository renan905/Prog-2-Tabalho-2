#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <ctype.h>
#include <iomanip>

#include <string>

using namespace std;

void media();
void merge(int numArquivos);
void procesamento(int numArquivoSaida);
void ordenacao(int argc, char **argv);
int menor(struct structLinhas *Linhas, bool linhasLidas[], int numArquivoSaida);

// Quicksort
int particiona(struct structLinhas *v, int beg, int end, int pivo);
void quickSort2(struct structLinhas *v, int beg, int end);
void quickSort(struct structLinhas *v, int n);

struct structLinhas{
    char colunaChave[50];
    long double valor = 0;
};

int main(int argc, char *argv[]){
    // Checar se o numero de argumento esta correto
    if (argc < 4){
        cout << "Argumentos Insuficientes!\n";
        return -1;
    }
    if (argc > 5){
        cout << "Argumentos em Excesso!\n";
        return -1;
    }

    ordenacao(argc, argv);

    return 0;
}

// ---------- QUICKSORT retirado dos slides das aulas teoricas ----------
// particiona o subvetor v[beg, ..., end - 1]
int particiona(struct structLinhas *v, int beg, int end, int pivo) {
    char *chavePivo = new char [strlen(v[pivo].colunaChave)+1];
    strcpy(chavePivo, v[pivo].colunaChave);

    //colocamos o pivo temporariamente na ultima posição
    swap(v[pivo], v[end-1]);
    // ao acharmos um elemento menor do que o pivo, vamos coloca-lo
    // na posicao "pos"
    int pos = beg;
    for(int i = beg; i < end-1; i++) {
        if (strcmp(v[i].colunaChave, chavePivo) < 0) {
            swap(v[pos], v[i]);
            pos++;
        }
    }
    //coloque o pivo depois do ultimo elemento menor que ele
    swap(v[pos],v[end-1]);
    delete[] chavePivo;
    return pos;
}

void quickSort2(struct structLinhas *v, int beg, int end) {
    if(beg == end) return;
    int pos = particiona(v, beg, end, beg);
    quickSort2(v, beg, pos);
    quickSort2(v, pos + 1, end);
}

void quickSort(struct structLinhas *v, int n) {
    quickSort2(v, 1, n);
}

void procesamento(int numArquivoSaida){

    char arqSaida[50];

    ifstream processador[numArquivoSaida];
    string srtLinha;

    structLinhas *Linhas = new structLinhas[numArquivoSaida];
    bool linhasLidas[numArquivoSaida];

    //  Abertura dos arquivos
    for (int i = 0; i < numArquivoSaida; i++){

        sprintf(arqSaida, "%d.txt", i);
        processador[i].open(arqSaida);
        linhasLidas[i] = true; // Contador de linhas

        // Ler linha
        getline(processador[i], srtLinha, '\n');
        char *chaveLinha = new char [srtLinha.length()+1];
        strcpy(chaveLinha, srtLinha.c_str());
        strcpy(Linhas[i].colunaChave, chaveLinha);
        
        delete[] chaveLinha;
    }
    
    // Declaracao do arquivo ordenado
    ofstream saida("Arquivo[ordenado].txt");
    int arquivosAbertos = numArquivoSaida;

    while(arquivosAbertos != 0){

        int posMenor = menor(Linhas, linhasLidas, numArquivoSaida);
        saida << setprecision(15) << fixed << Linhas[posMenor].colunaChave << endl;

        if (processador[posMenor].peek() == EOF){
            processador[posMenor].close();
            linhasLidas[posMenor] = false;
            arquivosAbertos--;
        }
        else{
            // Ler linha
            getline(processador[posMenor], srtLinha, '\n');
            char *chaveLinha = new char [srtLinha.length()+1];
            strcpy(chaveLinha, srtLinha.c_str());
            strcpy(Linhas[posMenor].colunaChave, chaveLinha);

            delete[] chaveLinha;
        }
    }

    delete[] Linhas;

    for (int i = 0; i < numArquivoSaida; i++){
        sprintf(arqSaida, "%d.txt", i);
        processador[i].close();
    }

    saida.close();
    media();
}

int menor(struct structLinhas *Linhas, bool linhasLidas[], int numArquivoSaida){
    int posMenor = 0;
    while (posMenor < numArquivoSaida){
        if (linhasLidas[posMenor] == true) break;
        else posMenor++;
    }

    for(int i = 0; i < numArquivoSaida; i++){
        if ((linhasLidas[i] == true) && (strcmp(Linhas[i].colunaChave, Linhas[posMenor].colunaChave) < 0)){
            posMenor = i;
        }
    }
    return posMenor;
}

void media(){

    ifstream arquivoOrdenado("Arquivo[ordenado].txt");
    string srtLinha;

    getline(arquivoOrdenado, srtLinha);
    char *chave = new char[srtLinha.length() + 1];
    strcpy(chave, srtLinha.c_str());

    char *chave2 = strtok(chave, ",");
    char chaveDeComparacao[srtLinha.length() + 1];
    long double valorMedia = atof(strtok(NULL, ","));;
    long int totalElementos = 1;

    while (getline(arquivoOrdenado, srtLinha)) {
        char *chave21 = new char[srtLinha.length() + 1];
        strcpy(chave21, srtLinha.c_str());
        strcpy(chaveDeComparacao, strtok(chave21, ","));

        if (strcmp(chave2, chaveDeComparacao) != 0) {
            cout << fixed << setprecision(20) << chave2 << ", " << valorMedia / totalElementos << endl;
            strcpy(chave2, chaveDeComparacao);
            valorMedia = 0;
            totalElementos = 0;
        }
        
        valorMedia += atof(strtok(NULL, ","));
        totalElementos++;
        
        delete[] chave21;
    }

    cout << fixed << setprecision(15) << chave2 << ", " << valorMedia / totalElementos << endl;
    delete[] chave;

    arquivoOrdenado.close();
}

void ordenacao(int argc, char **argv){
    // Leituras da linha de comando
    int maxLinhas = atoi(argv[2]);

    int posicaoChave = 0, posicaoValor = 0; //Guarda a posicao do chave e da coluna de calculo
    int contadorLinhas = 0, contadorParametros = 0, numIteracoes = 0; // Contadores
    int numArquivoSaida = 0; //Numero do arquivo de saida

    bool primeira = false;
    
    // Variavel de leitura
    string linha;

    // Abertura do arquivo
    ifstream arquivo(argv[1]);
    
    // Verifica chave e quantidade de colunas
    getline(arquivo, linha, '\n');
    char *chave = new char [linha.length() +1];
    strcpy(chave, linha.c_str());

    // Contagem do numero de colunas
    int numeroColunas = 1;
    for (int i = 0; chave[i] != '\0'; i++){
        if (chave[i] == ',') numeroColunas++;
    }

    delete[] chave;

    // Volta para o inicio do arquivo apos ler o numero de colunas
    arquivo.seekg (0, arquivo.beg);
    while(numIteracoes < numeroColunas){
        // Se for a ultima coluna le o ignorando '\n' se nao le ignorando ','
        if ((numeroColunas-1) != contadorParametros){
            getline(arquivo, linha, ',');
        }
        else{
            getline(arquivo, linha, '\n');
        }

        char *chave = new char [linha.length() +1];
        strcpy(chave, linha.c_str());
        // Determinar qual a coluna da chave
        if (strcmp(chave, argv[3]) == 0) posicaoChave = numIteracoes;
        if (strcmp(chave, argv[4]) == 0) posicaoValor = numIteracoes;
        numIteracoes++;
        contadorParametros++;
        delete[] chave;

    }

    numIteracoes = 0;
    contadorParametros = 0;
    char arqSaida[20];

    while (arquivo.peek() != EOF){
        
        structLinhas *Linhas = new structLinhas[maxLinhas+1];
        sprintf(arqSaida, "%d.txt", numArquivoSaida);
        ofstream saida(arqSaida);

        while(contadorLinhas < maxLinhas && arquivo.peek() != EOF){
                
            // Se for a ultima coluna le o ignorando '\n' se nao le ignorando ','
            if ((numeroColunas-1) != contadorParametros){
                getline(arquivo, linha, ',');
            }
            else if ((numeroColunas-1) == contadorParametros){
                getline(arquivo, linha, '\n');
            }
            char *chave = new char [linha.length() +1];

            // Converte String para vetor de char
            strcpy(chave, linha.c_str());
            if (contadorParametros == posicaoChave){
                strcpy(Linhas[contadorLinhas+1].colunaChave, chave);
            }
            if (contadorParametros == posicaoValor){
                Linhas[contadorLinhas+1].valor = stod(chave);
            }

            numIteracoes++;
            contadorParametros++;

            // Contador de Linhas
            if (numeroColunas == contadorParametros){
                contadorLinhas++;
                contadorParametros = 0;
            }            
            delete[] chave;
        }

        quickSort(Linhas, contadorLinhas);
        // Gravar valores no arquivo
        for (int i = 1; i <= contadorLinhas; i++){
            saida << Linhas[i].colunaChave << ',';
            saida << Linhas[i].valor << endl;
        }

        saida.close();

        numArquivoSaida++;
        contadorLinhas = 0;
        delete[] Linhas;
    }

    // Fechamento do arquivo
    arquivo.close();

    procesamento(numArquivoSaida);
}