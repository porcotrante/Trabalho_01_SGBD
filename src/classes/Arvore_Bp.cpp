#include "../headers/Arvore_Bp.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

Arvore_Bp::Arvore_Bp(int n_filhos, bool folha){ //construtor de um nó não raiz
    this->ordem = (n_filhos - 1)/2.0;
    this->raiz = false;
    this->folha = folha;

    //alocando os vetores de chaves de busca e filhos
    this->chaves = std::vector<int>();
    this->filhos = std::vector<Arvore_Bp*>();
    this->prox_folha = NULL;
    this->ant_folha = NULL;
}

void Arvore_Bp::lerArquivo(int id){
    std::ifstream arquivo("./index.txt");
    std::string linha;
    int numeroLinha = 0;

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo.\n";
    }
    
    std::string linhaEncontrada;

    while (std::getline(arquivo, linha)) {
        numeroLinha++;
        if (numeroLinha == 1) continue;

        std::stringstream ss(linha);
        std::string campoIdStr;

        if (std::getline(ss, campoIdStr, ';')) {
            try {
                int campoId = std::stoi(campoIdStr);
                if (campoId == id) {
                    linhaEncontrada = linha;
                    break;
                }
            } catch (std::invalid_argument& e) {
                std::cerr << "ID inválido na linha " << numeroLinha << ": " << campoIdStr << '\n';
            }
        }
    }
}

void Arvore_Bp::busca(int valor){
    if (folha)
    {   //vai recursivamente em todas as folhas
        //cout << Arvore_Bp::buscaRec(valor) << endl; //TODO: Escrever no arquivo de retorno ao invés de printar
            std::ofstream arquivo("out.txt", std::ios::app); // Abre o arquivo de saída
            if (arquivo.is_open()) {
                int resultado = Arvore_Bp::buscaRec(valor);
                arquivo << "BUS=:" << valor << "/" << resultado << std::endl; // Escreve o resultado no formato especificado
                arquivo.close();
            } else {
                std::cerr << "Erro ao abrir o arquivo de saída." << std::endl;
            }
        
    }
    else {
        int i = 0;
        while (i < chaves.size() && chaves[i] <= valor)
        {
            if (chaves[i] == valor) break;
            i++;
        }
        if (chaves[i] == valor && filhos[i]->chaves.back() != valor) i++; //para tratar casos onde há mais chaves iguais do que espaços em um nó
             
        filhos[i]->busca(valor);
    }
}

int Arvore_Bp::buscaRec(int valor){
        int count = 0;
        for (int i = 0; i < chaves.size(); i++)
        {
            if (chaves[i] == valor)
            {
                count++; //contando quantas tuplas tem a chave buscada
            }
        }
        if (prox_folha != NULL)
        {
            count += prox_folha->buscaRec(valor);
        }
        return count;
}

