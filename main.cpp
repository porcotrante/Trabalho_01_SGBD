#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include "./src/headers/Arvore_Bp.h"
#include "./src/headers/Utils.h"
using namespace std;

int main() {
    std::ofstream arquivo("index.txt");
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

    getline(entrada, linha);
    n_filhos = stoi(linha.substr(4));
    Arvore_Bp raiz = Arvore_Bp(n_filhos);

    saida << linha << endl;

    while (std::getline(entrada, linha)) {
        if (linha.rfind("INC:", 0) == 0) {
            int valor = std::stoi(linha.substr(4));
            raiz.inserir(valor);

            std::cout << "Inserido: " << valor << "\n";
        } else if (linha.rfind("BUS=:", 0) == 0) {
            int valor = std::stoi(linha.substr(5));
            raiz.busca(valor);
        }
    }

    saida.close();
    ofstream saida1("out.txt", std::ios::app);

    // Escrever a altura da árvore no final do arquivo de saída
    saida1 << "H/" << Utils::getAltura() << endl;

    entrada.close();
    saida1.close();

    return 0;
}