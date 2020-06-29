#include <iostream>
#include <memory>
#include <math.h>

// --- Operation ---
template <typename T>
Operation<T>::Operation()
{
    this->_nType = nodeType::operation;
}

// --- UnaryOperation ---
template <typename T>
UnaryOperation<T>::UnaryOperation(BaseNode *rhs)
{
    this->addInputs(rhs);
    rhs->addConsumers(this);
}

// --- BinaryOperation ---
template <typename T>
BinaryOperation<T>::BinaryOperation(BaseNode *lhs, BaseNode *rhs)
{
    this->addInputs(lhs);
    this->addInputs(rhs);
    lhs->addConsumers(this);
    rhs->addConsumers(this);
}

// --- add operation ---

template <typename T, typename T1, typename T2>
Add<T, T1, T2>::Add(BaseNode &a, BaseNode &b) : BinaryOperation<T>(&a, &b)
{
    this->_opType = operationType::addition;
}

template <typename T, typename T1, typename T2>
Add<T, T1, T2>::Add(BaseNode *a, BaseNode *b) : BinaryOperation<T>(a, b)
{
    this->_opType = operationType::addition;
}

template <typename T, typename T1, typename T2>
void Add<T, T1, T2>::compute()
{
    std::cout << "Compute add operation ..." << std::endl;
    std::vector<BaseNode *> &inputs = this->getInputs();
    std::shared_ptr<T1> A = inputs[0]->getValue<T1>();
    std::shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // broadcast column or row vectors
    if (A->rows() != B->rows() & A->cols() == B->cols())
    {
        if (B->rows() == 1)
        {

            this->setValue(A->rowwise() + B->row(0));
        }
        else if (A->rows() == 1)
        {
            this->setValue(B->rowwise() + A->row(0));
        }
    }
    else if (A->cols() != B->cols() & A->rows() == B->rows())
    {
        if (B->cols() == 1)
        {
            this->setValue(A->colwise() + B->col(0));
        }
        else if (A->cols() == 1)
        {
            this->setValue(B->colwise() + A->col(0));
        }
    }
    else
    {
        // element-wise addition without broadcasting
        this->setValue((*A) + (*B));
    }
}

