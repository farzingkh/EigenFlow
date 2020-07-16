#ifndef NODE_H
#define NODE_H

#include <vector>
#include <Eigen/Dense>
#include <Eigen/Core>
#include <memory>
#include <mutex>
#include <condition_variable>

// A matrix of ints with a dynamic size, Use it when the size is not known
typedef Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic> matXXi;

// enum for operation types
enum class operationType
{
    NA,
    addition,
    negative,
    multiply,
    dotproduct,
    sigmoid,
    log,
    sum
};

// Node types
enum nodeType
{
    NA,
    minimizer,
    operation,
    variable,
    placeholder
};

// Base node class
class BaseNode
{
public:
    void addInputs(BaseNode *n);
    void eraseInput(BaseNode *n);
    void addConsumers(BaseNode *n);
    void eraseConsumer(BaseNode *n);
    void setName(std::string n);

    // get output value of this node
    template <typename T>
    std::shared_ptr<T> getValue();

    // get total gradient from node's consumer
    template <typename T>
    T getGradient();

    // set gradient from consumer
    template <typename T>
    void setGrad(T t);
    

    // make this abstract base class
    virtual void clearGrads() = 0;
    virtual void compute() = 0;
    virtual void gradient() = 0;

    nodeType getNodeType();
    operationType getOperationType();
    std::vector<BaseNode *> &getConsumers();
    std::vector<BaseNode *> &getInputs();
    std::string getName();

    // keep the size of consumers as an atomic data
    std::atomic_int consumerSize_{0};

protected:
    std::string _name = " ";
    nodeType _nType;       // node type
    operationType _opType; // type if node is operation
    std::mutex BaseMtx_;

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
    std::shared_ptr<T> getValue();
    T getGradient();

    void setValue(T &&t);
    void setGrad(T t);
    void clearGrads();

protected:
    std::mutex NodeMtx_;
    std::mutex DataMtx_;
    std::condition_variable cond_;

private:
    // ouput might be shared
    std::shared_ptr<T> _output{nullptr};
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
    Variable(Variable<T> const &v);
    Variable(Variable<T> &&v);
    Variable<T> &operator=(Variable<T> const &v);
    Variable<T> &operator=(Variable<T> &&v);
    

    void compute();
    void gradient();
    void updateValue(float lr);
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