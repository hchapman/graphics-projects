#include "Frustum.h"
#include <math.h>

namespace RE330
{

    void Frustum::updateProjectionMatrix() {
        std::cout << far << "," << near << " -> " << (far+near)/(far-near) << std::endl;
        p = Matrix4(1/(aspect*tan(fov/2)), 0, 0, 0,
                    0, 1/(tan(fov/2)), 0, 0,
                    0, 0, -(far+near)/(far-near), -(2*far*near)/(far-near),
                    0, 0, -1, 0);
    }

}
