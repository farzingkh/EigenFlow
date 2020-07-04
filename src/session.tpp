
template <typename T>
void Session::Run(BaseNode *n, std::unordered_map<std::string, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> *> feed)
{
    // empty node list
    _nodesList.clear();
    // obtain inputs for node n in post-order, to resolve inputs befor computation of an operation
    updateNodesList(n);

    for (auto m : _nodesList)
    {
        // if it's a placeholder then feed the data
        if (m->getNodeType() == nodeType::placeholder)
        {
            // set the output value
            Placeholder<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>> *plc = static_cast<Placeholder<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>> *>(m);
            plc->setValue(std::move(*feed[plc->getName()]));
        } // if it's a operation then compute the value
        else if (m->getNodeType() == nodeType::operation)
        {
            // compute add and set the output value
            m->compute();
        }
    }
    // empty node list
    _nodesList.clear();
}

// get post order list of nodes
void Session::updateNodesList(BaseNode *n)
{
    // only operations have input nodes
    if (n->getNodeType() == nodeType::operation)
    {
        auto list = n->getInputs();
        for (auto &m : list)
        {
            updateNodesList(m);
        }
    }
    _nodesList.push_back(n);
}

// Return  nodes list
std::vector<BaseNode *> Session::getNodesList()
{
    return _nodesList;
}