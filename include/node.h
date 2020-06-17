#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>
#include <Eigen/Dense>
#include <Eigen/Core>

// A matrix of ints with a dynamic size, Use it when the size is not known
typedef Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> matXXi;

// enum for operation types
enum class operationType
{
    addition,
    negative,
    multiply,
    dotproduct,
    sigmoid,
    log
};

// Node types
enum nodeType
{
    operation,
    variable,
    placeholder
};

// Base node class
class BaseNode
{
public:
    void addInputs(BaseNode *n);
    void addConsumers(BaseNode *n);
    void setName(std::string n);

    template <typename T>
    T getValue();

    template <typename T>
    T getGradient(int i);
    
    template <typename T>
    T getOutGradient(); // get output gradient from node's consumer

    virtual void compute() = 0;
    virtual void gradient() = 0;

    nodeType getNodeType();
    operationType getOperationType();
    std::vector<BaseNode *> &getConsumers();
    std::vector<BaseNode *> &getInputs();
    std::string getName();

protected:
    std::string _name = " ";
    nodeType _nType;       // node type
    operationType _opType; // type if node is operation

private:
    std::vector<BaseNode *> _consumers = {}; // parent nodes
    std::vector<BaseNode *> _inputs = {};    // child nodes
};

/* Class for nodes of the computational graph; 
each node is one of the three:
 - an operation
 - a variable
 - a placeholder
*/
template <typename T>
class Node : public BaseNode
{
public:
    T getValue();
    T getGradient(int i);

    void setValue(T &&t);
    void setGrad(T &t);

private:
    std::unique_ptr<T> _output = nullptr;
    std::vector<std::unique_ptr<T>> _grad;

    bool _dataAvailable = false;
    bool _gradientAvailable = false;
};

// A class for variables of type T
template <typename T>
class Variable : public Node<T>
{
public:
    Variable(T &&a);
    Variable(Variable<T> &v);
    Variable(Variable<T> &&v);

    void compute();
    void gradient();
};

// A class for placeholders for values of type T
template <typename T>
class Placeholder : public Node<T>
{
public:
    Placeholder(std::string n);

    void compute();
    void gradient();
};

#include "../src/node.tpp"

#endif /* NODE_H */