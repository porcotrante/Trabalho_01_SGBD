#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "./src/headers/Arvore_Bp.h"
using namespace std;

int main() {
    ifstream entrada("in.txt");
    ofstream saida("out.txt");

    if (!entrada.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada in.txt" << endl;
        return 1;
    }
    if (!saida.is_open()) {
        cerr << "Erro ao criar o arquivo de saída out.txt" << endl;
        return 1;
    }

    string linha;
    int n_filhos = 0;
    Arvore_Bp* raiz = nullptr;
    Arvore_Bp** raiz_ptr = &raiz;

    // Ler a primeira linha (quantidade de filhos)
    if (getline(entrada, linha)) {
        saida << linha << endl; // Escreve a linha inicial no arquivo de saída
        if (linha.rfind("FLH/", 0) == 0) { // Verifica se começa com "FLH/"
            n_filhos = stoi(linha.substr(4));
            raiz = new Arvore_Bp(n_filhos, nullptr); // Cria a raiz inicial
        } else {
            cerr << "Formato inválido na primeira linha do arquivo de entrada." << endl;
            return 1;
        }
    }

    // Processar as operações
    while (getline(entrada, linha)) {
        if (linha.rfind("INC:", 0) == 0) { // Operação de inserção
            int valor = stoi(linha.substr(4));
            (*raiz_ptr)->inserir(valor, false, nullptr, raiz_ptr);
            saida << "INC:" << valor << "/1" << endl; // Sempre insere 1 tupla
        } else if (linha.rfind("BUS=:", 0) == 0) { // Operação de busca
            int valor = stoi(linha.substr(5));
            int resultado = (*raiz_ptr)->buscaRec(valor);
            saida << "BUS=:" << valor << "/" << resultado << endl;
        } else {
            cerr << "Comando inválido: " << linha << endl;
        }
    }

    // Escrever a altura da árvore no final do arquivo de saída
    if (raiz) {
        saida << "H/" << raiz->altura << endl;
    }

    entrada.close();
    saida.close();

    return 0;
}