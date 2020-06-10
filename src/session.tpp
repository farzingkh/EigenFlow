
template <typename T>
T Session::Run(BaseNode *n, std::unordered_map<std::string, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> *> feed)
{
    // obtain inputs for node n in post-order, to resolve inputs befor computation of an operation
    getNodesList(n);

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
    return n->getValue<T>();
}

// Return post order list of nodes

void Session::getNodesList(BaseNode *n)
{
    if (n->getNodeType() == nodeType::operation) // only operations have input nodes
    {
        auto list = n->getInputs();
        for (auto &m : list)
        {
            getNodesList(m);
        }
    }
    _nodesList.push_back(n);
}