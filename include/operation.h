#ifndef OPERATION_H
#define OPERATION_H

//#include "node.h"

// Class for operations with data value of type T
template <typename T>
class Operation : public Node<T>
{
public:
    Operation();
    virtual void compute() = 0;
    virtual void gradient() = 0;
};

// Operations

// addition operation with T return type value, T1 and T2 input type value
template <typename T, typename T1, typename T2>
class Add : public Operation<T>
{
public:
    Add(BaseNode &a, BaseNode &b);
    Add(BaseNode *a, BaseNode *b);

    void compute();
    void gradient();
};

// negative operation
template <typename T>
class Negative : public Operation<T>
{
public:
    Negative(BaseNode &a);
    Negative(BaseNode *a);

    void compute();
    void gradient();
};

template <typename T, typename T1, typename T2>
class Multiply : public Operation<T>
{
public:
    Multiply(BaseNode &a, BaseNode &b);
    Multiply(BaseNode *a, BaseNode *b);

    void compute();
    void gradient();
};

template <typename T>
class Sigmoid : public Operation<T>
{
public:
    Sigmoid(BaseNode &a);
    Sigmoid(BaseNode *a);

    void compute();
    void gradient();
};

#include "../src/operation.tpp"

#endif /* OPERATION_H */