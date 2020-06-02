#include "../include/operation.h"
#include <memory>
#include <iostream>

// --- BaseNode ---
void BasNode::setName(std::string n) { _name = n; }

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

nodeType BaseNode::getNodeType()
{
    return _nType;
}

std::string BasNode::getName() { return _name; }

std::vector<BaseNode *> &BaseNode::getInputs() { return _inputs; }

std::vector<BaseNode *> BaseNode::getConsumers() { return _consumers; }

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
Variable<T>::Variable(T &&a) : _dataAvailable(true), _output(std::move(std::unique_ptr<T>((new T(a)))))
{
    this->_nType = nodeType::variable;
    std::cout << "Variable contructor ..." << std::endl;
}

template <typename T>
Variable<T>::Variable(Variable<T> &v) : _dataAvailable(true), _output(std::move(std::unique_ptr<T>(new T(v.getValue()))))
{
    this->_nType = nodeType::variable;
    std::cout << "Variable copy contructor ..." << std::endl;
}


// --- Placeholder ---

template <typename T>
Placeholder<T>::Placeholder(std::string n)
{
    this->_nType = nodeType::placeholder;
    static_cast<Node<Placeholder, T> *>(this)->setName(n);
}

