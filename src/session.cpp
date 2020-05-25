#include "session.h"

Session::Run(Node *n, std::unordered_map<std::string, T *> feed)
{
    // obtain inputs for node n in post-order
    getNodesList(n);

    for (Node *n : _nodesList)
    {
        // if it's a placeholder then feed the data
        if (n.getType() == Node::nodeType::placeholder)
        {
            n.setValue(feed[n.getName()]);
        } // if it's a operation then compute the value
        else if (n.getType() == Node::nodeType::operation)
        {
            n.getValue();
        }
    }
    return n.getValue()
}