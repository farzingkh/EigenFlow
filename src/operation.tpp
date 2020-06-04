#include <iostream>
#include <memory>

// --- Operation ---

template <typename T>
Operation<T>::Operation()
{
    this->_nType = nodeType::operation;
}

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
add<T>::add(Variable<T> &&a, Variable<T> &&b)
{   

    a.addConsumers(this);
    b.addConsumers(this);

    this->addInputs(pvA);
    this->addInputs(pvB);
}

template <typename T>
void add<T>::compute()
{
    Node<T> *pNode = static_cast<Node<T> *>(this);
    std::cout << "Compute add operation ..." << std::endl;
    std::vector<std::shared_ptr<BaseNode>> inputs = this->getInputs();
    pNode->setValue(inputs[0]->getValue<T>() + inputs[1]->getValue<T>());
}

// --- negative operation---

template <typename T>
negative<T>::negative(BaseNode &a)
{
    this->_opType = operationType::negative;
    this->addInputs(&a);
    a.addConsumers(this->shared_from_this());
}

template <typename T>
void negative<T>::compute()
{
    Node<T> *pN = static_cast<Node<T> *>(this);
    std::cout << "Compute negative operation ..." << std::endl;
    std::vector<std::shared_ptr<BaseNode>> inputs = this->getInputs();
    pN->setValue(-(inputs[0]->getValue<T>()));
}
