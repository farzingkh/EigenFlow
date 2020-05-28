#include "../include/operation.h"
#include <iostream>

// --- BaseNode ---

void BaseNode::addInputs(BaseNode *n)
{
    _inputs.push_back(n);
}

void BaseNode::addConsumers(BaseNode *n)
{
    _consumers.push_back(n);
}

std::vector<BaseNode *> BaseNode::getInputs() { return _inputs; }

std::vector<BaseNode *> BaseNode::getConsumers() { return _consumers; }

// --- Node  ---
template <typename T>
Node<T>::Node(nodeType tp) : _tp(tp){};

template <typename T>
void Node<T>::setName(std::string n) { _name = n; }

template <typename T>
T Node<T>::getValue()
{
    nodeType ntp = getType();
    switch (ntp)
    {
    case (nodeType::operation):
        return static_cast<Operation<T> *>(this)->getValue();
    case (nodeType::variable):
        return static_cast<Variable<T> *>(this)->getValue();
    case (nodeType::placeholder):
        return static_cast<Placeholder<T> *>(this)->getValue();
    }
    return {};
}

template <typename T>
void Node<T>::setValue(T t)
{
    nodeType ntp = getType();
    switch (ntp)
    {
    case (nodeType::operation):
        static_cast<Operation<T> *>(this)->setValue(t);
        break;
    case (nodeType::variable):
        static_cast<Variable<T> *>(this)->setValue(t);
        break;
    case (nodeType::placeholder):
        break;
    }
    return;
}

template <typename T>
nodeType Node<T>::getType() { return _tp; }

template <typename T>
std::string Node<T>::getName() { return _name; }

// --- Variable ---

template <typename T>
Variable<T>::Variable(T a) : Node<T>(nodeType::variable), _output(a), _dataAvailable(true) {}

template <typename T>
T Variable<T>::getValue()
{
    std::cout << "Variable get value..." << std::endl;
    if (_dataAvailable)
    {
        return _output;
    }
    else
    {
        std::cout << "Data not available" << std::endl;
        return {};
    }
}

// --- Placeholder ---

template <typename T>
Placeholder<T>::Placeholder(std::string n) : Node<T>(nodeType::placeholder)
{
    static_cast<Node<T> *>(this)->setName(n);
}

template <typename T>
void Placeholder<T>::setValue(T t)
{
    _dataAvailable = true;
    _output = t;
}

template <typename T>
T Placeholder<T>::getValue()
{
    std::cout << "Placeholder get value..." << std::endl;
    if (_dataAvailable)
    {
        return _output;
    }
    else
    {
        std::cout << "Data is not available, feed the data first... " << std::endl;
        return {};
    }
}
