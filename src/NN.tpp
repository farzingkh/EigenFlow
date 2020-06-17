#include <iostream>

using namespace Eigen;

NN::NN()
{
    _graph = Graph();
    _session = Session();
};

template <typename T>
Matrix<T, Dynamic, Dynamic> NN::run(BaseNode *n, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed)
{
    return _session.Run<T>(n, feed);
}

template <typename T>
Variable<Matrix<T, Dynamic, Dynamic>> *NN::variable(Matrix<T, Dynamic, Dynamic> &&t)
{
    std::cout << "nn variable constructor" << std::endl;
    auto v = std::shared_ptr<Variable<Matrix<T, Dynamic, Dynamic>>>(new Variable<Matrix<T, Dynamic, Dynamic>>(std::move(t)));
    _graph.addNodeOne<Variable, Matrix<T, Dynamic, Dynamic>>(v);
    return v.get();
}

template <typename T>
Placeholder<Matrix<T, Dynamic, Dynamic>> *NN::placeholder(std::string n)
{
    std::cout << "nn placeholder constructor" << std::endl;
    auto plc = std::shared_ptr<Placeholder<Matrix<T, Dynamic, Dynamic>>>(new Placeholder<Matrix<T, Dynamic, Dynamic>>(n));
    _graph.addNodeOne<Placeholder, Matrix<T, Dynamic, Dynamic>>(plc);
    return plc.get();
}

template <typename T>
Add<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *NN::add(BaseNode *a, BaseNode *b)
{
    auto c = std::shared_ptr<Add<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>>(new Add<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(a, b));
    _graph.addNodeTwo<Add, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(c);
    return c.get();
}

template <typename T>
Negative<Matrix<T, Dynamic, Dynamic>> *NN::negative(BaseNode *a)
{
    auto c = std::shared_ptr<Negative<Matrix<T, Dynamic, Dynamic>>>(new Negative<Matrix<T, Dynamic, Dynamic>>(a));
    _graph.addNodeOne<Negative, Matrix<T, Dynamic, Dynamic>>(c);
    return c.get();
}

template <typename T>
Multiply<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *NN::multiply(BaseNode *a, BaseNode *b)
{
    auto c = std::shared_ptr<Multiply<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>>(new Multiply<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(a, b));
    _graph.addNodeTwo<Multiply, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(c);
    return c.get();
}

template <typename T>
MatMultiply<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *NN::matmultiply(BaseNode *a, BaseNode *b)
{
    auto c = std::shared_ptr<MatMultiply<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>>(new MatMultiply<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(a, b));
    _graph.addNodeTwo<MatMultiply, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(c);
    return c.get();
}

template <typename T>
Dot<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *NN::dot(BaseNode *a, BaseNode *b)
{
    auto c = std::shared_ptr<Dot<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>>(new Dot<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(a, b));
    _graph.addNodeTwo<Dot, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(c);
    return c.get();
}

template <typename T>
Sigmoid<Matrix<T, Dynamic, Dynamic>> *NN::sigmoid(BaseNode *a)
{
    auto c = std::shared_ptr<Sigmoid<Matrix<T, Dynamic, Dynamic>>>(new Sigmoid<Matrix<T, Dynamic, Dynamic>>(a));
    _graph.addNodeOne<Sigmoid, Matrix<T, Dynamic, Dynamic>>(c);
    return c.get();
}

template <typename T>
Log<Matrix<T, Dynamic, Dynamic>> *NN::log(BaseNode *a)
{
    auto c = std::shared_ptr<Log<Matrix<T, Dynamic, Dynamic>>>(new Log<Matrix<T, Dynamic, Dynamic>>(a));
    _graph.addNodeOne<Log, Matrix<T, Dynamic, Dynamic>>(c);
    return c.get();
}

template <typename T>
Sum<Matrix<T, Dynamic, Dynamic>> *NN::sum(BaseNode *a)
{
    auto c = std::shared_ptr<Sum<Matrix<T, Dynamic, Dynamic>>>(new Sum<Matrix<T, Dynamic, Dynamic>>(a));
    _graph.addNodeOne<Sum, Matrix<T, Dynamic, Dynamic>>(c);
    return c.get();
}