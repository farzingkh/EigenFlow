#ifndef GRADIENT_DESCENT
#define GRADIENT_DESCENT

#include <deque>

// forward declaration
template <typename T>
class Minimizer;

class GradientDescentOptimizer
{
public:
    GradientDescentOptimizer(int lr);
    void computeGradients(BaseNode *loss);

    template <typename T>
    Minimizer<T> minimize(BaseNode *loss);

private:
    int learningRate_;
    std::vector<BaseNode *> variableNodesList_;
};

#endif /* GRADIENT_DESCENT */