void Arvore_Bp::inserir(int valor, bool split, Arvore_Bp* filhoSplit, Arvore_Bp** raiz_ptr) {
    if (folha) {
        // Inserção em folha
        auto it = chaves.begin();
        while (it != chaves.end() && *it <= valor) it++;
        chaves.insert(it, valor); //TODO: Escrever no arquivo de retorno
        std::ofstream arquivo("out.txt", std::ios::app);
        if (arquivo.is_open()) {
            arquivo << "INC:" << valor << "/1" << std::endl; // Sempre insere 1 tupla
            arquivo.close();
        } else {
            std::cerr << "Erro ao abrir o arquivo de saída." << std::endl;
        }
        // Se houver overflow, fazer split
        if (chaves.size() > ordem * 2) {
            int mid = chaves.size() / 2;

            // Cria novo nó folha (irmão)
            Arvore_Bp* irmao = new Arvore_Bp(ordem * 2 + 1, true); //TODO: escrever no arquivo de informações do índicie
            irmao->pai = pai;
            irmao->prox_folha = prox_folha; //o irmão herda o prox_nó desse nó
            prox_folha = irmao; //o próximo nó deste nó agora é o irmão
            irmao->ant_folha = this; //o nó anterior do irmão aponta para esse nó

            // Move metade superior das chaves para o irmão
            irmao->chaves.assign(chaves.begin() + mid, chaves.end());
            chaves.erase(chaves.begin() + mid, chaves.end());

            int chave_promovida = irmao->chaves.front(); // em B+ promove-se a menor do irmão

            if (pai) {
                pai->inserir(chave_promovida, true, irmao, raiz_ptr);
            } else {
                // Criar nova raiz
                Arvore_Bp* nova_raiz = new Arvore_Bp(ordem * 2 + 1, false); //TODO: escrever no arquivo de informações do índicie
                *raiz_ptr = nova_raiz;
                nova_raiz->altura = altura+1;
                nova_raiz->chaves.push_back(chave_promovida);
                nova_raiz->filhos.push_back(this);
                nova_raiz->filhos.push_back(irmao);
                this->pai = nova_raiz;
                irmao->pai = nova_raiz;
                nova_raiz->raiz = true;
                this->raiz = false;
            }
        }
    } else if (!split) {
        // Inserção comum em nó interno
        int i = 0;
        while (i < chaves.size() && chaves[i] <= valor) i++;
        filhos[i]->inserir(valor, false, nullptr, raiz_ptr);
    } else {
        // Inserção após split de filho
        int i = 0;
        while (i < chaves.size() && chaves[i] <= valor) i++;
        chaves.insert(chaves.begin() + i, valor);
        filhos.insert(filhos.begin() + i + 1, filhoSplit);
        filhoSplit->pai = this;

        // Se overflow, faz split também
        if (chaves.size() > ordem * 2) {
            int mid = chaves.size() / 2;
            int chave_promovida = chaves[mid];

            // Cria nó irmão
            Arvore_Bp* irmao = new Arvore_Bp(ordem * 2 + 1, false);
            irmao->pai = pai; //TODO: escrever no arquivo de informações do índicie

            // Move metade superior das chaves e filhos
            irmao->chaves.assign(chaves.begin() + mid + 1, chaves.end());
            irmao->filhos.assign(filhos.begin() + mid + 1, filhos.end());

            // Atualiza pai dos filhos do novo nó
            for (auto f : irmao->filhos) {
                if (f) f->pai = irmao;
            }

            // Remove do nó atual
            chaves.erase(chaves.begin() + mid, chaves.end());
            filhos.erase(filhos.begin() + mid + 1, filhos.end());

            if (pai) {
                pai->inserir(chave_promovida, true, irmao, raiz_ptr);
            } else {
                // Criar nova raiz
                Arvore_Bp* nova_raiz = new Arvore_Bp(ordem * 2 + 1, false); //TODO: escrever no arquivo de informações do índicie
                *raiz_ptr = nova_raiz;
                nova_raiz->altura = altura+1;
                nova_raiz->chaves.push_back(chave_promovida);
                nova_raiz->filhos.push_back(this);
                nova_raiz->filhos.push_back(irmao);
                this->pai = nova_raiz;
                irmao->pai = nova_raiz;
                nova_raiz->raiz = true;
                this->raiz = false;
                
            }
        }
    }
}


Arvore_Bp::Arvore_Bp(int n_filhos, char* file_url){ //construtor da raiz
    //alocando as configurações do nó
    this->ordem = (n_filhos - 1)/2.0;
    this->raiz = true;
    this->folha = true;

    //alocando os vetores de chaves de busca e filhos
    this->chaves = std::vector<int>();
    this->filhos = std::vector<Arvore_Bp*>();

    this->pai = NULL;
    this->prox_folha = NULL;
    this->ant_folha = NULL;
    this->altura = 0;

    //TODO: Fazer as inserções automaticamente a partir do arquivo
    //as inserções modificam por referência um ponteiro raiz_ptr para a raiz externo (ver como é a main), para fazer essas inserções sucessivas, talvez seja melhor torná-lo um campo da classe? Fica ao seu critério Marilu
}

Arvore_Bp::~Arvore_Bp(){}