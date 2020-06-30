#ifndef GRADIENT_DESCENT
#define GRADIENT_DESCENT

#include <deque>
#include <unordered_map>
#include <future>

// forward declaration
template <typename T>
class Minimizer;

class GradientDescentOptimizer
{
public:
    GradientDescentOptimizer(float lr);
    void computeGradients(BaseNode *loss);
    void getNodeQueue(BaseNode *loss);

    template <typename T>
    Minimizer<T> minimize(BaseNode *loss);

    float learningRate_;
    std::vector<BaseNode *> variableNodesList_;

private:
    std::deque<BaseNode *> nodeQueue_;
};

#include "../src/gradientDescentOptimizer.tpp"

#endif /* GRADIENT_DESCENT */