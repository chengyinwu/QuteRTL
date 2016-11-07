/****************************************************************************
  FileName     [ vlpStack.h ]
  Package      [ vlp ]
  Synopsis     [ Header of Stack ]
  Author       [ Hu-Hsi(Louis)Yeh ]
  Copyright    [ Copyleft(c) 2005 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef VLP_STACK_H
#define VLP_STACK_H

#include <iostream>

using namespace std;
template <class T>
class Stack
{
   public:
   Stack(): _data(0), _size(0), _capacity(0) {}
   Stack(size_t n): _size(n), _capacity(n) { _data = new T[n]; }
   ~Stack() { if (_capacity != 0) delete []_data; else return; }

   size_t size() const { return _size; }

   void push(const T& value) {
      if(_capacity == 0) { // When Stack is null
         _data = new T(value);
         _size = 1;
         _capacity =1;
      }
      else {
         if(_size < _capacity) {
            ++_size;
            _data[_size-1] = value;
         }
         else if(_size == _capacity) {
            expand(2*_capacity);
            ++_size;
            _data[_size-1] = value;
         }	
         else {//_size > _capacity
            cerr << "Stack error" << endl;
      	    exit(1);
         }
      }
   }

   T pop() {
      T target;
      if (_size > 0) {
         target = _data[_size-1];
         _data[_size-1].~T();
         --_size;
         return target;
      }
      else
         return target;
      
   }

   void reserve(size_t n) {
      if(n > _capacity)
      {
         T* temp = new T[n];
         memmove(temp, _data, _size*sizeof(T));
         delete[] _data;
         _data = temp;
         _capacity = n;
      }
   }

   void resize(size_t n) {
      T* temp1 = new T[n];// need T default constructor!!
      T* temp2 = new T[n];
      delete[] temp1;
      int mov_size; 
      if(n > _size)   // resize from small to big
         mov_size = _size; 
      else            // resize from big to small
         mov_size = n;
      memmove(temp2, _data, mov_size*sizeof(T));
      delete[] _data;
      _data = temp2;
      _size = n;
      _capacity = n;
   }

   void clear() {
      delete[] _data;
      _size = 0;
      _capacity = 0;
   }

private:
   T*           _data;
   size_t       _size;
   size_t       _capacity;

   void expand(size_t n) {
      T* temp = new T[n];
      memmove(temp, _data, _size*sizeof(T));
      delete[] _data;
      _data = temp;	
      _capacity = n;
   }
};

#endif
