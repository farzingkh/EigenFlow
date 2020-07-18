
template <template <typename> class U, typename T>
BaseNode* Graph::addNodeOne(std::unique_ptr<U<T>> n)
{
    _baseNodes.push_back(std::move(n));
    return _baseNodes.back().get();
}

template <template <typename, typename, typename> class U, typename T, typename T1, typename T2>
BaseNode* Graph::addNodeTwo(std::unique_ptr<U<T, T1, T2>> n)
{
    _baseNodes.push_back(std::move(n));
    return _baseNodes.back().get();
}

std::vector<std::unique_ptr<BaseNode>> &Graph::getNodes()
{
    return _baseNodes;
}
