#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
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

    while (std::getline(entrada, linha)) {
        if (linha.rfind("INC:", 0) == 0) {
            int valor = std::stoi(linha.substr(4));
            (*raiz_ptr)->inserir(valor, false, nullptr, raiz_ptr);

            std::cout << "Inserido: " << valor << "\n";
        } else if (linha.rfind("BUS=:", 0) == 0) {
            int valor = std::stoi(linha.substr(5));
            (*raiz_ptr)->busca(valor);
        }
    }

    saida.close();
    ofstream saida1("out.txt", std::ios::app);

    // Escrever a altura da árvore no final do arquivo de saída
    if (raiz) {
        saida1 << "H/" << (*raiz_ptr)->altura << endl;
    }

    entrada.close();
    saida1.close();

    return 0;
}