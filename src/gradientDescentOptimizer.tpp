#include <iostream>

GradientDescentOptimizer::GradientDescentOptimizer(float lr) : learningRate_(lr) {}

void GradientDescentOptimizer::computeGradients(BaseNode *loss)
{
    std::vector<Locking_ptr<BaseNode>> nodes;
    // clear nodes list and queue
    NodesList_.clear();
    nodeQueue_.clear();
    // get node queue in level order traversal like BFS
    getNodeQueue(loss);
    // store ftrs to wait for them later
    std::vector<std::future<void>> ftrs;
    // clear gradients from previous epoch
    while (!nodeQueue_.empty())
    {
        auto node = nodeQueue_.front();
        nodes.push_back(node);
        node->clearGrads();
        nodeQueue_.pop_front();
    }
    // calculate gradients
    for(auto &node : nodes)
    {
        ftrs.emplace_back(std::async(std::launch::async, [node] {  node->gradient(); }));
    }
    // wait for results
    for_each(ftrs.begin(), ftrs.end(), [](std::future<void> &ftr) { ftr.wait(); });

    //std::cout << "gradients computed! " << std::endl;
}

template <typename T>
Minimizer<T> GradientDescentOptimizer::minimize(BaseNode *loss)
{
    // Instantiate a minimzer object and return it
    return Minimizer<T>(this, loss);
}

void GradientDescentOptimizer::getNodeQueue(BaseNode *loss)
{
    // Do BFS 
    std::deque<Locking_ptr<BaseNode>> nodeQueue;
    std::unordered_map<BaseNode *, bool> visitedNodes;
    nodeQueue.push_front(Locking_ptr<BaseNode>(loss));
    while (!nodeQueue.empty())
    {
        Locking_ptr<BaseNode> node = nodeQueue.front();
        nodeQueue_.push_back(node);
        // cash in node list
        NodesList_.push_back(node);
        visitedNodes[node.get()] = true;
        nodeQueue.pop_front();
        auto nodes = node->getInputs();
        // go through all inputs of the node
        for (auto &n : nodes)
        {
            // check if the node is visited before
            if (visitedNodes[n.get()] != true)
            {
                // if node not visited add to queue
                nodeQueue.push_back(n);
            }
        }
    }
}