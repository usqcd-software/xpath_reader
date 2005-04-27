/* $Id: xml_array.h,v 1.1 2005-04-27 19:48:33 edwards Exp $ 
 *
 * File: xml_array.h
 *
 * Description: 
 *
 * This file has been blatantly stolen from QDP++ (where it was known 
 * as multi.h.) It provides templated array functionality. 
 * Unlike multi.h which defined 1, 2, 3 and 4D arrays, here I am 
 * restricting myself to 1D arrays. These can hold other 1D arrays 
 *
 */

#ifndef XML_ARRAY_H
#define XML_ARRAY_H

#include <iostream>
#include <cmath>

namespace XMLArray {

  //! Container for a multi-dimensional 1D array
  template<typename T> class Array
  {
  public:
    Array() {F=0;n1=0;}
    explicit Array(int ns1) {F=0;n1=0;resize(ns1);}
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
      {
	if (ns1 < 0) 
	{
	  std::cerr<<"invalid resize in 1d\n";
	  exit(1);
	}
	delete[] F; 
	n1 = ns1;
     
	F = new(std::nothrow) T[n1];
	if( F == 0x0 ) { 
	  std::cerr << "Failed to resize array" << std::endl << std::flush;
	  exit(-1);
	}
      
      }
  
    //! Size of array
    int size() const {return n1;}
    int size1() const {return n1;}
  
    //! Equal operator uses underlying = of T
    Array<T>& operator=(const Array<T>& s1)
      {
	if (size() != s1.size())   // a simple check avoids resizing always
	  resize(s1.size());
    
	for(int i=0; i < n1; ++i)
	  F[i] = s1.F[i];
	return *this;
      }
  
    //! Equal operator uses underlying = of T
    template<typename T1>
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

    //! add to all ellements
    /*! Uses the underlying += */
    Array<T>& operator+=(const T& s1)
      {
	for(int i=0; i < n1; ++i)
	  F[i] += s1 ;
	return *this;
      }

    //! subtruct from all ellements
    /*! Uses the underlying -= */
    Array<T>& operator-=(const T& s1)
      {
	for(int i=0; i < n1; ++i)
	  F[i] -= s1 ;
	return *this;
      }
  
    //! multiply   all ellements
    /*! Uses the underlying *= */
    Array<T>& operator*=(const T& s1)
      {
	for(int i=0; i < n1; ++i)
	  F[i] *= s1 ;
	return *this;
      }

    //! devide  all ellements
    /*! Uses the underlying *= */
    Array<T>& operator/=(const T& s1)
      {
	for(int i=0; i < n1; ++i)
	  F[i] /= s1 ;
	return *this;
      }

    //!unary -
    /*! Uses the underlying unary - */
    Array<T>& operator-()
      {
	for(int i=0; i < n1; ++i)
	  F[i] = -F[i] ;
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


  //---------------------------------------------------------------
  // Comparisons/recombinations

  //! Concatenate two Array's
  template<typename T>
  inline Array<T> concat(const Array<T>& l, const Array<T>& r)
  {
    Array<int> nz(l.size() + r.size());
    int j = 0;
    for(int i=0; i < l.size(); ++i)
      nz[j++] = l[i];
  
    for(int i=0; i < r.size(); ++i)
      nz[j++] = r[i];

    return nz;
  }

  //! Check if two Array's are the same
  template<typename T>
  inline bool operator==(const Array<T>& n1, const Array<T>& n2)
  {
    if (n1.size() == 0 || n1.size() != n2.size())
      return false;
    
    for(int i=0; i < n1.size(); ++i)
      if (n2[i] != n1[i])
	return false;

    return true;
  }
  

  //! Check if two Array's are no the same
  template<typename T>
  inline bool operator!=(const Array<T>& n1, const Array<T>& n2)
  {
    return ! (n1 == n2);
  }


  //---------------------------------------------------------------
  // Basic math support
  //
  //! add Arrays
  template< typename T> 
  Array<T> operator+(const Array<T>& a, const Array<T>& b)
  {
    Array<T> c(a); 
    c+=b;
    return c;
  }
  
  //! subtract Arrays
  template< typename T> 
  Array<T> operator-(const Array<T>& a, const Array<T>& b)
  {
    Array<T> c(a); 
    c-=b;
    return c;
  }
  
  //! multiply Arrays
  template< typename T> 
  Array<T> operator*(const Array<T>& a, const Array<T>& b)
  {
    Array<T> c(a); 
    c*=b;
    return c;
  }
  
  //!divide Arrays
  template< typename T> 
  Array<T> operator/(const Array<T>& a, const Array<T>& b)
  {
    Array<T> c(a); 
    c/=b;
    return c;
  }

  //! scalar + Array
  template< typename T> 
  Array<T> operator+(const T& s, const Array<T>& a)
  {
    Array<T> c(a); 
    c+=s;
    return c;
  }

  //! Array + scalar
  template< typename T> 
  Array<T> operator+(const Array<T>& a, const T& s)
  {
    Array<T> c(a); 
    c+=s;
    return c;
  }
  
  //! scalar - Array
  template< typename T> 
  Array<T> operator-(const T& s, const Array<T>& a)
  {
    Array<T> c(-a); 
    c+=s;
    return c;
  }
  //! Array - scalar
  template< typename T> 
  Array<T> operator-(const Array<T>& a, const T& s)
  {
    Array<T> c(a); 
    c-=s;
    return c;
  }

  //! scalar * Array
  template< typename T> 
  Array<T> operator*(const T& s, const Array<T>& a)
  {
    Array<T> c(a); 
    c*=s;
    return c;
  }

  //! Array * scalar
  template< typename T> 
  Array<T> operator*(const Array<T>& a, const T& s)
  {
    Array<T> c(a); 
    c*=s;
    return c;
  }

  //! scalar / Array
  template< typename T> 
  Array<T> operator/(const T& s, const Array<T>& a)
  {
    Array<T> c(a.size());
    c = s;
    c/= a;
    return c;
  }

  //! Array / scalar
  template< typename T> 
  Array<T> operator/(const Array<T>& a, const T& s)
  {
    Array<T> c(a); 
    c/=s;
    return c;
  }

  //! sqrt
  template< typename T> 
  Array<T> sqrt(const Array<T>& a)
  {
    Array<T> c(a.size()); 
    for(int i(0);i<a.size();i++)
    {
      T tt;
      tt = a[i];
      c[i] = std::sqrt(a[i]);
    }
    return c;
  }

} // namespace XMLArray

#endif
