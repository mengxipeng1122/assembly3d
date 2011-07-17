/*
 * Copyright (c) 2011 Peter Vasil
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iosfwd>
#include <iostream>

template<class T>
class Vector2
{
 public:
  Vector2()
  {
    set(T(), T());
  }
  Vector2(T new_x, T new_y)
  {
    set(new_x, new_y);
  }
  explicit Vector2(T value)
  {
    set(value, value);
  }

  void set(T new_x, T new_y)
  {
    x() = new_x;
    y() = new_y;
  }

  T& x(){return _c[0];}
  T& y(){return _c[1];}
  T x() const{return _c[0];}
  T y() const{return _c[1];}

  void operator=(T value)
  {
    set(value, value);
  }

  void operator+=(const Vector2& other)
  {
    set(x() + other.x(), y() + other.y());
  }
  void operator-=(const Vector2& other)
  {
    set(x() - other.x(), y() - other.y());
  }
  void operator*=(T factor)
  {
    set(x() * factor, y() * factor);
  }
  void operator/=(T factor)
  {
    set(x() / factor, y() / factor);
  }

  T lengthSquared() const
  {
    return x()*x() + y()*y();
  }
  T length() const
  {
    return std::sqrt(lengthSquared());
  }
  void normalize()
  {
    operator/=(length());
  }
  void invert()
  {
    x() = -x();
    y() = -y();
  }

  operator const T*() const
  {
    return _c;
  }

  template <class U>
  Vector2<U> convert() const
  {
    return Vector2<U>(static_cast<U>(x()), static_cast<U>(y()));
  }

 private:
  T _c[2];
};

template<class T>
inline const Vector2<T> operator+(const Vector2<T>& one, const Vector2<T>& two)
{
  return Vector2<T>(one.x() + two.x(), one.y() + two.y());
}
template<class T>
inline const Vector2<T> operator-(const Vector2<T>& one, const Vector2<T>& two)
{
  return Vector2<T>(one.x() - two.x(), one.y() - two.y());
}
template<class T>
inline const Vector2<T> operator-(const Vector2<T>& one)
{
  return Vector2<T>(-one.x(), -one.y());
}
template<class T>
inline const Vector2<T> operator*(const Vector2<T>& one, T factor)
{
  return Vector2<T>(one.x() * factor, one.y() * factor);
}
template<class T>
inline const Vector2<T> operator*(T factor, const Vector2<T>& one)
{
  return Vector2<T>(one.x() * factor, one.y() * factor);
}
template<class T>
inline const Vector2<T> operator/(const Vector2<T>& one, T factor)
{
  return Vector2<T>(one.x() / factor, one.y() / factor);
}

template<class T>
inline T operator*(const Vector2<T>& one, const Vector2<T>& two){ //dotproduct
  return one.x() * two.x() + one.y() * two.y();
}

template<class T>
inline bool operator==(const Vector2<T>& one, const Vector2<T>& two)
{
  return one.x() == two.x() and one.y() == two.y();
}
template<class T>
inline bool operator!=(const Vector2<T>& one, const Vector2<T>& two)
{
  return !(one == two);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Vector2<T>& vec)
{
  os << '(' << vec.x() << ',' << ' ' << vec.y() << ')';
  return os;
}

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;
typedef Vector2<size_t> Vector2u;

#endif // VECTOR2_H
