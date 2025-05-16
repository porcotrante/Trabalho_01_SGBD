#include "../headers/Arvore_Bp.h"
#include "../headers/Utils.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

Arvore_Bp::Arvore_Bp(int n_filhos, int tipo){ //construtor de um nó não raiz
    this->tipo = tipo;

    //alocando os vetores de chaves de busca e filhos
    this->chaves = std::vector<int>();
    this->filhos = std::vector<int>();
    this->prox_folha = -1;
    this->ant_folha = -1;
}

void Arvore_Bp::lerArquivo(int id){
    std::ifstream arquivo("./index.txt");
    std::string linha;
    int numeroLinha = 0;

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo.\n";
    }
    
    std::string linhaDesejada;

    while (std::getline(arquivo, linha)) {
        numeroLinha++;
        if (numeroLinha <= 3) continue;

        std::stringstream ss(linha);
        std::string campoIdStr;

        if (std::getline(ss, campoIdStr, ';')) {
            try {
                int campoId = std::stoi(campoIdStr);
                if (campoId == id) {
                    linhaDesejada = linha;
                    break;
                }
            } catch (std::invalid_argument& e) {
                std::cerr << "ID inválido na linha " << numeroLinha << ": " << campoIdStr << '\n';
            }
        }
    }

    if (!linhaDesejada.empty()) {
        
        std::stringstream ss(linhaDesejada);
        std::string campo;
        int anterior = -1, proximo = -1;

        // Extrair os campos (assumindo a seguinte ordem no arquivo):
        // id;tipo;chaves;filhos;pai;anterior;proximo;referencias
        std::getline(ss, campo, ';'); this->id = std::stoi(campo);
        std::getline(ss, campo, ';'); this->tipo = std::stoi(campo);
        // Chaves
        std::getline(ss, campo, ';');
        std::stringstream ssChaves(campo);
        std::string chave;
        if (campo != "_")
        {
            chaves.clear();
            while (std::getline(ssChaves, chave, ',')) {
                chaves.push_back(std::stoi(chave));
            }
        }

        // Pai
        std::getline(ss, campo, ';'); pai = std::stoi(campo);

        // Filhos
        std::getline(ss, campo, ';');
        std::stringstream ssFilhos(campo);
        std::string filho;
        if (campo != "_")
        {
            filhos.clear();
            while (std::getline(ssFilhos, filho, ',')) {
                filhos.push_back(std::stoi(filho));
            }
        }
  
        // Anterior
        std::getline(ss, campo, ';'); if (campo != "_") ant_folha = std::stoi(campo);

        // Próximo
        std::getline(ss, campo, ';'); if (campo != "_") prox_folha = std::stoi(campo);
        // Referências
        std::getline(ss, campo, ';');
        referencias.clear();

        if (campo != "_" && campo.size() >= 2 && campo.front() == '[' && campo.back() == ']') {
            std::string inner = campo.substr(1, campo.size() - 2); // remove colchetes externos

            size_t pos = 0;
            referencias.clear();
            while (pos < inner.size()) {
                if (inner[pos] == '[') {
                    size_t end = inner.find(']', pos);
                    if (end != std::string::npos) {
                        std::string grupo = inner.substr(pos + 1, end - pos - 1);
                        std::stringstream ssGrupo(grupo);
                        std::string valor;
                        std::vector<int> subgrupo;

                        while (std::getline(ssGrupo, valor, ',')) {
                            if (!valor.empty())
                                subgrupo.push_back(std::stoi(valor));
                        }

                        referencias.push_back(subgrupo);
                        pos = end + 1;
                        if (pos < inner.size() && inner[pos] == ';') pos++;
                    } else {
                        break; // erro de formato
                    }
                } else {
                    pos++;
                }
            }
        }
    }
}

