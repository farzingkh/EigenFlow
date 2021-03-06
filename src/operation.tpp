#include <iostream>
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
    // use Locking_ptr<BaseNode> to cast to BaseNode
    Locking_ptr<BaseNode> rhsptr(rhs);
    Locking_ptr<BaseNode> ptrthis(this);
    // add inpupts
    ptrthis->addInputs(rhs);
    // add consumers
    rhsptr->addConsumers(this);
}

// --- BinaryOperation ---

template <typename T>
BinaryOperation<T>::BinaryOperation(BaseNode *lhs, BaseNode *rhs)
{
    // use Locking_ptr<BaseNode> to cast to BaseNode
    Locking_ptr<BaseNode> rhsptr(rhs);
    Locking_ptr<BaseNode> lhsptr(lhs);
    Locking_ptr<BaseNode> ptrthis(this);
    // add inputs
    ptrthis->addInputs(lhs);
    ptrthis->addInputs(rhs);
    // add consumers
    lhsptr->addConsumers(this);
    rhsptr->addConsumers(this);
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
    //std::cout << "Compute add operation ..." << std::endl;
    std::vector<Locking_ptr<BaseNode>> inputs = this->getInputs();
    // get value of input nodes
    Locking_shared_ptr<T1> A = inputs[0]->getValue<T1>();
    Locking_shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // broadcast column or row vectors if both inputs have same number of columns
    if (A->rows() != B->rows() & A->cols() == B->cols())
    {
        if (B->rows() == 1)
        {
            // add B to each row of A
            this->setValue(A->rowwise() + B->row(0));
        }
        else if (A->rows() == 1)
        {
            // Add A to each row of B
            this->setValue(B->rowwise() + A->row(0));
        }
    }
    // broadcast column or row vectors if both inputs have same number of rows
    else if (A->cols() != B->cols() & A->rows() == B->rows())
    {
        if (B->cols() == 1)
        {
            // add B to each column of A
            this->setValue(A->colwise() + B->col(0));
        }
        else if (A->cols() == 1)
        {
            // add A to each colun of B
            this->setValue(B->colwise() + A->col(0));
        }
    }
    // if A is scalar
    else if (A->cols() == 1 & A->rows() == 1)
    {
        this->setValue((*A)(0) + B->array());
    }
    // if B is scalar
    else if (B->cols() == 1 & B->rows() == 1)
    {
        this->setValue((*B)(0) + A->array());
    }
    else
    {
        // they are same size so element-wise addition without broadcasting
        this->setValue(A->array() + B->array());
    }
}

