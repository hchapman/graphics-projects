#ifndef __Camera_h__
#define __Camera_h__

#include "RE330_global.h"
#include "Frustum.h"
#include "Matrix4.h"
#include <iostream>

namespace RE330
{
    /** Camera specification.
    @remarks
        This class specifies a camera. The default camera is located at
        (0,0,10) in world coordinates and looks towards (0,0,0).
    */
    class RE330_EXPORT Camera : public Frustum
    {
    private:
        Matrix4 v;               // The View Matrix
        Vector4 e;               // The Center of Projection
        Vector4 u;               // The Up Vector
        Vector4 l;               // The Look at Point

    public:
        friend std::ostream& operator<< (std::ostream &out, Camera &c);

        Camera() : v(1,0,0,0, 0,1,0,0, 0,0,1,-10, 0,0,0,1),
            e(0,0,10,1), u(0,1,0,0), l(0,0,0,1) {};

        // Accessor Methods
        const Matrix4 &getViewMatrix() const { return v; }
        const Vector4 &getCenter() const { return e; }
        const Vector4 &getUpVector() const { return u; }
        const Vector4 &getLookPoint() const { return l; }

        // Updates the view matrix from member vectors.
        void updateViewMatrix();

        // Mutator Methods
        void setCenter(Vector4 center)   { e = center; updateViewMatrix(); }
        void setUpVector(Vector4 up)     { u = up;     updateViewMatrix(); }
        void setLookPoint(Vector4 point) { l = point;  updateViewMatrix(); }

        // Creates a view matrix from a center of projection,
        // a look at point, and an up vector.
        void createViewMatrix(Vector4 center, Vector4 point, Vector4 up)
        { e = center; l = point; u = up; updateViewMatrix(); }

    };

    inline std::ostream& operator<< (std::ostream &out, Camera &c) {
        out << "Camera("
            << c.v << ","
            << c.e << ","
            << c.u << ","
            << c.l << "):" << (Frustum&)c ;
        return out;
    }
}

#endif
