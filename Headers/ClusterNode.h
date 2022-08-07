//
// Created by Marcos Paulo on 02/08/2022.
//

#ifndef TESTEGRAFOS_CLUSTERNODE_H
#define TESTEGRAFOS_CLUSTERNODE_H
class ClusterNode
{
public:

    ClusterNode(int id, std::string external_id, float weight, float sumEdgesWeights, int numEdges){
        this->id = id;
        this->external_id = external_id;
        this->weight = weight;
        this->sumEdgesWeights = sumEdgesWeights;
        this->numEdges = numEdges;
        this->value = sumEdgesWeights + weight - (float)numEdges;
    }
    ClusterNode(){};
    ~ClusterNode(){};

    ClusterNode* getProx(){return prox;};
    void setProx(ClusterNode *p){prox = p;};
    int getId() const {return id;}
    void setId(int id) {ClusterNode::id = id;}
    const std::string &getExternalId() const {return external_id;}
    void setExternalId(const std::string &externalId) {external_id = externalId;}
    float getWeight() const {return weight;}
    void setWeight(float weight) {ClusterNode::weight = weight;}
    float getSumEdgesWeights() const {return sumEdgesWeights;}
    void setSumEdgesWeights(float sumEdgesWeights) {ClusterNode::sumEdgesWeights = sumEdgesWeights;}
    int getNumEdges() const {return numEdges;}
    void setNumEdges(int numEdges) {ClusterNode::numEdges = numEdges;}
    float getValue() const {return value;}
    void setValue(float value) {ClusterNode::value = value;}

private:
    ClusterNode *prox; // ponteiro para o proximo
    int id;
    std::string external_id; //necessario?
    float weight;
    float sumEdgesWeights;
    int numEdges;
    float value;
};
#endif //TESTEGRAFOS_CLUSTERNODE_H
