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
        node->gradient();
        visitedNodes[node] = true;
        auto nodes = node->getInputs();
        for (auto n : nodes)
        {
            if (visitedNodes[node] != true)
            {
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

template <typename T>
Minimizer<T>::Minimizer(std::vector<BaseNode *> &nodeList) : nodesList_(nodeList) {}

template <typename T>
void minimizer<T>::compute()
{
    for (auto n : nodesList_)
    {
        if (n->getNodeType() == nodeType::variable)
        {
            n->setValue()
        }
    }
    return;
}