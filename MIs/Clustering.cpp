//
// Created by Marcos Paulo on 31/07/2022.
//

#include "../Headers/Graph.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include "../Headers/Cluster.h"

using namespace std;

bool compareNodes(const ClusterNode& node1, const ClusterNode& node2){
    return node1.getValue() > node2.getValue();
}

int getRandomIndex(float alfa, int order){
    return (int)((rand() % order)*alfa);
}

int getIndex(float* probabilities,int numAlfas){
    int alfa = -1;
    float random;
    float infLimit;
    float upperLimit;

    random = (float)(rand() % 100);
    for (int i = 0; i < numAlfas; ++i) {
        if(i == 0) {
            infLimit = 0;
            upperLimit = probabilities[0];
        }
        else {
            infLimit += probabilities[i - 1];
            upperLimit += probabilities[i];
        }
        if(random >= (infLimit*100) && random <= (upperLimit*100)){
            alfa = i;
            break;
        }
    }

    return alfa;
}

void updateProbabilities(float *probabilities, float* alfaAverage, float qualityBestSol, int numAlfas){
    //cout << "CALCULANDO PROBABILIDADES " << endl;
    float sumP = 0;
    for (int i = 0; i < numAlfas; ++i) {
        float q = qualityBestSol / alfaAverage[i];
        probabilities[i] = q;
        sumP += probabilities[i];
    }
    for (int i = 0; i < numAlfas; ++i) {
        probabilities[i] = probabilities[i] / sumP;
    }
}

vector<ClusterNode> Graph::orderVertices() {
    vector<ClusterNode> vertices;

    for (Node* node = this->getFirstNode(); node != nullptr; node = node->getNextNode()){
        float sum = 0;
        int numEdges = 0;
        for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            sum += edge->getWeight();
            numEdges++;
        }
        vertices.emplace_back(ClusterNode(node->getId(),node->getIdExternal(), node->getWeight(), sum, numEdges));

    }

    sort(vertices.begin(), vertices.end(), compareNodes);

    cout << "Vetor ordenado: ";
    for (auto & vertice : vertices){
        cout << vertice.getExternalId() << " ";
    }
    cout << endl;


    return vertices;
}

Cluster* Graph::guloso() {

    int order = this->getOrder();
    if (numClusters > order) {
        cout << "ERRO: numero de clusters maior que o numero de vertices" << endl;
        abort();
    }

    auto vertices = orderVertices();
    float *lowerLimits = this->getLowerLimits();
    float *higherLimits = this->getHigherLimits();
    Cluster *clusters = new Cluster[numClusters];

    for (int i = 0; i < numClusters; i++){
        clusters[i].setLowerLimit(lowerLimits[i]);
        clusters[i].setHigherLimit(higherLimits[i]);
        /*cout << "Limites: " << endl;
        cout << clusters[i].getLowerLimit() << " " << clusters[i].getHigherLimit() << endl;*/
    }


    for (int i = 0; i < order; i++) {
        Node *n = this->getNode(vertices.at(i).getId());
        int bestCluster = -1;
        float auxValue = 0;
        float value = 0;
        float auxLimit = 0;
        float sum;
        int j;

        for (j = 0; j < numClusters; j++) {

            if (clusters[j].getPrimeiro() == nullptr) {
                float v = clusters[j].getHigherLimit() - n->getWeight();
                if (v > auxValue) {
                    value = 0;
                    bestCluster = j;
                    auxValue = v;
                }
            } else {
                sum = 0;
                if(clusters[j].getActualLimit() + n->getWeight() <= clusters[j].getHigherLimit()) {
                    for (ClusterNode *c = clusters[j].getPrimeiro(); c != nullptr; c = c->getProx()) {
                        Edge *edge = n->hasEdgeBetween(c->getId());
                        if (edge != nullptr){
                            sum += edge->getWeight();
                        }
                    }
                }
                float lower = clusters[j].getLowerLimit();
                float actual = clusters[j].getActualLimit();
                if (actual == lower - 1 && lower >= 5)
                    actual = lower - 3;
                if (actual >= lower)
                    actual = lower - 1;
                float critery = lower - actual;
                float auxSum = sum * (critery*critery);


                if (auxSum > auxLimit) {
                    auxLimit = auxSum;
                    value = sum;
                    bestCluster = j;
                }
            }
        }
        if(bestCluster == -1){
            float indBestCluster = 0;
            for (j = 0; j < numClusters; j++) {
                float lower = clusters[j].getLowerLimit();
                float actual = clusters[j].getActualLimit();
                if((lower - actual) > indBestCluster) {
                    indBestCluster = lower - actual;
                    bestCluster = j;
                }
            }
            if(indBestCluster == 0){
                for (j = 0; j < numClusters; j++) {
                    float higher = clusters[j].getHigherLimit();
                    float actual = clusters[j].getActualLimit();
                    if((higher - actual) > indBestCluster) {
                        indBestCluster = higher - actual;
                        bestCluster = j;
                    }
                }
            }
        }
        /*cout << "Value=" << value << " ";
        cout << "Inserindo ";
        cout << "NO: " << n->getIdExternal() << "- Peso: " << n->getWeight();
        cout << " no cluster " << bestCluster << " Soma parcial: ";*/
        clusters[bestCluster].insereFinal(n->getId(), n->getWeight(), value);
        //cout << clusters[bestCluster].getSumEdges() << endl;
    }

    float quality = 0;
    //cout << "Soma final de cada cluster: " << endl;
    for (int i = 0; i < numClusters; ++i) {
        /*cout << "Cluster[" << i << "] = " << clusters[i].getSumEdges() << endl;
        cout << "Limite minimo: " << clusters[i].getLowerLimit() << endl;
        cout << "Limite final: " << clusters[i].getActualLimit() << endl << endl;*/
        quality += clusters[i].getSumEdges();
    }
    cout << "QUALIDADE: " << quality << endl;


    return clusters;
}

