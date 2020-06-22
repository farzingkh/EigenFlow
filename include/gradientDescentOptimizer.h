#ifndef GRADIENT_DESCENT
#define GRADIENT_DESCENT

#include <deque>

class GradientDescentOptimizer
{
public:
    GradientDescentOptimizer(int lr);
    void computeGradients(BaseNode *loss);
    BaseNode *minimize(BaseNode *loss);

private:
    int learningRate_;
};

template <typename T>
class Minimizer : public Operation<T>
{
public:
    Minimizer(std::vector<BaseNode *>& nodeList);
    void compute();

private:
    std::vector<BaseNode *> nodesList_ = {};

};

#endif /* GRADIENT_DESCENT */