void Arvore_Bp::inserirArquivo(){
    std::ifstream entrada("./index.txt");
    int numLinhas = 0;
    std::string temp;

    std::ofstream arquivo("./index.txt", std::ios::app);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita.\n";
        return;
    }

    // ID
    arquivo << this->id << ";";

    // Tipo (folha = 1 ou 0)
    arquivo << this->tipo << ";";

    // Chaves
    if (chaves.empty()) {
        arquivo << "_;";
    } else {
        for (size_t i = 0; i < chaves.size(); ++i) {
            arquivo << chaves[i];
            if (i != chaves.size() - 1)
                arquivo << ",";
        }
        arquivo << ";";
    }

    // Pai
    arquivo << pai << ";";

    // Filhos (só se não for folha)
    if (tipo == 1) {
        arquivo << "_;";
    } else {
        if (filhos.empty()) {
            arquivo << "_;";
        } else {
            for (size_t i = 0; i < filhos.size(); ++i) {
                arquivo << filhos[i];
                if (i != filhos.size() - 1)
                    arquivo << ",";
            }
            arquivo << ";";
        }
    }

    // Anterior
    if (ant_folha == -1) arquivo << "_;";
    else arquivo << ant_folha << ";";

    // Próximo
    if (prox_folha == -1) arquivo << "_;";
    else arquivo << prox_folha << ";";

    // Referências (só se for folha)
    if (referencias.empty()) {
    arquivo << "_";
    } else {
        arquivo << "[";
        for (size_t i = 0; i < referencias.size(); ++i) {
            arquivo << "[";
            for (size_t j = 0; j < referencias[i].size(); ++j) {
                arquivo << referencias[i][j];
                if (j != referencias[i].size() - 1)
                    arquivo << ",";
            }
            arquivo << "]";
        }
        arquivo << "]";
    }

    arquivo << '\n';
    arquivo.close();

    pai = 0;
    prox_folha = -1;
    ant_folha = -1;
    tipo = -1;
    id = -1;
    chaves.clear();
    filhos.clear();
    referencias.clear();
}

void Arvore_Bp::atualizarArquivo(){
    std::ifstream arquivoEntrada("./index.txt");
    if (!arquivoEntrada.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para leitura.\n";
        return;
    }

    std::vector<std::string> linhas;
    std::string linha;
    int linhaAtual = 0;

    // Lê todas as linhas
    while (std::getline(arquivoEntrada, linha)) {
        if (linhaAtual-3+1 != this->id) {
            linhas.push_back(linha); // mantém linhas que não serão modificadas
        } else {
            std::ostringstream novaLinha;

            // ID
            novaLinha << this->id << ";";

            // Tipo
            novaLinha << this->tipo << ";";

            // Chaves
            if (chaves.empty()) {
                novaLinha << "_;";
            } else {
                for (size_t i = 0; i < chaves.size(); ++i) {
                    novaLinha << chaves[i];
                    if (i != chaves.size() - 1)
                        novaLinha << ",";
                }
                novaLinha << ";";
            }

            // Pai
            novaLinha << pai << ";";

            // Filhos
            if (tipo == 1) {
                novaLinha << "_;";
            } else {
                if (filhos.empty()) {
                    novaLinha << "_;";
                } else {
                    for (size_t i = 0; i < filhos.size(); ++i) {
                        novaLinha << filhos[i];
                        if (i != filhos.size() - 1)
                            novaLinha << ",";
                    }
                    novaLinha << ";";
                }
            }

            // Anterior
            novaLinha << (ant_folha == -1 ? "_" : std::to_string(ant_folha)) << ";";
            
            novaLinha << (prox_folha == -1 ? "_" : std::to_string(prox_folha)) << ";";

            // Referências
            if (referencias.empty()) {
                novaLinha << "_";
            } else {
                novaLinha << "[";
                for (size_t i = 0; i < referencias.size(); ++i) {
                    novaLinha << "[";
                    for (size_t j = 0; j < (referencias[i]).size(); ++j) {
                        novaLinha << referencias[i][j];
                        if (j != (referencias[i]).size() - 1)
                            novaLinha << ",";
                    }
                    novaLinha << "]";
                }
                novaLinha << "]";
            }

            linhas.push_back(novaLinha.str());
        }

        linhaAtual++;
    }

    arquivoEntrada.close();

    // Reescreve o arquivo com as modificações
    std::ofstream arquivoSaida("./index.txt", std::ios::trunc);
    if (!arquivoSaida.is_open()) {
        std::cerr << "Erro ao abrir o arquivo para escrita.\n";
        return;
    }

    for (const auto& l : linhas) {
        arquivoSaida << l << '\n';
    }

    arquivoSaida.close();

    pai = 0;
    prox_folha = -1;
    ant_folha = -1;
    tipo = -1;
    id = -1;
    chaves.clear();
    filhos.clear();
    referencias.clear();
}

