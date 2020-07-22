#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

// a container class that ownes the nodes
class Graph
{
public:
    // add nodes of unary operation to graph
    template <template <typename> class U, typename T>
    BaseNode *addNodeOne(std::unique_ptr<U<T>> n);

    // add nodes of binary operation to graph
    template <template <typename, typename, typename> class U, typename T, typename T1, typename T2>
    BaseNode *addNodeTwo(std::unique_ptr<U<T, T1, T2>> n);

    // get nodes from graph
    std::vector<std::unique_ptr<BaseNode>> &getNodes();

private:
    std::vector<std::unique_ptr<BaseNode>> _baseNodes;
};

#include "../src/graph.tpp"

#endif /* GRAPH_H */