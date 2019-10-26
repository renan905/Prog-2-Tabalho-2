#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <string.h>
#include <math.h>
#include <iomanip>

using namespace std;
int definirColuna(char* &nomeArquivo, char* &chave);
void ordenarArquivo(char* &nomeArquivo, int linhasMemoria, int &pos1, int &pos2);
int criarArquivos(char* &nomeArquivo, int linhasMemoria, int &pos1, int &pos2);
void ordenaVetor(char* matrizStrings[], int lidas);
void merge(int numArquivos);
int menor(char *matriz[],bool lerArquivos[],int numArquivos);

int main(int argc, char *argv[]) {
    int pos1, pos2;
    pos1 = definirColuna(argv[1], argv[3]);
    pos2 = definirColuna(argv[1], argv[4]);
    ordenarArquivo(argv[1], atoi(argv[2]), pos1, pos2);
    return 0;   
}

void merge(int numArquivos){

    bool lerArquivos[numArquivos];
    char abrirArquivo[30]; 

    fstream lerLinha[numArquivos];
    string linhaLida;
    ofstream salvarArquivo;
    salvarArquivo.open("Ordenado.txt",ios::app);

    char *matriz[numArquivos];
    int t = numArquivos;
    int marcador;

    for(int i=0; i<numArquivos; i++){
        lerArquivos[i]= true;
        sprintf(abrirArquivo, "ArquivoTemporario%d.txt", i );
        lerLinha[i].open(abrirArquivo);
    }

    for(int i=0; i<numArquivos;i++){
        if( getline(lerLinha[i], linhaLida, '\n')){
            char *linhaChar1 = new char [linhaLida.length()+1];
            strcpy(linhaChar1, linhaLida.c_str());
            matriz[i]=linhaChar1;
        }   
    }
    while(t!=0){
        int marcador = menor(matriz,lerArquivos,numArquivos);
        salvarArquivo << matriz[marcador] << endl;

        if(lerLinha[marcador].peek()==EOF){
            lerArquivos[marcador]=false;    t--;
            lerLinha[marcador].close();
        }
        else{
            getline(lerLinha[marcador], linhaLida, '\n');
            delete[]matriz[marcador];
            char *linhaChar1 = new char [linhaLida.length()+1];  strcpy(linhaChar1, linhaLida.c_str());
            matriz[marcador]=linhaChar1;
        }
    
    }
    for(int i=0; i<numArquivos; i++)
        delete[]matriz[i];  //Daqui pra baixo faz a média, vou colocar em uma função ainda.

    ifstream arquivoLer; arquivoLer.open("Ordenado.txt");
    string linhaLid;
    long double total=0.0000000000000000000000001;
    long int contador=1;

    getline(arquivoLer, linhaLid);

    char *linhaChar2 = new char [linhaLid.length()+1];
    strcpy(linhaChar2, linhaLid.c_str()); 

    

    char *linhaChar = strtok(linhaChar2, ",");
    char compara [linhaLid.length()+1];

    total = atoi(strtok(NULL, ","));

    while( getline(arquivoLer, linhaLid) ) {
         char *linhaChar1 = new char [linhaLid.length()+1];
         strcpy(linhaChar1, linhaLid.c_str());
         strcpy(compara,strtok(linhaChar1, ","));

         if(strcmp(linhaChar,compara)!=0){
            cout << fixed<< linhaChar << ", " << setprecision(15) << total/contador << endl;
            strcpy(linhaChar,compara);
            total=0;
            contador=0;
        }
        total += atof(strtok(NULL, ","));
        contador++;
        
        delete[]linhaChar1;
    }
    cout << linhaChar << ", " << total/contador << endl;
    delete[]linhaChar2;
}


int menor(char *matriz[], bool lerArquivos[], int numArquivos){
    int marcador=0;

    while(marcador<numArquivos){
        if(lerArquivos[marcador])
            break;
        else
            marcador++;
    }
    for(int i=0; i<numArquivos; i++){
        if (lerArquivos[i] && strcmp(matriz[i],matriz[marcador])<0)
            marcador=i;
    }
    return marcador;
}

void ordenarArquivo(char* &nomeArquivo, int linhasMemoria, int &pos1, int &pos2){
    int linhasArquivoSemCabecario = criarArquivos(nomeArquivo, linhasMemoria, pos1, pos2);
    int totalArquivos;  char novosArquivos[30];
    if(linhasArquivoSemCabecario%linhasMemoria==0 ){totalArquivos = linhasArquivoSemCabecario/linhasMemoria;}
    else{totalArquivos = linhasArquivoSemCabecario/linhasMemoria + 1;}
    //remove(nomeArquivo); essa linha exclui o arquivo desordenado. VAmos deixar comentado enquanto fazemos os testes.
    merge(totalArquivos); //aqui virá a chamada do merge externo;
    for(int i=1; i<=totalArquivos; i++){ //nosso código salva do 1 em diante e vai até o último, por isso =1 e <=
        sprintf(novosArquivos, "ArquivoTemporario%d.txt", i );
        //remove(novosArquivos); essa é a nossa linha que remove os arquivos menores criados pra ordenar.
    }   }
