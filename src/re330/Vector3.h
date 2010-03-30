#ifndef __Vector3_h__
#define __Vector3_h__

#include "RE330_global.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

namespace RE330
{
    template <class T = float> 
    class RE330_EXPORT Vector3
    {
    private:
        T x, y, z;
    public:
        // Constructors and assignment
        Vector3<T>() : x(0), y(0), z(0) {}

        Vector3<T>(const T a, const T b, const T c) :
            x(a), y(b), z(c) {}

        Vector3<T>(const T *k) : x(k[0]), y(k[1]), z(k[2]) {}

        T operator[] (int i) const { return (&x)[i]; }

        Vector3<T>& operator= (const Vector3<T>& k)
        {
            x = k[0];
            y = k[1];
            z = k[2];
            return *this;
        }

        Vector3<T>& operator= (const T* k)
        {
            x = k[0];
            y = k[1];
            z = k[2];
            return *this;
        }

        Vector3<T>(const Vector3<T>& k) {(*this) = k;}

        // Overloaded operators
        // Addition
        Vector3<T>& operator+= (const Vector3<T>& k)
        {
            x += k[0];
            y += k[1];
            z += k[2];
            return *this;
        }

        Vector3<T> operator+ (const Vector3<T>& k) const
        {
            return Vector3<T>(*this) += k;

        }
        // Subtraction
        Vector3<T>& operator-= (const Vector3<T>& k)
        {
            x -= k[0];
            y -= k[1];
            z -= k[2];
            return *this;

        }
        Vector3<T> operator- (const Vector3<T>& k) const
        {
            return Vector3<T>(*this) -= k;
        }

        // Scalar division
        Vector3<T>& operator/= (const T i)
        {
            x /= i;
            y /= i;
            z /= i;
            return *this;
        }

        Vector3<T> operator/ (const T i) const
        {
            return Vector3<T>(*this) /= i;
        }

        // Scalar multiplication
        Vector3<T>& operator*= (const T i)
        {
            x *= i;
            y *= i;
            z *= i;
            return *this;
        }

        Vector3<T> operator* (const T i) const
        {
            return Vector3<T>(*this) *= i;
        }

        // Division through by another vector
        template <class U>
        Vector3<T> operator/= (const Vector3<U> k) 
        {
            x /= k[0];
            y /= k[1];
            z /= k[2];
            return *this;
        }

        template <class U>
        Vector3<T> operator/ (const Vector3<U> k) const
        {
            return Vector3<T>(*this) /= k;
        }

        // Multiplication through by another vector
        // Careful! This supposes that U*T returns a U.
        template <class U>
        Vector3<T> times(const Vector3<U> k) const
        {
            return Vector3<T>(x * k[0], y * k[1], z * k[2]) ;
        }

        // Cross multiplication
        Vector3<T>& operator*= (const Vector3<T>& k)
        {
            T a = x; T b = y; T c = z;
            x = b*k[2] - c*k[1];
            y = c*k[0] - a*k[2];
            z = a*k[1] - b*k[0];
            return *this;
        }

        Vector3<T> operator* (const Vector3<T>& k) const
        {
            return Vector3<T>(*this) *= k;
        }

        // Dot multiplication
        template <class U>
        T operator^ (const Vector3<U>& k) const
        {
            return x*k[0] + y*k[1] + z*k[2];
        }

        // Equality test within epsilon
        bool operator== (const Vector3<T>& k) const
        {
            return ((x - k[0] <  EPSILON) &&
                    (y - k[1] <  EPSILON) &&
                    (z - k[2] <  EPSILON) &&
                    (x - k[0] > -EPSILON) &&
                    (y - k[1] > -EPSILON) &&
                    (z - k[2] > -EPSILON));
        }

        // Inequality test
        bool operator!= (const Vector3<T>& k) const
        {
            return !(*this == k);
        }

        // Magnitude of the vector
        T len () const
        {
            return sqrt((x*x) + (y*y) + (z*z));
        }

        // Normalizes the vector
        Vector3<T> normalize ()
        {
            if (len() == 0) return (*this);
            return (*this) /= len();
        }

        Vector3<T> reciprocal () const
        {
            return Vector3<T> (1/x, 1/y, 1/z);
        }

        // Return the sum (taxicab distance from origin)
        T sum() const
        {
            return (x + y + z);
        }

        // Stream into an ostream object
        inline std::ostream& streamOut(std::ostream& out) const 
        {
            out << "Vector3(" << x << "," << y << "," << z << ")";
            return out;
        }
    };

    template <class T> 
    inline std::ostream& operator<< (std::ostream& out, 
                                     const Vector3<T>& v) {
        return v.streamOut(out);
    }

    template <class T>
    inline Vector3<T>& operator/ (const T t, const Vector3<T>& v) {
        return Vector3<T>(t / v[0], t / v[1], t / v[2]);
    }
}

#endif
