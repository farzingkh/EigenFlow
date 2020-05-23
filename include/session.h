#include "graph.h"
#include <unordered_map>

template <typename T>
class Session
{
public:
    Session();
    // Runs calculation of the node:
    // Takes input data for placeholders with an unordered map using placeholder's name
    auto Run(*Node, std::unordered_map<std::string, *T> feed);

private:
    void getNodesList();
    std::vector<*Node> _nodesList;
};