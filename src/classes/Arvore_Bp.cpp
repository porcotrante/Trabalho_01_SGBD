#include "../headers/Arvore_Bp.h"
#include <iostream>
#include <vector>

Arvore_Bp::Arvore_Bp(int n_filhos, bool folha){ //construtor de um nó não raiz
    this->ordem = (n_filhos - 1)/2;
    this->raiz = false;
    this->folha = folha;

    //alocando os vetores de chaves de busca e filhos
    this->chaves = std::vector<int>(); //-1 representa uma chave inválida, pois todos os anos sçao maiores q 0
    this->filhos = std::vector<Arvore_Bp*>();
}

void Arvore_Bp::inserir_chave(int i, int chave){
    this->chaves[i] = chave;
}

void Arvore_Bp::inserir_filho(int i, Arvore_Bp* filho){
    this->filhos[i] = filho;
}

void Arvore_Bp::busca(int valor){
    if (this->folha){
        int count = 0;
        for (int i = 0; i < this->chaves.size(); i++)
        {
            if (this->chaves[i] == valor)
            {
                
            }
            
        }
        
    }
}

Arvore_Bp::Arvore_Bp(int n_filhos, char* file_url){ //construtor da raiz
    //alocando as configurações do nó
    this->ordem = (n_filhos - 1)/2;
    this->raiz = true;
    this->folha = false;

    //alocando os vetores de chaves de busca e filhos
    this->chaves = std::vector<int>();
    this->filhos = std::vector<Arvore_Bp*>();
    
}

Arvore_Bp::~Arvore_Bp(){}