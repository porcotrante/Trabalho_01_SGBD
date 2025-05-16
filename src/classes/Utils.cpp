#include "../headers/Utils.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

void Utils::construirHeader(int height, int root, int n_children){
    std::ifstream arquivoEntrada("./index.txt");
    if (!arquivoEntrada.is_open()) {
        std::cerr << "Erro ao abrir o arquivo index.txt para leitura." << std::endl;
        return;
    }

    std::vector<std::string> linhas;
    std::string linha;

    // Ignora as duas primeiras linhas antigas (título e cabeçalho anterior)
    int contador = 0;
    while (std::getline(arquivoEntrada, linha)) {
        contador++;
        if (contador <= 2) continue;  // pula as duas primeiras
        linhas.push_back(linha);      // preserva as demais
    }

    arquivoEntrada.close();

    std::ofstream arquivoSaida("./index.txt", std::ios::trunc);
    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir o arquivo index.txt para escrita." << std::endl;
        return;
    }

    // Primeira linha: títulos metadados da árvore
    arquivoSaida << "height;root;n_children\n";

    // Segunda linha: metadados da árvore
    arquivoSaida << height << ";" << root << ";" << n_children << "\n";

    // Terceira linha: títulos metadados dos nós;
    arquivoSaida << "id;type;keys;parent;children;prev;next;refs\n";

    // Reescreve as linhas de dados (nós da árvore)
    for (const auto& l : linhas) {
        arquivoSaida << l << '\n';
    }

    arquivoSaida.close();
}

void Utils::atualizarHeader(int height, int root, int n_children){
    std::ifstream arquivoEntrada("./index.txt");
    if (!arquivoEntrada.is_open()) {
        std::cerr << "Erro ao abrir o arquivo index.txt para leitura." << std::endl;
        return;
    }

    std::vector<std::string> linhas;
    std::string linha;

    while (std::getline(arquivoEntrada, linha)) {
        linhas.push_back(linha);
    }

    arquivoEntrada.close();

    if (linhas.size() < 3) {
        std::cerr << "O arquivo index.txt está incompleto (espera-se ao menos 3 linhas)." << std::endl;
        return;
    }

    // Substitui a segunda linha com o novo header
    linhas[1] = std::to_string(height) + ";" + std::to_string(root) + ";" + std::to_string(n_children);

    // Reescreve o arquivo com as alterações
    std::ofstream arquivoSaida("./index.txt", std::ios::trunc);
    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir o arquivo index.txt para escrita." << std::endl;
        return;
    }

    for (const auto& l : linhas) {
        arquivoSaida << l << '\n';
    }

    arquivoSaida.close();
}

int Utils::getAltura(){
    std::ifstream arquivo("index.txt");
    std::string linha;
    int linhaAtual = 0;

    if (arquivo.is_open()) {
        while (std::getline(arquivo, linha)) {
            linhaAtual++;
            if (linhaAtual == 2) {
                std::stringstream ss(linha);
                std::string campo;
                if (std::getline(ss, campo, ';')) {
                    return std::stoi(campo);
                }
            }
        }
        arquivo.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo.\n";
    }

    return -1; // valor de erro
}

int Utils::getRaiz(){
    std::ifstream arquivo("index.txt");
    std::string linha;
    int linhaAtual = 0;

    if (arquivo.is_open()) {
        while (std::getline(arquivo, linha)) {
            linhaAtual++;
            if (linhaAtual == 2) {
                std::stringstream ss(linha);
                std::string campo;
                std::getline(ss, campo, ';'); // pular Altura
                if (std::getline(ss, campo, ';')) {
                    return std::stoi(campo);
                }
            }
        }
        arquivo.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo.\n";
    }

    return -1; // valor de erro
}

int Utils::getFilhos(){
    std::ifstream arquivo("index.txt");
    std::string linha;
    int linhaAtual = 0;

    if (arquivo.is_open()) {
        while (std::getline(arquivo, linha)) {
            linhaAtual++;
            if (linhaAtual == 2) {
                std::stringstream ss(linha);
                std::string campo;
                std::getline(ss, campo, ';'); // pular Altura
                std::getline(ss, campo, ';'); // pular Raiz
                if (std::getline(ss, campo, ';')) {
                    return std::stoi(campo);
                }
            }
        }
        arquivo.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo.\n";
    }

    return -1; // valor de erro
}

std::vector<int> Utils::getTuplas(int ano) {
        std::ifstream arquivo("vinhos.csv");
        std::string linha;
        std::vector<int> ids;

        if (!arquivo.is_open()) {
            std::cerr << "Erro ao abrir o arquivo!" << std::endl;
            return ids;
        }

        while (std::getline(arquivo, linha)) {
            std::stringstream ss(linha);
            std::string vinho_id_str, rotulo, ano_colheita_str, tipo;

            std::getline(ss, vinho_id_str, ',');
            std::getline(ss, rotulo, ',');
            std::getline(ss, ano_colheita_str, ',');
            std::getline(ss, tipo, ',');

            try {
                int vinho_id = std::stoi(vinho_id_str);
                int ano_colheita = std::stoi(ano_colheita_str);

                if (ano_colheita == ano) {
                    ids.push_back(vinho_id);
                }
            } catch (...) {
                // Ignora linhas mal formatadas
                continue;
            }
        }

        return ids;
    }

int Utils::getNextId(){
    std::ifstream entrada("./index.txt");
    int numLinhas = 0;
    std::string temp;

    // Conta o número de linhas (para definir o novo id)
    while (std::getline(entrada, temp)) {
        numLinhas++;
    }
    entrada.close();

    return numLinhas - 3 + 1; // linha onde será inserido, começando do 1 (assumindo linha 1 = cabeçalho)
}