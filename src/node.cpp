#include "../include/graph.h"
#include <iostream>

// --- Node  ---

void Node::addInputs(Node *n)
{
    _inputs.push_back(n);
}

void Node::addConsumers(Node *n)
{
    _consumers.push_back(n);
}

Node::nodeType Node::getType() { return _tp; }

std::vector<Node *> Node::getInputs() { return _inputs; }

std::vector<Node *> Node::getConsumers() { return _consumers; }

// --- Operation ---

Operation::Operation()
{
    _tp = Node::nodeType::operation;
}

// --- Variable ---

template <typename T>
Variable::Variable(T a)
{
    _tp = Node::nodeType::variable;
    _output = a;
}

Node *Variable::Node()
{
    return _baseNode;
}

virtual T Variable::getValue() override
{
    return _output;
}

// --- Placeholder ---

template <typename T>
void Placeholder::Placeholder(std::string n)
{
    _tp = Node::nodeType::placeholder;
    _name = n;
}

template <typename T>
void Placeholder::setValue(T t)
{
    _dataAvailable = true;
    _output = t;
}

Node *Placeholder::Node()
{
    return _baseNode;
}

template <typename T>
virtual T Placeholder::getValue() override
{
    if (_dataAvailable)
    {
        return _output;
    }
    else
    {
        std::cout << "Placeholder data is not available, feed the data first... " << std::endl;
        return {};
    }
}

std::string Placeholder::getName()
{
    return _name;
}

// --- add ---

template <typename T>
void add::add(Node *a, Node *b)
{
    _baseNode.addInputs(a);
    _baseNode.addInputs(b);

    a.addConsumer(this);
    b.addConsumer(this);
}

virtual auto getValue() override
{
    std::cout << "Compute add" << std::endl;
    _output = a.getValue() + b.getValue();
    return _output;
}
