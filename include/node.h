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
class BaseNode
{
public:
    void addInputs(BaseNode *n);
    void addConsumers(BaseNode *n);

    template <typename T>
    T getValue();

    std::vector<BaseNode *> getConsumers();
    std::vector<BaseNode *> &getInputs();

protected:
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
template <template <typename> class U, typename T>
class Node : public BaseNode
{
public:
    void setName(std::string n);

    T getValue();
    void setValue(T t);
    std::string getName();

private:
    std::string _name = " ";
};

// A class for variables of type T
template <typename T>
class Variable : public Node<Variable, T>
{
public:
    Variable(T &&a);
    Variable(Variable<T> &v);
    T getValue();

private:
    std::unique_ptr<T> _output = nullptr;
    bool _dataAvailable = false;
};

// A class for placeholders for values of type T
template <typename T>
class Placeholder : public Node<Placeholder, T>
{
public:
    Placeholder(std::string n);
    T getValue();
    void setValue(T t);

private:
    std::unique_ptr<T> _output = nullptr;
    bool _dataAvailable;
};

#include "../src/node.tpp"

#endif /* NODE_H */