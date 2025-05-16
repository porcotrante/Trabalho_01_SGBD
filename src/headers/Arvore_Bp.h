#ifndef ARVORE_BP_H
#define ARVORE_BP_H
#include <vector>

class Arvore_Bp
{
public:
    std::vector<int> chaves; //TODO: guardar no arquivo e não aqui
    std::vector<int> filhos; //TODO: guardar no arquivo e não aqui
    std::vector<std::vector<int>> referencias; //TODO: guardar no arquivo e não aqui
    int pai; //TODO: guardar no arquivo e não aqui
    int prox_folha; //TODO: guardar no arquivo e não aqui
    int ant_folha; //TODO: guardar no arquivo e não aqui
    int tipo; //0 para normal, 1 para folha
    int id;
    Arvore_Bp(int n_filhos); //construtor para a raiz
    Arvore_Bp(int n_filhos, int tipo); //construtor para um nó não raiz (pode ser folha ou não)
    ~Arvore_Bp();
    void inserir(int valor);
    void inserirNaoFolha(int valor, int split_filho, int raiz, int n_filhos, int altura);
    void busca(int valor);
    int buscaRec(int valor);
    void lerArquivo(int id);
    void inserirArquivo();
    void atualizarArquivo();
};

#endif