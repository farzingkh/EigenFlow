#include "../nclude/gradientDescentOptimizer.h"

GradientDescentOptimizer::GradientDescentOptimizer(int lr) : learningRate_(lr) {}

void GradientDescentOptimizer::computeGradients(BaseNode *loss)
{
    std::deque<BaseNode *> nodeQueue;
    std::map<BaseNode *, bool visited> visitedNodes;
    nodeQueue.push_front(loss);
    while (!nodeQueue.empty())
    {
        BaseNode *node = nodeQueue.pop_front();
        // calculate gradients
        node->gradient();
        // set node as visited
        visitedNodes[node] = true;
        auto nodes = node->getInputs();
        // go through all inputs of the node
        for (auto n : nodes)
        {
            // check if the node is visited before
            if (visitedNodes[node] != true)
            {
                // if node not visited add to queue
                nodeQueue.push_back(n);
            }
        }
    }
}

BaseNode *GradientDescentOptimizer::minimize(BaseNode *loss)
{
    std::vector<BaseNode *> nodeList;

    return BaseNode();
}

