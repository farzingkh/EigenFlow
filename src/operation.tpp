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
    std::cout << "Compute add operation ..." << std::endl;
    std::vector<BaseNode *> inputs = this->getInputs();
    T1 A = inputs[0]->getValue<T1>();
    T2 B = inputs[1]->getValue<T2>();
    // broadcast column or row vectors
    if (A.rows() != B.rows() & A.cols() == B.cols())
    {
        if (B.rows() == 1)
        {

            this->setValue(A.rowwise() + B.row(0));
        }
        else if (A.rows() == 1)
        {
            this->setValue(B.rowwise() + A.row(0));
        }
    }
    else if (A.cols() != B.cols() & A.rows() == B.rows())
    {
        if (B.cols() == 1)
        {
            this->setValue(A.colwise() + B.col(0));
        }
        else if (A.cols() == 1)
        {
            this->setValue(B.colwise() + A.col(0));
        }
    }
    else
    {
        // element-wise addition without broadcasting
        this->setValue(A + B);
    }
}

template <typename T, typename T1, typename T2>
void Add<T, T1, T2>::gradient()
{
    std::cout << "Compute Add operation geradient ..." << std::endl;

    // get output gradient from consumer
    T grad = ((BaseNode *)this)->getOutGradient<T>();

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
    std::cout << "Compute negative operation ..." << std::endl;
    this->setValue(-(((BaseNode *)this)->getInputs()[0]->getValue<T>()));
}

template <typename T>
void Negative<T>::gradient()
{
    std::cout << "Compute negative operation geradient ..." << std::endl;
    this->setGrad(-(((BaseNode *)this)->getOutGradient<T>()));
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
    std::cout << "Compute multiplication operation..." << std::endl;
    std::vector<BaseNode *> inputs = this->getInputs();
    // multiplication of scalar and matrix
    T1 A = inputs[0]->getValue<T1>();
    T2 B = inputs[1]->getValue<T2>();
    // if neither A or B is scalar perform matrix multiplication
    this->setValue(A.array() * B.array());
}

template <typename T, typename T1, typename T2>
void Multiply<T, T1, T2>::gradient()
{
    std::cout << "Compute multiplication operation gradient..." << std::endl;
    // get output gradient from consumer
    T G = ((BaseNode *)this)->getOutGradient<T>();
    // get inputs of this node
    std::vector<BaseNode *> inputs = this->getInputs();
    T1 A = inputs[0]->getValue<T1>();
    T2 B = inputs[1]->getValue<T2>();
    // calculate and set gradient for first input "A"
    this->setGrad(G.array() * A.array());
    // calculate and set gradient for first input "B"
    this->setGrad(G.array() * B.array());
}

// --- MatMultiply Operation ---

template <typename T, typename T1, typename T2>
MatMultiply<T, T1, T2>::MatMultiply(BaseNode *a, BaseNode *b)
{
    this->_opType = operationType::multiply;

    this->addInputs(a);
    this->addInputs(b);

    a->addConsumers(this);
    b->addConsumers(this);
}

template <typename T, typename T1, typename T2>
void MatMultiply<T, T1, T2>::compute()
{
    std::cout << "Compute matrix multiplication operation..." << std::endl;
    std::vector<BaseNode *> inputs = this->getInputs();
    // multiplication of scalar and matrix
    T1 A = inputs[0]->getValue<T1>();
    T2 B = inputs[1]->getValue<T2>();
    // if neither A or B is scalar perform matrix multiplication
    this->setValue(A * B);
}

template <typename T, typename T1, typename T2>
void MatMultiply<T, T1, T2>::gradient()
{
    std::cout << "Compute matrix multiplication operation gradient..." << std::endl;
    // get output gradient from consumer
    T G = ((BaseNode *)this)->getOutGradient<T>();
    // get inputs of this node
    std::vector<BaseNode *> inputs = this->getInputs();
    T1 A = inputs[0]->getValue<T1>();
    T2 B = inputs[1]->getValue<T2>();
    // calculate and set gradient for first input "A"
    this->setGrad(G * B.transpose());
    // calculate and set gradient for first input "B"
    this->setGrad(A.transpose() * G);
}

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
    std::cout << "Compute dot product operation ..." << std::endl;
    this->setValue(((BaseNode *)this)->getInputs()[0]->getValue<T1>().dot(((BaseNode *)this)->getInputs()[1]->getValue<T2>()));
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
    std::cout << "Compute sigmoid operation ..." << std::endl;
    this->setValue(1 / (1 + exp(-(((BaseNode *)this)->getInputs()[0]->getValue<T>().array()))));
}

template <typename T>
void Sigmoid<T>::gradient() { return; }