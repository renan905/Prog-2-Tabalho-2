#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <ctype.h>
#include <iomanip>

#include <string>



using namespace std;

// Quicksort
int particiona(struct structLinhas *v, int beg, int end, int pivo);
void quickSort2(struct structLinhas *v, int beg, int end);
void quickSort(struct structLinhas *v, int n);

void procesamento(int numArquivoSaida);
void lerLinha(ifstream processador[], struct Media *Linhas, int nPro, int &nLinha);

int menor(struct structLinhas *Linhas, bool linhasLidas[], int numArquivoSaida);

void merge(int numArquivos);

struct structLinhas{
    char colunaChave[50];
    long double valor = 0;
};

struct Media{
    char chave[50];
    long double valor = 0;
    int nPro = 0; // Numero do arquivo de saida
    int totalValores = 0;
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

    int posicaoChave = 0, posicaoValor = 0; //Guarda a posicao do chave e da coluna de calculo
    int contadorLinhas = 0, contadorParametros = 0, linhaAtual = 0, numIteracoes = 0; // Contadores
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
    // Volta para o inicio do arquivo apos ler o numero de colunas
    // arquivo.seekg (0, arquivo.beg);
    numIteracoes = 0;
    contadorParametros = 0;
    int cabecalho = 0;


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
        linhaAtual++;
        delete[] Linhas;
    }

    // Fechamento do arquivo
    arquivo.close();
    
    procesamento(numArquivoSaida);

    if (DEBUG_MODE == true){
        cout << endl << "--- DEBUG INFO ----" << endl;
        cout << "Numero de Colunas: "<< numeroColunas << endl;
        cout << "Numero de Linhas: "<< maxLinhas << endl;
        cout << "Total de Arquivos Criados: "<< numArquivoSaida << endl;
        cout << "Chave de Agregacao: "<< argv[3] << " - " << posicaoChave << endl;
        cout << "Coluna da Calculo: "<< argv[4] << " - " << posicaoValor<< endl;
    }

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

    // char *Linhas = new char [numArquivoSaida];
    structLinhas *Linhas = new structLinhas[numArquivoSaida];

    bool linhasLidas[numArquivoSaida];

    int nPro = 0, k = 0, nLinha = 0;

    //  Abertura dos arquivos
    for (int i = 0; i < numArquivoSaida; i++){

        sprintf(arqSaida, "%d.txt", i);
        processador[i].open(arqSaida);
        linhasLidas[i] = true; // Contador de linhas

        // cout << "Aberto: " << arqSaida << endl;

        // Ler linha
        getline(processador[i], srtLinha, '\n');
        char *chaveLinha = new char [srtLinha.length()+1];
        strcpy(chaveLinha, srtLinha.c_str());
        strcpy(Linhas[i].colunaChave, chaveLinha);

        
        delete[] chaveLinha;
    }
    

    // Declaracao do arquivo ordenado
    ofstream saida("Ordenado.txt");
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
            // Linhas[posMenor] = chaveLinha;

            delete[] chaveLinha;
        }

    }

    delete[] Linhas;

    for (int i = 0; i < numArquivoSaida; i++){
        sprintf(arqSaida, "%d.txt", i);
        processador[i].close();
        remove(arqSaida);
        // cout << "fechado: " << arqSaida << endl;
    }

    // delete[] Linhas;

    saida.close();
}

void lerLinha(ifstream processador[], struct Media *Linhas, int nPro, int &nLinha){
    // nPro = Numero do processador
    if (processador[nPro].peek() != EOF){
        string linha;

        getline(processador[nPro], linha, ',');
        char *chave1 = new char [linha.length() + 1];
        strcpy(chave1, linha.c_str());
        strcpy(Linhas[nPro].chave, chave1);
    
        getline(processador[nPro], linha, '\n');
        char *chave2 = new char [linha.length() + 1];
        strcpy(chave2, linha.c_str());
        Linhas[nPro].valor = stod(chave2);
        
        delete[] chave1;
        delete[] chave2;
    }

}

int menor(struct structLinhas *Linhas, bool linhasLidas[], int numArquivoSaida){
    int posMenor = 0;

    while (posMenor < numArquivoSaida){
        if (linhasLidas[posMenor]) break;
        else posMenor++;
    }

    for(int i = 0; i < numArquivoSaida; i++){
        if (linhasLidas[i] && (strcmp(Linhas[i].colunaChave, Linhas[posMenor].colunaChave) < 0)){
            posMenor = i;
        }
    }
    return posMenor;
}
