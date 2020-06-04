#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"

class Graph
{
public:
    template <typename T>
    void addPlaceholder();

    template <typename T>
    void addOperation();

    template <typename T>
    void addVariable();

    std::vector<Operation<T> *> getOperations();
    std::vector<Placeholder<T> *> getPlaceholders();
    std::vector<Variable<T> *> getVariables();

private:
    std::vector<Operation<T> *> _operations;
    std::vector<Variable<T> *> _variables;
    std::vector<Placeholder<T> *> _placeholders;
};

#endif /* GRAPH_H */