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
    Variable<T> variable(T &&t);

    template <typename T>
    Placeholder<T> placeholder();

    template <typename Tout, typename Tin1, typename Tin2>
    Add<Tout> add(Variable<Tin1> &&a, Variable<Tin2> &&b);

    template <typename T> 
    T run(BaseNode &n, std::unordered_map<std::string, T *> feed);

private:
    Graph _graph;
    Session _session;
};

#include "../src/NN.tpp"
#endif /* NN_H */