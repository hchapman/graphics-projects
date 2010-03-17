#ifndef __Vector3_h__
#define __Vector3_h__

#include "RE330_global.h"
#include <stdio.h>
#include <iostream>
#include <math.h>

namespace RE330
{
    template <class T = float> class RE330_EXPORT Vector3
    {
    private:
        T x, y, z;
    public:
        friend std::ostream& operator<<<T>(std::ostream& out, const Vector3& v);

        // Constructors and assignment
        Vector3<T>() : x(0), y(0), z(0) {}

        Vector3<T>(const float a, const float b, const float c) :
            x(a), y(b), z(c) {}

        Vector3<T>(const float *k) : x(k[0]), y(k[1]), z(k[2]) {
            std:: cout << *this; }

        float operator[] (int i) const { return (&x)[i]; }

        Vector3<T>& operator= (const Vector3<T>& k)
        {
            x = k[0];
            y = k[1];
            z = k[2];
            return *this;
        }

        Vector3<T>& operator= (const float* k)
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
        Vector3<T>& operator/= (const float i)
        {
            x /= i;
            y /= i;
            z /= i;
            return *this;
        }

        Vector3<T> operator/ (const float i) const
        {
            return Vector3<T>(*this) /= i;
        }

        // Scalar multiplication
        Vector3<T>& operator*= (const float i)
        {
            x *= i;
            y *= i;
            z *= i;
            return *this;
        }

        Vector3<T> operator* (const float i) const
        {
            return Vector3<T>(*this) *= i;
        }

        // Cross multiplication
        Vector3<T>& operator*= (const Vector3<T>& k)
        {
            float a = x; float b = y; float c = z;
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
        float operator^ (const Vector3<T>& k) const
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
        float len () const
        {
            return sqrt((x*x) + (y*y) + (z*z));
        }

        // Normalizes the vector
        Vector3<T> normalize ()
        {
            if (len() == 0) return (*this);
            return (*this) /= len();
        }
    };

    template <class T = float> inline std::ostream& operator<<<T>(std::ostream& out, const Vector3<T>& v) {
        out << "Vector3(" << v.x << "," << v.y << "," << v.z << ")";
        return out;
    }
}

#endif
