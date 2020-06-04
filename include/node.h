#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>
#include <Eigen/Dense>

// A matrix of ints with a dynamic size, Use it when the size is not known
typedef Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> matrixXi;

// enum for operation types
enum class operationType
{
    addition,
    negative
};

// Node types
enum nodeType
{
    operation,
    variable,
    placeholder
};

// Base node class
class BaseNode : public std::enable_shared_from_this<BaseNode>
{
public:
    void addInputs(BaseNode *n);
    void addConsumers(BaseNode *n);
    void setName(std::string n);

    template <typename T>
    T getValue();

    virtual void compute() = 0;

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
    void setValue(T t);

private:
    std::unique_ptr<T> _output = nullptr;
    bool _dataAvailable = false;
};

// A class for variables of type T
template <typename T>
class Variable : public Node<T>
{
public:
    Variable(T &&a);
    Variable(const Variable<T> &v);
    void compute();
};

// A class for placeholders for values of type T
template <typename T>
class Placeholder : public Node<T>
{
public:
    Placeholder(std::string n);
    void compute();
};

#include "../src/node.tpp"

#endif /* NODE_H */