#include "graph.h"
#include <unordered_map>

template <typename T>
class Session
{
public:
    // Runs calculation of the node and returns the output value for the node;
    // Takes input data for placeholders with an unordered map using placeholder's name
    auto Run(Node* n, std::unordered_map<std::string, T* t> feed);

private:
    // Perform post-order traversal     
    void getNodesList(Node* n); 
    std::vector<Node*> _nodesList = {};
};

