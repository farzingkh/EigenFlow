#include "../include/operation.h"
#include <memory>
#include <iostream>

// --- BaseNode ---
void BaseNode::setName(std::string n)
{
    std::lock_guard<std::mutex> lck(BaseMtx_);
    _name = n;
}

void BaseNode::addInputs(BaseNode *n)
{
    std::lock_guard<std::mutex> lck(BaseMtx_);
    _inputs.push_back(n);
}

void BaseNode::addConsumers(BaseNode *n)
{
    std::lock_guard<std::mutex> lck(BaseMtx_);
    _consumers.push_back(n);
}

template <typename T>
std::shared_ptr<T> BaseNode::getValue()
{
    return dynamic_cast<Node<T> *>(this)->getValue();
}

template <typename T>
T BaseNode::getGradient(int i)
{
    return dynamic_cast<Node<T> *>(this)->getGradient(i);
}

template <typename T>
T BaseNode::getOutGradient()
{
    std::vector<BaseNode *> consumers = this->getConsumers();
    // Initialize node's gradient
    T grad;
    // check if node has a consumer
    if (consumers.size() > 0)
    {
        // Go through all consumers to get total derivative
        for (auto cons : consumers)
        {
            // check which input node of the consumer is this; cnosidering we only have one consumer
            int nodeIndex;
            std::vector<BaseNode *> consumerInputs = cons->getInputs();
            for (int i = 0; i < consumerInputs.size(); i++)
            {
                if (this == consumerInputs[i])
                {
                    int nodeIndex = i;
                }
            }
            // get the output gradient for this node and add to toal gradient
            T g = cons->getGradient<T>(nodeIndex);
            grad.setZero(g.rows(), g.cols());
            grad += g;
        }
        return grad;
    }
    else
    {
        //return 1 if there is no consumre
        std::cout << "No consumer" << std::endl;
        grad.setOnes(1, 1);
        return grad;
    }
}

std::string BaseNode::getName()
{
    std::lock_guard<std::mutex> lck(BaseMtx_);
    return _name;
}

std::vector<BaseNode *> &BaseNode::getInputs()
{
    std::lock_guard<std::mutex> lck(BaseMtx_);
    return _inputs;
}

std::vector<BaseNode *> &BaseNode::getConsumers()
{
    std::lock_guard<std::mutex> lck(BaseMtx_);
    return _consumers;
}

nodeType BaseNode::getNodeType()
{
    std::lock_guard<std::mutex> lck(BaseMtx_);
    return _nType;
}

operationType BaseNode::getOperationType()
{
    std::lock_guard<std::mutex> lck(BaseMtx_);
    return _opType;
}

// --- Node  ---

template <typename T>
std::shared_ptr<T> Node<T>::getValue()
{
    //std::cout << "Variable get value..." << std::endl;
    // lock to avoid data race
    std::unique_lock<std::mutex> lock(NodeMtx_);
    if (_dataAvailable)
    {
        std::cout << "Output get: " << *_output << ", size: " << (*_output).rows() << "," << (*_output).cols() << std::endl;
        return _output;
    }
    else
    {
        std::cout << "Data not available" << std::endl;
        return std::shared_ptr<T>(nullptr);
    }
}

template <typename T>
T Node<T>::getGradient(int i)
{
    // lock to avoid data race
    std::unique_lock<std::mutex> lock(NodeMtx_);
    // use condition variable to wait until gradient is available
    cond_.wait(lock, [this]() { return _gradientAvailable; });
    std::cout << "Gradient get: " << *(_grad[i]) << ", size: " << (*(_grad[i])).rows() << "," << (*(_grad[i])).cols() << std::endl;
    return *(_grad[i]);
}

template <typename T>
void Node<T>::setValue(T &&t)
{
    std::unique_lock<std::mutex> lock(NodeMtx_);
    _dataAvailable = true;
    _output = std::shared_ptr<T>(new T(t));
    std::cout << "Output set: " << *_output << ", size: " << (*_output).rows() << "," << (*_output).cols() << std::endl;
}

template <typename T>
void Node<T>::setGrad(T t)
{
    // lock to avoid data races
    std::unique_lock<std::mutex> lock(NodeMtx_);
    // create unique pointer of grad and append to _grad
    std::cout << "Gradient set: " << t << ", size: " << t.rows() << "," << t.cols() << std::endl;
    _grad.push_back(std::move(std::unique_ptr<T>((new T(t)))));
    // gradient and value must have same dimensions
    if (t.cols() != (*_output).cols() or t.rows() != (*_output).rows())
    {
        std::cout << "Gradient and output have different dimensions!" << std::endl;
    }
    // check if gradient of all consumers are set
    // use >= as a node might not have a consumer
    if (_grad.size() >= this->getConsumers().size())
    {
        _gradientAvailable = true;
    }
}

// --- Variable ---

template <typename T>
Variable<T>::Variable(T &&a)
{
    std::cout << "Variable contructor ..." << std::endl;
    this->_nType = nodeType::variable;
    this->setValue(std::move(a));
}

template <typename T>
Variable<T>::Variable(Variable<T> &v)
{
    std::cout << "Variable copy contructor ..." << std::endl;
    // lock other side 
    std::unique_lock<std::mutex> rhs_baselk(v.BaseMtx_, std::defer_lock);
    std::unique_lock<std::mutex> rhs_nodelk(v.NodeMtx_, std::defer_lock);
    std::lock(rhs_baselk, rhs_nodelk);
    // copy
    this->_nType = nodeType::variable;
    // set value locks the node, no need to create a lock
    this->setValue((&v)->getValue());
}
template <typename T>
Variable<T>::Variable(Variable<T> &&v)
{
    std::cout << "Variable move contructor ..." << std::endl;
    // lock
    std::unique_lock<std::mutex> rhs_baselk(v.BaseMtx_, std::defer_lock);
    std::unique_lock<std::mutex> rhs_nodelk(v.NodeMtx_, std::defer_lock);
    // lock this to change output value
    std::unique_lock<std::mutex> lhs_nodelk(this->NodeMtx_, std::defer_lock);
    std::lock(rhs_baselk, rhs_nodelk, lhs_nodelk);
    // move
    this->_nType = nodeType::variable;
    this->_output = std::move(v->_output);
}

template <typename T>
void Variable<T>::compute()
{
    return;
}

template <typename T>
void Variable<T>::gradient()
{
    std::cout << "Variable gradient ..." << std::endl;
    this->setGrad(((BaseNode *)this)->getOutGradient<T>());
}

template <typename T>
void Variable<T>::updateGradient(float lr)
{
    //variable has only one input gradient
    T grad = ((BaseNode *)this)->getGradient<T>(0);
    std::shared_ptr<T> output = ((BaseNode *)this)->getValue<T>();
    // update variable values based on learning rate and gradient
    T og = *output - (grad * lr);
    this->setGrad(og);
}

// --- Placeholder ---

template <typename T>
Placeholder<T>::Placeholder(std::string n)
{
    this->_nType = nodeType::placeholder;
    this->setName(n);
}

template <typename T>
void Placeholder<T>::compute()
{
    return;
}

template <typename T>
void Placeholder<T>::gradient()
{
    this->setGrad(((BaseNode *)this)->getOutGradient<T>());
}