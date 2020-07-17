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

template <typename T, template <typename> class U>
U<Matrix<T, Dynamic, Dynamic>> *NN::UnaryOperation(BaseNode *a)
{
    // Create a shared_ptr of new U class
    auto v = std::shared_ptr<U<Matrix<T, Dynamic, Dynamic>>>(new U<Matrix<T, Dynamic, Dynamic>>(a));
    // Copy the shared_ptr into graph to make sure its life is extended as necessary
    _graph.addNodeOne<U, Matrix<T, Dynamic, Dynamic>>(v);
    // return the raw pointer 
    return v.get();
}

template <typename T, template <typename, typename, typename> class U>
U<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *NN::NN::BinaryOperation(BaseNode *a, BaseNode *b)
{
    // Create a shared_ptr of new U class
    auto c = std::shared_ptr<U<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>>(new U<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(a, b));
    // Copy the shared_ptr into graph
    _graph.addNodeTwo<U, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(c);
    // return the raw pointer
    return c.get();
}

template <typename T>
Variable<Matrix<T, Dynamic, Dynamic>> *NN::variable(Matrix<T, Dynamic, Dynamic> &&t)
{
    //std::cout << "nn variable constructor" << std::endl;
    auto v = std::shared_ptr<Variable<Matrix<T, Dynamic, Dynamic>>>(new Variable<Matrix<T, Dynamic, Dynamic>>(std::move(t)));
    _graph.addNodeOne<Variable, Matrix<T, Dynamic, Dynamic>>(v);
    return v.get();
}

template <typename T>
Placeholder<Matrix<T, Dynamic, Dynamic>> *NN::placeholder(std::string n)
{
    //std::cout << "nn placeholder constructor" << std::endl;
    auto plc = std::shared_ptr<Placeholder<Matrix<T, Dynamic, Dynamic>>>(new Placeholder<Matrix<T, Dynamic, Dynamic>>(n));
    _graph.addNodeOne<Placeholder, Matrix<T, Dynamic, Dynamic>>(plc);
    return plc.get();
}

template <typename T>
Add<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *NN::add(BaseNode *a, BaseNode *b)
{

    return BinaryOperation<T, Add>(a, b);
}

template <typename T>
Negative<Matrix<T, Dynamic, Dynamic>> *NN::negative(BaseNode *a)
{
    return UnaryOperation<T, Negative>(a);
}

template <typename T>
Multiply<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *NN::multiply(BaseNode *a, BaseNode *b)
{
    return BinaryOperation<T, Multiply>(a, b);
}

template <typename T>
MatMultiply<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *NN::matmultiply(BaseNode *a, BaseNode *b)
{
    return BinaryOperation<T, MatMultiply>(a, b);
}

template <typename T>
Dot<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *NN::dot(BaseNode *a, BaseNode *b)
{
    return UnaryOperation<T, Dot>(a);
}

template <typename T>
Sigmoid<Matrix<T, Dynamic, Dynamic>> *NN::sigmoid(BaseNode *a)
{
    return UnaryOperation<T, Sigmoid>(a);
}

template <typename T>
Log<Matrix<T, Dynamic, Dynamic>> *NN::log(BaseNode *a)
{
    return UnaryOperation<T, Log>(a);
}

template <typename T>
Sum<Matrix<T, Dynamic, Dynamic>> *NN::sum(BaseNode *a, int axis)
{
    return UnaryOperation<T, Sum>(a);
}

template <typename T>
void NN::checkAllGradient(BaseNode *loss, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed)
{
    _session.updateNodesList(loss);
    auto nodes = _session.getNodesList();
    for (auto &n : nodes)
    {
        checkGradient(n.get(), loss, feed);
    }
    std::cout << "All gradients are correct!\n";
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
        //std::cout << "Loss+:" << outP << std::endl;
        swapNodes(n, &newNodeP);
        _session.Run(loss, feed);
        // swap the node with other new node
        swapNodes(&newNodeN, n);
        _session.Run(loss, feed);
        matxxT outN = *(loss->getValue<matxxT>());
        //std::cout << "Loss-:" << outN << std::endl;
        // swap the node back in and compute the graph
        swapNodes(n, &newNodeN);
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
    std::vector<Locking_ptr<BaseNode>> consumers = out->getConsumers();
    // only end node (i.e. loss) has no consumers
    for (auto cns : consumers)
    {
        // remove other node from its consumers' input
        cns->eraseInput(out);
        // add this node to consumer's input
        cns->addInputs(in);
        // remove consumers of other node
        out->eraseConsumer(cns.get());
        // add consumers to new node
        in->addConsumers(cns.get());
    }
}