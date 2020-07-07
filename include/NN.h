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
    Variable<Matrix<T, Dynamic, Dynamic>> *variable(Matrix<T, Dynamic, Dynamic> &&t);

    template <typename T>
    Placeholder<Matrix<T, Dynamic, Dynamic>> *placeholder(std::string n);

    template <typename T>
    Add<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *add(BaseNode *a, BaseNode *b);

    template <typename T>
    Negative<Matrix<T, Dynamic, Dynamic>> *negative(BaseNode *a);

    template <typename T>
    Multiply<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *multiply(BaseNode *a, BaseNode *b);

    template <typename T>
    MatMultiply<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *matmultiply(BaseNode *a, BaseNode *b);

    template <typename T>
    Dot<Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>, Matrix<T, Dynamic, Dynamic>> *dot(BaseNode *a, BaseNode *b);

    template <typename T>
    Sigmoid<Matrix<T, Dynamic, Dynamic>> *sigmoid(BaseNode *a);

    template <typename T>
    Log<Matrix<T, Dynamic, Dynamic>> *log(BaseNode *a);

    template <typename T>
    Sum<Matrix<T, Dynamic, Dynamic>> *sum(BaseNode *a, int axis);

    template <typename T>
    void run(BaseNode *n, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed);

    // helper function to check gradient calculations
    template <typename T>
    void checkAllGradient(BaseNode *loss, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed);

protected:
    template <typename T>
    void checkGradient(BaseNode *n, BaseNode *loss, std::unordered_map<std::string, Matrix<T, Dynamic, Dynamic> *> feed);

    void swapNodes(BaseNode *a, BaseNode *b);

private:
    Graph _graph;
    Session _session;
};

#include "../src/NN.tpp"
#endif /* NN_H */