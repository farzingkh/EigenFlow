#include <iostream>

// --- Operation ---

template <typename T>
Operation<T>::Operation(opType ot) : Node<T>(nodeType::operation), _otp(ot) {}

template <typename T>
T Operation<T>::getValue()
{
    std::cout << "Operation get value..." << std::endl;
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

template <typename T>
opType Operation<T>::getType()
{
    return _otp;
}

template <typename T>
void Operation<T>::setValue(T t)
{
    _dataAvailable = true;
    _output = t;
}

template <typename T>
void Operation<T>::setType(opType otp)
{
    _otp = otp;
}

template <typename T>
void Operation<T>::compute()
{
    std::cout << "Operation compute ..." << std::endl;
    opType opt = getType();
    switch (opt)
    {
    case opType::addition:
        static_cast<add<T> *>(this)->compute();
        break;
    case opType::negative:
        static_cast<negative<T> *>(this)->compute();
        break;
    }
};

// --- add operation ---

template <typename T>
add<T>::add(BaseNode &a, BaseNode &b) : Operation<T>(opType::addition)
{
    this->addInputs(&a);
    this->addInputs(&b);

    a.addConsumers(this);
    b.addConsumers(this);
}

/*

template <typename T>
add<T>::add(Node<T> &&a, Node<T> &&b) : Operation<T>(opType::addition)
{

    a.addConsumers(this);
    b.addConsumers(this);

    this->addInputs(std::move(a));
    this->addInputs(std::move(b));
}
*/

template <typename T>
void add<T>::compute()
{
    Node<T> *pNode = static_cast<Node<T> *>(this);
    std::cout << "Compute add operation ..." << std::endl;
    std::vector<BaseNode *> inputs = pNode->getInputs();
    pNode->setValue(static_cast<Node<T> *>(inputs[0])->getValue() + static_cast<Node<T> *>(inputs[1])->getValue());
}

// --- negative operation---

template <typename T>
negative<T>::negative(BaseNode &a) : Operation<T>(opType::negative)
{
    this->addInputs(&a);

    a.addConsumers(this);
}

template <typename T>
void negative<T>::compute()
{
    Node<T> *pNode = static_cast<Node<T> *>(this);
    std::cout << "Compute negative operation ..." << std::endl;
    std::vector<BaseNode *> inputs = pNode->getInputs();
    pNode->setValue(-(static_cast<Node<T> *>(inputs[0])->getValue()));
}