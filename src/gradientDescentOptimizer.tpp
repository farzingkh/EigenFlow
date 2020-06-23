
GradientDescentOptimizer::GradientDescentOptimizer(float lr) : learningRate_(lr) {}

void GradientDescentOptimizer::computeGradients(BaseNode *loss)
{
    std::deque<BaseNode *> nodeQueue;
    std::unordered_map<BaseNode *, bool> visitedNodes;
    nodeQueue.push_front(loss);
    while (!nodeQueue.empty())
    {
        BaseNode *node = nodeQueue.front();
        nodeQueue.pop_front();
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
                // if variable add to variable list
                if (n->getNodeType() == nodeType::variable)
                {
                    variableNodesList_.push_back(n);
                }
            }
        }
    }
}

template <typename T>
Minimizer<T> GradientDescentOptimizer::minimize(BaseNode *loss)
{
    // compute gradients and cash variable nodes list
    computeGradients(loss);
    // Instantiate a minimzer object and return it
    return  Minimizer<T>(variableNodesList_, learningRate_);
}
