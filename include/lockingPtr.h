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
    Locking_ptr(T *ptr, std::mutex *mtx);
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
    std::atomic<std::mutex *> Mtx_;

private:
    T *ptr_;
};

template <typename T>
class Locking_shared_ptr
{
public:
    Locking_shared_ptr(T *ptr);
    Locking_shared_ptr(T *ptr, std::mutex *mtx);
    Locking_shared_ptr(std::shared_ptr<T> ptr);

    Locking_shared_ptr(Locking_shared_ptr<T> const &other);
    Locking_shared_ptr(Locking_shared_ptr<T> &&other);

    ~Locking_shared_ptr();

    Locking_shared_ptr<T> &operator=(Locking_shared_ptr<T> &&other);
    Locking_shared_ptr<T> &operator=(Locking_shared_ptr<T> const &other);
    T *operator->() const;
    T &operator*() const;
    bool operator==(Locking_shared_ptr<T> const &rhs);

    void reset(T *ptr);
    T *get() const;

protected:
    std::atomic<std::mutex *> Mtx_;

private:
    std::shared_ptr<T> ptr_;
};

template <typename T>
class Locking_unique_ptr
{
public:
    Locking_unique_ptr(T *ptr);
    Locking_unique_ptr(T *ptr, std::mutex *mtx);
    Locking_unique_ptr(std::unique_ptr<T> &&ptr);
    Locking_unique_ptr(Locking_unique_ptr<T> &&other);

    ~Locking_unique_ptr();

    Locking_unique_ptr<T> &operator=(Locking_unique_ptr<T> &&other);
    T *operator->() const;
    T &operator*() const;
    bool operator==(Locking_unique_ptr<T> const &rhs);

    void reset(T *ptr);
    T *get() const;

protected:
    std::atomic<std::mutex *> Mtx_;

private:
    Locking_unique_ptr<T> &operator=(Locking_unique_ptr<T> const &other) = delete;
    Locking_unique_ptr(Locking_unique_ptr<T> const &other) = delete;
    std::unique_ptr<T> ptr_;
};

#include "../src/lockingPtr.tpp"

#endif /* LOCKING_PTR */