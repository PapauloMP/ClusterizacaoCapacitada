cmake_minimum_required(VERSION 3.22)
project(GrafosT2)

set(CMAKE_CXX_STANDARD 14)

include_directories(Headers)

add_executable(GrafosT2
        Headers/Cluster.h
        Headers/ClusterNode.h
        Headers/Edge.h
        Headers/Graph.h
        Headers/Node.h
        Headers/Menu.h
        MIs/Cluster.cpp
        MIs/Clustering.cpp
        MIs/Edge.cpp
        MIs/Graph.cpp
        MIs/Node.cpp
        MIs/Menu.cpp
        main.cpp)
