#include "../include/session.h"
#include "../include/node.h"
#include "../include/operation.h"
#include <iostream>

int main()
{
    Variable<int> A(1);
    Variable<int> B(2);
    //negative<int> E(B);

    auto C = add<int>(A, B);
    //auto D = add<int>(negative<int>(B), add<int>(A, B));

    std::unordered_map<std::string, int *> feed = {};

    Session<int> s;
    int r = s.Run(C, feed);

    std::cout << r << std::endl;
}