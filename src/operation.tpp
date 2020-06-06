#include <iostream>
#include <memory>

// --- Operation ---

template <typename T>
Operation<T>::Operation()
{
    this->_nType = nodeType::operation;
}

// --- add operation ---

template <typename T, typename T1, typename T2>
Add<T, T1, T2>::Add(BaseNode &a, BaseNode &b)
{
    this->_opType = operationType::addition;

    this->addInputs(&a);
    this->addInputs(&b);

    a.addConsumers(this);
    b.addConsumers(this);
}

template <typename T, typename T1, typename T2>
Add<T, T1, T2>::Add(BaseNode *a, BaseNode *b)
{
    this->_opType = operationType::addition;

    this->addInputs(a);
    this->addInputs(b);

    a->addConsumers(this);
    b->addConsumers(this);
}

template <typename T, typename T1, typename T2>
void Add<T, T1, T2>::compute()
{
    Node<T> *pNode = static_cast<Node<T> *>(this);
    std::cout << "Compute add operation ..." << std::endl;
    std::vector<BaseNode *> inputs = this->getInputs();
    pNode->setValue(inputs[0]->getValue<T1>() + inputs[1]->getValue<T2>());
}

template <typename T, typename T1, typename T2>
void Add<T, T1, T2>::gradient() { return; };

// --- negative operation---

template <typename T>
Negative<T>::Negative(BaseNode &a)
{
    this->_opType = operationType::negative;
    this->addInputs(&a);
    a.addConsumers(this);
}

template <typename T>
Negative<T>::Negative(BaseNode *a)
{
    this->_opType = operationType::negative;
    this->addInputs(a);
    a->addConsumers(this);
}

template <typename T>
void Negative<T>::compute()
{
    Node<T> *pN = static_cast<Node<T> *>(this);
    std::cout << "Compute negative operation ..." << std::endl;
    std::vector<BaseNode *> inputs = this->getInputs();
    pN->setValue(-(inputs[0]->getValue<T>()));
}

template <typename T>
void Negative<T>::gradient() { return; };

// --- Multiply Operation ---

template <typename T, typename T1, typename T2>
Multiply<T, T1, T2>::Multiply(BaseNode *a, BaseNode *b)
{
    this->_opType = operationType::multiply;

    this->addInputs(a);
    this->addInputs(b);

    a->addConsumers(this);
    b->addConsumers(this);
}

template <typename T, typename T1, typename T2>
void Multiply<T, T1, T2>::compute()
{
    Node<T> *pNode = static_cast<Node<T> *>(this);
    std::cout << "Compute multiplication operation ..." << std::endl;
    std::vector<BaseNode *> inputs = this->getInputs();
    pNode->setValue(inputs[0]->getValue<T1>() * inputs[1]->getValue<T2>());
}

template <typename T, typename T1, typename T2>
void Multiply<T, T1, T2>::gradient() { return; };