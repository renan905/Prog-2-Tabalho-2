#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>

#include <string>


using namespace std;

// Quicksort
int particiona(struct structLinhas *v, int beg, int end, int pivo);
void quickSort2(struct structLinhas *v, int beg, int end);
void quickSort(struct structLinhas *v, int n);

int contarLinhas(char **argv){
    int numeroLinhas = 0;
    ifstream arquivo(argv[1]);
    while (arquivo.peek() != EOF){
        arquivo.ignore();
        numeroLinhas++;
    }
    return numeroLinhas;
}

struct structLinhas{
    char colunaChave[50];
    long double valor = 0;
};

int main(int argc, char *argv[]){
    bool DEBUG_MODE = false;
    int XXX = 0;
    int YYY = 0;

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
    int totalLinha = contarLinhas(argv);

    // char *argv[3] = new char [strlen(argv[3])+1];
    // strcpy (argv[3], argv[3]);
    // char *argv[4] = new char [strlen(argv[4])+1];
    // strcpy (argv[4], argv[4]);

    int posicaoChave = 0, posicaoValor = 0; //Guarda a posicao do chave e da coluna de calculo
    int contadorLinhas = 0, contadorParametros = 0, linhaAtual = 0, numIteracoes = 0; // Contadores
    int nArquivoSaida = 1; //Numero do arquivo de saida

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

        while(contadorLinhas-1 < maxLinhas && arquivo.peek() != EOF){
                
            // Se for a ultima coluna le o ignorando '\n' se nao le ignorando ','
            if ((numeroColunas-1) != contadorParametros){
                getline(arquivo, linha, ',');
                XXX++;
            }
            else if ((numeroColunas-1) == contadorParametros){
                getline(arquivo, linha, '\n');
                XXX++;
            }

            char *chave = new char [linha.length() +1];

            // Converte String para vetor de char
            strcpy(chave, linha.c_str());
            
            if (contadorParametros == posicaoChave){
                strcpy(Linhas[contadorLinhas].colunaChave, chave);
            }

            if (contadorParametros == posicaoValor){
                Linhas[contadorLinhas].valor = stod(chave);
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

        if (arquivo.peek() != EOF){
            quickSort(Linhas, maxLinhas);
            // Gravar valores no arquivo
            for (int i = 1; i <= maxLinhas; i++){
                saida << Linhas[i].colunaChave << ',';
                saida << Linhas[i].valor << endl;
                YYY++;
            }
        }
        
        // else if (arquivo.peek() != EOF){
        //     quickSort(Linhas, contadorLinhas+1);
        //     // Gravar valores no arquivo
        //     for (int i = 1; i <= contadorLinhas; i++){
        //         saida << Linhas[i].colunaChave << ',';
        //         saida << Linhas[i].valor << endl;
        //         YYY++;
        //     }
        // }

        saida.close();

        // if (DEBUG_MODE == true){
        //     printf("Arquivo: %s gerado com sucesso\n", arqSaida );
        // }

        nArquivoSaida++;
        contadorLinhas = 0;
        linhaAtual++;
        delete[] Linhas;
    }

    // Fechamento do arquivo
    arquivo.close();

    if (DEBUG_MODE == true){
        cout << endl << "--- DEBUG INFO ----" << endl;
        cout << "Numero de Colunas: "<< numeroColunas << endl;
        cout << "Numero de Linhas: "<< maxLinhas << endl;
        cout << "Total de Arquivos Criados: "<< nArquivoSaida-1 << endl;
        cout << "Chave de Agregacao: "<< argv[3] << " - " << posicaoChave << endl;
        cout << "Coluna da Calculo: "<< argv[4] << " - " << posicaoValor<< endl;
        cout << "Leituras: "<< XXX/numeroColunas << endl;
        cout << "Saidas: "<< YYY << endl;
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
        if ( strcmp(v[i].colunaChave, chavePivo) < 0) {
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
    quickSort2(v, 0, n+1);
}

// void merge(){

//     for (int i = 0; i < n; i++){
//         leitura[i].
//     }
// }