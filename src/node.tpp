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

void BaseNode::eraseInput(BaseNode *n)
{
    std::lock_guard<std::mutex> lck(BaseMtx_);
    for (int i = 0; i < _inputs.size(); i++)
    {
        if (_inputs[i] == n)
        {
            _inputs.erase(_inputs.begin() + i);
        }
    }
}

void BaseNode::addConsumers(BaseNode *n)
{
    std::lock_guard<std::mutex> lck(BaseMtx_);
    _consumers.push_back(n);
}

void BaseNode::eraseConsumer(BaseNode *n)
{
    std::lock_guard<std::mutex> lck(BaseMtx_);
    for (int i = 0; i < _consumers.size(); i++)
    {
        if (_consumers[i] == n)
        {
            _consumers.erase(_consumers.begin() + i);
        }
    }
}

template <typename T>
std::shared_ptr<T> BaseNode::getValue()
{
    return dynamic_cast<Node<T> *>(this)->getValue();
}

template <typename T>
T BaseNode::getGradient()
{
    return dynamic_cast<Node<T> *>(this)->getGradient();
}

template <typename T>
void BaseNode::setGrad(T t)
{
    dynamic_cast<Node<T> *>(this)->setGrad(t);
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
T Node<T>::getGradient()
{
    // lock to avoid data race
    std::unique_lock<std::mutex> lock1(NodeMtx_, std::adopt_lock);
    // get consumers
    std::vector<BaseNode *> consumers = this->getConsumers();
    // Initialize node's gradient
    T grad;
    // check if node has a consumer
    if (consumers.size() > 0)
    {
        // wait until gradient data is available
        cond_.wait(lock1, [this]() { return this->_gradientAvailable; });
        grad.setZero(_grad[0]->rows(), _grad[0]->cols());
        // Go through all consumers to get total derivative
        for (auto &&g : _grad)
        {
            grad += *g;
        }
        std::cout << "Total gradient get: " << grad << ", size: " << grad.rows() << "," << grad.cols() << std::endl;
        return grad;
    }
    else
    {
        //return 1s if there is no consumre
        std::cout << "No consumer" << std::endl;
        grad.setOnes((*_output).rows(), (*_output).cols());
        return grad;
    }
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
    // lock to avoid data race
    std::unique_lock<std::mutex> lock1(NodeMtx_, std::adopt_lock);

    // create unique pointer of grad and append to _grad
    std::cout << "Gradient set: " << t << ", size: " << t.rows() << "," << t.cols() << std::endl;

    // check if it's next epoch
    if (_gradientAvailable)
    {
        std::cout << "consumer size:" << this->getConsumers().size() << std::endl;
        std::cout << "Gradient size:" << _grad.size() << std::endl;
        // reset gradients
        _gradientAvailable = false;
        _grad.clear();
        // add new gradient
        _grad.push_back(std::move(std::unique_ptr<T>((new T(t)))));
    }
    else
    {
        _grad.push_back(std::move(std::unique_ptr<T>((new T(t)))));
    }

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
        // notify all threads waiting for this data
        cond_.notify_all();
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
}

template <typename T>
void Variable<T>::updateValue(float lr)
{
    std::cout << "Variable update value ..." << std::endl;
    //variable has only one input gradient
    T grad = this->getGradient();
    std::shared_ptr<T> output = this->getValue();
    // update variable values based on learning rate and gradient
    this->setValue(*output - (grad * lr));
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
    return;
}