void Arvore_Bp::busca(int valor){
    lerArquivo(Utils::getRaiz());
    while (tipo != 1)
    {
        int i = 0;
        while (i < chaves.size() && chaves[i] <= valor)
        {
            if (chaves[i] == valor) break;
            i++;
        }
        lerArquivo(filhos[i]);
    }
    //vai recursivamente em todas as folhas
    std::ofstream arquivo("out.txt", std::ios::app); // Abre o arquivo de saída
    if (arquivo.is_open()) {
        int resultado = Arvore_Bp::buscaRec(valor);
        arquivo << "BUS=:" << valor << "/" << resultado << std::endl; // Escreve o resultado no formato especificado
        arquivo.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo de saída." << std::endl;
    }
}

int Arvore_Bp::buscaRec(int valor){
        int count = 0;
        for (int i = 0; i < chaves.size(); i++)
        {
            if (chaves[i] == valor)
            {
                count += (referencias[i]).size(); //contando quantas tuplas tem a chave buscada
            }
        }
        
        return count;
}

void Arvore_Bp::inserir(int valor) {

    int raiz = Utils::getRaiz();
    int altura = Utils::getAltura();
    int n_filhos = Utils::getFilhos();

    lerArquivo(raiz);
    while (tipo != 1)
    {
        int i = 0;
        while (i < chaves.size() && chaves[i] <= valor)
        {
            if (chaves[i] == valor) break;
            i++;
        }
        lerArquivo(filhos[i]);
    }

    //inserção na folha
    auto it = chaves.begin();
    while (it != chaves.end() && *it <= valor) it++;
    // calcular o índice 

    int index = std::distance(chaves.begin(), it);
    auto tuplas = Utils::getTuplas(valor);

    // insere por índice nos dois vetores sincronizados
    chaves.insert(chaves.begin() + index, valor);
    referencias.insert(referencias.begin() + index, tuplas);

    std::ofstream arquivo("out.txt", std::ios::app);
    if (arquivo.is_open()) {
        arquivo << "INC:" << valor << "/"<< tuplas.size() << std::endl;
        arquivo.close();
    } else {
        std::cerr << "Erro ao abrir o arquivo de saída." << std::endl;
    }
    

    if (chaves.size() > n_filhos - 1) //se passar do limite, split
    {
        int mid = chaves.size() / 2;

        // Cria novo nó folha (irmão)
        Arvore_Bp* irmao = new Arvore_Bp(n_filhos, 1); //TODO: criando o objeto auxiliar irmão
        irmao->id = Utils::getNextId();
        irmao->pai = pai;
        irmao->prox_folha = prox_folha; //o irmão herda o prox_nó desse nó
        prox_folha = irmao->id; //o próximo nó deste nó agora é o irmão
        irmao->ant_folha = this->id; //o nó anterior do irmão aponta para esse nó

        // Move metade superior das chaves para o irmão
        irmao->chaves.assign(chaves.begin() + mid, chaves.end());
        irmao->referencias.assign(referencias.begin() + mid, referencias.end());
        chaves.erase(chaves.begin() + mid, chaves.end());
        referencias.erase(referencias.begin() + mid, referencias.end());

        int paiId = pai;
        int thisId = this->id;
        int irmaoId = irmao->id;
        int chave_promovida = irmao->chaves.front();

        atualizarArquivo();
        irmao->inserirArquivo();
        delete irmao; //destruindo o objeto irmão

        if (thisId != raiz)
        {
            lerArquivo(paiId);
            inserirNaoFolha(chave_promovida, irmaoId, raiz, n_filhos, altura);
        }
        else {
            chaves.insert(chaves.begin(), chave_promovida);
            filhos.insert(filhos.begin(), thisId);
            filhos.insert(std::next(filhos.begin()), irmaoId);
            id = irmaoId + 1;
            tipo = 0;
            pai = 0;

            int rootId = id;
            raiz = rootId;
            altura++;
            Utils::atualizarHeader(altura, raiz, n_filhos);

            inserirArquivo();

            lerArquivo(thisId);
            pai = rootId;
            atualizarArquivo();

            lerArquivo(irmaoId);
            pai = rootId;
            atualizarArquivo();
        }
    }
    else{
        atualizarArquivo();
    }
}

