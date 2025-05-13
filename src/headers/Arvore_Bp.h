#ifndef ARVORE_BP_H
#define ARVORE_BP_H
#include <vector>

class Arvore_Bp
{
private:
    int ordem;
    int altura;
    bool raiz;
    bool folha;
    std::vector<int> chaves;
    std::vector<Arvore_Bp*> filhos;
public:
    Arvore_Bp(int n_filhos, char* file_url); //construtor para a raiz
    Arvore_Bp(int n_filhos, bool folha); //construtor para um nó não raiz (pode ser folha ou não)
    ~Arvore_Bp();
    void inserir();
    void busca(int valor);
    void inserir_chave(int i,int chave);
    void inserir_filho(int i,Arvore_Bp* filho);
};

#endif