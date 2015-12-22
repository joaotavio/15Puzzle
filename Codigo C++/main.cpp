#include <cstdlib>
#include <string>
#include <set>
#include <unordered_map>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <list>
#include <limits.h>
#include <cmath>

using namespace std;

#define TAM_TABULEIRO 4
int tabuleiroFinal[TAM_TABULEIRO][TAM_TABULEIRO] = {{ 1,  2, 3,  4},
                                                    {12, 13, 14, 5},
                                                    {11,  0, 15, 6},
                                                    {10,  9,  8, 7}};

class Tabuleiro {
    
public:
    int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO];
    int linha_0;
    int coluna_0;
    int g;
    int h;
    string hash;
    
    Tabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO]);
    Tabuleiro(const Tabuleiro& t);
    Tabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO], int linha_0, int coluna_0);
    Tabuleiro(int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO], int linha_0, int coluna_0, int g);
    
    string gerarHash();
    list<Tabuleiro> gerarSucessores();
    Tabuleiro trocarPosicao0(int linha, int coluna);
    int heuristica1();
    int heuristica2();
    int heuristica3();
    int heuristica4();
    int heuristica5();
    friend bool operator<(const Tabuleiro &t1, const Tabuleiro &t2);
    friend bool operator==(const Tabuleiro &t1, const Tabuleiro &t2);
};

Tabuleiro::Tabuleiro(const Tabuleiro& t){
    memcpy(tabuleiro, t.tabuleiro, TAM_TABULEIRO*TAM_TABULEIRO * sizeof(int));
    this->linha_0 = t.linha_0;
    this->coluna_0 = t.coluna_0;
    this->g = t.g;
    this->h = t.h;
    this->hash = t.hash;
}

Tabuleiro::Tabuleiro(int tab[TAM_TABULEIRO][TAM_TABULEIRO]){
    memcpy(tabuleiro, tab, TAM_TABULEIRO*TAM_TABULEIRO * sizeof(int));
    this->hash = gerarHash();
}

Tabuleiro::Tabuleiro(int tab[TAM_TABULEIRO][TAM_TABULEIRO], int linha_0, int coluna_0) : Tabuleiro(tab, linha_0, coluna_0, 0){}

Tabuleiro::Tabuleiro(int tab[TAM_TABULEIRO][TAM_TABULEIRO], int linha_0, int coluna_0, int g){
    memcpy(tabuleiro, tab, TAM_TABULEIRO*TAM_TABULEIRO * sizeof(int));
    this->linha_0 = linha_0;
    this->coluna_0 = coluna_0;
    this->g = g;
    this->h = heuristica3();
    this->hash = gerarHash();
}

string Tabuleiro::gerarHash(){
    string hash = "";
    char num[20];
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            sprintf(num, "%d-", tabuleiro[i][j]);
            hash += num;
        }
    }
    return hash;
}

list<Tabuleiro> Tabuleiro::gerarSucessores(){
    list<Tabuleiro> sucessores;
    if (coluna_0 > 0) {
        sucessores.push_back(trocarPosicao0(linha_0, coluna_0 - 1));
    }
    if (coluna_0 < 3) {
        sucessores.push_back(trocarPosicao0(linha_0, coluna_0 + 1));
    }
    if (linha_0 > 0) {
        sucessores.push_back(trocarPosicao0(linha_0 - 1, coluna_0));
    }
    if (linha_0 < 3) {
        sucessores.push_back(trocarPosicao0(linha_0 + 1, coluna_0));
    }
    return sucessores;
}

Tabuleiro Tabuleiro::trocarPosicao0(int linha, int coluna){
    int tab[TAM_TABULEIRO][TAM_TABULEIRO];
    memcpy(tab, tabuleiro, TAM_TABULEIRO*TAM_TABULEIRO * sizeof(int));
    tab[this->linha_0][this->coluna_0] = tab[linha][coluna];
    tab[linha][coluna] = 0;

    return Tabuleiro(tab, linha, coluna, this->g + 1);
}

int Tabuleiro::heuristica1(){
    int h1 = 0;
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            if (tabuleiroFinal[i][j] != tabuleiro[i][j]) {
                h1++;
            }
        }
    }
    return h1;
}

