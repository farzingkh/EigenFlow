#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>
#include <Eigen/Dense>

// A matrix of ints with a dynamic size, Use it when the size is not known
typedef Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> matrixXi;

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
    std::vector<BaseNode *> getInputs();
    std::vector<BaseNode *> getConsumers();

private:
    std::vector<BaseNode *> _consumers = {}; // child nodes
    std::vector<BaseNode *> _inputs = {};    // parent nodes
};

/* Class for nodes of the computational graph; 
each node is one of the three:
 - an operation
 - a variable
 - a placeholder
*/
template <typename T>
class Node: public BaseNode
{
public:
    Node(nodeType tp);

    void setName(std::string n);

    T getValue();
    void setValue(T t);

    nodeType getType();
    std::string getName();

private:
    nodeType _tp;
    std::string _name = "";
};

// A class for variables of type T
template <typename T>
class Variable : public Node<T>
{
public:
    Variable(T a);
    T getValue();

private:
    T _output;
    bool _dataAvailable = false;
};

// A class for placeholders for values of type T
template <typename T>
class Placeholder : public Node<T>
{
public:
    Placeholder(std::string n);
    T getValue();
    void setValue(T t);

private:
    T _output;
    bool _dataAvailable;
};

#include "../src/node.tpp"

#endif /* NODE_H */