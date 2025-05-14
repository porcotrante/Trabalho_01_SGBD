#ifndef ARVORE_BP_H
#define ARVORE_BP_H
#include <vector>

class Arvore_Bp
{
public:
    std::vector<int> chaves; //TODO: guardar no arquivo e não aqui
    std::vector<Arvore_Bp*> filhos; //TODO: guardar no arquivo e não aqui
    Arvore_Bp* pai; //TODO: guardar no arquivo e não aqui
    Arvore_Bp* prox_folha; //TODO: guardar no arquivo e não aqui
    Arvore_Bp* ant_folha; //TODO: guardar no arquivo e não aqui
    bool raiz;
    int altura;
    float ordem;
    bool folha;
    Arvore_Bp(int n_filhos, char* file_url); //construtor para a raiz
    Arvore_Bp(int n_filhos, bool folha); //construtor para um nó não raiz (pode ser folha ou não)
    ~Arvore_Bp();
    void inserir(int valor, bool split, Arvore_Bp* filho_split, Arvore_Bp** raiz_ptr);
    void busca(int valor);
    int buscaRec(int valor);
};

#endif