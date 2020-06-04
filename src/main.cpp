#include "../include/NN.h"
#include <iostream>

int main()
{
    NN nn = NN();
    //Variable<int> A = NN.variable(1);
    //Variable<int> B = NN.variable(2);
    //negative<int> E(B);

    auto C = nn.add<int, int, int>(nn.variable<int>(1), nn.variable<int>(2));
    //auto D = add<int>(negative<int>(B), add<int>(A, B));

    std::unordered_map<std::string, int *> feed = {};
    int r = nn.run(C, feed);

    std::cout << r << std::endl;
}