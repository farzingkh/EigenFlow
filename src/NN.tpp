#include <iostream>
#include <assert.h>

using namespace Eigen;

NN::NN()
{
    _graph = Graph();
    _session = Session();
};

template <typename T>
void NN::run(BaseNode *n, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed)
{
    _session.Run<T>(n, feed);
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
Sum<Matrix<T, Dynamic, Dynamic>> *NN::sum(BaseNode *a, int axis)
{
    auto c = std::shared_ptr<Sum<Matrix<T, Dynamic, Dynamic>>>(new Sum<Matrix<T, Dynamic, Dynamic>>(a, axis));
    _graph.addNodeOne<Sum, Matrix<T, Dynamic, Dynamic>>(c);
    return c.get();
}

template <typename T>
void NN::checkAllGradient(BaseNode *loss, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed)
{
    _session.updateNodesList(loss);
    auto nodes = _session.getNodesList();
    for (auto n : nodes)
    {
        checkGradient(n, loss, feed);
    }
}

// Check node gradient numerically
template <typename T>
void NN::checkGradient(BaseNode *n, BaseNode *loss, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed)
{
    typedef Matrix<T, Dynamic, Dynamic> matxxT;
    // define epsilon and threshold
    long double E = 1.0e-14;
    long double err = 1.0e-3;
    matxxT grad;
    auto value1 = (*(n->getValue<Matrix<T, Dynamic, Dynamic>>())).array() + E;
    auto value2 = (*(n->getValue<Matrix<T, Dynamic, Dynamic>>())).array() - E;
    // check if n is loss node
    if (n == loss)
    {
        // get numerical gradient directly
        grad = (value1 - value2) / (2 * E);
    }
    else
    {
        // otherwise create variable nodes and run the session on nodes
        Variable<matxxT> newNodeP(value1);
        Variable<matxxT> newNodeN(value2);
        // swap the node with the new variable node
        swapNodes(&newNodeP, n);
        // compute value of loss
        _session.Run(loss, feed);
        matxxT outP = *(loss->getValue<matxxT>());
        std::cout << "Loss+:" << outP << std::endl;
        swapNodes(n, &newNodeP);
        _session.Run(loss, feed);
        // swap the node with other new node
        swapNodes(&newNodeN, n);
        _session.Run(loss, feed);
        matxxT outN = *(loss->getValue<matxxT>());
        std::cout << "Loss-:" << outN << std::endl;
        // swap the node back in and compute the graph
        swapNodes(n, &newNodeN);
        _session.Run(loss, feed);
        //std::cout << "Loss: " << *(loss->getValue<matxxT>()) << std::endl;
        // find numerical gradient and check the node gradient
        grad = (outP - outN) / (2 * E);
    }
    auto er = grad.sum() - n->getGradient<matxxT>().sum();
    assert(er < err);
    std::cout << "Numerical gradient: " << grad.sum() << std::endl;
    std::cout << "Node gradient: " << n->getGradient<matxxT>().sum() << std::endl;
}

// swap nodes in and out of computational graph
void NN::swapNodes(BaseNode *in, BaseNode *out)
{
    std::vector<BaseNode *> consumers = out->getConsumers();
    // only end node (i.e. loss) has no consumers
    for (auto cns : consumers)
    {
        // remove other node from its consumers' input
        cns->eraseInput(out);
        // add this node to consumer's input
        cns->addInputs(in);
        // remove consumers of other node
        out->eraseConsumer(cns);
        // add consumers to new node
        in->addConsumers(cns);
    }
}