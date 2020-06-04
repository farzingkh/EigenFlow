#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

class Graph
{
public:
    template <typename T>
    void addPlaceholder(std::shared_ptr<Placeholder<T>> plc);

    template <typename T>
    void addOperation(std::shared_ptr<Operation<T>> op);

    template <typename T>
    void addVariable(std::shared_ptr<Variable<T>> vr);

    std::vector<std::shared_ptr<BaseNode>> &getNodes();

private:
    std::vector<std::shared_ptr<BaseNode>> _baseNodes;
};

#include "../src/graph.tpp"

#endif /* GRAPH_H */