int Tabuleiro::heuristica2(){
    int h2 = 0;
    int vetor[16];
    vetor[0] = tabuleiro[0][0]; vetor[1] = tabuleiro[0][1]; vetor[2] = tabuleiro[0][2];
    vetor[3] = tabuleiro[0][3]; vetor[4] = tabuleiro[1][3]; vetor[5] = tabuleiro[2][3];
    vetor[6] = tabuleiro[3][3]; vetor[7] = tabuleiro[3][2]; vetor[8] = tabuleiro[3][1];
    vetor[9] = tabuleiro[3][0]; vetor[10] = tabuleiro[2][0]; vetor[11] = tabuleiro[1][0];
    vetor[12] = tabuleiro[1][1]; vetor[13] = tabuleiro[1][2]; vetor[14] = tabuleiro[2][2];
    vetor[15] = tabuleiro[2][1];
    
    for (int i = 1; i < 16; i++) {
        if (vetor[i] != (vetor[i-1]+1) && vetor[i-1] != 0){
            h2++;
        }
    }
    return h2;
}

int Tabuleiro::heuristica3(){
    int posicao[TAM_TABULEIRO * TAM_TABULEIRO][2];
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            posicao[tabuleiroFinal[i][j]][0] = i;
            posicao[tabuleiroFinal[i][j]][1] = j;
        }
    }
    int h3 = 0, posI, posJ;
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            posI = posicao[tabuleiro[i][j]][0];
            posJ = posicao[tabuleiro[i][j]][1];
            h3 += abs(i - posI) + abs(j - posJ);
        }
    }
    return h3;
}

int Tabuleiro::heuristica4(){
    int h1 = heuristica1();
    int h2 = heuristica2();
    int h3 = heuristica3();
    int h4 = (int) (0.0*h1 + 0.05*h2 + 0.95*h3);
    return h4;
}

int Tabuleiro::heuristica5(){
    int h1 = heuristica1();
    int h2 = heuristica2();
    int h3 = heuristica3();
    return max(h1, max(h2, h3));
}

set<Tabuleiro> abertos;
unordered_map<string, int> hash_valorG;

bool operator<(const Tabuleiro &t1, const Tabuleiro &t2){
    if (t1.hash == t2.hash){
        return false;
    }
    int f1 = t1.g + t1.h;
    int f2 = t2.g + t2.h;
    if (f1 == f2) {
        if (hash_valorG.find(t2.hash) != hash_valorG.end()){
            return true;
        }
        else {
            return false;
        }
        
    }
    return f1 < f2;
}

bool operator==(const Tabuleiro &t1, const Tabuleiro &t2){
    return t1.hash == t2.hash;
}

void a_estrela(Tabuleiro s){
    Tabuleiro resposta(tabuleiroFinal);
    
    unordered_map<string, Tabuleiro> fechados;
    list<Tabuleiro> sucessores;
    list<Tabuleiro>::iterator it;
    
    bool achou = false;
    abertos.insert(s);
    hash_valorG.insert(make_pair(s.hash, s.g));
    
    int movimentos = 0;
    int i = 0;
    
    while (!abertos.empty() && !achou){
        //printf("I: %d\n", i++);
        i++;
        Tabuleiro atual = *abertos.begin();
        abertos.erase(atual);
        hash_valorG.erase(atual.hash);
        fechados.insert(make_pair(atual.hash, atual));
        
        if (atual == resposta){
            movimentos = atual.g;
            achou = true;
            break;
        }
        sucessores = atual.gerarSucessores();
        for (it = sucessores.begin(); it != sucessores.end(); it++){
            if (fechados.find(it->hash) != fechados.end()){
                continue;
            }
            
            int antigo_g = (hash_valorG.find(it->hash) != hash_valorG.end()) ? hash_valorG.at(it->hash) : INT_MAX;
            if (it->g >= antigo_g){
                continue;
            }
            
            abertos.erase(*it);
            abertos.insert(*it);
            hash_valorG.insert(make_pair(it->hash, it->g));
        }
    }
    
    if (achou){
        printf("\nACHOU -> %d\nITERACOES: %d\n", movimentos, i);
    } else {
        printf("NAO ACHOU");
    }
}

Tabuleiro lerEntrada(){
    int tabuleiro[TAM_TABULEIRO][TAM_TABULEIRO];
    int linha0, coluna0;
    for (int i = 0; i < TAM_TABULEIRO; i++) {
        for (int j = 0; j < TAM_TABULEIRO; j++) {
            scanf("%d", &tabuleiro[i][j]);
            if (tabuleiro[i][j] == 0){
                linha0 = i;
                coluna0 = j;
            }
        }
    }
    Tabuleiro t(tabuleiro, linha0, coluna0);
    return t;
}

int main(int argc, char** argv) {
    Tabuleiro t = lerEntrada();

    a_estrela(t);
    
    return 0;
}