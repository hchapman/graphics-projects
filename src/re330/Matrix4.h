#ifndef __Matrix4__
#define __Matrix4__

#include <assert.h>
#include <iostream>
#include "Vector4.h"
#include "Vector3.h"

namespace RE330
{
    /** Matrix class for 4x4 matrices.
    */
    template <class T = float>
    class RE330_EXPORT Matrix4
    {
    protected:
        union {
            T m[4][4];
            T _m[16];
        };

    public:
        static inline Matrix4<T> rotateX(T a)
        {
            return Matrix4<T>(
                1, 0, 0, 0,
                0, cos(a), -sin(a), 0,
                0, sin(a), cos(a), 0,
                0, 0, 0, 1);
        }

        static inline Matrix4<T> rotateY(T a)
        {
            return Matrix4<T>(
                cos(a), 0, sin(a), 0,
                0, 1, 0, 0,
                -sin(a), 0, cos(a), 0,
                0, 0, 0, 1);
        }

        static inline Matrix4<T> rotateZ(T a)
        {
            return Matrix4<T>(
                cos(a), -sin(a), 0, 0,
                sin(a), cos(a), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);
        }

        static inline Matrix4<T> rotateA(Vector3<T> axis, T a)
        {
            Vector3<T> n = axis;
            n.normalize();
            return Matrix4<T>(
                n[0]*n[0] + cos(a) * (1 - n[0]*n[0]),
                n[0]*n[1] * (1 - cos(a)) - n[2] * sin(a),
                n[0]*n[2] * (1 - cos(a)) + n[1] * sin(a),
                0,

                n[0] * n[1] * (1 - cos(a)) + n[2] * sin(a),
                n[1] * n[1] + cos(a) * (1 - n[1] * n[1]),
                n[1] * n[2] * (1 - cos(a)) - n[0] * sin(a),
                0,

                n[0] * n[2] * (1 - cos(a)) - n[1] * sin(a),
                n[1] * n[2] * (1 - cos(a)) + n[0] * sin(a),
                n[2] * n[2] + cos(a) * (1 - n[2]*n[2]),
                0,

                0, 0, 0, 1
                );
        }

        static inline Matrix4<T> scale(T x, T y, T z)
        {
            return Matrix4<T>(
                x, 0, 0, 0,
                0, y, 0, 0,
                0, 0, z, 0,
                0, 0, 0, 1
                );
        }

        static inline Matrix4<T> scale(Vector3<T> axis)
        {
            return scale(axis[0], axis[1], axis[2]);
        }

        static inline Matrix4<T> translate(T x, T y, T z)
        {
            return Matrix4<T>(
                1, 0, 0, x,
                0, 1, 0, y,
                0, 0, 1, z,
                0, 0, 0, 1
                );
        }

        static inline Matrix4<T> translate(Vector3<T> v)
        {
            return translate(v[0], v[1], v[2]);
        }

        static const Matrix4<T> IDENTITY;

        inline Matrix4<T>() {}

        inline Matrix4<T>(
            T m00, T m01, T m02, T m03,
            T m10, T m11, T m12, T m13,
            T m20, T m21, T m22, T m23,
            T m30, T m31, T m32, T m33 )
        {
            _m[0] = m00;
            _m[1] = m01;
            _m[2] = m02;
            _m[3] = m03;
            _m[4] = m10;
            _m[5] = m11;
            _m[6] = m12;
            _m[7] = m13;
            _m[8] = m20;
            _m[9] = m21;
            _m[10] = m22;
            _m[11] = m23;
            _m[12] = m30;
            _m[13] = m31;
            _m[14] = m32;
            _m[15] = m33;
        }

        inline Matrix4<T> (const T *m)
        {
            Matrix4<T>(m[0], m[1], m[2], m[3],
                       m[4], m[5], m[6], m[7],
                       m[8], m[9], m[10], m[11],
                       m[12], m[13], m[14], m[15]);
        }

