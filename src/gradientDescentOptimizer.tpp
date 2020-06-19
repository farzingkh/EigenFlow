#include "../nclude/gradientDescentOptimizer.h"

GradientDescentOptimizer::GradientDescentOptimizer(int lr) : learningRate_(lr) {}

void GradientDescentOptimizer::computeGradients(BaseNode *loss)
{
    std::deque<BaseNode *> nodeQueue;
    std::deque<BaseNode *> visitedNodes;

    auto nodes = loss->getInputs();
    for (auto n : nodes)
    {
        nodeQueue.push_back(n);
    }

    while (!nodeQueue.empty())
    {
        BaseNode *node = nodeQueue.pop_back();
        if (node->getNodeType() == nodeType::operation)
        {
            ndoe->gradient();
        }
        else
        {
            node->getOperationType
        }
    }
}

BaseNode *GradientDescentOptimizer::minimize(BaseNode *loss)
{
    return BaseNode();
}