Cluster* Graph::gulosoRand(float alfa, int numInter){

    int order = this->getOrder();
    if (numClusters > order) {
        cout << "ERRO: numero de clusters maior que o numero de vertices" << endl;
        abort();
    }

    auto vertices = orderVertices();
    float *lowerLimits = this->getLowerLimits();
    float *higherLimits = this->getHigherLimits();
    Cluster *bestSol;
    float qualitySol = 0;

    for (int x = 0; x < numInter; x++) {
        Cluster *clusters = new Cluster[numClusters];
        auto verticesCopy = vertices;

        for (int i = 0; i < numClusters; i++){
            clusters[i].setLowerLimit(lowerLimits[i]);
            clusters[i].setHigherLimit(higherLimits[i]);
            //cout << "Limites: " << endl;
            //cout << clusters[i].getLowerLimit() << " " << clusters[i].getHigherLimit() << endl;
        }

        for (int i = 0; i < order; i++) {
            int cn = getRandomIndex(alfa, order-i);
            Node *n = this->getNode(verticesCopy.at(cn).getId());
            verticesCopy.erase(verticesCopy.begin() + cn);
            int bestCluster = -1;
            float auxValue = 0;
            float value = 0;
            float auxLimit = 0;
            float sum;
            int j;

            for (j = 0; j < numClusters; j++) {

                if (clusters[j].getPrimeiro() == nullptr) {
                    float v = clusters[j].getHigherLimit() - n->getWeight();
                    if (v > auxValue) {
                        value = 0;
                        bestCluster = j;
                        auxValue = v;
                    }
                } else {
                    sum = 0;
                    if(clusters[j].getActualLimit() + n->getWeight() <= clusters[j].getHigherLimit()) {
                        for (ClusterNode *c = clusters[j].getPrimeiro(); c != nullptr; c = c->getProx()) {
                            Edge *edge = n->hasEdgeBetween(c->getId());
                            if (edge != nullptr){
                                sum += edge->getWeight();
                            }
                        }
                    }
                    float lower = clusters[j].getLowerLimit();
                    float actual = clusters[j].getActualLimit();
                    if (actual == lower - 1 && lower >= 5)
                        actual = lower - 3;
                    if (actual >= lower)
                        actual = lower - 1;
                    float critery = lower - actual;
                    float auxSum = sum * (critery*critery);

                    if (auxSum > auxLimit) {
                        auxLimit = auxSum;
                        value = sum;
                        bestCluster = j;
                    }
                }
            }
            if(bestCluster == -1){
                float indBestCluster = 0;
                for (j = 0; j < numClusters; j++) {
                    float lower = clusters[j].getLowerLimit();
                    float actual = clusters[j].getActualLimit();
                    if((lower - actual) > indBestCluster) {
                        indBestCluster = lower - actual;
                        bestCluster = j;
                    }
                }
                if(indBestCluster == 0){
                    for (j = 0; j < numClusters; j++) {
                        float higher = clusters[j].getHigherLimit();
                        float actual = clusters[j].getActualLimit();
                        if((higher - actual) > indBestCluster) {
                            indBestCluster = higher - actual;
                            bestCluster = j;
                        }
                    }
                }
            }
            clusters[bestCluster].insereFinal(n->getId(), n->getWeight(), value);
        }

        float quality = 0;
        //cout << "Soma final de cada cluster: " << endl;
        for (int i = 0; i < numClusters; ++i) {
            /*cout << "Cluster[" << i << "] = " << clusters[i].getSumEdges() << endl;
            cout << "Limite minimo: " << clusters[i].getLowerLimit() << endl;
            cout << "Limite final: " << clusters[i].getActualLimit() << endl << endl;*/
            quality += clusters[i].getSumEdges();
        }
        cout << "QUALIDADE: " << quality << endl;

        if(quality > qualitySol){
            qualitySol = quality;
            bestSol = clusters;
        }else{
            delete [] clusters;
        }

    }
    cout << endl;
    cout << "SOLUCAO FINAL:" << endl;
    cout << "Melhor cluster = " << bestSol->getSumEdges() << endl;
    //cout << "Limite minimo: " << bestSol->getLowerLimit() << endl;
    cout << "Limite final: " << bestSol->getActualLimit() << endl;
    cout << "Qualidade: " << qualitySol << endl;

    return bestSol;
}

