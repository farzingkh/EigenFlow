#include "../include/operation.h"
#include <memory>
#include <iostream>

// --- BaseNode ---
void BaseNode::setName(std::string n) { _name = n; }

void BaseNode::addInputs(BaseNode *n)
{
    _inputs.push_back(n);
}

void BaseNode::addConsumers(BaseNode *n)
{
    _consumers.push_back(n);
}

template <typename T>
T BaseNode::getValue()
{
    return static_cast<Node<T> *>(this)->getValue();
}

std::string BaseNode::getName() { return _name; }

std::vector<BaseNode *> &BaseNode::getInputs() { return _inputs; }

std::vector<BaseNode *> &BaseNode::getConsumers() { return _consumers; }

nodeType BaseNode::getNodeType() { return _nType; }

operationType BaseNode::getOperationType() { return _opType; }

// --- Node  ---

template <typename T>
T Node<T>::getValue()
{
    std::cout << "Variable get value..." << std::endl;
    if (_dataAvailable)
    {
        std::cout << "Output is: " << *_output << std::endl;
        return *_output;
    }
    else
    {
        std::cout << "Data not available" << std::endl;
        return T();
    }
}

template <typename T>
void Node<T>::setValue(T t)
{
    _dataAvailable = true;
    _output.reset(new T(t));
}

// --- Variable ---

template <typename T>
Variable<T>::Variable(T &&a)
{
    this->_nType = nodeType::variable;
    this->setValue(a);
    std::cout << "Variable contructor ..." << std::endl;
}

template <typename T>
Variable<T>::Variable(const Variable<T> &v)
{
    std::cout << "Variable copy contructor ..." << std::endl;
    this->_nType = nodeType::variable;
    this->setValue((&v)->getValue());
}

template <typename T>
void Variable<T>::compute() { return; }

// --- Placeholder ---

template <typename T>
Placeholder<T>::Placeholder(std::string n)
{
    this->_nType = nodeType::placeholder;
    this->setName(n);
}

template <typename T>
void Placeholder<T>::compute() { return; }