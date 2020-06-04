#ifndef NN_H
#define NN_H

#include "graph.h"
#include "session.h"
#include "operation.h"

class NN
{
public:
    NN();

    template <typename T>
    Variable<T> *variable(T &&t);

    template <typename T>
    Placeholder<T> *placeholder();

    template <typename T,typename T1, typename T2>
    Add<T,T1,T2> *add(BaseNode *a, BaseNode *b);

    template <typename T>
    Negative<T> *negative(BaseNode *a);

    template <typename T>
    T run(BaseNode *n, std::unordered_map<std::string, T *> feed);

private:
    Graph _graph;
    Session _session;
};

#include "../src/NN.tpp"
#endif /* NN_H */