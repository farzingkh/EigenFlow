#include "node.h"

class Graph
{
public:
    void addPlaceholder();
    void addOperation();
    void addVariable();

    std::vector<Node*> getOperations();
    std::vector<Node*> getPlaceholders();
    std::vector<Node*> getVariables();

private:
    std::vector<Node*> _operations;
    std::vector<Node*> _variables;
    std::vector<Node*> _placeholders;
}