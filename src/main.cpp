#include "../include/NN.h"
#include <iostream>

int main()
{

    typedef Eigen::Matrix<long double, Eigen::Dynamic, Eigen::Dynamic> matxxf;

    NN nn = NN();
    int const STEPS = 2;

    // number of training samples
    int m = 1;

    // matrix of scalar 1
    Eigen::Matrix<long double, 1, 1> one;
    one << 1;

    // Bias (1*m)
    Eigen::Matrix<long double, 1, 1> b;
    b << 0;

    // Wieghts (nh*nx)
    Eigen::Matrix<long double, 1, 2> W;
    W << 1, 1;

    // Training Data (nx*m)
    Eigen::Matrix<long double, 2, 1> X;
    X << 3, 2;
    matxxf x = X;

    // Labels (nh*m)
    Eigen::Matrix<long double, 1, 1> Y;
    Y << 1;

    // activation unit sigmoid(w^T*x+b) (nh*m)
    BaseNode *G = nn.sigmoid<long double>(nn.add<long double>(nn.matmultiply<long double>(nn.variable<long double>(std::move(W)), nn.placeholder<long double>("X")), nn.variable<long double>(std::move(b))));

    // create loss function -(y*log(a)+(1-y)*log(1-y))
    BaseNode* L = nn.negative<long double>(nn.multiply<long double>(nn.variable<long double>(Y), nn.log<long double>(G)));

    // intermidiate loss function
    //BaseNode *L = nn.negative<float>(nn.add<float>(nn.multiply<float>(nn.variable<float>(Y), nn.log<float>(G)), nn.multiply<float>(nn.add<float>(nn.variable<float>(one), nn.negative<float>(nn.variable<float>(Y))), nn.log<float>(nn.add<float>(nn.variable<float>(one), nn.negative<float>(G))))));

    // Create gradient descent optimization
    auto opt = GradientDescentOptimizer(0.01).minimize<matxxf>(L);

    // Create a map to feed data to the placeholders (i.e. X = X)
    std::unordered_map<std::string, matxxf *>
        feed = {};
    feed["X"] = &x;

    // Run the session and get the value of the operation
    //matxxf g = nn.run<float>(G, feed);
    nn.run<long double>(L, feed);

    for (int i = 1; i < STEPS; i++)
    {
        nn.run<long double>(&opt, feed);
        nn.run<long double>(L, feed);
        //std::cout << "Computational Operation Output for loss:" << std::endl;
        //std::cout << *(L->getValue<matxxf>()) << std::endl;
    }

    // create minimization operation
    nn.checkAllGradient(L,feed);
}
