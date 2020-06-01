#include "../include/operation.h"
#include <memory>
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

template <typename T>
T BaseNode::getValue()
{
    switch (_nType)
    {
    case nodeType::variable:
        return static_cast<Variable<T> *>(this)->getValue();
        break;
    case nodeType::placeholder:
        return static_cast<Placeholder<T> *>(this)->getValue();
        break;
    case nodeType::operation:
        return static_cast<Operation<T> *>(this)->getValue();
        break;
    }
}

nodeType BaseNode::getNodeType()
{
    return _nType;
}

std::vector<BaseNode *> &BaseNode::getInputs() { return _inputs; }

std::vector<BaseNode *> BaseNode::getConsumers() { return _consumers; }

// --- Node  ---

template <template <typename> class U, typename T>
void Node<U, T>::setName(std::string n) { _name = n; }

template <template <typename> class U, typename T>
T Node<U, T>::getValue()
{
    return static_cast<U<T> *>(this)->getValue();
}

template <template <typename> class U, typename T>
void Node<U, T>::setValue(T t)
{
    static_cast<U<T> *>(this)->setValue(t);
}

template <template <typename> class U, typename T>
std::string Node<U, T>::getName() { return _name; }

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

template <typename T>
T Variable<T>::getValue()
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

// --- Placeholder ---

template <typename T>
Placeholder<T>::Placeholder(std::string n)
{
    this->_nType = nodeType::placeholder;
    static_cast<Node<Placeholder, T> *>(this)->setName(n);
}

template <typename T>
void Placeholder<T>::setValue(T t)
{
    _dataAvailable = true;
    _output.reset(new T(t));
}

template <typename T>
T Placeholder<T>::getValue()
{
    std::cout << "Placeholder get value..." << std::endl;
    if (_dataAvailable)
    {
        std::cout << "Output is: " << *_output << std::endl;
        return *_output;
    }
    else
    {
        std::cout << "Data is not available, feed the data first... " << std::endl;
        return T();
    }
}
