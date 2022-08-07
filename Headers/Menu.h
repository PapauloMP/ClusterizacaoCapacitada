//
// Created by Marcos Paulo on 07/08/2022.
//

#ifndef GRAFOST2_MENU_H
#define GRAFOST2_MENU_H
#include <fstream>
#include "Graph.h"

using namespace std;

class Menu {
public:
    Menu(Graph *graph, const string& outputPath, float* alfa);

    bool UI();
    void start();
    void printGraph();
    void printEdgeFromGraph(Graph *graphSource);
    ///CCP
    void guloso();
    void gulosoRand();
    void gulosoRandReativo();

private:

    ofstream outputFile;
    Graph* graph;
    float* alfa;
    ///Menu
    void selectMenu(int option);

    void printCCP(Cluster* bestSol, int numClusters, string type);
};
#endif //GRAFOST2_MENU_H
