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

    template <typename T>
    BaseNode *variable(Matrix<T, Dynamic, Dynamic> &&t);

    template <typename T>
    BaseNode *placeholder(std::string n);

    template <typename T>
    BaseNode *add(BaseNode *a, BaseNode *b);

    template <typename T>
    BaseNode *negative(BaseNode *a);

    template <typename T>
    BaseNode *multiply(BaseNode *a, BaseNode *b);

    template <typename T>
    BaseNode *matmultiply(BaseNode *a, BaseNode *b);

    template <typename T>
    BaseNode *dot(BaseNode *a, BaseNode *b);

    template <typename T>
    BaseNode *sigmoid(BaseNode *a);

    template <typename T>
    BaseNode *log(BaseNode *a);

    template <typename T>
    BaseNode *sum(BaseNode *a, int axis);

    template <typename T>
    void run(BaseNode *n, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed);

    // helper function to check gradient calculations
    template <typename T>
    void checkAllGradient(BaseNode *loss, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed);

private:
    template <typename T, template <typename> class U>
    BaseNode *UnaryOperation(BaseNode *a);

    template <typename T, template <typename, typename, typename> class U>
    BaseNode *BinaryOperation(BaseNode *a, BaseNode *b);

    template <typename T>
    void checkGradient(BaseNode *n, BaseNode *loss, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed);

    void swapNodes(BaseNode *a, BaseNode *b);

    Graph _graph;
    Session _session;
};

#include "../src/NN.tpp"
#endif /* NN_H */