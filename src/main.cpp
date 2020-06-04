#include "../include/NN.h"
#include <iostream>

int main()
{
    NN nn = NN();
    //Variable<int> A = NN.variable(1);
    //Variable<int> B = NN.variable(2);
    //negative<int> E(B);

    auto C = nn.multiply<float,int,float>(nn.variable<int>(3),(nn.add<float,int,float>(nn.add<int,int,int>(nn.variable<int>(1),nn.variable<int>(2)), nn.negative<float>(nn.add<float,float,float>(nn.variable<float>(2.5),nn.variable<float>(1.5))))));
    //auto D = add<int>(negative<int>(B), add<int>(A, B));

    std::unordered_map<std::string, float *> feed = {};
    float r = nn.run<float>(C, feed);

    std::cout << r << std::endl;
}

