// -*- C++ -*-
/* $Id: xml_array2d.h,v 1.1 2006-09-10 22:10:44 edwards Exp $
 *
 * File: xml_array2d.h
 *
 * Description: 
 *
 * A 2d array class
 */

#ifndef XML_ARRAY2D_H
#define XML_ARRAY2D_H

#include <iostream>
#include <cmath>
#include "xml_array.h"

namespace XMLArray 
{
  //! Container for a Array-dimensional 2D array
  template<typename T> class Array2d
  {
  public:
    Array2d() {F=0;n1=n2=sz=0;}
    explicit Array2d(int ns2, int ns1) {F=0;resize(ns2,ns1);}
    ~Array2d() {delete[] F;}

    //! Copy constructor
    Array2d(const Array2d& s): n1(s.n1), n2(s.n2), sz(s.sz), F(0)
      {
	resize(n2,n1);

	for(int i=0; i < sz; ++i)
	  F[i] = s.F[i];
      }

    //! Allocate mem for the array
    void resize(int ns2, int ns1) 
      {
	delete[] F; 
	n1 = ns1; 
	n2 = ns2;  
	sz = n1*n2; 
	F = new(std::nothrow) T[sz];
	if( F == 0x0 ) { 
	  std::cerr << "Unable to new memory in Array2d::resize()\n";
	  exit(1);
	}
      }

    //! Size of array
    int size1() const {return n1;}
    int size2() const {return n2;}

    //! Equal operator uses underlying = of T
    Array2d<T>& operator=(const Array2d<T>& s1)
      {
	resize(s1.size2(), s1.size1());   // always resize

	for(int i=0; i < sz; ++i)
	  F[i] = s1.F[i];
	return *this;
      }

    //! Add-replace on each element
    /*! Uses underlying += */
    Array<T>& operator+=(const Array<T>& s1)
      {
	if (size1() != s1.size() || size2() != s1.size2())
	{
	  std::cerr << "Sizes incompatible in +=\n";
	  exit(1);
	}
    
	for(int i=0; i < sz; ++i)
	  F[i] += s1.F[i];
	return *this;
      }
  
    //! Subtract-replace on each element
    /*! Uses underlying -= */
    Array<T>& operator-=(const Array<T>& s1)
      {
	if (size1() != s1.size() || size2() != s1.size2())
	{
	  std::cerr << "Sizes incompatible in -=\n";
	  exit(1);
	}
    
	for(int i=0; i < sz; ++i)
	  F[i] -= s1.F[i];
	return *this;
      }
  
    //! Mult-replace on each element
    /*! Uses underlying *= */
    Array<T>& operator*=(const Array<T>& s1)
      {
	if (size1() != s1.size() || size2() != s1.size2())
	{
	  std::cerr << "Sizes incompatible in *=\n";
	  exit(1);
	}
    
	for(int i=0; i < sz; ++i)
	  F[i] *= s1.F[i];
	return *this;
      }
  
    //! Divide-replace on each element
    /*! Uses underlying /= */
    Array<T>& operator/=(const Array<T>& s1)
      {
	if (size1() != s1.size() || size2() != s1.size2())
	{
	  std::cerr << "Sizes incompatible in /=\n";
	  exit(1);
	}
    
	for(int i=0; i < sz; ++i)
	  F[i] /= s1.F[i];
	return *this;
      }

    //! multiply   all ellements
    /*! Uses the underlying *= */
    Array<T>& operator*=(const T& s1)
      {
	for(int i=0; i < sz; ++i)
	  F[i] *= s1 ;
	return *this;
      }

    //! devide  all ellements
    /*! Uses the underlying *= */
    Array<T>& operator/=(const T& s1)
      {
	for(int i=0; i < sz; ++i)
	  F[i] /= s1 ;
	return *this;
      }

    //!unary -
    /*! Uses the underlying unary - */
    Array<T>& operator-()
      {
	for(int i=0; i < sz; ++i)
	  F[i] = -F[i] ;
	return *this;
      }

    //! Return ref to an element
    T& operator()(int j, int i) {return F[i+n1*j];}

    //! Return const ref to an element
    const T& operator()(int j, int i) const {return F[i+n1*j];}

    //! Subscripting: pointer to row j
    inline T* operator[](const int j) {return F+n1*j;}

    //! Subscripting: pointer to row j
    inline const T* operator[](const int j) const {return F+n1*j;}

  private:
    int n1;
    int n2;
    int sz;
    T *F;
  };

} // namespace XMLArray

#endif