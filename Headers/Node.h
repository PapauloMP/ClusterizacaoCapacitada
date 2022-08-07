//
// Created by Marcos Paulo on 04/06/2022.
//

#ifndef TESTEGRAFOS_NODE_H
#define TESTEGRAFOS_NODE_H


#include <string>
#include "Edge.h"

using namespace std;

class Node{

    // Attributes
private:
    Edge* first_edge;
    Edge* last_edge;
    int id;
    string id_external;
    unsigned int in_degree;
    unsigned int out_degree;
    float weight;
    Node* next_node;

public:
    // Constructor
    Node(int id, string id_external = "", float weight = 1);
    // Destructor
    ~Node();
    // Getters
    Edge* getFirstEdge();
    Edge* getLastEdge();
    int getId();
    unsigned int getInDegree();
    unsigned int getOutDegree();
    float getWeight();
    Node* getNextNode();
    string getIdExternal();
    // Setters
    void setNextNode(Node* node);
    void setWeight(float weight);
    // Other methods
    bool searchEdge(int target_id);
    void insertEdge(int target_id, float weight);
    void removeAllEdges();
    void removeEdge(bool directed, Node* target_node);
    void incrementOutDegree();
    void decrementOutDegree();
    void incrementInDegree();
    void decrementInDegree();
    Edge* hasEdgeBetween(int target_id);
    // Auxiliar methods
};

#endif //TESTEGRAFOS_NODE_H
