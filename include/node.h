#include <vector>
#include <memory>
#include <Eigen/Dense>

// A matrix of ints with a dynamic size, Use it when the size is not known
typedef Eigen::Matrix<int, Dynamic, Dynamic> matrixXi;

/* Class for nodes of the computational graph; 
each node is one of the three:
 - an operation
 - a variable
 - a placeholder
*/
class Node
{
public:
    enum nodeType
    {
        operation,
        variable,
        placeholder
    };

    void addInputs(Node *n);
    void addConsumer(Node *n);

    virtual auto getValue();
    nodeType getType();
    std::vector<Node *> getInputs();
    std::vector<Node *> getConsumers();

private:
    std::vector<Node *> _consumers = {}; // child nodes
    std::vector<Node *> _inputs = {};    // parent nodes
    nodeType _tp;
};

// Class for operations with data value of type T1
class Operation : public Node
{
public:
    Operation();
    virtual auto getValue() override;

private:
    auto _output;
};

// A class for variables of type T
template <typename T>
class Variable : public Node
{
public:
    Variable(T a);

    virtual T getValue() override;
    Node *Node(); // return the base node class

private:
    T _output;
    Node *_baseNode{this}; // Get the base Node class
};

// A class for placeholders for values of type T
template <typename T>
class Placeholder : public Node
{
public:
    Placeholder(std::string n);
    void setValue(T *t);

    Node *Node(); // return the base node class
    virtual T getValue() override;
    std::string getName();

private:
    T _output;
    std::string _name;
    Node *_baseNode{this}; // Object slicing
    bool _dataAvailable;
};

// Operations

// A class for add operation with T type value
template <typename T>
class add : public Operation
{
public:
    void add(Node *a, Node *b); 
    Node *Node(); // return the base node class
    virtual auto getValue() override;

private:
    Node *_baseNode{this}; // Object slicing
};
