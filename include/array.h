/* $Id: array.h,v 1.4 2003-07-03 12:42:04 edwards Exp $ 
 *
 * File: array.h
 *
 * Description: 
 *
 * This file has been blatantly stolen from QDP++ (where it was known 
 * as multi.h.) It provides templated array functionality. 
 * Unlike multi.h which defined 1, 2, 3 and 4D arrays, here I am 
 * restricting myself to 1D arrays. These can hold other 1D arrays 
 *
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>

//! Container for a multi-dimensional 1D array
template<class T> class Array
{
 public:
  Array() {F=0;n1=0;}
  Array(T *f, int ns1) {F=f; n1=ns1;}
  explicit Array(int ns1) {F=0;resize(ns1);}
  ~Array() {delete[] F;}
  
  //! Copy constructor
  Array(const Array& s): n1(s.n1), F(0)
    {
      resize(n1);
      
      for(int i=0; i < n1; ++i)
	F[i] = s.F[i];
    }
  
  //! Allocate mem for the array
  void resize(int ns1) 
    {if (ns1 < 0) {std::cerr<<"invalid resize in 1d\n";exit(1);}; delete[] F; n1=ns1; F = new T[n1];}
  
  //! Size of array
  int size() const {return n1;}
  int size1() const {return n1;}
  
  //! Equal operator uses underlying = of T
  Array<T>& operator=(const Array<T>& s1)
  {
    if (F == 0)
      resize(s1.size());
    
    for(int i=0; i < n1; ++i)
      F[i] = s1.F[i];
    return *this;
  }
  
  //! Equal operator uses underlying = of T
  template<class T1>
    Array<T>& operator=(const T1& s1)
  {
    if (F == 0)
      {
	std::cerr << "left hand side not initialized\n";
	exit(1);
      }
    
    for(int i=0; i < n1; ++i)
      F[i] = s1;
    return *this;
  }
  
  //! Set equal to a old-style C 1-D array
  Array<T>& operator=(const T s1[])
  {
    if (F == 0)
      {
	std::cerr << "left hand side not initialized\n";
	exit(1);
      }
    
    for(int i=0; i < n1; ++i)
      F[i] = s1[i];
    return *this;
  }
  
  //! Add-replace on each element
  /*! Uses underlying += */
  Array<T>& operator+=(const Array<T>& s1)
  {
    if (size() != s1.size())
      {
	std::cerr << "Sizes incompatible in +=\n";
	  exit(1);
      }
    
    for(int i=0; i < n1; ++i)
      F[i] += s1.F[i];
    return *this;
  }
  
  //! Subtract-replace on each element
  /*! Uses underlying -= */
  Array<T>& operator-=(const Array<T>& s1)
  {
    if (size() != s1.size())
      {
	std::cerr << "Sizes incompatible in -=\n";
	exit(1);
      }
    
    for(int i=0; i < n1; ++i)
      F[i] -= s1.F[i];
    return *this;
  }
  
  //! Mult-replace on each element
  /*! Uses underlying *= */
  Array<T>& operator*=(const Array<T>& s1)
  {
    if (size() != s1.size())
      {
	std::cerr << "Sizes incompatible in *=\n";
	exit(1);
      }
    
    for(int i=0; i < n1; ++i)
      F[i] *= s1.F[i];
    return *this;
  }
  
  //! Divide-replace on each element
  /*! Uses underlying /= */
  Array<T>& operator/=(const Array<T>& s1)
  {
    if (size() != s1.size())
      {
	std::cerr << "Sizes incompatible in /=\n";
	exit(1);
      }
    
    for(int i=0; i < n1; ++i)
      F[i] /= s1.F[i];
    return *this;
  }
  
  //! Return ref to a column slice
  const T* slice() const {return F;}
  
  //! Return ref to an element
  T& operator()(int i) {return F[i];}
  
  //! Return const ref to an element
  const T& operator()(int i) const {return F[i];}
  
  //! Return ref to an element
  T& operator[](int i) {return F[i];}
  
  //! Return const ref to an element
  const T& operator[](int i) const {return F[i];}
  
 private:
  int n1;
  T *F;
};

#endif
