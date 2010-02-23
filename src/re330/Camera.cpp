#include "Camera.h"

namespace RE330 {
    void Camera::updateViewMatrix() {
        Vector3 a,b,c,d;  
        c = (e-l).asVector3().normalize();
        // x_c is the cross product of up and z_c, normalized
        a = (u.asVector3() * c).normalize();
        // y_c is the cross product of z_c and x_c, normalized
        b = (c * a).normalize();   
        // The translation column
        d = e.asVector3() * -1; 

        // Finally, change the view matrix
        v = Matrix4(a[0], a[1], a[2], 0,
                    b[0], b[1], b[2], 0,
                    c[0], c[1], c[2], 0,
                    0, 0, 0, 1) *
            Matrix4(1, 0, 0, d[0],
                    0, 1, 0, d[1],
                    0, 0, 1, d[2],
                    0, 0, 0, 1);
        std::cout << v << std::endl;
    }
}