void Arvore_Bp::inserirNaoFolha(int valor, int split_filho, int raiz, int n_filhos, int altura) {
    // Inserção após split de filho
    int i = 0;
    while (i < chaves.size() && chaves[i] <= valor) i++;
    chaves.insert(chaves.begin() + i, valor);
    filhos.insert(filhos.begin() + i + 1, split_filho);

    if (chaves.size() > n_filhos - 1) //se passar do limite, split
    {
        int mid = chaves.size() / 2;

        Arvore_Bp* irmao = new Arvore_Bp(n_filhos, 0); //TODO: criando o objeto auxiliar irmão
        irmao->id = Utils::getNextId();
        irmao->pai = pai;

        // Move metade superior das chaves para o irmão
        irmao->chaves.assign(chaves.begin() + mid, chaves.end());
        irmao->filhos.assign(filhos.begin() + mid, filhos.end());
        chaves.erase(chaves.begin() + mid, chaves.end());
        filhos.erase(filhos.begin() + mid, filhos.end());

        int paiId = pai;
        int thisId = this->id;
        int irmaoId = irmao->id;
        int chave_promovida = irmao->chaves.front();

        atualizarArquivo();
        irmao->inserirArquivo();
        delete irmao;

        if (thisId != raiz)
        {
            lerArquivo(paiId);
            inserirNaoFolha(chave_promovida, irmaoId, raiz, n_filhos, altura);
        }
        else {
            chaves.insert(chaves.begin(), chave_promovida);
            filhos.insert(filhos.begin(), thisId);
            filhos.insert(std::next(filhos.begin()), irmaoId);
            id = irmaoId + 1;
            tipo = 0;
            pai = 0;

            int rootId = id;
            raiz = rootId;
            altura++;
            Utils::atualizarHeader(altura, raiz, n_filhos);

            inserirArquivo();

            lerArquivo(thisId);
            pai = rootId;
            atualizarArquivo();

            lerArquivo(irmaoId);
            pai = rootId;
            atualizarArquivo();
        }
    }else{
        atualizarArquivo();
    }
    
}

Arvore_Bp::Arvore_Bp(int n_filhos){ //construtor da raiz
    //alocando as configurações do nó
    this->tipo = 1;
    Utils::construirHeader(0, 1, n_filhos);

    //alocando os vetores de chaves de busca e filhos
    this->chaves = std::vector<int>();
    this->filhos = std::vector<int>();
    this->referencias = std::vector<std::vector<int>>();

    this->id = 1;
    this->pai = 0;
    this->prox_folha = -1;
    this->ant_folha = -1;

    inserirArquivo();

    //TODO: Fazer as inserções automaticamente a partir do arquivo
    //as inserções modificam por referência um ponteiro raiz_ptr para a raiz externo (ver como é a main), para fazer essas inserções sucessivas, talvez seja melhor torná-lo um campo da classe? Fica ao seu critério Marilu
}

Arvore_Bp::~Arvore_Bp(){}