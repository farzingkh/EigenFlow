
NN::NN()
{
    _graph = Graph();
    _session = Session();
};

template <typename T>
Variable<T> NN::variable(T &&t)
{
    auto v = std::shared_ptr<Variable<T>>(new Variable<T>(std::move(t)));
    _graph.addVariable<T>(v);
    return *v;
}

template <typename T>
Placeholder<T> NN::placeholder()
{
    auto plc = std::shared_ptr<Placeholder<T>>(new Placeholder<T>());
    _graph.addPlaceholder<T>(plc);
    return *plc;
}

template <typename Tout, typename Tin1, typename Tin2>
Add<Tout> NN::add(Variable<Tin1> &&a, Variable<Tin2> &&b)
{
    auto A = std::shared_ptr<Variable<Tin1>>(new Variable<Tin1>(a));
    auto B = std::shared_ptr<Variable<Tin2>>(new Variable<Tin2>(b));

    _graph.addVariable<Tin1>(A);
    _graph.addVariable<Tin2>(B);

    auto C = Add<Tout>(A.get(), B.get());

    return C;
}

template <typename T>
T NN::run(BaseNode &n, std::unordered_map<std::string, T *> feed)
{
    T r = _session.Run<T>(n, feed);
    return r;
}
