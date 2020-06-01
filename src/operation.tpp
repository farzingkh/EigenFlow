#include <iostream>
#include <memory>

// --- Operation ---

template <typename T>
Operation<T>::Operation()
{
    this->_nType = nodeType::operation;
}

template <typename T>
T Operation<T>::getValue()
{
    std::cout << "Operation get value..." << std::endl;
    if (_dataAvailable)
    {
        std::cout << "Operation output is: " << *_output << std::endl;
        return *_output;
    }
    else
    {
        std::cout << "Data not available" << std::endl;
        return T();
    }
}

template <typename T>
void Operation<T>::setValue(T t)
{
    _dataAvailable = true;
    _output.reset(new T(t));
}

template <typename T>
void Operation<T>::compute()
{
    std::cout << "Operation compute ..." << std::endl;
    switch (this->_opType)
    {
    case operationType::addition:
        static_cast<add<T> *>(this)->compute();
        break;
    case operationType::negative:
        static_cast<negative<T> *>(this)->compute();
        break;
    }
};

// --- add operation ---

template <typename T>
add<T>::add(BaseNode &a, BaseNode &b)
{
    this->_opType = operationType::addition;

    this->addInputs(&a);
    this->addInputs(&b);

    a.addConsumers(this);
    b.addConsumers(this);
}

template <typename T>
add<T>::add(BaseNode &&a, BaseNode &&b)
{
    this->setName("Add");

    auto pvb = static_cast<Variable<T> *>(&b);
    auto pvB = new Variable<T>(*pvb);

    std::cout << pvB->getValue() << std::endl;
    std::cout << pvb->getValue() << std::endl;

    a.addConsumers(this);
    b.addConsumers(this);
}

template <typename T>
void add<T>::compute()
{
    Node<Operation, T> *pNode = static_cast<Node<Operation, T> *>(this);
    std::cout << "Compute add operation ..." << std::endl;
    auto inputs = pNode->getInputs();
    auto n = static_cast<Operation<T> *>(inputs[0]);
    auto m = static_cast<Operation<T> *>(inputs[1]);
    std::cout << "Variable " << n->getName() << " is " << n->getValue() << std::endl;
    std::cout << "Variable " << m->getName() << " is " << m->getValue() << std::endl;
    pNode->setValue(n->getValue() + m->getValue());
}

// --- negative operation---

template <typename T>
negative<T>::negative(BaseNode &a)
{
    this->_opType =operationType::negative;
    this->addInputs(&a);
    a.addConsumers(this);
}

template <typename T>
void negative<T>::compute()
{
    Operation<T> *pOp = static_cast<Operation<T> *>(this);
    std::cout << "Compute negative operation ..." << std::endl;
    std::vector<BaseNode *> inputs = this->getInputs();
    pOp->setValue(-(inputs[0]->getValue<T>()));
}