        inline Matrix4<T> (const T **m)
        {
            Matrix4<T>(m[0][0], m[0][1], m[0][2], m[0][3],
                       m[1][0], m[1][1], m[1][2], m[1][3],
                       m[2][0], m[2][1], m[2][2], m[2][3],
                       m[3][0], m[3][1], m[3][2], m[3][3]);
        }
        
        // Construct a new Matrix4<T> from 4 column Vector4<T>s
        inline Matrix4<T> (const Vector4<T> a, const Vector4<T> b,
                           const Vector4<T> c, const Vector4<T> d)
        {
            Matrix4<T>(a[0], b[0], c[0], d[0],
                       a[1], b[1], c[1], d[1],
                       a[2], b[2], c[2], d[2],
                       a[3], b[3], c[3], d[3]);
        }
        
        inline Matrix4<T>& operator= (const Matrix4<T>& n)
        {
            m[0][0] = n.m[0][0];
            m[0][1] = n.m[0][1];
            m[0][2] = n.m[0][2];
            m[0][3] = n.m[0][3];
            m[1][0] = n.m[1][0];
            m[1][1] = n.m[1][1];
            m[1][2] = n.m[1][2];
            m[1][3] = n.m[1][3];
            m[2][0] = n.m[2][0];
            m[2][1] = n.m[2][1];
            m[2][2] = n.m[2][2];
            m[2][3] = n.m[2][3];
            m[3][0] = n.m[3][0];
            m[3][1] = n.m[3][1];
            m[3][2] = n.m[3][2];
            m[3][3] = n.m[3][3];
            return *this;
        }

        inline Matrix4<T>(const Matrix4<T>& n) { (*this) = n; }
        
        inline Matrix4<T> operator* (const Matrix4<T> &m2) const
        {
            Matrix4<T> n = Matrix4<T>();
            n.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] +
                m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
            n.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] +
                m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
            n.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] +
                m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
            n.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] +
                m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

            n.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] +
                m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
            n.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] +
                m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
            n.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] +
                m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
            n.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] +
                m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

            n.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] +
                m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
            n.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] +
                m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
            n.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] +
                m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
            n.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] +
                m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

            n.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] +
                m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
            n.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] +
                m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
            n.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] +
                m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
            n.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] +
                m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];
            return n;
        }

        inline Matrix4<T>& operator*= (const Matrix4<T> &m2)
        {
            return *this = *this * m2;
        }

        inline Vector4<T> operator*(const Vector4<T>& k) const
        {
            return Vector4<T>(m[0][0]*k[0] + m[0][1]*k[1] + 
                              m[0][2]*k[2] + m[0][3]*k[3],

                              m[1][0]*k[0] + m[1][1]*k[1] + 
                              m[1][2]*k[2] + m[1][3]*k[3],

                              m[2][0]*k[0] + m[2][1]*k[1] + 
                              m[2][2]*k[2] + m[2][3]*k[3],

                              m[3][0]*k[0] + m[3][1]*k[1] + 
                              m[3][2]*k[2] + m[3][3]*k[3]);
        }

        inline T operator[] (int i) const { return _m[i]; }

        inline Matrix4<T> transpose() const
        {
            Matrix4<T> r;
            for(int i=0; i<4; i++)
            {
                for(int j=0; j<4; j++)
                {
                    r._m[i*4+j]=_m[j*4+i];
                }
            }
            return r;
        }

        inline T* getElementPointer()
        {
            return static_cast<T *>(_m);
        }

        inline std::ostream& streamOut (std::ostream &out) {
            out << "Matrix4(";
            for (int i = 0; i < 4; i++) {
                out << "(";
                for (int j = 0; j < 4; j++)
                    out << m[i][j] << ((j == 3) ? ")" : ",");
                out << ((i==3) ? ")" : ",");
            }
            return out;
        }
    };

    template <class T>
    inline std::ostream& operator<< (std::ostream &out, Matrix4<T> &m)
    {
        return m.streamOut(out);
    }

}

#endif
