#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

class Graph
{
public:
    template <template <typename> class U, typename T>
    void addNodeOne(std::shared_ptr<U<T>> n);

    template <template <typename, typename, typename> class U, typename T, typename T1, typename T2>
    void addNodeTwo(std::shared_ptr<U<T,T1,T2>> n);

    std::vector<std::shared_ptr<BaseNode>> &getNodes();

private:
    std::vector<std::shared_ptr<BaseNode>> _baseNodes;
};

#include "../src/graph.tpp"

#endif /* GRAPH_H */