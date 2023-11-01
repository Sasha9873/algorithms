#pragma once 
#include <stdexcept>

template <typename T, int N>
class Array {
 public:
  Array() = default;

  Array(std::initializer_list<T> lst) {  // Do not touch.
    std::copy(lst.begin(), lst.end(), arr_);
  }

  T& Front();
  const T& Front() const;

  T& Back();
  const T& Back() const;

  int Size();

  bool Empty();

  T* Data();
  const T* Data() const;

  T& operator[](int i);
  const T& operator[](int i) const;

  void Swap(Array<T, N>& cur);

 private:
  T arr_[N];
};


template<typename T, int N>
T& Array<T, N>::Front() 
{
  return arr_[0];
}

template<typename T, int N>
const T& Array<T, N>::Front() const
{
  return arr_[0];
}

template<typename T, int N>
T& Array<T, N>::Back() 
{
  return arr_[N - 1];
}

template<typename T, int N>
const T& Array<T, N>::Back() const
{
  return arr_[N - 1];
}

template<typename T, int N>
int Array<T, N>::Size() 
{
  return N;
}

template<typename T, int N>
bool Array<T, N>::Empty() 
{
  return !N;
}

template<typename T, int N>
T* Array<T, N>::Data() 
{
  return arr_;
}

template<typename T, int N>
const T* Array<T, N>::Data() const
{
  return arr_;
}

template<typename T, int N>
T& Array<T, N>::operator[](int i)
{
  return arr_[i];
}

template<typename T, int N>
const T& Array<T, N>::operator[](int i) const
{
  return arr_[i];
}

template<typename T>
void swap(T& first, T& second)
{
  T tmp = std::move(first);
  first = std::move(second);
  second = std::move(tmp);
}

template<typename T, int N>
void Array<T, N>::Swap(Array<T, N>& cur)
{
    for(size_t i = 0; i < N; ++i)
      swap(arr_[i], cur[i]);
}
