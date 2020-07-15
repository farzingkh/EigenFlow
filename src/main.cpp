#include "../include/NN.h"
#include <iostream>

int main()
{

    typedef Eigen::Matrix<long double, Eigen::Dynamic, Eigen::Dynamic> matxxf;

    // Initialize
    NN nn = NN();
    int const STEPS = 10000;

    // number of training samples
    int m = 1;

    /* Use nn.spaceholders for constant variables, any nn.variables will be updated during optimization but placeholder values are kept constant */

    // matrix of scalar 1
    Eigen::Matrix<long double, 1, 1> One;
    One << 1;
    matxxf n = One;
    BaseNode *one = nn.placeholder<long double>("one");

    // Bias (m*1)
    Eigen::Matrix<long double, 1, 1> B;
    B << 0.1;
    BaseNode *b = nn.variable<long double>(std::move(B));

    // Wieghts (nh*nx)
    Eigen::Matrix<long double, 1, 2> W;
    W << 0.1, 0.2;
    BaseNode *w = nn.variable<long double>(std::move(W));

    // Training Data (nx*m)
    Eigen::Matrix<long double, 2, 1> X;
    X << 3, 2;
    matxxf x = X;

    // Labels (1*m)
    Eigen::Matrix<long double, 1, 1> Y;
    Y << 1;
    matxxf yy = Y;
    BaseNode *y = nn.placeholder<long double>("Y");

    // activation unit sigmoid(w^T*x+b) (nh*m)
    BaseNode *a = nn.sigmoid<long double>(nn.add<long double>(nn.matmultiply<long double>(w, nn.placeholder<long double>("X")), b));

    // intermidiate loss function
    // create loss function -(y*log(a)+(1-y)*log(1-a))
    BaseNode *L = nn.negative<long double>(nn.add<long double>(nn.matmultiply<long double>(y, nn.log<long double>(a)), nn.matmultiply<long double>(nn.add<long double>(one, nn.negative<long double>(y)), nn.log<long double>(nn.add<long double>(one, nn.negative<long double>(a))))));

    // Create gradient descent optimization
    auto opt = GradientDescentOptimizer(0.01).minimize<matxxf>(L);

    // Create a map to feed data to the placeholders (i.e. "X" = X)
    std::unordered_map<std::string, matxxf *>
        feed = {};
    feed["X"] = &x;
    feed["Y"] = &yy;
    feed["one"] = &n;

    // Run the session and get the value of the operation
    //matxxf g = nn.run<float>(G, feed);
    nn.run<long double>(L, feed);

    for (int i = 1; i < STEPS; i++)
    {
        nn.run<long double>(&opt, feed);
        nn.run<long double>(L, feed);
        if (i % 1000 == 0)
        {
            std::cout << "Step " << i << std::endl;
            std::cout << "Activation: " << *(a->getValue<matxxf>()) << std::endl;
            std::cout << "loss: " << *(L->getValue<matxxf>()) << std::endl;
            std::cout << "Weights: " << *(w->getValue<matxxf>()) << std::endl;
            std::cout << "Bias: " << *(b->getValue<matxxf>()) << std::endl;
        }
    }

    /* Check gradients -- Make sure to set learning rate to zero befor checking!! -- */
    nn.checkAllGradient(L, feed);
}
