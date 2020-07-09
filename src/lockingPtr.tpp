#include <iostream>
#include <thread>

template <typename T>
Locking_ptr<T>::Locking_ptr(T *ptr, std::mutex *mtx) : ptr_(ptr), Mtx_(mtx) {}

template <typename T>
Locking_ptr<T>::~Locking_ptr() {}

template <typename T>
Locking_ptr<T>::Locking_ptr(Locking_ptr<T> const &other)
{
    std::cout << "Locking_ptr copy constructor..." << std::endl;
    ptr_ = other.ptr_;
    Mtx_ = other.Mtx_;
}

template <typename T>
Locking_ptr<T>::Locking_ptr(Locking_ptr<T> &&other)
{
    std::cout << "Locking_ptr move constructor..." << std::endl;
    ptr_ = std::move(other.ptr_);
    Mtx_ = std::move(other.Mtx_);
}

template <typename T>
Locking_ptr<T> &Locking_ptr<T>::operator=(Locking_ptr<T> &&other)
{
    std::cout << "Locking_ptr move assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_.reset(std::move(other.ptr_));
        Mtx_ = std::move(other.Mtx_);
    }
    return *this;
}

template <typename T>
Locking_ptr<T> &Locking_ptr<T>::operator=(Locking_ptr<T> const &other)
{
    std::cout << "Locking_ptr copy assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_.reset(other.ptr_);
        Mtx_ = other.Mtx_;
    }
    return *this;
}

template <typename T>
T *Locking_ptr<T>::operator->()
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return ptr_.get();
}

template <typename T>
T &Locking_ptr<T>::operator*()
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return *ptr_;
}

template <typename T>
void Locking_ptr<T>::reset(T *ptr)
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    ptr_.reset(ptr);
}