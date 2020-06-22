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

#endif /* GRADIENT_DESCENT */