template <typename T, typename T1, typename T2>
void Add<T, T1, T2>::gradient()
{
    //std::cout << "Compute Add operation geradient ..." << std::endl;
    T grad = this->getGradient();
    std::vector<Locking_ptr<BaseNode>> inputs = this->getInputs();
    Locking_shared_ptr<T1> A = inputs[0]->getValue<T1>();
    Locking_shared_ptr<T2> B = inputs[1]->getValue<T2>();
    /* Check for broadcasting. If Gradient is larger than A, then A was broadcasted.
     Broadcasted variable is as though it has that many consumers. So the gradient is 
     the total gradient  (the sum of gradients in the broadcasted direction) */
    if (grad.cols() > A->cols() or grad.rows() > A->rows())
    {
        T g;
        g.setOnes(A->rows(), A->cols());
        if (A->rows() == 1 & A->cols() == 1)
        {
            // if A is scalar
            auto a = grad.sum();
            T gr(1, 1);
            gr << a;
            inputs[0]->setGrad<T>(gr);
        }
        else if (A->rows() == 1)
        {
            // broadcasted in columns direction
            T gr = g * grad;
            inputs[0]->setGrad<T>(gr);
        }
        else if (A->cols() == 1)
        {
            // broadcasted in rows direction
            T gr = grad * g;
            inputs[0]->setGrad<T>(gr);
        }
    }
    else
    {
        // No broadcasting; Input gradient is the same as output gradient
        inputs[0]->setGrad<T>(grad);
    }
    // If Gradient is larger than B, then B was broadcasted
    if (grad.cols() > B->cols() or grad.rows() > B->rows())
    {
        T g;
        g.setOnes(B->rows(), B->cols());
        if (B->rows() == 1 & B->cols() == 1)
        {
            // if B is scalar
            auto a = grad.sum();
            T gr(1, 1);
            gr << a;
            inputs[1]->setGrad<T>(gr);
        }
        else if (B->rows() == 1)
        {
            // broadcasted in columns direction
            T gr = g * grad;
            inputs[1]->setGrad<T>(gr);
        }
        else if (B->cols() == 1)
        {
            // broadcasted in rows direction
            T gr = grad * g;
            inputs[1]->setGrad<T>(gr);
        }
    }
    else
    {
        // No broadcasting; Input gradient is the same as output gradient
        inputs[1]->setGrad<T>(grad);
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
    //std::cout << "Compute negative operation ..." << std::endl;
    Locking_ptr<BaseNode> ptrthis(this);
    this->setValue(-(*(ptrthis->getInputs()[0]->getValue<T>())));
}

template <typename T>
void Negative<T>::gradient()
{
    // get inputs of this node; it only has one input
    std::vector<Locking_ptr<BaseNode>> inputs = this->getInputs();
    //std::cout << "Compute negative operation geradient ..." << std::endl;
    inputs[0]->setGrad<T>(-(this->getGradient()));
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
    //std::cout << "Compute multiplication operation..." << std::endl;
    std::vector<Locking_ptr<BaseNode>> inputs = this->getInputs();
    // multiplication of scalar and matrix
    Locking_shared_ptr<T1> A = inputs[0]->getValue<T1>();
    Locking_shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // perform matrix multiplication
    this->setValue(A->array() * B->array());
}

template <typename T, typename T1, typename T2>
void Multiply<T, T1, T2>::gradient()
{
    //std::cout << "Compute multiplication operation gradient..." << std::endl;
    // get output gradient from consumer
    T G = this->getGradient();
    // get inputs of this node
    std::vector<Locking_ptr<BaseNode>> inputs = this->getInputs();
    Locking_shared_ptr<T1> A = inputs[0]->getValue<T1>();
    Locking_shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // calculate and set gradient for first input "A"
    inputs[0]->setGrad<T>(G.array() * B->array());
    // calculate and set gradient for first input "B"
    inputs[1]->setGrad<T>(G.array() * A->array());
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
    //std::cout << "Compute matrix multiplication operation..." << std::endl;
    std::vector<Locking_ptr<BaseNode>> inputs = this->getInputs();
    // get input node values
    Locking_shared_ptr<T1> A = inputs[0]->getValue<T1>();
    Locking_shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // perform matrix multiplication
    this->setValue((*A) * (*B));
}

template <typename T, typename T1, typename T2>
void MatMultiply<T, T1, T2>::gradient()
{
    //std::cout << "Compute matrix multiplication operation gradient..." << std::endl;
    // get output gradient from consumer
    T G = this->getGradient();
    // get inputs of this node
    std::vector<Locking_ptr<BaseNode>> inputs = this->getInputs();
    Locking_shared_ptr<T1> A = inputs[0]->getValue<T1>();
    Locking_shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // calculate and set gradient for first input "A"
    T C = G * B->transpose();
    inputs[0]->setGrad<T>(C);
    // calculate and set gradient for second input "B"
    T D = A->transpose() * G;
    inputs[1]->setGrad<T>(D);
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
    //std::cout << "Compute dot product operation ..." << std::endl;
    Locking_ptr<BaseNode> ptrthis(this);
    this->setValue(ptrthis->getInputs()[0]->getValue<T1>().dot(ptrthis->getInputs()[1]->getValue<T2>()));
}

template <typename T, typename T1, typename T2>
void Dot<T, T1, T2>::gradient()
{
    //std::cout << "Compute dot product operation gradient..." << std::endl;
    // get output gradient from consumer
    T G = this->getGradient();
    // get inputs of this node
    std::vector<Locking_ptr<BaseNode>> inputs = this->getInputs();
    Locking_shared_ptr<T1> A = inputs[0]->getValue<T1>();
    Locking_shared_ptr<T2> B = inputs[1]->getValue<T2>();
    // calculate and set gradient for first input "A"
    T C = G * B->transpose();
    inputs[0]->setGrad<T>(C);
    // calculate and set gradient for first input "B"
    T D = A->transpose() * G;
    inputs[1]->setGrad<T>(D);
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
    Locking_ptr<BaseNode> ptrthis(this);
    //std::cout << "Compute sigmoid operation ..." << std::endl;
    this->setValue(1 / (1 + exp(-(ptrthis->getInputs()[0]->getValue<T>()->array()))));
}

template <typename T>
void Sigmoid<T>::gradient()
{
    //std::cout << "Compute sigmoid gradient..." << std::endl;
    // Cast this to BaseNode and wrap around with a locking class
    Locking_ptr<BaseNode> ptrthis(this);
    // get inputs of this node
    std::vector<Locking_ptr<BaseNode>> inputs = this->getInputs();
    // get output gradient from consumer
    T G = this->getGradient();
    // get sigmoid value
    Locking_shared_ptr<T> sig = ptrthis->getValue<T>();
    // compute gradient
    T grad = G.array() * sig->array() * (1 - sig->array());
    // set gradient for input
    inputs[0]->setGrad<T>(grad);
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
    // Cast to this to BaseNode and wrape around with a lock
    Locking_ptr<BaseNode> ptrthis(this);
    //std::cout << "Compute log operation ..." << std::endl;
    this->setValue(log(ptrthis->getInputs()[0]->getValue<T>()->array()));
}

template <typename T>
void Log<T>::gradient()
{
    //std::cout << "Compute log gradient..." << std::endl;
    // get output gradient from consumer
    T G = this->getGradient();
    // copy inputs of this node to local variable to avoid data
    std::vector<Locking_ptr<BaseNode>> inputs = this->getInputs();
    // get log input value
    Locking_shared_ptr<T> log = inputs[0]->getValue<T>();
    // compute gradient; elementwise division
    T grad = G.array() / log->array();
    // set gradient for input
    inputs[0]->setGrad<T>(grad);
}

// --- Sum ---

template <typename T>
Sum<T>::Sum(BaseNode &a, int axis) : UnaryOperation<T>(&a), _axis(axis)
{
    this->_opType = operationType::sum;
}

template <typename T>
Sum<T>::Sum(BaseNode *a, int axis) : UnaryOperation<T>(a), _axis(axis)
{
    this->_opType = operationType::sum;
}

template <typename T>
void Sum<T>::compute()
{
    Locking_ptr<BaseNode> ptrthis(this);
    //std::cout << "Compute Sum operation ..." << std::endl;
    if (_axis == 0)
    {
        // if axis = 0 then sum colwise
        this->setValue(ptrthis->getInputs()[0]->getValue<T>().colwise().sum());
    }
    else if (_axis == 1)
    {
        // if axis = 1 then sum rowwise
        this->setValue(ptrthis->getInputs()[0]->getValue<T>().rowwise().sum());
    }
}

template <typename T>
void Sum<T>::gradient()
{
    //std::cout << "Compute sum operation gradient..." << std::endl;
    // get output gradient from consumer
    T G = this->getGradient();
    T g;
    // get inputs of this node
    std::vector<Locking_ptr<BaseNode>> inputs = this->getInputs();
    Locking_shared_ptr<T> A = inputs[0]->getValue<T>();
    if (G.rows() == 1)
    {
        g = G.replicate(A->rows(), 1);
        inputs[0]->setGrad<T>(g);
    }
    else if (G.cols() == 1)
    {
        g = G.replicate(1, A->cols());
        inputs[0]->setGrad<T>(g);
    }
    else
    {
        inputs[0]->setGrad<T>(G);
    }
}

/// --- Minimizaer Operation ----

template <typename T>
Minimizer<T>::Minimizer(GradientDescentOptimizer *grd, BaseNode *loss)
{
    grdOpt_ = Locking_ptr<GradientDescentOptimizer>(grd, &gMtx_);
    loss_ = Locking_ptr<BaseNode>(loss);
    learningRate_ = grd->learningRate_;
}

template <typename T>
Minimizer<T>::Minimizer(Minimizer<T> &&other)
{
    //std::cout << " Minimizer move contructor..." << std::endl;
    // move members
    grdOpt_ = std::move(other.grdOpt_);
    loss_ = std::move(other.loss_);
    learningRate_ = std::move(other.learningRate_);
}

template <typename T>
Minimizer<T> &Minimizer<T>::operator=(Minimizer<T> &&other)
{
    //std::cout << " Minimizer move assignment contructor..." << std::endl;
    if (this != &other)
    {
        // move members
        grdOpt_ = std::move(other.grdOpt_);
        loss_ = std::move(other.loss_);
        learningRate_ = std::move(other.learningRate_);
    }
    return *this;
}

// Compute updates the variable gradients based on learning rate
template <typename T>
void Minimizer<T>::compute()
{
    //std::cout << "Compute Minimization operation ..." << std::endl;
    // compute grdients 
    grdOpt_->computeGradients(loss_.get());
    // iterate through nodes and update variable values
    auto list = grdOpt_->getNodeQueue(loss_.get());
    for (auto &n : list)
    {
        if (n->getNodeType() == nodeType::variable)
        {
            auto v = static_cast<Variable<T> *>(n.get());
            v->updateValue(learningRate_);
        }
    }
}

template <typename T>
void Minimizer<T>::gradient() { return; }