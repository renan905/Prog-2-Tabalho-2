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

void leLinha(ifstream processador[], struct structLinhas *Linhas, int nPro, int nLinha);
void peneira(struct structLinhas *Linhas, int m, int maxLinhas);
void heapfy(struct structLinhas *Linhas, int maxLinhas);

struct structLinhas{
    char colunaChave[50];
    long double valor = 0;
    int nPro = 0;
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
    quickSort2(v, 1, n);
}


// ---------- MERGESORT retirado dos slides das aulas teoricas ----------

/* Supondo que v[p...q-1] e
v[q...r-1] estejam ordenados */
// void merge(int *v, int p, int q, int maxLinhas) {
//     int tam = maxLinhas-p;
//     int *aux = new int[tam];
//     int i = p; //cursor 1
//     int j = q; //cursor 2
//     int k = 0; //cursor para aux

//     while(i < q && j < maxLinhas) {
//         if (v[i] <= v[j])
//             aux[k++] = v[i++];
//         else
//             aux[k++] = v[j++];
//     }

//     while(i < q)
//         aux[k++] = v[i++];
        
//     while(j < maxLinhas)
//         aux[k++] = v[j++];

//     for(k = 0; k < tam; k++)
//         v[p+k] = aux[k];

//     delete []aux;
// }

// /* Ordena o vetor v entre as posicoes p e r-1 */
// void mergeSort(int *v, int p, int maxLinhas) {
//     // com um elemento, já está ordenado
//     if (p < maxLinhas-1) {
//         int meio = (p+r) / 2;
//         // mergeSort(v, p, meio);
//         // mergeSort(v, meio, maxLinhas);
//         merge(v, p, meio, maxLinhas); //intercala
//     }
// }

// void mergeSort(int *v, int maxLinhas) {
//     mergeSort(v, 0, maxLinhas);
// }


void procesamento(int nArquivoSaida){
    char arqSaida[20];
    nArquivoSaida = nArquivoSaida-1;
    ifstream processador[nArquivoSaida+1];

    structLinhas *Linhas = new structLinhas[nArquivoSaida+1];

    int nPro = 1, k = 0, nLinha = 0;

    for (int i = nArquivoSaida; i > 0; i--){
        sprintf(arqSaida, "%d.txt", i);
        processador[i].open(arqSaida);
        cout << "aberto: " << arqSaida << endl;
    }

    

    // structLinhas **Linhas;
    // Linhas = new structLinhas*[nArquivoSaida+1];

    // for (int i = 0; i <= nArquivoSaida+1; i++){
    //     Linhas[i] = new structLinhas[1];
    // }


    

    for (int i = 1; i <= nArquivoSaida; i++){
        leLinha(processador, Linhas, nPro, nLinha);
        Linhas[nPro].nPro = nPro;
        cout << Linhas[nLinha].nPro << endl;
        nPro++;
        nLinha++;
    }



    heapfy(Linhas, nArquivoSaida);
        


    for (int i = 1; i <= nArquivoSaida; i++){
        sprintf(arqSaida, "%d.txt", i);
        processador[i].close();
        // remove(arqSaida);
        cout << "fechado: " << arqSaida << endl;
    }

    delete[] Linhas;
}




void peneira(struct structLinhas *Linhas, int m, int maxLinhas){
    int j = 2 * m;
    int t = Linhas[m].valor;
    // char *chave = new char [a[m].length() + 1];
    // srtcpy(chave, a[m]);
    // chave = a[m];

    while (j <= maxLinhas) {
        if (j < maxLinhas && Linhas[j+1].valor < Linhas[j].valor)
            j = j + 1;

        if (t < Linhas[j].valor)
            break;

        else if (t >= Linhas[j].valor) {
            Linhas[j/2].valor = Linhas[j].valor;
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
}


void leLinha(ifstream processador[], struct structLinhas *Linhas, int nPro, int nLinha){
    // nPro = Numero do processador
    if (processador[nPro].peek() != EOF){
        string linha;

        getline(processador[nPro], linha, ',');
        char *chave1 = new char [linha.length() + 1];
        strcpy(chave1, linha.c_str());
        strcpy(Linhas[nLinha].colunaChave, chave1);
    
        getline(processador[nPro], linha, '\n');
        char *chave2 = new char [linha.length() + 1];
        strcpy(chave2, linha.c_str());
        Linhas[nLinha].valor = stod(chave2);
        
        Linhas[nLinha].nPro++;

        delete[] chave1;
        delete[] chave2;
    }

}