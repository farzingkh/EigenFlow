#include <iostream>
#include <memory>
#include <math.h>

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
    T1 A = inputs[0]->getValue<T1>();
    T2 B = inputs[1]->getValue<T2>();
    // broadcast column or row vectors
    if (A.rows() != B.rows() & A.cols() == B.cols())
    {
        if (B.rows() == 1)
        {

            pNode->setValue(A.rowwise() + B.row(0));
        }
        else if (A.rows() == 1)
        {
            pNode->setValue(B.rowwise() + A.row(0));
        }
    }
    else if (A.cols() != B.cols() & A.rows() == B.rows())
    {
        if (B.cols() == 1)
        {
            pNode->setValue(A.colwise() + B.col(0));
        }
        else if (A.cols() == 1)
        {
            pNode->setValue(B.colwise() + A.col(0));
        }
    }
    else
    {
        // element-wise addition without broadcasting
        pNode->setValue(A + B);
    }
}

template <typename T, typename T1, typename T2>
void Add<T, T1, T2>::gradient()
{
    std::cout << "Compute Add operation geradient ..." << std::endl;

    // get output gradient from consumer 
    T grad = ((BaseNode*)this)->getOutGradient<T>();
    
    // get inputs of this node 
    std::vector<BaseNode *> inputs = this->getInputs();
    T1 A = inputs[0]->getValue<T1>();
    T2 B = inputs[1]->getValue<T2>();

    // Check for broadcasting
    // If Gradient is larger than A, then A was broadcasted
    if (grad.cols() > A.cols() or grad.rows() > A.rows())
    {
        // Input gradient is matrix ones of size B multiplied by output gradient
        T g;
        g.setOnes(B.rows(), B.cols());
        grad *= g;
        this->setGrad(grad);
    }
    else
    {
        // Input gradient is the same as output gradient
        this->setGrad(grad);
    }
    // If Gradient is larger than B, then B was broadcasted
    if (grad.cols() > B.cols() or grad.rows() > B.rows())
    {
        // Input gradient is matrix ones of size A multiplied by output gradient
        T g;
        g.setOnes(A.rows(), A.cols());
        grad *= g;
        this->setGrad(grad);
    }
    else
    {
        // Input gradient is the same as output gradient
        this->setGrad(grad);
    }
}

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
void Negative<T>::gradient()
{
    Node<T> *pN = static_cast<Node<T> *>(this);
    std::cout << "Compute negative operation geradient ..." << std::endl;
    std::vector<BaseNode *> consumer = this->getConsumers();
    pN->setGrad(-(consumer[0]->getGradient<T>()[0]));
}

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
    // multiplication of scalar and matrix
    T1 A = inputs[0]->getValue<T1>();
    T2 B = inputs[1]->getValue<T2>();
    // if A is scalar
    if (A.cols() == 1 and A.rows() == 1)
    {
        // perform element-wise multiplication
        pNode->setValue(A(0) * B.array());
    } // if B is scalar
    else if (B.cols() == 1 and B.rows() == 1)
    {
        // perform element-wise multiplication
        pNode->setValue(B(0) * A.array());
    }
    else
    {
        // if neither A or B is scalar perform matrix multiplication
        pNode->setValue(inputs[0]->getValue<T1>() * inputs[1]->getValue<T2>());
    }
}

template <typename T, typename T1, typename T2>
void Multiply<T, T1, T2>::gradient() { return; }

// --- DotProduct ---

template <typename T, typename T1, typename T2>
Dot<T, T1, T2>::Dot(BaseNode *a, BaseNode *b)
{
    this->_opType = operationType::dotproduct;

    this->addInputs(a);
    this->addInputs(b);

    a->addConsumers(this);
    b->addConsumers(this);
}

template <typename T, typename T1, typename T2>
void Dot<T, T1, T2>::compute()
{
    Node<T> *pNode = static_cast<Node<T> *>(this);
    std::cout << "Compute dot product operation ..." << std::endl;
    std::vector<BaseNode *> inputs = this->getInputs();
    pNode->setValue(inputs[0]->getValue<T1>().dot(inputs[1]->getValue<T2>()));
}

template <typename T, typename T1, typename T2>
void Dot<T, T1, T2>::gradient() { return; }

// --- Sigmoid ---

template <typename T>
Sigmoid<T>::Sigmoid(BaseNode &a)
{
    this->_opType = operationType::sigmoid;
    this->addInputs(&a);
    a.addConsumers(this);
}

template <typename T>
Sigmoid<T>::Sigmoid(BaseNode *a)
{
    this->_opType = operationType::sigmoid;
    this->addInputs(a);
    a->addConsumers(this);
}

template <typename T>
void Sigmoid<T>::compute()
{
    Node<T> *pNode = static_cast<Node<T> *>(this);
    std::cout << "Compute sigmoid operation ..." << std::endl;
    std::vector<BaseNode *> inputs = this->getInputs();
    pNode->setValue(1 / (1 + exp(-(inputs[0]->getValue<T>().array()))));
}

template <typename T>
void Sigmoid<T>::gradient() { return; }