#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <ctype.h>

#include <string>



using namespace std;

// Quicksort
int particiona(struct structLinhas *v, int beg, int end, int pivo);
void quickSort2(struct structLinhas *v, int beg, int end);
void quickSort(struct structLinhas *v, int n);

void procesamento(int nArquivoSaida);

void lerLinha(ifstream processador[], struct structLinhas *Linhas, int nPro, int &nLinha);
void peneira(struct structLinhas *Linhas, int m, int maxLinhas);
void heapfy(struct structLinhas *Linhas, int maxLinhas);


void swapStruct(struct structLinhas *v, int x, int y);

struct structLinhas{
    char colunaChave[50];
    long double valor = 0;
    int nPro = 0; // Numero do arquivo de saida
    int ultLido = 0; // Ultima linha lida
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
    int nArquivoSaida = 0; //Numero do arquivo de saida

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

        sprintf(arqSaida, "%d.txt", nArquivoSaida);
        
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

        nArquivoSaida++;
        contadorLinhas = 0;
        linhaAtual++;
        delete[] Linhas;
    }

    // Fechamento do arquivo
    arquivo.close();
    procesamento(nArquivoSaida);

    if (DEBUG_MODE == true){
        cout << endl << "--- DEBUG INFO ----" << endl;
        cout << "Numero de Colunas: "<< numeroColunas << endl;
        cout << "Numero de Linhas: "<< maxLinhas << endl;
        cout << "Total de Arquivos Criados: "<< nArquivoSaida-1 << endl;
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



void procesamento(int nArquivoSaida){
    char arqSaida[20];
    nArquivoSaida = nArquivoSaida-1;
    ifstream processador[nArquivoSaida+1];

    structLinhas *Linhas = new structLinhas[nArquivoSaida+1];

    int nPro = 0, k = 0, nLinha = 0;

    //  Abertura dos arquivos
    for (int i = 0; i < nArquivoSaida; i++){
        sprintf(arqSaida, "%d.txt", i);
        processador[i].open(arqSaida);
        Linhas[i].nPro = i;
        lerLinha(processador, Linhas, i, nLinha);
        cout << "aberto: " << arqSaida << endl;
        nLinha++; // Contador de linhas
    }
    
    structLinhas Menor;
    Menor.valor = Linhas[0].valor;

    for (int k = 0; k < 5; k++){
        for (int i = 0; i < nArquivoSaida; i++){
            if (Linhas[i].valor < Menor.valor) {
                Menor.valor = Linhas[i].valor;
                Menor.nPro = Linhas[i].nPro;
            }
        }

        cout << "Menor: " << Menor.valor << " - nPro: "<< Menor.nPro << endl;
        lerLinha(processador, Linhas, Menor.nPro, nLinha);
        Menor.valor = Linhas[Menor.nPro].valor;
        
    }


    for (int i = 0; i < nArquivoSaida; i++){
        sprintf(arqSaida, "%d.txt", i);
        processador[i].close();
        remove(arqSaida);
        cout << "fechado: " << arqSaida << endl;
    }

    delete[] Linhas;
}


void peneira(struct structLinhas *Linhas, int m, int maxLinhas){
    int j = 2 * m;
    int t = Linhas[m].valor;

    cout << "Valor: "<<Linhas[m].valor;
    cout << "  - nPro: "<< Linhas[m].nPro << endl;

    while (j <= maxLinhas) {
        if (Linhas[0].valor > Linhas[j].valor){
            swapStruct(Linhas, 0, j);
        } 

        if (j < maxLinhas && Linhas[j+1].valor < Linhas[j].valor){
            j = j + 1;
        }

        if (t < Linhas[j].valor) break;

        else if (t >= Linhas[j].valor) {
            Linhas[j/2] = Linhas[j];
            j = 2 * j;
        }
    }
    Linhas[j/2].valor = t;

}

void heapfy(struct structLinhas *Linhas, int maxLinhas) {
    int k;
    for(k = maxLinhas/2; k >= 1; k--) {
        peneira(Linhas, k, maxLinhas);
        
    }
    cout << Linhas[0].valor << endl;
}


void lerLinha(ifstream processador[], struct structLinhas *Linhas, int nPro, int &nLinha){
    // nPro = Numero do processador
    if (processador[nPro].peek() != EOF){
        string linha;

        getline(processador[nPro], linha, ',');
        char *chave1 = new char [linha.length() + 1];
        strcpy(chave1, linha.c_str());
        strcpy(Linhas[nPro].colunaChave, chave1);
    
        getline(processador[nPro], linha, '\n');
        char *chave2 = new char [linha.length() + 1];
        strcpy(chave2, linha.c_str());
        Linhas[nPro].valor = stod(chave2);
        
        // cout << Linhas[nPro].colunaChave << "  " << Linhas[nPro].valor << "  " << Linhas[nPro].nPro << endl;
        delete[] chave1;
        delete[] chave2;
    }

}

void swapStruct(struct structLinhas *v, int x, int y){
    structLinhas temp;
    temp = v[x];

    v[x] = v[y];
    v[y] = temp;
}