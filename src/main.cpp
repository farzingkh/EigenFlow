#include "../include/NN.h"
#include <assert.h>
#include <iostream>
int main()
{
    // Example with 2x2 matrix calculations

    typedef Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> matxxf;

    NN nn = NN();

    Eigen::Matrix<float, 1, 1> b;
    b << 0;

    Eigen::Matrix<float, 1, 2> W;
    W << 1, 1;

    Eigen::Matrix<float, 1, 2> X;
    X << 3, 2;

    matxxf x = X.transpose();

    // sigmoid(w^T*x+b)
    auto G = nn.sigmoid<float>(nn.add<float>(nn.multiply<float>(nn.variable<float>(std::move(W)), nn.placeholder<float>("X")), nn.variable<float>(std::move(b))));

    // Create map to feed data to the placeholders (i.e. X = X)
    std::unordered_map<std::string, matxxf *> feed = {};
    feed["X"] = &x;

    // Run the session and get the value of the operation
    matxxf r = nn.run<float>(G, feed);

    std::cout << "Computational Operation Output:" << std::endl;
    std::cout << r << std::endl;
}
