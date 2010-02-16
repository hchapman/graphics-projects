#ifndef __Vector3_h__
#define __Vector3_h__

#include "RE330_global.h"
#include <math.h>

namespace RE330 
{
	class RE330_EXPORT Vector3
	{
	private:
		float x, y, z;
    public:
        // Constructors and assignment
        Vector3() : x(0), y(0), z(0) {}
        Vector3(float a, float b, float c) : x(a), y(b), z(c) {}
        float operator[] (int i) const {return (&x)[i];}
        Vector3& operator= (const Vector3& k) {
            x = k[0];
            y = k[1];
            z = k[2];
            return *this;
        }
        Vector3& operator= (const float* k) {
            x = k[0];
            y = k[1];
            z = k[2];
            return *this;
        }
        Vector3(const Vector3& k) {(*this) = k;}

        // Overloaded operators
        // Addition
        Vector3& operator+= (const Vector3& k) {
            x += k[0];
            y += k[1];
            z += k[2];
            return *this;
        }
        Vector3& operator+ (const Vector3& k) const {
            return Vector3(*this) += k;
        }
        // Subtraction
        Vector3& operator-= (const Vector3& k) {
            x -= k[0];
            y -= k[1];
            z -= k[2];
            return *this;
        }
        Vector3& operator- (const Vector3& k) const {
            return Vector3(*this) -= k;
        }
        // Scalar division
        Vector3& operator/= (const float i) {
            x /= i;
            y /= i; 
            z /= i;
            return *this;
        }
        Vector3& operator/ (const float i) const {
            return Vector3(*this) /= i;
        }
        // Scalar multiplication
        Vector3& operator*= (const float i) {
            x *= i;
            y *= i; 
            z *= i;
            return *this;
        }
        Vector3& operator* (const float i) const {
            return Vector3(*this) *= i;
        }
        // Cross multiplication
        Vector3& operator*= (const Vector3& k) {
            x = y*k[2] - z*k[1];
            y = z*k[0] - x*k[2];
            z = x*k[1] - y*k[0];
            return *this;
        }
        Vector3& operator* (const Vector3& k) const {
            return Vector3(*this) *= k;
        }
        // Dot multiplication
        float operator^ (const Vector3& k) const {
            return x*k[0] + y*k[1] + z*k[2];
        }
        // Equality test within epsilon
        bool operator== (const Vector3& k) const {
            return ((x - k[0] < EPSILON) && (y - k[1] < EPSILON) && (z - k[2] < EPSILON));
        }
        // Inequality test
        bool operator!= (const Vector3& k) const {
            return !(*this == k);
        }

        // Magnitude of the vector
        float len () const {
            return sqrt((x*x) + (y*y) + (z*z));
        }
        // Normalizes the vector
        Vector3& normalize () {
            if (len() == 0) return (*this);
            return (*this) *= (1/(len()));
        }
	};

}

#endif

