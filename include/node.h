#include <vector>
#include <memory>
#include <Eigen/Dense>

// A matrix of ints with a dynamic size, Use it when the size is not known
typedef Eigen::Matrix<int, Dynamic, Dynamic> matrixXi;

/* Class for nodes of the computational graph; 
each node can have one of the three:
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
template <typename T>
class Operation : public Node
{
public:
    virtual T getValue();

private:
    T _output;
};

// A class for variables of type T
template <typename T>
class Variable : public Node
{
public:
    Variable(T a); // Constructor
    virtual T getValue() override;

private:
    T _output;
};

// A class for placeholders for values of type T
template <typename T>
class Placeholder : public Node
{
public:
    void setValue(T *t);
    void setName(std::string n);
    T getValue() override { return _output; };
    std::string getName();

private:
    T _output;
    std::string _name;
};

// Operations

// A class for add operation with T type value
template <typename T>
class add : public Operation<T>
{
public:
    // constructor creates a node of type operation
    void add(Node *a, Node *b);
    virtual T getValue() override { return _output; };
};
