#include <iostream>
#include <thread>

template <typename T>
Locking_ptr<T>::Locking_ptr(T *ptr) : ptr_(ptr), Mtx_(&(ptr->Mtx_)) {}

template <typename T>
Locking_ptr<T>::Locking_ptr(T *ptr, std::mutex *mtx) : ptr_(ptr), Mtx_(mtx) {}

template <typename T>
Locking_ptr<T>::~Locking_ptr() {}

template <typename T>
Locking_ptr<T>::Locking_ptr(Locking_ptr<T> const &other)
{
    //std::cout << "Locking_ptr copy constructor..." << std::endl;
    Mtx_.store(other.Mtx_.load());
    std::unique_lock<std::mutex> lc1(*(Mtx_.load()));
    ptr_ = other.ptr_;
}

template <typename T>
Locking_ptr<T>::Locking_ptr(Locking_ptr<T> &&other)
{
    //std::cout << "Locking_ptr move constructor..." << std::endl;
    Mtx_.store(other.Mtx_.load());
    std::unique_lock<std::mutex> lc1(*Mtx_);
    ptr_ = std::move(other.ptr_);
    other.Mtx_.store(nullptr);
}

template <typename T>
Locking_ptr<T> &Locking_ptr<T>::operator=(Locking_ptr<T> &&other)
{
    Mtx_.store(other.Mtx_.load());
    std::unique_lock<std::mutex> lc1(*Mtx_);
    //std::cout << "Locking_ptr move assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_ = std::move(other.ptr_);
        other.Mtx_.store(nullptr);
    }
    return *this;
}

template <typename T>
Locking_ptr<T> &Locking_ptr<T>::operator=(Locking_ptr<T> const &other)
{
    Mtx_.store(other.Mtx_.load());
    std::unique_lock<std::mutex> lc1(*Mtx_);
    //std::cout << "Locking_ptr copy assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_ = other.get();
    }
    return *this;
}

template <typename T>
T *Locking_ptr<T>::operator->() const
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return ptr_;
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
    ptr_ = ptr;
}

template <typename T>
T *Locking_ptr<T>::get() const
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return ptr_;
}
//-------- shared locking ptr ---------

template <typename T>
Locking_shared_ptr<T>::Locking_shared_ptr(T *ptr) : ptr_(ptr), Mtx_(&(ptr->Mtx_)){}

template <typename T>
Locking_shared_ptr<T>::Locking_shared_ptr(T *ptr, std::mutex *mtx) : ptr_(ptr), Mtx_(mtx) {}

template <typename T>
Locking_shared_ptr<T>::Locking_shared_ptr(std::shared_ptr<T> ptr)
{
    Mtx_.store((&ptr->Mtx_).load());
    ptr_ = ptr;
}

template <typename T>
Locking_shared_ptr<T>::~Locking_shared_ptr() { }

template <typename T>
Locking_shared_ptr<T>::Locking_shared_ptr(Locking_shared_ptr<T> const &other)
{
    //std::cout << "Locking_shared_ptr copy constructor..." << std::endl;
    Mtx_.store(other.Mtx_.load());
    std::unique_lock<std::mutex> lc1(*Mtx_);
    ptr_ = other.ptr_;
}

template <typename T>
Locking_shared_ptr<T>::Locking_shared_ptr(Locking_shared_ptr<T> &&other)
{
    //std::cout << "Locking_shared_ptr move constructor..." << std::endl;
    Mtx_.store(other.Mtx_.load());
    std::unique_lock<std::mutex> lc1(*Mtx_);
    ptr_ = std::move(other.ptr_);
    other.Mtx_.store(nullptr);
}

template <typename T>
Locking_shared_ptr<T> &Locking_shared_ptr<T>::operator=(Locking_shared_ptr<T> &&other)
{
    Mtx_.store(other.Mtx_.load());
    std::unique_lock<std::mutex> lc1(*Mtx_);
    //std::cout << "Locking_shared_ptr move assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_ = std::move(other.ptr_);
        other.Mtx_.store(nullptr);
    }
    return *this;
}

template <typename T>
Locking_shared_ptr<T> &Locking_shared_ptr<T>::operator=(Locking_shared_ptr<T> const &other)
{
    Mtx_.store(other.Mtx_.load());
    std::unique_lock<std::mutex> lc1(*Mtx_);
    //std::cout << "Locking_shared_ptr copy assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_ = other.ptr_;
    }
    return *this;
}

template <typename T>
T *Locking_shared_ptr<T>::operator->() const
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return ptr_.get();
}

template <typename T>
T &Locking_shared_ptr<T>::operator*() const
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return *ptr_;
}

template <typename T>
bool Locking_shared_ptr<T>::operator==(Locking_shared_ptr<T> const &rhs)
{
    return this->get() == rhs.get();
}

template <typename T>
void Locking_shared_ptr<T>::reset(T *ptr)
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    ptr_.reset(ptr);
}

template <typename T>
T *Locking_shared_ptr<T>::get() const
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return ptr_.get();
}

//-------- locking unique ptr ---------

template <typename T>
Locking_unique_ptr<T>::Locking_unique_ptr(T *ptr) : ptr_(ptr), Mtx_(&(ptr->Mtx_)){}

template <typename T>
Locking_unique_ptr<T>::Locking_unique_ptr(T *ptr, std::mutex *mtx) : ptr_(ptr), Mtx_(mtx) {}

template <typename T>
Locking_unique_ptr<T>::Locking_unique_ptr(std::unique_ptr<T> &&ptr)
{
    Mtx_.store((&ptr->Mtx_).load());
    std::unique_lock<std::mutex> lc1(*Mtx_);
    ptr_ = std::move(ptr);
}

template <typename T>
Locking_unique_ptr<T>::~Locking_unique_ptr() { }

template <typename T>
Locking_unique_ptr<T>::Locking_unique_ptr(Locking_unique_ptr<T> &&other)
{
    //std::cout << "Locking_shared_ptr move constructor..." << std::endl;
    Mtx_.store(other.Mtx_.load());
    std::unique_lock<std::mutex> lc1(*Mtx_);
    ptr_ = std::move(other.ptr_);
    other.Mtx_.store(nullptr);
}

template <typename T>
Locking_unique_ptr<T> &Locking_unique_ptr<T>::operator=(Locking_unique_ptr<T> &&other)
{
    Mtx_.store(other.Mtx_.load());
    std::unique_lock<std::mutex> lc1(*Mtx_);
    //std::cout << "Locking_shared_ptr move assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_ = std::move(other.ptr_);
        other.Mtx_.store(nullptr);
    }
    return *this;
}

template <typename T>
T *Locking_unique_ptr<T>::operator->() const
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return ptr_.get();
}

template <typename T>
T &Locking_unique_ptr<T>::operator*() const
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return *ptr_;
}

template <typename T>
bool Locking_unique_ptr<T>::operator==(Locking_unique_ptr<T> const &rhs)
{
    return this->get() == rhs.get();
}

template <typename T>
void Locking_unique_ptr<T>::reset(T *ptr)
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    ptr_.reset(ptr);
}

template <typename T>
T *Locking_unique_ptr<T>::get() const
{
    std::unique_lock<std::mutex> lc(*Mtx_);
    return ptr_.get();
}
