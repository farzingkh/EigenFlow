#ifndef OPERATION_H
#define OPERATION_H

// Class for operations with data value of type T
template <typename T>
class UnaryOperation : public Node<T>
{
public:
    UnaryOperation(BaseNode *rhs);
    virtual void compute() = 0;
    virtual void gradient() = 0;
};

template <typename T>
class BinaryOperation : public Node<T>
{
public:
    BinaryOperation(BaseNode *lhs, BaseNode *rhs);
    virtual void compute() = 0;
    virtual void gradient() = 0;
};

// Operations

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

template <typename T, typename T1, typename T2>
class Multiply : public BinaryOperation<T>
{
public:
    Multiply(BaseNode &a, BaseNode &b);
    Multiply(BaseNode *a, BaseNode *b);

    void compute();
    void gradient();
};

template <typename T, typename T1, typename T2>
class MatMultiply : public BinaryOperation<T>
{
public:
    MatMultiply(BaseNode &a, BaseNode &b);
    MatMultiply(BaseNode *a, BaseNode *b);

    void compute();
    void gradient();
};

template <typename T, typename T1, typename T2>
class Dot : public BinaryOperation<T>
{
public:
    Dot(BaseNode &a, BaseNode &b);
    Dot(BaseNode *a, BaseNode *b);

    void compute();
    void gradient();
};

template <typename T>
class Sigmoid : public UnaryOperation<T>
{
public:
    Sigmoid(BaseNode &a);
    Sigmoid(BaseNode *a);

    void compute();
    void gradient();
};

template <typename T>
class Log : public UnaryOperation<T>
{
public:
    Log(BaseNode &a);
    Log(BaseNode *a);

    void compute();
    void gradient();
};

template <typename T>
class Sum : public UnaryOperation<T>
{
public:
    Sum(BaseNode &a, int axis);
    Sum(BaseNode *a, int axis);

    void compute();
    void gradient();

private:
    int _axis = 0;
};

#include "../src/operation.tpp"

#endif /* OPERATION_H */