#include <iostream>

GradientDescentOptimizer::GradientDescentOptimizer(float lr) : learningRate_(lr) {}

void GradientDescentOptimizer::computeGradients(BaseNode *loss)
{
    Locking_ptr<BaseNode> lss{loss};
    // get node queue in level order traversal like BFS
    std::vector<Locking_ptr<BaseNode>> nodes = getNodeQueue(loss);
    // store ftrs to wait for them later
    std::vector<std::future<void>> ftrs;
    // clear gradients from previous epoch
    std::for_each(nodes.begin(), nodes.end(), [](Locking_ptr<BaseNode> n){ n->clearGrads(); });    
    // calculate gradients
    for (auto &node : nodes)
    {
        // calculate node gradients 
        ftrs.emplace_back(std::async(std::launch::async, [node] { node->gradient(); }));
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

std::vector<Locking_ptr<BaseNode>> GradientDescentOptimizer::getNodeQueue(BaseNode *loss)
{
    // Do level-order traversal 
    // create a deque 
    std::deque<Locking_ptr<BaseNode>> nodeQueue;
    // create a vector of nodes to return the nodes
    std::vector<Locking_ptr<BaseNode>> nodesList;
    // create a map for exitance ckeck in constant time
    std::unordered_map<BaseNode *, bool> visitedNodes;
    nodeQueue.push_front(Locking_ptr<BaseNode>(loss));
    while (!nodeQueue.empty())
    {
        Locking_ptr<BaseNode> node = nodeQueue.front();
        // cash in node list
        nodesList.push_back(node);
        // set node to visited
        visitedNodes[node.get()] = true;
        // remove the visited node from queue
        nodeQueue.pop_front();
        // get the inputs
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
    // return the node list
    return nodesList;
}
