#ifndef SESSION_H
#define SESSION_H

#include "graph.h"
#include <unordered_map>

template <typename T>
class Session
{
public:
    // Runs calculation of the node and returns the output value for the node;
    // Takes input data for placeholders with an unordered map using placeholder's name
    T Run(Node<T> &n, std::unordered_map<std::string, T *> feed);

private:
    // Perform post-order traversal
    void getNodesList(Node<T> *n);
    std::vector<Node<T> *> _nodesList = {};
};

#include "../src/session.tpp" 
#endif /* SESSION_H */