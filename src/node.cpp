#include "../include/graph.h"

// --- Node  ---

void Node::addInputs(Node *n) { return; }

void Node::addConsumers(Node *n) { return; }

Node::nodeType Node::getType() { return _tp; }

std::vector<Node*> Node::getInputs() { return _inputs; }

std::vector<Node*> Node::getConsumers() { return _consumers; }

// --- Operation ---

// --- Variable ---

template <typename T>
Variable::Variable(T a)
{
    _output = T;
}

T Variable::getValue()
{
    return _output;
}

// --- Placeholder ---
template <typename T>
void Placeholder::setValue(T t)
{
    _output = t;
}

void Placeholder::setName(std::string n)
{
    _name = n;
}

template <typename T>
T Placeholder::getValue()
{
    return _output;
}

std::string Placeholder::getName()
{
    return _name;
}

// --- add ---

void add::add(Node *a, Node *b)
{

}