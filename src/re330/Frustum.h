#ifndef __Frustum_h__
#define __Frustum_h__

#include "RE330_global.h"
#include "Matrix4.h"

namespace RE330
{
    /** This class defines a view frustum.
    @remarks
        The default view frustum has an aspect ratio of one and a field
        of view of about 45 degrees. The near and far clip planes are
        at 1 and 101 respectively.
    */
    class RE330_EXPORT Frustum
    {
    protected:
        Matrix4<float> p;
        float near, far, aspect, fov;

    public:
        friend std::ostream& operator<< (std::ostream &out, Frustum &f);

        Frustum() :	p(2,0,0,0, 0,2,0,0, 0,0,-1.02,-2.02, 0,0,-1.f,0.f),
            near(1), far(101), aspect(1), fov(M_PI/4) {};

        const Matrix4<float> &getProjectionMatrix() const { return p; }

        void updateProjectionMatrix();

        // Mutators for the Frustrum parameters
        void setNearPlane(float n)
        {
            near = n;
            updateProjectionMatrix();
        }

        void setFarPlane(float f)
        {
            far = f;
            updateProjectionMatrix();
        }

        void setAspectRatio(float r)
        {
            aspect = r;
            updateProjectionMatrix();
        }

        void setVerticalFOV(float t)
        {
            fov = t;
            updateProjectionMatrix();
        }

        // Change the projection matrix in one fell swoop
        void createProjectionMatrix(float n, float f, float r, float t)
        {
            near = n; far = f; aspect = r; fov = t; updateProjectionMatrix();
        }
    };

    inline std::ostream& operator<< (std::ostream &out, Frustum &f) {
        out << "Frustum("
            << f.p << ","
            << f.near << ","
            << f.far << ","
            << f.aspect << ","
            << f.fov << ")";
        return out;
    }

}

#endif
