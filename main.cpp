#include <iostream>
#include "./src/headers/Arvore_Bp.h"
using namespace std;

int main() {
    int n_filhos = 4;
    Arvore_Bp* raiz = new Arvore_Bp(n_filhos, nullptr); // raiz inicial
    Arvore_Bp** raiz_ptr = &raiz; // ponteiro para a raiz

    // Inserções
    (*raiz_ptr)->inserir(10, false, nullptr, raiz_ptr);
    (*raiz_ptr)->inserir(10, false, nullptr, raiz_ptr);
    (*raiz_ptr)->inserir(10, false, nullptr, raiz_ptr);
    (*raiz_ptr)->inserir(10, false, nullptr, raiz_ptr);
    (*raiz_ptr)->inserir(10, false, nullptr, raiz_ptr);
    (*raiz_ptr)->inserir(10, false, nullptr, raiz_ptr);

    // Busca exemplo
    (*raiz_ptr)->busca(10);

    return 0;
}