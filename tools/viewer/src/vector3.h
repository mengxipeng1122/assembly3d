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

#ifndef VECTOR3_H
#define VECTOR3_H

#include <iosfwd>
#include <cmath>
#include <iostream>

template <class T>
class Vector3
{
 public:
  Vector3()
  {
    set(T(), T(), T());
  }
  Vector3(T new_x, T new_y, T new_z)
  {
    set(new_x, new_y, new_z);
  }
  explicit Vector3(T value)
  {
    set(value, value, value);
  }

  void set(T new_x, T new_y, T new_z)
  {
    x() = new_x;
    y() = new_y;
    z() = new_z;
  }

  T& x(){return _c[0];}
  T& y(){return _c[1];}
  T& z(){return _c[2];}
  T x() const{return _c[0];}
  T y() const{return _c[1];}
  T z() const{return _c[2];}

  void operator=(T value)
  {
    set(value, value, value);
  }

  void operator+=(const Vector3& other)
  {
    set(x() + other.x(), y() + other.y(), z() + other.z());
  }
  void operator-=(const Vector3& other)
  {
    set(x() - other.x(), y() - other.y(), z() - other.z());
  }
  void operator*=(T factor)
  {
    set(x() * factor, y() * factor, z() * factor);
  }
  void operator/=(T factor)
  {
    set(x() / factor, y() / factor, z() / factor);
  }

  T lengthSquared() const
  {
    return x()*x() + y()*y() + z()*z();
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
    z() = -z();
  }

  operator const T*() const
  {
    return _c;
  }

  template <class U>
  Vector3<U> convert() const
  {
    return Vector3<U>(static_cast<U>(x()), static_cast<U>(y()), static_cast<U>(z()));
  }

 private:
  T _c[3];
};

template <class T>
inline const Vector3<T> operator+(const Vector3<T>& one, const Vector3<T>& two)
{
  return Vector3<T>(one.x() + two.x(), one.y() + two.y(), one.z() + two.z());
}
template <class T>
inline const Vector3<T> operator-(const Vector3<T>& one, const Vector3<T>& two)
{
  return Vector3<T>(one.x() - two.x(), one.y() - two.y(), one.z() - two.z());
}
template <class T>
inline const Vector3<T> operator-(const Vector3<T>& one)
{
  return Vector3<T>(-one.x(), -one.y(), -one.z());
}
template <class T>
inline const Vector3<T> operator*(const Vector3<T>& one, T factor)
{
  return Vector3<T>(one.x() * factor, one.y() * factor, one.z() * factor);
}
template <class T>
inline const Vector3<T> operator*(T factor, const Vector3<T>& one)
{
  return Vector3<T>(one.x() * factor, one.y() * factor, one.z() * factor);
}
template <class T>
inline const Vector3<T> operator/(const Vector3<T>& one, T factor)
{
  return Vector3<T>(one.x() / factor, one.y() / factor, one.z() / factor);
}

template <class T>
inline T operator*(const Vector3<T>& one, const Vector3<T>& two){ //dotproduct
  return one.x()*two.x() + one.y()*two.y() + one.z()*two.z();
}
template <class T>
inline T boundedDot(const Vector3<T>& one, const Vector3<T>& two)
{
  T dot = one * two;
  if (dot < 0)
    return 0;
  if (dot > 1)
    return 1;
  return dot;
}
template <class T>
inline Vector3<T> operator%(const Vector3<T>& one, const Vector3<T> two){ //crossproduct
  return Vector3<T>(one.y()*two.z() - one.z()*two.y(), one.z()*two.x() - one.x()*two.z(),
                    one.x()*two.y() - one.y()*two.x());
}

template <class T>
inline Vector3<T> normalize(const Vector3<T>& vec){return vec / vec.length();}

template <class T>
inline bool operator==(const Vector3<T>& one, const Vector3<T>& two)
{
  return one.x() == two.x() and one.y() == two.y() and one.z() == two.z();
}
template <class T>
inline bool operator!=(const Vector3<T>& one, const Vector3<T>& two)
{
  return !(one == two);
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Vector3<T>& vec)
{
  os << '(' << vec.x() << ',' << ' ' << vec.y() << ',' << ' ' << vec.z() << ')';
  return os;
}

typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;
typedef Vector3<size_t> Vector3u;

#endif // VECTOR3_H
