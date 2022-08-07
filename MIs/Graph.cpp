//
// Created by Marcos Paulo on 04/06/2022.
//
#include <iostream>
#include <fstream>
#include <utility>
#include "../Headers/Graph.h"


using namespace std;

///Construtor
Graph::Graph(bool directed, bool weighted_edge, bool weighted_node)
{
    this->directed = directed;
    this->weighted_edge = weighted_edge;
    this->weighted_node = weighted_node;
    this->first_node = this->last_node = nullptr;
    this->number_edges = 0;
    this->insertion_position = 0;
    this->order = 0;
}

/// Destrutor
Graph::~Graph()
{
    Node *next_node = this->first_node;

    while (next_node != nullptr)
    {
        next_node->removeAllEdges();
        Node *aux_node = next_node->getNextNode();
        delete next_node;
        next_node = aux_node;
    }
}

// Getters
int Graph::getOrder() const
{
    return this->order;
}
unsigned int Graph::getNumberEdges() const
{
    return this->number_edges;
}
bool Graph::getDirected() const
{
    return this->directed;
}
bool Graph::getWeightedEdge() const
{
    return this->weighted_edge;
}
bool Graph::getWeightedNode() const
{
    return this->weighted_node;
}
Node *Graph::getFirstNode()
{
    return this->first_node;
}
Node *Graph::getLastNode()
{
    return this->last_node;
}

string Graph::getExternalId(int id) {
    return this->mapInternalIdToExternalId[id];
}

int Graph::getInsertionPosition() {
    auto aux = this->insertion_position;
    this->insertion_position++;
    return aux;
}

float *Graph::getLowerLimits() const {return lowerLimits;}
void Graph::setLowerLimits(float *lowerLimits) {Graph::lowerLimits = lowerLimits;}
float *Graph::getHigherLimits() const {return higherLimits;}
void Graph::setHigherLimits(float *higherLimits) {Graph::higherLimits = higherLimits;}
float *Graph::getNodeWeights() const {return nodeWeights;}
void Graph::setNodeWeights(float *nodeWeights) {Graph::nodeWeights = nodeWeights;}
int Graph::getNumClusters() const {return numClusters;}
void Graph::setNumClusters(int numClusters) {Graph::numClusters = numClusters;}
const string &Graph::getClusterType() const {return clusterType;}
void Graph::setClusterType(const string &clusterType) {Graph::clusterType = clusterType;}

Node* Graph::insertNode(string id_external)
{
    auto node = new Node(this->getInsertionPosition(), move(id_external));

    if(this->getFirstNode() == nullptr)
    {
        this->first_node = this->last_node = node;
    }else{
        this->getLastNode()->setNextNode(node);
        this->last_node = node;
    }
    this->order++;
    return node;
}

void Graph::insertEdge(const string& id_external, const string& target_id_external, float weight) {
    auto source_node = this->getNodeFromExternalId(id_external);
    auto target_node = this->getNodeFromExternalId(target_id_external);
    if(source_node == nullptr)
    {
        source_node = this->insertNode(id_external);
        if(this->weighted_node)
            source_node->setWeight(this->nodeWeights[stoi(id_external)]);
    }
    if(target_node == nullptr)
    {
        target_node = this->insertNode(target_id_external);
        if(this->weighted_node)
            target_node->setWeight(this->nodeWeights[stoi(target_id_external)]);
    }

    this->insertEdge(source_node, target_node, weight);
}

void Graph::insertEdge(Node* source_node, Node* target_node, float weight) {
    //cout << "FUNCIONOU" << endl;
    if (source_node == nullptr || target_node == nullptr) {
        cout << "Erro: no de origem e/ou de destino nulo(s)" << endl;
        abort();
    }
    if (!source_node->searchEdge(target_node->getId())){ ///evita multiarestas
        source_node->insertEdge(target_node->getId(), weight);
        this->number_edges++;
    }
    if (!this->directed)
    {
        if (!target_node->searchEdge(source_node->getId())) {
            target_node->insertEdge(source_node->getId(), weight);
        }
    }
}
void Graph::removeFirstNode(int id){
    auto node = this->getFirstNode();
    this->first_node = node->getNextNode();

    if(this->last_node == node)
    {
        this->last_node = this->first_node;
    }
    delete node;
}
void Graph::removeLastNode(int id){
    auto node = this->getFirstNode();

    if(node != last_node)
        while (node->getNextNode() != last_node)
            node->getNextNode();
    else{
        delete last_node;
        first_node = last_node = nullptr;
    }
    node->setNextNode(nullptr);
    delete last_node;
    last_node = node;
}
///------------------
void Graph::removeNode(const string& id_external){
    if (searchNodeFromExternalId(id_external)) {

        Node* targetNode = getNodeFromExternalId(id_external);
        int id = targetNode->getId();
        // Primeiro remove as arestas que apontam para o nó, para então excluí-lo
        for(Node* auxNode = getFirstNode(); auxNode != nullptr; auxNode = auxNode->getNextNode()){
            if(auxNode->searchEdge(id)){
                auxNode->removeEdge(getDirected(), targetNode);
                number_edges--;
            }
        }

        auto node = this->getFirstNode();
        if (first_node->getId() == id) {
            this->number_edges -= first_node->getOutDegree();
            removeFirstNode(id);
        } else if (last_node->getId() == id) {
            this->number_edges -= last_node->getOutDegree();
            removeLastNode(id);
        } else {
            while (node->getNextNode() != nullptr) {

                if (node->getNextNode()->getId() == id) {
                    auto tobeRemoved = node->getNextNode();
                    node->setNextNode(tobeRemoved->getNextNode());
                    this->number_edges -= tobeRemoved->getOutDegree();
                    delete tobeRemoved;
                }
                node = node->getNextNode();
            }
        }

        this->order--;
    }
    else {
        cout << "Id invalido" << endl;
    }
}


bool Graph::searchNode(int id)
{
    auto node = this->getFirstNode();

    while(node != nullptr)
    {
        if(node->getId() == id)
        {
            return true;
        }
        node = node->getNextNode();
    }

    return false;
}


Node *Graph::getNode(int id)
{
    auto node = this->getFirstNode();

    while(node != nullptr)
    {
        if(node->getId() == id)
        {
            return node;
        }
        node = node->getNextNode();
    }

    return node;
}

bool Graph::searchNodeFromExternalId(const string& id_external) {
    auto node = this->getFirstNode();

    while(node != nullptr)
    {
        if(node->getIdExternal() == id_external)
        {
            return true;
        }
        node = node->getNextNode();
    }

    return false;
}

Node *Graph::getNodeFromExternalId(const string& id_external) {
    auto node = this->getFirstNode();

    while(node != nullptr)
    {
        if(node->getIdExternal() == id_external)
        {
            return node;
        }
        node = node->getNextNode();
    }

    return node;
}

void Graph::indexId() {
    this->mapInternalIdToExternalId = new string[this->getOrder()];

    Node *node = this->first_node;
    while (node != nullptr)
    {
        this->mapInternalIdToExternalId[node->getId()] = node->getIdExternal();
        node = node->getNextNode();
    }
}


