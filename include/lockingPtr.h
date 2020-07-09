#ifndef LOCKING_PTR
#define LOCKING_PTR

#include <memory>
#include <mutex>

// a wrapper class to avoid data races when accessing data from shared ptrs
template <typename T>
class Locking_ptr
{
public:
    Locking_ptr(T *ptr, std::mutex *mtx);

    ~Locking_ptr();
    Locking_ptr(Locking_ptr<T> const &other);
    Locking_ptr(Locking_ptr<T> &&other);
    Locking_ptr<T> &operator=(Locking_ptr<T> &&other);
    Locking_ptr<T> &operator=(Locking_ptr<T> const &other);

    T *operator->();
    T &operator*();

    void reset(T *ptr);

protected:
    std::mutex *Mtx_;

private:
    std::shared_ptr<T> ptr_;
};

#include "../src/lockingPtr.tpp"

#endif /* LOCKING_PTR */