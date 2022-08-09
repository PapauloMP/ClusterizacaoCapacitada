//
// Created by Marcos Paulo on 04/06/2022.
//

#ifndef TESTEGRAFOS_GRAPH_H
#define TESTEGRAFOS_GRAPH_H

#include <vector>
#include "Node.h"
#include "Edge.h"
#include "Cluster.h"

/*struct NodeStruct{
    int id;
    string external_id; //necessario?
    float weight;
    float sumEdgesWeights;
    int numEdges;
    float value;

    NodeStruct(int id, string external_id, float weight, float sumEdgesWeights, int numEdges){
        this->id = id;
        this->external_id = external_id;
        this->weight = weight;
        this->sumEdgesWeights = sumEdgesWeights;
        this->numEdges = numEdges;
        this->value = sumEdgesWeights + weight - numEdges;
    }
};*/

struct EdgeStruct{
    string fromNode;
    string toNode;
    float weight;

    EdgeStruct(string fromNode, string toNode, float weight) {
        this->fromNode = fromNode;
        this->toNode = toNode;
        this->weight = weight;
    }

    EdgeStruct(){
        weight = 1;
    };
};

class Graph{

    //Atributos
private:
    int insertion_position;
    int order;
    unsigned int number_edges;
    bool directed;
    bool weighted_edge;
    bool weighted_node;
    Node* first_node;
    Node* last_node;
    string* mapInternalIdToExternalId;

    float* lowerLimits;
    float* higherLimits;
    float* nodeWeights;
    int numClusters;
    string clusterType;

public:

    //Construtor
    Graph(bool directed, bool weighted_edge, bool weighted_node = false);

    //Destrutor
    ~Graph();

    //Métodos
    bool getWeightedEdge() const;
    bool getDirected() const;
    int getOrder() const;
    int getInsertionPosition();
    unsigned int getNumberEdges() const;
    bool getWeightedNode() const;
    Node *getFirstNode();
    Node* getNodeFromExternalId(const string& id_external);
    Node* getLastNode();
    string getExternalId(int id);
    float *getLowerLimits() const;
    void setLowerLimits(float *lowerLimits);
    float *getHigherLimits() const;
    void setHigherLimits(float *higherLimits);
    float *getNodeWeights() const;
    void setNodeWeights(float *nodeWeights);
    int getNumClusters() const;
    void setNumClusters(int numClusters);

    const string &getClusterType() const;

    void setClusterType(const string &clusterType);

    void indexId();
    void insertEdge(const string& id_external, const string& target_id_external, float weight);
    void removeNode(const string& id_external);
    bool searchNode(int id);
    bool searchNodeFromExternalId(const string& id_external);
    Node* getNode(int id);

    ///CCP
    vector<ClusterNode> orderVertices();
    Cluster* guloso();
    Cluster* gulosoRand(float alfa, int numIter);
    Cluster* gulosoRandReativo(float* alfa, int numIter, int bloco, int numAlfas);
    void printCCP(Cluster* bestSol, int numClusters, ofstream& outputFile);

private:

    void removeFirstNode(int id);
    void removeLastNode(int id);

    Node* insertNode(string id_external);
    void insertEdge(Node* source_node, Node* target_node, float weight);

    //Other methods
    //Node* getLastNode();
    //Node * insertNode(string id_external);

};
#endif //TESTEGRAFOS_GRAPH_H
