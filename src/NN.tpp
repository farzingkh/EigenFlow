#include <iostream>

NN::NN()
{
    _graph = Graph();
    _session = Session();
};

template <typename T>
Variable<T> *NN::variable(T &&t)
{
    std::cout << "nn variable constructor" << std::endl;
    auto v = std::shared_ptr<Variable<T>>(new Variable<T>(std::move(t)));
    _graph.addVariable<T>(v);
    return v.get();
}

template <typename T>
Placeholder<T> *NN::placeholder(std::string n)
{
     std::cout << "nn placeholder constructor" << std::endl;
    auto plc = std::shared_ptr<Placeholder<T>>(new Placeholder<T>(n));
    _graph.addPlaceholder<T>(plc);
    return plc.get();
}

template <typename T, typename T1, typename T2>
Add<T, T1, T2> *NN::add(BaseNode *a, BaseNode *b)
{
    auto c = std::shared_ptr<Add<T, T1, T2>>(new Add<T, T1, T2>(a, b));
    _graph.addOperation<T>(c);
    return c.get();
}

template <typename T>
Negative<T> *NN::negative(BaseNode *a)
{
    auto c = std::shared_ptr<Negative<T>>(new Negative<T>(a));
    _graph.addOperation<T>(c);
    return c.get();
}

template <typename T>
T NN::run(BaseNode *n, std::unordered_map<std::string, T *> feed)
{
    T r = _session.Run<T>(n, feed);
    return r;
}

template <typename T, typename T1, typename T2>
Multiply<T, T1, T2> *NN::multiply(BaseNode *a, BaseNode *b)
{
    auto c = std::shared_ptr<Multiply<T, T1, T2>>(new Multiply<T, T1, T2>(a, b));
    _graph.addOperation<T>(c);
    return c.get();
}