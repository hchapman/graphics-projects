#ifndef __Vector4_h__
#define __Vector4_h__

#include "RE330_global.h"
#include <math.h>

namespace RE330 
{
	class RE330_EXPORT Vector4
	{
	private:
		float x, y, z, w;
    public:
        // Constructors and assignment
        Vector4() : x(0), y(0), z(0), w(0) {}
        Vector4(float a, float b, float c, float d) 
            : x(a), y(b), z(c), w(d) {}
        Vector4(float *k) 
        { Vector4(k[0], k[1], k[2], k[3]); }
        float operator[] (int i) const {return (&x)[i];}
        Vector4& operator= (const Vector4& k) {
            x = k[0];
            y = k[1];
            z = k[2];
            w = k[3];
            return *this;
        }
        Vector4& operator= (const float* k) {
            x = k[0];
            y = k[1];
            z = k[2];
            w = k[3];
            return *this;
        }
        Vector4(const Vector4& k) {(*this) = k;}

        // Overloaded operators
        // Addition
        Vector4& operator+= (const Vector4& k) {
            x += k[0];
            y += k[1];
            z += k[2];
            w += k[3];
            return *this;
        }
        Vector4& operator+ (const Vector4& k) const {
            return Vector4(*this) += k;
        }
        // Subtraction
        Vector4& operator-= (const Vector4& k) {
            x -= k[0];
            y -= k[1];
            z -= k[2];
            w -= k[3];
            return *this;
        }
        Vector4& operator- (const Vector4& k) const {
            return Vector4(*this) -= k;
        }
        // Scalar division
        Vector4& operator/= (const float i) {
            x /= i;
            y /= i;
            z /= i;
            w /= i;
            return *this;
        }
        Vector4& operator/ (const float i) const {
            return Vector4(*this) /= i;
        }
        // Scalar multiplication
        Vector4& operator*= (const float i) {
            x *= i;
            y *= i;
            z *= i;
            w *= i;
            return *this;
        }
        Vector4& operator* (const float i) const {
            return Vector4(*this) *= i;
        }
        // No cross multiplication for the Vector4 class
        // Dot multiplication
        float operator^ (const Vector4& k) const {
            return x*k[0] + y*k[1] + z*k[2] + w*k[3];
        }
        // Equality test
        bool operator== (const Vector4& k) const {
            return ((x - k[0] < EPSILON) && (y - k[1] < EPSILON) && (z - k[2] < EPSILON) && (w - k[3] < EPSILON));
        }
        // Inequality test
        bool operator!= (const Vector4& k) const {
            return !(*this == k);
        }

        // Magnitude of the vector
        float len () const {
            return sqrt((x*x) + (y*y) + (z*z) + (w*w));
        }
        // Normalizes the vector
        Vector4& normalize () {
            if (len() == 0) return (*this);
            return (*this) /= len();
        }
	};

}

#endif

