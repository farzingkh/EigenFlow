#ifndef OPERATION_H
#define OPERATION_H

//#include "node.h"

// enum for operation types
enum class opType
{
    addition,
    negative
};

// Class for operations with data value of type T
template <typename T>
class Operation : public Node<T>
{
public:
    Operation(opType ot);
    T getValue();
    opType getType();

    void setValue(T t);
    void compute();
    void setType(opType otp);

private:
    T _output;
    opType _otp;
    bool _dataAvailable = false;
};

// Operations

// addition operation with T type value
template <typename T>
class add : public Operation<T>
{
public:
    add(BaseNode &a, BaseNode &b);
    //add(Node<T> &&a, Node<T> &&b);

    void compute();
};

// negative operation
template <typename T>
class negative : public Operation<T>
{
public:
    negative(BaseNode &a);
    void compute();
};

#include "../src/operation.tpp"

#endif /* OPERATION_H */