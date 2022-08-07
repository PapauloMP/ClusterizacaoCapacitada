//
// Created by Marcos Paulo on 04/06/2022.
//

#ifndef TESTEGRAFOS_EDGE_H
#define TESTEGRAFOS_EDGE_H


class Edge{

    // Attributes
private:
    int target_id;
    Edge* next_edge;
    float weight;

public:
    // Constructor
    Edge(int target_id);
    // Destructor
    ~Edge();

    // Getters
    int getTargetId();
    Edge* getNextEdge();
    float getWeight();
    // Setters
    void setNextEdge(Edge* edge);
    void setWeight(float weight);

};


#endif //TESTEGRAFOS_EDGE_H
