#ifndef SESSION_H
#define SESSION_H

#include "graph.h"
#include <unordered_map>

class Session
{
public:
    // Runs calculation of the node and returns the output value for the node;
    // Takes input data for placeholders with an unordered map using placeholder's name
    template <typename T>
    void Run(BaseNode *n, std::unordered_map<std::string, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> *> feed);
    std::vector<BaseNode *> getNodesList();
    void updateNodesList(BaseNode *n);

private:
    // Perform post-order traversal
    std::vector<BaseNode *> _nodesList = {};
};

#include "../src/session.tpp"
#endif /* SESSION_H */