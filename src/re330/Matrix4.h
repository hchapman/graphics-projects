#ifndef __Matrix4__
#define __Matrix4__

#include <assert.h>
#include "Vector4.h"
#include "Vector3.h"

namespace RE330
{
    /** Matrix class for 4x4 matrices.
	*/
    class RE330_EXPORT Matrix4
    {
    protected:
        union {
            float m[4][4];
            float _m[16];
        };

    public:
        static inline Matrix4 rotationX(float angle);
        static inline Matrix4 rotationY(float angle);
        static inline Matrix4 rotationZ(float angle);
        static inline Matrix4 rotationA(Vector3 axis, float angle);
        static inline Matrix4 scaling(float x, float y, float z);
        static inline Matrix4 scaling(Vector3 axis) {
            return scaling(axis[0], axis[1], axis[2], angle);
        }
		static const Matrix4 IDENTITY;

        inline Matrix4()
        {}
        inline Matrix4(
            float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33 )
        {
            m[0][0] = m00;
            m[0][1] = m01;
            m[0][2] = m02;
            m[0][3] = m03;
            m[1][0] = m10;
            m[1][1] = m11;
            m[1][2] = m12;
            m[1][3] = m13;
            m[2][0] = m20;
            m[2][1] = m21;
            m[2][2] = m22;
            m[2][3] = m23;
            m[3][0] = m30;
            m[3][1] = m31;
            m[3][2] = m32;
            m[3][3] = m33;
        }
        inline Matrix4 (const float *m) {
            Matrix4(m[0],m[1],m[2],m[3],
                    m[4],m[5],m[6],m[7],
                    m[8],m[9],m[10],m[11],
                    m[12],m[13],m[14],m[15]);
        }
        inline Matrix4 (const float **m) {
            Matrix4(m[0][0],m[0][1],m[0][2],m[0][3],
                    m[1][0],m[1][1],m[1][2],m[1][3],
                    m[2][0],m[2][1],m[2][2],m[2][3],
                    m[3][0],m[3][1],m[3][2],m[3][3]);
        }
        inline Matrix4& operator= (const Matrix4& n) {
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
        inline Matrix4(const Matrix4& n) {(*this) = n;}

        inline Matrix4 operator* (const Matrix4 &m2) const {
            Matrix4 n = Matrix4();
			n.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
			n.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
			n.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
			n.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

			n.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
			n.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
			n.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
			n.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

			n.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
			n.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
			n.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
			n.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

			n.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
			n.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
			n.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
			n.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];
            return n;
        }

        inline Matrix4& operator*= (const Matrix4 &m2) {
            return *this = *this * m2;
        }

        inline Vector4 operator*(const Vector4& k) const {
            return Vector4(m[0][0]*k[0] + m[0][1]*k[1] + m[0][2]*k[2] + m[0][3]*k[3],
                           m[1][0]*k[0] + m[1][1]*k[1] + m[1][2]*k[2] + m[1][3]*k[3],
                           m[2][0]*k[0] + m[2][1]*k[1] + m[2][2]*k[2] + m[2][3]*k[3],
                           m[3][0]*k[0] + m[3][1]*k[1] + m[3][2]*k[2] + m[3][3]*k[3]);
        }

		inline Matrix4 transpose() const
		{
			Matrix4 r;
			for(int i=0; i<4; i++) 
			{
				for(int j=0; j<4; j++)
				{
					r._m[i*4+j]=_m[j*4+i];
				}
			}
			return r;
		}

		inline float* getElementPointer()
		{
			return static_cast<float *>(_m);
		}
	};
	
}

#endif