int criarArquivos(char* &nomeArquivo, int linhasMemoria, int &pos1, int &pos2){ 
    ofstream arquivoSalvar; ifstream arquivoLer; arquivoLer.open(nomeArquivo);
    char novoArquivo[30];   string linhaLida; int lidas = 0; int contadorTok = 0; int contador = 0; int leiturasTotais = 0;
    char *matrizStrings[linhasMemoria];
    while( getline(arquivoLer, linhaLida) ) {
        if(leiturasTotais == 0){leiturasTotais++;} else{        
        char *linhaChar1 = new char [linhaLida.length()+1];  strcpy(linhaChar1, linhaLida.c_str()); //Conterá 1a coluna      
        char *linhaChar2 = new char [linhaLida.length()+1];  strcpy(linhaChar2, linhaLida.c_str()); //Conterá 2a coluna
        char *linhaChar3 = new char [linhaLida.length()+1];
        char *linhaChar4;   char *linhaChar5;
        linhaChar4 = strtok(linhaChar1, ",");
        while (linhaChar4 != NULL){
            if(contadorTok == pos1 ){
                strcpy(linhaChar3, linhaChar4);
                strcat(linhaChar3, ",");
                delete[]linhaChar1;
                break;  }   
            contadorTok++;  linhaChar4 = strtok(NULL, ","); }
        contadorTok = 0;    linhaChar5 = strtok(linhaChar2, ","); //chamo tok até chegar a 2 parte
        while (linhaChar5 != NULL){
            if(contadorTok == pos2 ){
                strcat(linhaChar3, linhaChar5);
                strcat(linhaChar3, ",");
                delete[]linhaChar2;
                break;  }
            contadorTok++;  linhaChar5 = strtok(NULL, ",");     }  
       matrizStrings[lidas] = linhaChar3; contadorTok = 0;  lidas++; leiturasTotais++;
        if (lidas == linhasMemoria){
            sprintf(novoArquivo, "ArquivoTemporario%d.txt", contador); //Nome para o novo arquivo com as linhas salvas;
            contador++;			
            ordenaVetor(matrizStrings, lidas);
            arquivoSalvar.open(novoArquivo, ios::app);
            for(int i=0; i<lidas; i++)
                arquivoSalvar << matrizStrings[i]<< "\n";
            arquivoSalvar.close();
            for(int i=0; i<lidas; i++)
                delete[]matrizStrings[i];
			lidas = 0; //para repetir o processo
        }  }    }
        if(lidas >0){
            
            sprintf(novoArquivo, "ArquivoTemporario%d.txt", contador);
            ordenaVetor(matrizStrings, lidas);
            arquivoSalvar.open(novoArquivo, ios::app);
            for(int i=0; i<lidas; i++)
                arquivoSalvar << matrizStrings[i]<< "\n";
            contador++;
            arquivoSalvar.close();  }
        arquivoLer.close();
        for(int i=0; i<lidas; i++)
            delete[]matrizStrings[i];
    return leiturasTotais-1;    }
void ordenaVetor(char* matrizStrings[], int lidas){
    for (int i = 0; i < lidas-1; i++) {
		int posMenor = i;
			for (int j = i+1; j < lidas; j++)
				if (strcmp(matrizStrings[j],matrizStrings[posMenor])<0)
					posMenor = j;
			  	char *temp;
			  	temp = matrizStrings[i]; 
			  	matrizStrings[i] = matrizStrings[posMenor]; 
			 	matrizStrings[posMenor] = temp; }  }
int definirColuna(char* &nomeArquivo, char* &chave){
    int cont = 0;
    ifstream arquivo;   string linha;   arquivo.open(nomeArquivo);
    getline(arquivo, linha);
    char *linhaChar = new char [linha.length()+1];  strcpy(linhaChar, linha.c_str());
    char *linhaTok;
    linhaTok = strtok(linhaChar, ",");
    while (linhaTok != NULL){
        if(strcmp(linhaTok, chave) == 0){
            delete[] linhaChar;
            return cont;    }   
        else{   cont++;   }
    linhaTok = strtok(NULL, ",");  }    }





