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
};

// Operations

// addition operation with T return type value, T1 and T2 input type value
template <typename T>
class Add : public Operation<T>
{
public:
    Add(BaseNode &a, BaseNode &b);
    Add(BaseNode *a, BaseNode *b);

    void compute();
};

// negative operation
template <typename T>
class Negative : public Operation<T>
{
public:
    Negative(BaseNode &a);
    void compute();
};

#include "../src/operation.tpp"

#endif /* OPERATION_H */