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

    // compute gradients
    void computeGradients(BaseNode *loss);

    // get node list doing level order traversal
    std::vector<Locking_ptr<BaseNode>> getNodeQueue(BaseNode *loss);

    template <typename T>
    Minimizer<T> minimize(BaseNode *loss);

    float learningRate_;
};

#include "../src/gradientDescentOptimizer.tpp"

#endif /* GRADIENT_DESCENT */