#ifndef GRADIENT_DESCENT
#define GRADIENT_DESCENT

#include <deque>

class GradientDescentOptimizer
{
public:
    GradientDescentOptimizer(int lr);
    void computeGradients(BaseNode *n);
    BaseNode *minimize(BaseNode *n);

private:
    int learningRate_;
    std::deque<BaseNode *> nodeQueue_;
};

#endif /* GRADIENT_DESCENT */