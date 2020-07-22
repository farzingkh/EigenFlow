#ifndef OPERATION_H
#define OPERATION_H

#include "../include/gradientDescentOptimizer.h"

// Class for operations with data value of type T
template <typename T>
class Operation : public Node<T>
{
public:
    Operation();
    virtual void compute() = 0;
    virtual void gradient() = 0;
};

template <typename T>
class UnaryOperation : public Operation<T>
{
public:
    UnaryOperation(BaseNode *rhs);
};

template <typename T>
class BinaryOperation : public Operation<T>
{
public:
    BinaryOperation(BaseNode *lhs, BaseNode *rhs);
};

// ---- Operations ----

// addition operation with T return type value, T1 and T2 input type value
template <typename T, typename T1, typename T2>
class Add : public BinaryOperation<T>
{
public:
    Add(BaseNode &a, BaseNode &b);
    Add(BaseNode *a, BaseNode *b);

    void compute();
    void gradient();
};

// negative operation
template <typename T>
class Negative : public UnaryOperation<T>
{
public:
    Negative(BaseNode &a);
    Negative(BaseNode *a);

    void compute();
    void gradient();
};

// Elementwise multiplication
template <typename T, typename T1, typename T2>
class Multiply : public BinaryOperation<T>
{
public:
    Multiply(BaseNode &a, BaseNode &b);
    Multiply(BaseNode *a, BaseNode *b);

    void compute();
    void gradient();
};

// Matrix multiply
template <typename T, typename T1, typename T2>
class MatMultiply : public BinaryOperation<T>
{
public:
    MatMultiply(BaseNode &a, BaseNode &b);
    MatMultiply(BaseNode *a, BaseNode *b);

    void compute();
    void gradient();
};

// Vector dot product
template <typename T, typename T1, typename T2>
class Dot : public BinaryOperation<T>
{
public:
    Dot(BaseNode &a, BaseNode &b);
    Dot(BaseNode *a, BaseNode *b);

    void compute();
    void gradient();
};

// Element-wise sigmoid function
template <typename T>
class Sigmoid : public UnaryOperation<T>
{
public:
    Sigmoid(BaseNode &a);
    Sigmoid(BaseNode *a);

    void compute();
    void gradient();
};

// Element-wise Log operation
template <typename T>
class Log : public UnaryOperation<T>
{
public:
    Log(BaseNode &a);
    Log(BaseNode *a);

    void compute();
    void gradient();
};

// Reduce sum operation
template <typename T>
class Sum : public UnaryOperation<T>
{
public:
    Sum(BaseNode &a, int axis);
    Sum(BaseNode *a, int axis);

    void compute();
    void gradient();

private:
    //  axis 0 is columnwise, axis 1 is rowwise
    int _axis = 0;
};

/* Minimization Operation. Minimization node doesn't have any inputs or consumers. 
It's only move constructable. */
template <typename T>
class Minimizer : public Operation<T>
{
public:
    Minimizer(GradientDescentOptimizer *grd, BaseNode *loss);
    Minimizer(Minimizer<T> &&other);
    Minimizer<T> &operator=(Minimizer<T> &&other);

    void compute();
    void gradient();

private:
    // copy/assgn deleted
    Minimizer(Minimizer<T> &other) = delete;
    Minimizer<T> &operator=(Minimizer<T> &other) = delete;
    // cash the optimization class to access methods
    Locking_ptr<GradientDescentOptimizer> grdOpt_ = Locking_ptr<GradientDescentOptimizer>(nullptr, &(this->Mtx_));
    Locking_ptr<BaseNode> loss_ = Locking_ptr<BaseNode>(nullptr, &(this->Mtx_));
    std::mutex gMtx_;
    float learningRate_;
};
#include "../src/operation.tpp"

#endif /* OPERATION_H */