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
    // don't pass inputs by ref to avoid data race
    std::vector<Locking_ptr<BaseNode>> getNodesList();

    template <typename T>
    Minimizer<T> minimize(BaseNode *loss);

    float learningRate_;
    std::shared_ptr<std::mutex> Mtx_ = std::shared_ptr<std::mutex>(new std::mutex());

private:
    void getNodeQueue(BaseNode *loss);

    std::vector<Locking_ptr<BaseNode>> NodesList_;
    std::deque<Locking_ptr<BaseNode>> nodeQueue_;
};

#include "../src/gradientDescentOptimizer.tpp"

#endif /* GRADIENT_DESCENT */