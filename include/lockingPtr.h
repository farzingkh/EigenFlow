#ifndef LOCKING_PTR
#define LOCKING_PTR

#include <memory>
#include <mutex>

// a wrapper class to avoid data races when accessing data from shared ptrs
template <typename T>
class Locking_ptr
{
public:
    Locking_ptr(T *ptr);
    Locking_ptr(T *ptr, std::mutex* mtx);
    Locking_ptr(Locking_ptr<T> const &other);
    Locking_ptr(Locking_ptr<T> &&other);

    ~Locking_ptr();

    Locking_ptr<T> &operator=(Locking_ptr<T> &&other);
    Locking_ptr<T> &operator=(Locking_ptr<T> const &other);
    T *operator->() const;
    T &operator*() const;
    bool operator==(Locking_ptr<T> const &rhs);

    void reset(T *ptr);
    T *get() const;

protected:
    std::mutex *Mtx_;

private:
    T* ptr_;
};

template <typename T, template <typename> class U>
class Locking_smart_ptr
{
public:
    Locking_smart_ptr(T *ptr);
    Locking_smart_ptr(T *ptr, std::mutex* mtx);
    Locking_smart_ptr(Locking_smart_ptr<T,U> const &other);
    Locking_smart_ptr(Locking_smart_ptr<T,U> &&other);

    ~Locking_smart_ptr();

    Locking_smart_ptr<T,U> &operator=(Locking_smart_ptr<T,U> &&other);
    Locking_smart_ptr<T,U> &operator=(Locking_smart_ptr<T,U> const &other);
    T *operator->() const;
    T &operator*() const;
    bool operator==(Locking_smart_ptr<T,U> const &rhs);

    void reset(T *ptr);
    T *get() const;

protected:
    std::mutex *Mtx_;

private:
    U<T> ptr_;
};
#include "../src/lockingPtr.tpp"

#endif /* LOCKING_PTR */