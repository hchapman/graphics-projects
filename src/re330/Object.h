#ifndef __Object_h__
#define __Object_h__

#include "RE330_global.h"
#include "VertexData.h"
#include "Matrix4.h"

namespace RE330
{
    /** This class provides an abstraction of scene objects.
    @remarks
        The SceneManager creates and deletes objects. Vertex data is stored
        in the VertexData member.
    */
    class RE330_EXPORT Object
    {
    public:
        inline void setTransformation(const Matrix4<float> &t) { mTransformation = t; }

        inline Matrix4<float> getTransformation() const { return mTransformation; }

        void printTransformation()
        {
            printf("(%f %f %f %f)\n", mTransformation[0], mTransformation[1],
                   mTransformation[2], mTransformation[3]);
            printf("(%f %f %f %f)\n", mTransformation[4], mTransformation[5],
                   mTransformation[6], mTransformation[7]);
            printf("(%f %f %f %f)\n", mTransformation[8], mTransformation[9],
                   mTransformation[10], mTransformation[11]);
            printf("(%f %f %f %f)\n", mTransformation[12], mTransformation[13],
                   mTransformation[14], mTransformation[15]);
            fflush(NULL);
        }

        VertexData vertexData;

    protected:

        Object() { mTransformation = Matrix4<float>::IDENTITY; }

        Matrix4<float> mTransformation;

        friend class SceneManager;
    };

}

#endif
