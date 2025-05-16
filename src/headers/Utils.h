#include <vector>
#ifndef UTILS_H
#define UTILS_H


class Utils
{
public:
    static void construirHeader(int height, int root, int n_children);
    static void atualizarHeader(int height, int root, int n_children);
    static int getAltura();
    static int getRaiz();
    static int getFilhos();
    static std::vector<int> getTuplas(int ano);
    static int getNextId();
};

#endif