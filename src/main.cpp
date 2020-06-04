#include "../include/NN.h"
#include <assert.h>
#include <iostream>

int main()
{
    // Example with 2x2 matrix calculations
    typedef Eigen::Matrix<int, 2, 2> mat;
    NN nn = NN();
    mat A;
    A << 1, 2, 1, 2;
    mat B;
    B << 1, 1, 1, 1;

    //Variable<int> A = NN.variable(1);
    //Variable<int> B = NN.variable(2);
    //negative<int> E(B);

    //auto C = nn.multiply<float,int,float>(nn.variable<int>(3),(nn.add<float,int,float>(nn.add<int,int,int>(nn.variable<int>(1),nn.variable<int>(2)), nn.negative<float>(nn.add<float,float,float>(nn.variable<float>(2.5),nn.variable<float>(1.5))))));
    //auto D = nn.multiply<mat, mat, mat>(nn.variable<mat>(std::move(B)), nn.negative<mat>(nn.variable<mat>(std::move(A))));

    // Define B*(-A) with B as a placeholder to feed later
    auto D = nn.multiply<mat, mat, mat>(nn.placeholder<mat>("B"), nn.negative<mat>(nn.variable<mat>(std::move(A))));

    // Create map to feed data to the placeholders
    std::unordered_map<std::string, mat *> feed = {};
    feed["B"] = &B;

    // Run the session and get the value of the operation
    mat r = nn.run<mat>(D, feed);

    std::cout << "Computational Operation Output:" << std::endl;
    std::cout << r << std::endl;
}
