
template <typename T>
void Session::Run(BaseNode *n, std::unordered_map<std::string, Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> *> feed)
{
    Locking_ptr<BaseNode> nptr(n);
    // empty node list
    _nodesList.clear();
    // obtain inputs for node n in post-order, to resolve inputs befor computation of an operation
    updateNodesList(nptr.get());

    for (auto m : _nodesList)
    {
        // if it's a placeholder then feed the data
        if (m->getNodeType() == nodeType::placeholder)
        {
            // set the output value
            Locking_ptr<Placeholder<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>>> plcptr(static_cast<Placeholder<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>> *>(m.get()));
            plcptr->setValue(std::move(*feed[plcptr->getName()]));
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
    Locking_ptr<BaseNode> nptr(n);
    // only operations have input nodes
    if (nptr->getNodeType() == nodeType::operation)
    {
        auto list = nptr->getInputs();
        for (auto &m : list)
        {
            updateNodesList(m.get());
        }
    }
    _nodesList.push_back(Locking_ptr<BaseNode>(nptr.get()));
}

// Return  nodes list
std::vector<Locking_ptr<BaseNode>> Session::getNodesList()
{
    return _nodesList;
}