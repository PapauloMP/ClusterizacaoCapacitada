//
// Created by Marcos Paulo on 07/08/2022.
//

#include <iostream>
#include <sstream>
#include "../Headers/Graph.h"
#include "../Headers/Menu.h"

Menu::Menu(Graph *graph, const string& outputPath, float* alfa){
    this->outputFile.open(outputPath);
    this->graph = graph;
    this->graph->indexId();
    this->alfa = alfa;
}

void Menu::start(){
    bool hold = true;

    while(hold){
        hold = this->UI();
    }

    this->outputFile.close();
}

bool Menu::UI(){
    int option;

    cout << "MENU" << endl;
    cout << "----" << endl;
    cout << "[1] Algoritmo guloso" << endl;
    cout << "[2] Algoritmo guloso randomizado adaptativo" << endl;
    cout << "[3] Algoritmo guloso randomizado adaptativo reativo" << endl;
    cout << "[0] Sair" << endl;
    cout << "Digite uma opcao: ";

    cin >> option;

    if(option == 0)
        return false;

    selectMenu(option);

    return true;
}

void Menu::selectMenu(int option) {

    switch (option) {
        case 1: {
            guloso();
            break;
        }
        case 2: {
            gulosoRand();
            break;
        }
        case 3: {
            gulosoRandReativo();
            break;
        }
        default: {
            cout << "Erro!" << endl << "Opcao invalida." << endl;
        }

    }
}

void Menu::guloso() {
    Cluster* c = this->graph->guloso();
    printCCP(c , this->graph->getNumClusters(), "GULOSO");
}
void Menu::gulosoRand() {
    Cluster* c = this->graph->gulosoRand(0.5, 10);
    printCCP(c , this->graph->getNumClusters(), "GULOSO RANDOMIZADO");
}
void Menu::gulosoRandReativo() {
    Cluster* c = this->graph->gulosoRandReativo(alfa, 20, 10, 5);
    printCCP(c , this->graph->getNumClusters(), "GULOSO RANDOMIZADO REATIVO");
}
void Menu::printCCP(Cluster *bestSol, int numClusters, string type){

    int cont = 0;
    outputFile << "MELHOR SOLUÇÃO (" << type << "):" << endl;
    for (int i = 0; i < numClusters; ++i) {
        outputFile << "Cluster " << i << ": ";
        for (ClusterNode *c = bestSol[i].getPrimeiro(); c != nullptr; c = c->getProx()){
            if(cont == 50)
                outputFile << endl;

            Node* n = this->graph->getNode(c->getId());
            outputFile << n->getIdExternal() << " ";
            cont++;
        }
        outputFile << endl;
    }
    outputFile << endl;

    float quality = 0;
    for (int i = 0; i < numClusters; ++i) {
        quality += bestSol[i].getSumEdges();
    }
    outputFile << "QUALIDADE: " << quality << endl << endl;

}

