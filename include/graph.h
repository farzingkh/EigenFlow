#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

template <typename T>
class Graph
{
public:
    void addPlaceholder();
    void addOperation();
    void addVariable();

    std::vector<Node<T>*> getOperations();
    std::vector<Node<T>*> getPlaceholders();
    std::vector<Node<T>*> getVariables();

private:
    std::vector<Node<T>*> _operations;
    std::vector<Node<T>*> _variables;
    std::vector<Node<T>*> _placeholders;
};

#endif /* GRAPH_H */