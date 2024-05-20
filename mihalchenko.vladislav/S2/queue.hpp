#ifndef QUEUE_HPP
#define QUEUE_HPP
#include "list.hpp"

namespace mihalchenko
{
  template < typename T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue< T > &copy);
    ~Queue() = default;
    void push(const T &data);
    const T pop();
    T &operator[](const size_t ind);
    void clear();
    size_t getSize();
    void setSize(const size_t size);

  private:
    List< T > containQueue;
  };
}

template < typename T >
mihalchenko::Queue< T >::Queue(const Queue< T > &copy)
{
  containQueue(copy);
}

template < typename T >
void mihalchenko::Queue< T >::push(const T &data)
{
  containQueue.push_back(data);
}

template < typename T >
const T mihalchenko::Queue< T >::pop()
{
  T res = containQueue.getT();
  containQueue.pop_front();
  return res;
}

template < typename T >
T &mihalchenko::Queue< T >::operator[](size_t index)
{
  return containQueue[index];
}

template < typename T >
void mihalchenko::Queue< T >::clear()
{
  containQueue.clear();
}

template < typename T >
size_t mihalchenko::Queue< T >::getSize()
{
  return containQueue.getSize();
}

template < typename T >
void mihalchenko::Queue< T >::setSize(const size_t size)
{
  containQueue.setSize(size);
}

#endif
