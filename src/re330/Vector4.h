#ifndef __Vector4_h__
#define __Vector4_h__

#include "RE330_global.h"
#include <math.h>
#include <Vector3.h>
#include <iostream>

namespace RE330 
{
    template <class T = float>
	class RE330_EXPORT Vector4
	{
	private:
		T x, y, z, w;
    public:
        // Constructors and assignment
        Vector4<T>() : x(0), y(0), z(0), w(0) {}

        Vector4<T>(T a, T b, T c, T d) 
            : x(a), y(b), z(c), w(d) {}

        Vector4<T>(T *k) 
            : x(k[0]), y(k[1]), z(k[2]), w(k[3]) {}

        // Make a Vector4 from a Vector3 and a fourth T
        Vector4<T>(const Vector3<T> k, const T h)
            : x(k[0]), y(k[1]), z(k[2]), w(h) {}

        // Make a Vector3 from the Vector4 (stripping the fourth T)
        Vector3<T> asVector3() const { return Vector3<T>(&x); }

        T operator[] (int i) const {return (&x)[i];}
        Vector4<T>& operator= (const Vector4<T>& k) {
            x = k[0];
            y = k[1];
            z = k[2];
            w = k[3];
            return *this;
        }
        Vector4<T>& operator= (const T* k) {
            x = k[0];
            y = k[1];
            z = k[2];
            w = k[3];
            return *this;
        }
        Vector4<T>(const Vector4<T>& k) : x(k[0]), y(k[1]), z(k[2]), w(k[3]) {}
        template <class U>
            Vector4<T>(const Vector4<U>& k) 
            : x((T)(k[0])), y((T)(k[1])), z((T)(k[2])), w((T)(k[3])) {}

        // Overloaded operators
        // Addition
        Vector4<T>& operator+= (const Vector4<T>& k) {
            x += k[0];
            y += k[1];
            z += k[2];
            w += k[3];
            return *this;
        }
        Vector4<T> operator+ (const Vector4<T>& k) const {
            return Vector4<T>(*this) += k;
        }
        // Subtraction
        Vector4<T>& operator-= (const Vector4<T>& k) {
            x -= k[0];
            y -= k[1];
            z -= k[2];
            w -= k[3];
            return *this;
        }
        Vector4<T> operator- (const Vector4<T>& k) const {
            return Vector4<T>(*this) -= k;
        }
        // Scalar division
        Vector4<T>& operator/= (const T i) {
            x /= i;
            y /= i;
            z /= i;
            w /= i;
            return *this;
        }
        Vector4<T> operator/ (const T i) const {
            return Vector4<T>(*this) /= i;
        }
        // Scalar multiplication
        Vector4<T>& operator*= (const T i) {
            x *= i;
            y *= i;
            z *= i;
            w *= i;
            return *this;
        }
        Vector4<T> operator* (const T i) const {
            return Vector4<T>(*this) *= i;
        }
        // No cross multiplication for the Vector4 class
        // Dot multiplication
        template <class U>
        T operator^ (const Vector4<U>& k) const {
            return x*k[0] + y*k[1] + z*k[2] + w*k[3];
        }

        // Equality test
        bool operator== (const Vector4<T>& k) const {
            return ((x - k[0] < EPSILON) && 
                    (y - k[1] < EPSILON) && 
                    (z - k[2] < EPSILON) && 
                    (w - k[3] < EPSILON));
        }
        // Inequality test
        bool operator!= (const Vector4<T>& k) const {
            return !(*this == k);
        }

        // Magnitude of the vector
        T len () const {
            return sqrt((x*x) + (y*y) + (z*z) + (w*w));
        }
        // Normalizes the vector
        Vector4<T> normalize () {
            if (len() == 0) return (*this);
            return (*this) /= len();
        }

        inline std::ostream& streamOut(std::ostream& out)
        {
            out << "Vector4(" << x << "," << y << "," << z << "," << w << ")";
            return out;
        }
	};

    template <class T>
    inline std::ostream& operator<< (std::ostream &out, Vector4<T> &v) {
        return v.streamOut(out);
    }

}

#endif

