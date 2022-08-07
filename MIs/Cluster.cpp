//
// Created by Marcos Paulo on 02/08/2022.
//
#include <iostream>
#include "../Headers/Cluster.h"


Cluster::Cluster()
{
    primeiro = nullptr;
    lowerLimit = -1;
    higherLimit = -1;
    actualLimit = 0;
    sumEdges = 0;
}

Cluster::~Cluster()
{
    ClusterNode *p = primeiro;

    while(p != nullptr)
    {
        primeiro = primeiro->getProx();
        delete p;
        p = primeiro;
    }
}

void Cluster::insereInicio(int val)
{

    ClusterNode *p = new ClusterNode();
    p->setProx(primeiro);
    primeiro = p;
}

void Cluster::insereFinal(int id, float nodeWeight, float edgeWeight)
{
    ClusterNode *t = new ClusterNode();
    t->setId(id);
    t->setProx(nullptr);
    this->actualLimit += nodeWeight;
    this->sumEdges += edgeWeight;

    if (primeiro != nullptr){
        ClusterNode *p;
        for (p = primeiro; p->getProx() != nullptr; p = p->getProx());
        p->setProx(t);

    }
    else{
        primeiro = t;
    }
}

void Cluster::removeInicio()
{
    if(primeiro != nullptr)
    {
        ClusterNode *p = primeiro;

        primeiro = primeiro->getProx();

        delete p;
    }
}

void Cluster::removeFinal()
{
    if (primeiro != nullptr){

        ClusterNode *ap = nullptr, *p;
        for (p = primeiro; p->getProx() != nullptr; p = p->getProx()){
            ap = p;
        }

        if (ap == nullptr){ // lista com 1 nó
            primeiro = nullptr;
        }
        else{
            ap->setProx(nullptr);
        }
        delete p;
    }
}

void Cluster::imprimir()
{
    ClusterNode *p = primeiro;
    while(p != nullptr)
    {
        //std::cout<<p->getInfo()<<"    ";
        p = p->getProx();
    }
    std::cout<<std::endl<<std::endl<<std::endl;
}

float Cluster::getLowerLimit() const {return lowerLimit;}
void Cluster::setLowerLimit(float lowerLimit) {Cluster::lowerLimit = lowerLimit;}
float Cluster::getHigherLimit() const {return higherLimit;}
void Cluster::setHigherLimit(float higherLimit) {Cluster::higherLimit = higherLimit;}
float Cluster::getActualLimit() const {return actualLimit;}
float Cluster::getSumEdges() const {return sumEdges;}
bool Cluster::isReachedMinLimit() const {return  reachedMinLimit;}
void Cluster::setReachedMinLimit(bool reachedMinLimit) {Cluster::reachedMinLimit = reachedMinLimit;}
