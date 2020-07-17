#include <iostream>
#include <thread>

template <typename T>
Locking_ptr<T>::Locking_ptr(T *ptr) : ptr_(ptr), Mtx_(&(ptr->BaseMtx_)) {}

template <typename T>
Locking_ptr<T>::Locking_ptr(T *ptr, std::recursive_mutex *mtx) : ptr_(ptr), Mtx_(mtx) {}

template <typename T>
Locking_ptr<T>::~Locking_ptr() {}

template <typename T>
Locking_ptr<T>::Locking_ptr(Locking_ptr<T> const &other)
{
    //std::cout << "Locking_ptr copy constructor..." << std::endl;
    Mtx_ = other.Mtx_;
    std::unique_lock<std::recursive_mutex> lc1(*Mtx_);
    ptr_ = other.ptr_;
}

template <typename T>
Locking_ptr<T>::Locking_ptr(Locking_ptr<T> &&other)
{
    //std::cout << "Locking_ptr move constructor..." << std::endl;
    Mtx_ = other.Mtx_;
    std::unique_lock<std::recursive_mutex> lc1(*Mtx_);
    ptr_ = std::move(other.ptr_);
    other.Mtx_ = nullptr;
}

template <typename T>
Locking_ptr<T> &Locking_ptr<T>::operator=(Locking_ptr<T> &&other)
{
    Mtx_ = other.Mtx_;
    std::unique_lock<std::recursive_mutex> lc1(*Mtx_);
    //std::cout << "Locking_ptr move assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_ = std::move(other.ptr_);
        other.Mtx_ = nullptr;
    }
    return *this;
}

template <typename T>
Locking_ptr<T> &Locking_ptr<T>::operator=(Locking_ptr<T> const &other)
{
    Mtx_ = other.Mtx_;
    std::unique_lock<std::recursive_mutex> lc1(*Mtx_);
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
    std::unique_lock<std::recursive_mutex> lc(*Mtx_);
    return ptr_;
}

template <typename T>
T &Locking_ptr<T>::operator*() const
{
    std::unique_lock<std::recursive_mutex> lc(*Mtx_);
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
    std::unique_lock<std::recursive_mutex> lc(*Mtx_);
    ptr_ = ptr;
}

template <typename T>
T *Locking_ptr<T>::get() const
{
    std::unique_lock<std::recursive_mutex> lc(*Mtx_);
    return ptr_;
}
//--------

#include <iostream>
#include <thread>

template <typename T, template <typename> class U>
Locking_smart_ptr<T, U>::Locking_smart_ptr(T *ptr) : ptr_(ptr), Mtx_(&(ptr->BaseMtx_))
{
}

template <typename T, template <typename> class U>
Locking_smart_ptr<T, U>::Locking_smart_ptr(T *ptr, std::recursive_mutex *mtx) : ptr_(ptr), Mtx_(mtx) {}

template <typename T, template <typename> class U>
Locking_smart_ptr<T, U>::~Locking_smart_ptr() {}

template <typename T, template <typename> class U>
Locking_smart_ptr<T, U>::Locking_smart_ptr(Locking_smart_ptr<T, U> const &other)
{
    //std::cout << "Locking_smart_ptr copy constructor..." << std::endl;
    Mtx_ = other.Mtx_;
    std::unique_lock<std::recursive_mutex> lc1(*Mtx_);
    ptr_ = other.ptr_;
}

template <typename T, template <typename> class U>
Locking_smart_ptr<T, U>::Locking_smart_ptr(Locking_smart_ptr<T, U> &&other)
{
    //std::cout << "Locking_smart_ptr move constructor..." << std::endl;
    Mtx_ = other.Mtx_;
    std::unique_lock<std::recursive_mutex> lc1(*Mtx_);
    ptr_ = std::move(other.ptr_);
    other.Mtx_ = nullptr;
}

template <typename T, template <typename> class U>
Locking_smart_ptr<T, U> &Locking_smart_ptr<T, U>::operator=(Locking_smart_ptr<T, U> &&other)
{
    Mtx_ = other.Mtx_;
    std::unique_lock<std::recursive_mutex> lc1(*Mtx_);
    //std::cout << "Locking_smart_ptr move assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_ = std::move(other.ptr_);
        other.Mtx_ = nullptr;
    }
    return *this;
}

template <typename T, template <typename> class U>
Locking_smart_ptr<T, U> &Locking_smart_ptr<T, U>::operator=(Locking_smart_ptr<T, U> const &other)
{
    Mtx_ = other.Mtx_;
    std::unique_lock<std::recursive_mutex> lc1(*Mtx_);
    //std::cout << "Locking_smart_ptr copy assignment constructor..." << std::endl;
    if (this != &other)
    {
        ptr_ = other.ptr_;
    }
    return *this;
}

template <typename T, template <typename> class U>
T *Locking_smart_ptr<T, U>::operator->() const
{
    std::unique_lock<std::recursive_mutex> lc(*Mtx_);
    return ptr_.get();
}

template <typename T, template <typename> class U>
T &Locking_smart_ptr<T, U>::operator*() const
{
    std::unique_lock<std::recursive_mutex> lc(*Mtx_);
    return *ptr_;
}

template <typename T, template <typename> class U>
bool Locking_smart_ptr<T, U>::operator==(Locking_smart_ptr<T, U> const &rhs)
{
    return this->get() == rhs.get();
}

template <typename T, template <typename> class U>
void Locking_smart_ptr<T, U>::reset(T *ptr)
{
    std::unique_lock<std::recursive_mutex> lc(*Mtx_);
    ptr_.reset(ptr);
}

template <typename T, template <typename> class U>
T *Locking_smart_ptr<T, U>::get() const
{
    std::unique_lock<std::recursive_mutex> lc(*Mtx_);
    return ptr_.get();
}
