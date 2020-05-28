
template <typename T>
T Session<T>::Run(Node<T> &n, std::unordered_map<std::string, T *> feed)
{
    // obtain inputs for node n in post-order, to resolve inputs befor computation of an operation
    getNodesList(&n);

    for (Node<T> *m : _nodesList)
    {
        // if it's a placeholder then feed the data
        if (m->getType() == nodeType::placeholder)
        {
            // set the output value
            m->setValue(*feed[m->getName()]);
        } // if it's a operation then compute the value
        else if (m->getType() == nodeType::operation)
        {
            // compute add and set the output value
            Operation<T> *op = static_cast<Operation<T> *>(m);
            op->compute();
        }
    }
    return n.getValue();
}

// Return post order list of nodes
template <typename T>
void Session<T>::getNodesList(Node<T> *n)
{
    if (n->getType() == nodeType::operation) // only operations have input nodes
    {
        std::vector<BaseNode *> list = {};
        list = n->getInputs();
        for (auto &m : list)
        {
            Session::getNodesList(static_cast<Node<T> *>(m));
        }
    }
    _nodesList.push_back(n);
}