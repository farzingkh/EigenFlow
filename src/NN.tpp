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
BaseNode *NN::UnaryOperation(BaseNode *a)
{
    // Create a shared_ptr of new U class
    auto v = std::unique_ptr<U<Matrix<T, Dynamic, Dynamic>>>(new U<Matrix<T, Dynamic, Dynamic>>(a));
    // Copy the shared_ptr into graph to make sure its life is extended as necessary
    return _graph.addNodeOne<U, Matrix<T, Dynamic, Dynamic>>(std::move(v));
}

template <typename T, template <typename, typename, typename> class U>
BaseNode *NN::BinaryOperation(BaseNode *a, BaseNode *b)
{
    // Create a shared_ptr of new U class
    auto c = std::unique_ptr<U<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>>(new U<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(a, b));
    // Copy the shared_ptr into graph
    return _graph.addNodeTwo<U, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>>(std::move(c));
}

template <typename T>
BaseNode *NN::variable(Matrix<T, Dynamic, Dynamic> &&t)
{
    //std::cout << "nn variable constructor" << std::endl;
    auto v = std::unique_ptr<Variable<Matrix<T, Dynamic, Dynamic>>>(new Variable<Matrix<T, Dynamic, Dynamic>>(std::move(t)));
    return _graph.addNodeOne<Variable, Matrix<T, Dynamic, Dynamic>>(std::move(v));

}

template <typename T>
BaseNode *NN::placeholder(std::string n)
{
    //std::cout << "nn placeholder constructor" << std::endl;
    auto plc = std::unique_ptr<Placeholder<Matrix<T, Dynamic, Dynamic>>>(new Placeholder<Matrix<T, Dynamic, Dynamic>>(n));
    return _graph.addNodeOne<Placeholder, Matrix<T, Dynamic, Dynamic>>(std::move(plc));

}

template <typename T>
BaseNode *NN::add(BaseNode *a, BaseNode *b)
{

    return BinaryOperation<T, Add>(a, b);
}

template <typename T>
BaseNode *NN::negative(BaseNode *a)
{
    return UnaryOperation<T, Negative>(a);
}

template <typename T>
BaseNode *NN::multiply(BaseNode *a, BaseNode *b)
{
    return BinaryOperation<T, Multiply>(a, b);
}

template <typename T>
BaseNode *NN::matmultiply(BaseNode *a, BaseNode *b)
{
    return BinaryOperation<T, MatMultiply>(a, b);
}

template <typename T>
BaseNode *NN::dot(BaseNode *a, BaseNode *b)
{
    return UnaryOperation<T, Dot>(a);
}

template <typename T>
BaseNode *NN::sigmoid(BaseNode *a)
{
    return UnaryOperation<T, Sigmoid>(a);
}

template <typename T>
BaseNode *NN::log(BaseNode *a)
{
    return UnaryOperation<T, Log>(a);
}

template <typename T>
BaseNode *NN::sum(BaseNode *a, int axis)
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
    // create +E and -E values
    matxxT value1 = n->getValue<Matrix<T, Dynamic, Dynamic>>()->array() + E;
    matxxT value2 = n->getValue<Matrix<T, Dynamic, Dynamic>>()->array() - E;
    // check if n is loss node
    if (n == loss)
    {
        // get numerical gradient directly
        grad = (value1 - value2) / (2 * E);
    }
    else
    {
        // otherwise create variable nodes and run the session on nodes
        Variable<matxxT> newNodeP(std::move(value1));
        Variable<matxxT> newNodeN(std::move(value2));
        // swap the node with the new variable node
        swapNodes(&newNodeP, n);
        // compute value of loss
        _session.Run(loss, feed);
        matxxT outP = *loss->getValue<matxxT>();
        //std::cout << "Loss+:" << outP << std::endl;
        // swap the node with other new node
        swapNodes(&newNodeN, &newNodeP);
        _session.Run(loss, feed);
        matxxT outN = *loss->getValue<matxxT>();
        //std::cout << "Loss-:" << outN << std::endl;
        // swap the node back in and compute the graph
        swapNodes(n, &newNodeN);
        // find numerical gradient and check the node gradient
        grad = (outP - outN) / (2 * E);
    }
    // As E is added to each element of the matrix we consider the difference of the sum of matrices
    auto er = grad.sum() - n->getGradient<matxxT>().sum();
    // check if the error is within the threshold
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