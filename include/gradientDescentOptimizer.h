#ifndef GRADIENT_DESCENT
#define GRADIENT_DESCENT

#include <deque>
#include <unordered_map>

// forward declaration
template <typename T>
class Minimizer;

class GradientDescentOptimizer
{
public:
    GradientDescentOptimizer(float lr);
    void computeGradients(BaseNode *loss);

    template <typename T>
    Minimizer<T> minimize(BaseNode *loss);

private:
    float learningRate_;
    std::vector<BaseNode *> variableNodesList_;
};

#include "../src/gradientDescentOptimizer.tpp"

#endif /* GRADIENT_DESCENT */