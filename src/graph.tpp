
template <typename T>
void Graph::addPlaceholder(std::shared_ptr<Placeholder<T>> plc)
{
    _baseNodes.push_back(std::move(plc));
}

template <typename T>
void Graph::addOperation(std::shared_ptr<Operation<T>> op)
{
    _baseNodes.push_back(std::move(op));
}

template <typename T>
void Graph::addVariable(std::shared_ptr<Variable<T>> vr)
{
    _baseNodes.push_back(std::move(vr));
}

std::vector<std::shared_ptr<BaseNode>> &Graph::getNodes()
{
    return _baseNodes;
}
