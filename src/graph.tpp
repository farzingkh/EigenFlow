
template <template <typename> class U, typename T>
void Graph::addNodeOne(std::shared_ptr<U<T>> n)
{
    _baseNodes.push_back(std::move(n));
}

template <template <typename, typename, typename> class U, typename T, typename T1, typename T2>
void Graph::addNodeTwo(std::shared_ptr<U<T, T1, T2>> n)
{
    _baseNodes.push_back(std::move(n));
}

std::vector<std::shared_ptr<BaseNode>> &Graph::getNodes()
{
    return _baseNodes;
}
