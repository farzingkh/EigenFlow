#ifndef NN_H
#define NN_H

#include "graph.h"
#include "session.h"
#include "operation.h"
#include "gradientDescentOptimizer.h"

using namespace Eigen;

class NN
{
public:
    NN();

    // variable node for learnable parameters of the model
    template <typename T>
    BaseNode *variable(Matrix<T, Dynamic, Dynamic> &&t);

    // placeholder node for contant prameters of the node that needs to be fed later with data
    template <typename T>
    BaseNode *placeholder(std::string n);

    // add operation node
    template <typename T>
    BaseNode *add(BaseNode *a, BaseNode *b);

    // negative operation node
    template <typename T>
    BaseNode *negative(BaseNode *a);

    // element-wise multiplication operation
    template <typename T>
    BaseNode *multiply(BaseNode *a, BaseNode *b);

    // matrix multiplication node
    template <typename T>
    BaseNode *matmultiply(BaseNode *a, BaseNode *b);

    // dot product operation node
    template <typename T>
    BaseNode *dot(BaseNode *a, BaseNode *b);

    // sigmoid operation node
    template <typename T>
    BaseNode *sigmoid(BaseNode *a);

    // element-wise log operation node
    template <typename T>
    BaseNode *log(BaseNode *a);

    // reduce sum operation node
    template <typename T>
    BaseNode *sum(BaseNode *a, int axis);

    // run session for operation node 
    template <typename T>
    void run(BaseNode *n, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed);

    // helper function to check gradient calculations
    template <typename T>
    void checkAllGradient(BaseNode *loss, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed);

private:
    // factory method to create unary operations
    template <typename T, template <typename> class U>
    BaseNode *UnaryOperation(BaseNode *a);

    // factory method to create binary operations
    template <typename T, template <typename, typename, typename> class U>
    BaseNode *BinaryOperation(BaseNode *a, BaseNode *b);

    // helper function to check gradient of a node numerically
    template <typename T>
    void checkGradient(BaseNode *n, BaseNode *loss, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed);

    // helper function to swap nodes in and out of graph
    void swapNodes(BaseNode *a, BaseNode *b);

    Graph _graph;
    Session _session;
};

#include "../src/NN.tpp"

#endif /* NN_H */