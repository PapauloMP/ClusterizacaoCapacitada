#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include "Headers/Node.h"
#include "Headers/Graph.h"
#include "Menu.h"


vector<EdgeStruct>* readTxt(Graph* graph, const string& path, bool isWeighted, bool isWeightedNode);
vector<EdgeStruct>* loadGraph(Graph* graph, const string& inputFilePath,bool isWeightedEdge, bool isWeightedNode);
void printGraph(Graph* grafo, vector<EdgeStruct>* ListEdge);

int main(int argc, char** argv) {

    /*if(argc != 4){
        cout << "ERRO: parametros nao correspondem com a sintaxe do comando" << endl;
        cout << "Sintaxe correta: ./exec <arquivo_entrada> <arquivo_saida> <Tipo_Instancia>" << endl;
        abort();
    }

    string inputFilePath = argv[1];
    string outputFilePath = argv[2];
    string type = (argv[3]);*/

    auto start = std::chrono::steady_clock::now();

    string inputFilePath = "../RanReal240_01.txt";
    string outputFilePath = "../OutputCCP2.txt";
    //ofstream outputFile;
    //cout << "A: " << outputFile.is_open();
    //outputFile.open(outputFilePath, ios::out);
    //cout << "A: " << outputFile.is_open();

    bool isDirected = false;
    bool isWeightedEdge = true;
    bool isWeightedNode = true;

    auto* grafo = new Graph(isDirected, isWeightedEdge, isWeightedNode);

    loadGraph(grafo, inputFilePath, isWeightedEdge, isWeightedNode);


    srand(time(NULL));
    float alfa[5] = {0.05, 0.10, 0.15, 0.30, 0.50};
    for (int i = 0; i < 5; i++) {
        cout << alfa[i] << " ";
    }
    cout << endl;

    Menu* menu = new Menu(grafo, outputFilePath, alfa);
    menu->start();

    //menu->guloso();
    //menu->gulosoRand();
    //menu->gulosoRandReativo();

    auto end = std::chrono::steady_clock::now();
    auto elapsed  = end - start;

    cout << endl;
    cout << "TEMPO DE PROCESSAMENTO TOTAL: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count() << "ms" << endl;


    return 0;
}

vector<EdgeStruct>* loadGraph(Graph* graph, const string& inputFilePath, bool isWeighted, bool isWeightedNode){
    auto ListEdge = readTxt(graph, inputFilePath, isWeighted, isWeightedNode);
    string edgeShape = graph->getDirected() ? "->" : "--";

    if(isWeighted) {
        for (auto &i: *ListEdge) {
            cout << i.fromNode << edgeShape << i.toNode << " = " << i.weight << endl;
            graph->insertEdge(i.fromNode, i.toNode, i.weight);
        }
    }
    else{
        for (auto& i : *ListEdge){
            cout << i.fromNode << edgeShape << i.toNode << " = " << 1 << endl;
            graph->insertEdge(i.fromNode, i.toNode, 1);
        }
    }
    return ListEdge;
}

vector<EdgeStruct>* readTxt(Graph* graph, const string& path, bool isWeighted, bool isWeightedNode){
    try {
        auto ListEdge = new vector<EdgeStruct>();

        ifstream file;
        file.open(path);

        if(file.is_open()){
            ///ler o cabeçalho
            string line;
            getline(file, line);
            stringstream header(line);

            string auxOrder;
            string n;
            string clusterType;

            getline(header, auxOrder, ' ');
            getline(header, n, ' ');
            getline(header, clusterType, ' ');

            int order = stoi(auxOrder);
            int numClusters = stoi(n);
            float* lowerLimit = new float[numClusters];
            float* higherLimit = new float[numClusters];
            string centinel;

            string aux;
            int i = 0, j = 0;
            getline(header, aux, ' ');
            while(aux != "W"){ ///ler limites
                if(i % 2 == 0) {
                    lowerLimit[j] = stof(aux);
                }
                else {
                    higherLimit[j] = stof(aux);
                    j++;
                }
                i++;
                getline(header, aux, ' ');
            }
            centinel = aux;

            float *nodeWeights = new float[order];
            ///ler pesos dos nos
            if (isWeightedNode) {
                //cout << "Peso dos nos: ";
                getline(header, line, ' ');
                for (int k = 0; k < order; ++k) {
                    nodeWeights[k] = stof(line);
                    getline(header, line, ' ');
                }
                //cout << endl << endl;
            }

            ///ler o grafo
            auto edge = EdgeStruct();
            string toNode, fromNode, weight;

            while (getline(file, line)){

                stringstream ss(line);

                getline(ss, fromNode, ' ');
                getline(ss, toNode, ' ');

                if(isWeighted){
                    getline(ss, weight, ' ');
                    edge.weight = stof(weight);
                }

                edge.fromNode = fromNode;
                edge.toNode = toNode;

                ListEdge->push_back(edge);
            }

            graph->setNumClusters(numClusters);
            graph->setClusterType(clusterType);
            graph->setLowerLimits(lowerLimit);
            graph->setHigherLimits(higherLimit);
            graph->setNodeWeights(nodeWeights);
            ///TESTES
            cout << "Ordem: " << order << endl;
            cout << "Numero de clusters: " << graph->getNumClusters() << endl;
            cout << "ClusterType: " << graph->getClusterType() << endl;
            cout << "Centinel: " << centinel << endl;

            float* l = graph->getLowerLimits();
            float* h = graph->getHigherLimits();
            float* nw = graph->getNodeWeights();
            cout << "Limites: ";
            for (int k = 0; k < numClusters; k++) {
                cout << l[k] << "-" << h[k] << " ";
            }
            cout << endl;
            cout << "Peso dos nos: ";
            for (int k = 0; k < order; ++k) {
                cout << nw[k] << " ";
            }
            cout << endl << endl;


            return ListEdge;
        }else{
            cout << "Erro ao tentar abrir o arquivo: " << path;
            abort();
        }
    } catch (exception &e) {

    }
    return nullptr;
}

void printGraph(Graph* grafo, vector<EdgeStruct>* ListEdge){
    for (auto& i : *ListEdge){
        cout << i.fromNode << " -> " << i.toNode << " = " << i.weight << endl;
        grafo->insertEdge(i.fromNode, i.toNode, i.weight);
    }
}