Cluster* Graph::gulosoRandReativo(float* alfa, int numInter, int bloco, int numAlfas) {
    int order = this->getOrder();
    if (numClusters > order) {
        cout << "ERRO: numero de clusters maior que o numero de vertices" << endl;
        abort();
    }

    auto vertices = orderVertices();
    float *lowerLimits = this->getLowerLimits();
    float *higherLimits = this->getHigherLimits();

    Cluster *bestSol;
    float qualitySol = 0;

    float probabilities[numAlfas];

    float alfaAverage[numAlfas];
    float auxAverage[numAlfas];
    int numUsesForAlfa[numAlfas];
    for (int i = 0; i < numAlfas; i++) {
        numUsesForAlfa[i] = 1;
        alfaAverage[i] = 1;
        probabilities[i] = 1/(float)numAlfas;
        auxAverage[i] = 0;
    }
    int auxAlfaIndex = 0;
    int alfaIndex = 0;

    for (int x = 0; x < numInter; x++) {

        if(x % bloco == 0 && x != 0){
            updateProbabilities(probabilities, alfaAverage, qualitySol, numAlfas);
            //cout << "PROBABILIDADES: " << endl;
            //for (int i = 0; i < numAlfas; ++i) {
                //cout << "Probabilidade[" << i << "]: " << probabilities[i] << endl;
            //}
        }
        Cluster *clusters = new Cluster[numClusters];
        auto verticesCopy = vertices;

        for (int i = 0; i < numClusters; i++){
            clusters[i].setLowerLimit(lowerLimits[i]);
            clusters[i].setHigherLimit(higherLimits[i]);
            //cout << "Limites: " << endl;
            //cout << clusters[i].getLowerLimit() << " " << clusters[i].getHigherLimit() << endl;
        }

        for (int i = 0; i < order; i++) {
            if(auxAlfaIndex < numAlfas)
                alfaIndex = auxAlfaIndex;
            else
                alfaIndex = getIndex (probabilities, numAlfas);

            //cout << "alfa: " << alfaIndex << endl << endl;
            int cn = getRandomIndex(alfa[alfaIndex], order-i);
            Node *n = this->getNode(verticesCopy.at(cn).getId());
            verticesCopy.erase(verticesCopy.begin() + cn);
            int bestCluster = -1;
            float auxValue = 0;
            float value = 0;
            float auxLimit = 0;
            float sum;
            int j;

            for (j = 0; j < numClusters; j++) {

                if (clusters[j].getPrimeiro() == nullptr) {
                    float v = clusters[j].getHigherLimit() - n->getWeight();
                    if (v > auxValue) {
                        value = 0;
                        bestCluster = j;
                        auxValue = v;
                    }
                } else {
                    sum = 0;

                    if(clusters[j].getActualLimit() + n->getWeight() <= clusters[j].getHigherLimit()) {
                        for (ClusterNode *c = clusters[j].getPrimeiro(); c != nullptr; c = c->getProx()) {
                            Edge *edge = n->hasEdgeBetween(c->getId());
                            if (edge != nullptr){
                                sum += edge->getWeight();
                            }
                        }
                    }

                    float lower = clusters[j].getLowerLimit();
                    float actual = clusters[j].getActualLimit();
                    if (actual == lower - 1 && lower >= 5)
                        actual = lower - 3;
                    if (actual >= lower)
                        actual = lower - 1;
                    float critery = lower - actual;
                    float auxSum = sum * (critery*critery);

                    if (auxSum > auxLimit) {
                        auxLimit = auxSum;
                        value = sum;
                        bestCluster = j;
                    }
                }
            }
            if(bestCluster == -1){
                float indBestCluster = 0;
                for (j = 0; j < numClusters; j++) {
                    float lower = clusters[j].getLowerLimit();
                    float actual = clusters[j].getActualLimit();
                    if((lower - actual) > indBestCluster) {
                        indBestCluster = lower - actual;
                        bestCluster = j;
                    }
                }
                if(indBestCluster == 0){
                    for (j = 0; j < numClusters; j++) {
                        float higher = clusters[j].getHigherLimit();
                        float actual = clusters[j].getActualLimit();
                        if((higher - actual) > indBestCluster) {
                            indBestCluster = higher - actual;
                            bestCluster = j;
                        }
                    }
                }
            }

            clusters[bestCluster].insereFinal(n->getId(), n->getWeight(), value);
        }

        float quality = 0;
        //cout << "Soma final de cada cluster: " << endl;
        for (int i = 0; i < numClusters; ++i) {
            //cout << "Cluster[" << i << "] = " << clusters[i].getSumEdges() << endl;
            //cout << "Limite minimo: " << clusters[i].getLowerLimit() << endl;
            //cout << "Limite final: " << clusters[i].getActualLimit() << endl << endl;
            quality += clusters[i].getSumEdges();
        }

        auxAverage[alfaIndex] += quality;
        alfaAverage[alfaIndex] = auxAverage[alfaIndex] / (float)numUsesForAlfa[alfaIndex];
        //cout << "aux: " << auxAverage[alfaIndex] << "alfaAverage: " << alfaAverage[alfaIndex] << "numUses: " << numUsesForAlfa[alfaIndex] << endl;
        numUsesForAlfa[alfaIndex]++;
        cout << "QUALIDADE: " << quality << endl;

        if(quality > qualitySol){
            qualitySol = quality;
            //cout << endl << endl;
            //cout << "NOVA SOLUCAO !!" << endl << endl;
            bestSol = clusters;
        }else{
            delete [] clusters;
        }

        if (auxAlfaIndex < numAlfas){
            auxAlfaIndex++;
        }
    }
    cout << "PROBABILIDADES: " << endl;
    for (int i = 0; i < numAlfas; ++i) {
        cout << "Probabilidade[" << i << "]: " << probabilities[i] << endl;
    }

    cout << endl;
    cout << "SOLUCAO FINAL:" << endl;
    cout << "Melhor cluster = " << bestSol->getSumEdges() << endl;
    cout << "Limite minimo: " << bestSol->getLowerLimit() << endl;
    cout << "Limite final: " << bestSol->getActualLimit() << endl;
    cout << "Qualidade: " << qualitySol << endl;

    /*cout << "ALFAS: " << endl;
    for (int i = 0; i < numAlfas; ++i) {
        cout << "Alfa[" << i << "]: " << alfaAverage[i] << endl;
    }*/

    return bestSol;
}