template <typename T, typename T1, typename T2>
void Add<T, T1, T2>::gradient()
{
    std::cout << "Compute Add operation geradient ..." << std::endl;

    // get output gradient from consumer
    T grad = ((BaseNode *)this)->getOutGradient<T>();

    // get inputs of this node
    std::vector<BaseNode *> &inputs = this->getInputs();
    std::shared_ptr<T1> A = inputs[0]->getValue<T1>();
    std::shared_ptr<T2> B = inputs[1]->getValue<T2>();

    // Check for broadcasting
    // If Gradient is larger than A, then A was broadcasted
    if (grad.cols() > A->cols() or grad.rows() > A->rows())
    {
        // Input gradient is matrix ones of size B multiplied by output gradient
        T g;
        g.setOnes(B->rows(), B->cols());
        grad *= g;
        this->setGrad(grad);
    }
    else
    {
        // Input gradient is the same as output gradient
        this->setGrad(grad);
    }
    // If Gradient is larger than B, then B was broadcasted
    if (grad.cols() > B->cols() or grad.rows() > B->rows())
    {
        // Input gradient is matrix ones of size A multiplied by output gradient
        T g;
        g.setOnes(A->rows(), A->cols());
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
Negative<T>::Negative(BaseNode &a) : UnaryOperation<T>(&a)
{
    this->_opType = operationType::negative;
}

template <typename T>
Negative<T>::Negative(BaseNode *a) : UnaryOperation<T>(a)
{
    this->_opType = operationType::negative;
}

template <typename T>
void Negative<T>::compute()
{
    std::cout << "Compute negative operation ..." << std::endl;
    T v = -(*(((BaseNode *)this)->getInputs()[0]->getValue<T>()));
    this->setValue(std::move(v));
}

template <typename T>
void Negative<T>::gradient()
{
    std::cout << "Compute negative operation geradient ..." << std::endl;
    this->setGrad(-(((BaseNode *)this)->getOutGradient<T>()));
}

// --- Multiply Operation ---

template <typename T, typename T1, typename T2>
Multiply<T, T1, T2>::Multiply(BaseNode &a, BaseNode &b) : BinaryOperation<T>(&a, &b)
{
    this->_opType = operationType::multiply;
}

template <typename T, typename T1, typename T2>
Multiply<T, T1, T2>::Multiply(BaseNode *a, BaseNode *b) : BinaryOperation<T>(a, b)
{
    this->_opType = operationType::multiply;
}

template <typename T, typename T1, typename T2>
void Multiply<T, T1, T2>::compute()
{
    std::cout << "Compute multiplication operation..." << std::endl;
    std::vector<BaseNode *> &inputs = this->getInputs();
    // multiplication of scalar and matrix
    std::shared_ptr<T1> A = inputs[0]->getValue<T1>();
    std::shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // perform matrix multiplication
    this->setValue(A->array() * B->array());
}

template <typename T, typename T1, typename T2>
void Multiply<T, T1, T2>::gradient()
{
    std::cout << "Compute multiplication operation gradient..." << std::endl;
    // get output gradient from consumer
    T G = ((BaseNode *)this)->getOutGradient<T>();
    // get inputs of this node
    std::vector<BaseNode *> &inputs = this->getInputs();
    std::shared_ptr<T1> A = inputs[0]->getValue<T1>();
    std::shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // calculate and set gradient for first input "A"
    this->setGrad(G.array() * A->array());
    // calculate and set gradient for first input "B"
    this->setGrad(G.array() * B->array());
}

// --- MatMultiply Operation ---

template <typename T, typename T1, typename T2>
MatMultiply<T, T1, T2>::MatMultiply(BaseNode &a, BaseNode &b) : BinaryOperation<T>(&a, &b)
{
    this->_opType = operationType::multiply;
}

template <typename T, typename T1, typename T2>
MatMultiply<T, T1, T2>::MatMultiply(BaseNode *a, BaseNode *b) : BinaryOperation<T>(a, b)
{
    this->_opType = operationType::multiply;
}

template <typename T, typename T1, typename T2>
void MatMultiply<T, T1, T2>::compute()
{
    std::cout << "Compute matrix multiplication operation..." << std::endl;
    std::vector<BaseNode *> &inputs = this->getInputs();
    // multiplication of scalar and matrix
    std::shared_ptr<T1> A = inputs[0]->getValue<T1>();
    std::shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // perform matrix multiplication
    this->setValue((*A) * (*B));
}

template <typename T, typename T1, typename T2>
void MatMultiply<T, T1, T2>::gradient()
{
    std::cout << "Compute matrix multiplication operation gradient..." << std::endl;
    // get output gradient from consumer
    T G = ((BaseNode *)this)->getOutGradient<T>();
    // get inputs of this node
    std::vector<BaseNode *> &inputs = this->getInputs();
    std::shared_ptr<T1> A = inputs[0]->getValue<T1>();
    std::shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // calculate and set gradient for first input "A"
    T C = G * B->transpose();
    this->setGrad(C);
    // calculate and set gradient for second input "B"
    T D = A->transpose() * G;
    this->setGrad(D);
}

// --- DotProduct ---

template <typename T, typename T1, typename T2>
Dot<T, T1, T2>::Dot(BaseNode &a, BaseNode &b) : BinaryOperation<T>(&a, &b)
{
    this->_opType = operationType::dotproduct;
}

template <typename T, typename T1, typename T2>
Dot<T, T1, T2>::Dot(BaseNode *a, BaseNode *b) : BinaryOperation<T>(a, b)
{
    this->_opType = operationType::dotproduct;
}

template <typename T, typename T1, typename T2>
void Dot<T, T1, T2>::compute()
{
    std::cout << "Compute dot product operation ..." << std::endl;
    this->setValue(((BaseNode *)this)->getInputs()[0]->getValue<T1>().dot(((BaseNode *)this)->getInputs()[1]->getValue<T2>()));
}

template <typename T, typename T1, typename T2>
void Dot<T, T1, T2>::gradient()
{
    std::cout << "Compute dot product operation gradient..." << std::endl;
    // get output gradient from consumer
    T G = ((BaseNode *)this)->getOutGradient<T>();
    // get inputs of this node
    std::vector<BaseNode *> &inputs = this->getInputs();
    std::shared_ptr<T1> A = inputs[0]->getValue<T1>();
    std::shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // calculate and set gradient for first input "A"
    T C = G * B->transpose();
    this->setGrad(C);
    // calculate and set gradient for first input "B"
    T D = A->transpose() * G;
    this->setGrad(D);
}

// --- Sigmoid ---

template <typename T>
Sigmoid<T>::Sigmoid(BaseNode &a) : UnaryOperation<T>(&a)
{
    this->_opType = operationType::sigmoid;
}

template <typename T>
Sigmoid<T>::Sigmoid(BaseNode *a) : UnaryOperation<T>(a)
{
    this->_opType = operationType::sigmoid;
}

template <typename T>
void Sigmoid<T>::compute()
{
    std::cout << "Compute sigmoid operation ..." << std::endl;
    this->setValue(1 / (1 + exp(-(((BaseNode *)this)->getInputs()[0]->getValue<T>()->array()))));
}

template <typename T>
void Sigmoid<T>::gradient()
{
    std::cout << "Compute sigmoid gradient..." << std::endl;
    // get output gradient from consumer
    T G = ((BaseNode *)this)->getOutGradient<T>();
    // get sigmoid value
    std::shared_ptr<T> sig = ((BaseNode *)this)->getValue<T>();
    // compute gradient
    T grad = G.array() * sig->array() * (1 - sig->array());
    this->setGrad(grad);
}

// --- log ---

template <typename T>
Log<T>::Log(BaseNode &a) : UnaryOperation<T>(&a)
{
    this->_opType = operationType::log;
}

template <typename T>
Log<T>::Log(BaseNode *a) : UnaryOperation<T>(a)
{
    this->_opType = operationType::log;
}

template <typename T>
void Log<T>::compute()
{
    std::cout << "Compute log operation ..." << std::endl;
    this->setValue(log(((BaseNode *)this)->getInputs()[0]->getValue<T>()->array()));
}

template <typename T>
void Log<T>::gradient()
{
    std::cout << "Compute log gradient..." << std::endl;
    // get output gradient from consumer
    T G = ((BaseNode *)this)->getOutGradient<T>();
    // get log value
    std::shared_ptr<T> log = ((BaseNode *)this)->getValue<T>();
    // compute gradient; elementwise division
    T grad = G.array() / log->array();
    this->setGrad(grad);
}

// --- Sum ---

template <typename T>
Sum<T>::Sum(BaseNode &a, int axis) : UnaryOperation<T>(&a), _axis(axis)
{
    this->_opType = operationType::sum;
}

template <typename T>
Sum<T>::Sum(BaseNode *a, int axis) : BinaryOperation<T>(a), _axis(axis)
{
    this->_opType = operationType::sum;
}

template <typename T>
void Sum<T>::compute()
{
    std::cout << "Compute Sum operation ..." << std::endl;
    if (_axis == 0)
    {
        // if axis = 0 then sum colwise
        this->setValue(((BaseNode *)this)->getInputs()[0]->getValue<T>()->colwise().sum());
    }
    else if (_axis == 1)
    {
        // if axis = 1 then sum rowwise
        this->setValue(((BaseNode *)this)->getInputs()[0]->getValue<T>()->rowwise().sum());
    }
}

template <typename T>
void Sum<T>::gradient()
{
    std::cout << "Compute sum operation gradient..." << std::endl;
    // get output gradient from consumer
    T G = ((BaseNode *)this)->getOutGradient<T>();
    // get inputs of this node
    std::vector<BaseNode *> &inputs = this->getInputs();
    std::shared_ptr<T> A = inputs[0]->getValue<T>();
    // Input gradient is matrix ones of size A multiplied by output gradient
    T g;
    g.setOnes(A->rows(), A->cols());
    g *= G;
    this->setGrad(g);
}

/// --- Minimizaer Operation ----

template <typename T>
Minimizer<T>::Minimizer(GradientDescentOptimizer *grd, BaseNode *loss) : grdOpt_(grd), loss_(loss) {}

template <typename T>
Minimizer<T>::Minimizer(Minimizer<T> &&other)
{
    std::cout << " Minimizer move contructor..." << std::endl;
    grdOpt_ = other.grdOpt_;
    loss_ = other.loss_;
    other.grdOpt_ = nullptr;
    other.loss_ = nullptr;
}

template <typename T>
Minimizer<T> &Minimizer<T>::operator=(Minimizer<T> &&other)
{
    std::cout << " Minimizer move assignment contructor..." << std::endl;
    if (this != &other)
    {
        grdOpt_ = other.grdOpt_;
        loss_ = other.loss_;
        other.grdOpt_ = nullptr;
        other.loss_ = nullptr;
    }
    return *this;
}

// Compute updates the variable gradients based on learning rate
template <typename T>
void Minimizer<T>::compute()
{
    grdOpt_->computeGradients(loss_);

    for (auto n : grdOpt_->variableNodesList_)
    {
        if (n->getNodeType() == nodeType::variable)
        {
            static_cast<Variable<T> *>(n)->updateGradient(grdOpt_->learningRate_);
        }
    }
}

template <typename T>
void Minimizer<T>::gradient() { return; }