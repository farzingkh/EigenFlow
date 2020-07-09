#include <iostream>
#include <thread>

template <typename T>
Locking_ptr<T>::Locking_ptr(T *ptr) : ptr_(ptr), Mtx_(&(ptr->DataMtx_)) {}

template <typename T>
Locking_ptr<T>::Locking_ptr(T *ptr, std::mutex *mtx) : ptr_(ptr), Mtx_(mtx) {}

template <typename T>
Locking_ptr<T>::~Locking_ptr() {}

template <typename T>
Locking_ptr<T>::Locking_ptr(Locking_ptr<T> const &other)
{
    std::cout << "Locking_ptr copy constructor..." << std::endl;
    Mtx_ = other.Mtx_;
    std::unique_lock<std::mutex> lc1(*Mtx_);
    ptr_ = other.ptr_;
}

template <typename T>
Locking_ptr<T>::Locking_ptr(Locking_ptr<T> &&other)
{
    std::cout << "Locking_ptr move constructor..." << std::endl;
    Mtx_ = std::move(other.Mtx_);
    std::unique_lock<std::mutex> lc1(*Mtx_);
    ptr_ = std::move(other.ptr_);
}

template <typename T>
Locking_ptr<T> &Locking_ptr<T>::operator=(Locking_ptr<T> &&other)
{
    Mtx_ = std::move(other.Mtx_);
    std::unique_lock<std::mutex> lc1(*Mtx_);
    std::cout << "Locking_ptr move assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_.reset(std::move(other.ptr_));
    }
    return *this;
}

template <typename T>
Locking_ptr<T> &Locking_ptr<T>::operator=(Locking_ptr<T> const &other)
{
    Mtx_ = other.Mtx_;
    std::unique_lock<std::mutex> lc1(*Mtx_);
    std::cout << "Locking_ptr copy assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_.reset(other.get());
    }
    return *this;
}

template <typename T>
T *Locking_ptr<T>::operator->() const
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return ptr_.get();
}

template <typename T>
T &Locking_ptr<T>::operator*() const
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return *ptr_;
}

template <typename T>
bool Locking_ptr<T>::operator==(Locking_ptr<T> const &rhs)
{
    return this->get() == rhs.get();
}

template <typename T>
void Locking_ptr<T>::reset(T *ptr)
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    ptr_.reset(ptr);
}

template <typename T>
T *Locking_ptr<T>::get() const
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return ptr_.get();
}
