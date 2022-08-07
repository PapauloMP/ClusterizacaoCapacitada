//
// Created by Marcos Paulo on 04/06/2022.
//

#include "../Headers/Node.h"
#include <iostream>

using namespace std;

/**************************************************************************************************
 * Defining the Node's methods
**************************************************************************************************/

///Construtor
Node::Node(int id, string id_external, float weight) {

    this->id = id;
    this->id_external = id_external;
    this->in_degree = 0;
    this->out_degree = 0;
    this->weight = weight;
    this->first_edge = nullptr;
    this->last_edge = nullptr;
    this->next_node = nullptr;
}

/// Destrutor
Node::~Node(){

    this->removeAllEdges();

}

///Getters
Edge* Node::getFirstEdge(){

    return this->first_edge;

}

Edge* Node::getLastEdge(){

    return this->last_edge;

}

int Node::getId(){

    return this->id;

}

unsigned int Node::getInDegree(){

    return this->in_degree;

}

unsigned int Node::getOutDegree(){

    return this->out_degree;

}

float Node::getWeight(){

    return this->weight;

}

Node* Node::getNextNode(){

    return this->next_node;

}

///Setters

void Node::setNextNode(Node* next_node){

    this->next_node = next_node;

}

void Node::setWeight(float weight){

    this->weight = weight;

}

///Other methods
void Node::insertEdge(int target_id, float weight){
    // Verifica se tem pelo menos uma aresta no nó
    if(this->first_edge != nullptr){
        // Alocando a nova aresta e mantendo a integridade da lista de adjacência
        Edge* edge = new Edge(target_id);
        edge->setWeight(weight);
        this->last_edge->setNextEdge(edge);
        this->last_edge = edge;
    }
    else{
        // Alocando a nova aresta e mantendo a integridade da lista de adjacência
        this->first_edge = new Edge(target_id);
        this->first_edge->setWeight(weight);
        this->last_edge = this->first_edge;

    }
    this->out_degree++;
}

///TODO - test
void Node::removeAllEdges(){
    // Verifica se tem pelo menos uma aresta no nó
    if(this->first_edge != nullptr){
        Edge* next;
        Edge* first = this->first_edge;
        // Removendo todas as arestas do nó
        while(first != nullptr){
            next = first->getNextEdge();
            delete first;
            first = next;
        }

    }
    this->first_edge = this->last_edge = nullptr;
}

void Node::removeEdge(bool directed, Node* target_node){
    // Verifica se a aresta a ser removida está no nó
    int id = target_node->getId();
    if(this->searchEdge(id)){

        Edge* aux = this->first_edge;
        Edge* previous = nullptr;

        // Procurando a aresta a ser removida
        while(aux->getTargetId() != id){
            previous = aux;
            aux = aux->getNextEdge();
        }
        // Mantendo a integridade da lista de adjacência
        if (previous != nullptr)
            previous->setNextEdge(aux->getNextEdge());
        else
            this->first_edge = aux->getNextEdge();

        if (aux == this->last_edge)
            this->last_edge = previous;

        delete aux;

        if (directed)
            this->decrementOutDegree();
        else {
            this->decrementInDegree();
            target_node->decrementInDegree();
        }
    }
    else{
        cout << "Aresta nao encontrada" << endl;
    }
}

bool Node::searchEdge(int target_id){
    // Verifica se tem pelo menos uma aresta no nó
    if(this->first_edge != nullptr){
        // Procurando por uma aresta específica cujo targetId é igual a target_id
        for(Edge* aux = this->first_edge; aux != nullptr; aux = aux->getNextEdge())
            if(aux->getTargetId() == target_id)
                return true;

    }

    return false;

}

void Node::incrementInDegree(){

    this->in_degree++;

}

void Node::incrementOutDegree(){

    this->out_degree++;

}

void Node::decrementInDegree(){

    this->in_degree--;

}

void Node::decrementOutDegree(){

    this->out_degree--;

}

Edge* Node::hasEdgeBetween(int target_id)
{
    for(Edge *auxEdge = this->first_edge; auxEdge != nullptr; auxEdge = auxEdge->getNextEdge())
    {
        if(auxEdge->getTargetId() == target_id)
            return auxEdge;
    }
    return nullptr;
}

string Node::getIdExternal() {
    return this->id_external;
}