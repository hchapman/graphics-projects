#include "Camera.h"

namespace RE330 {
    void Camera::updateViewMatrix() {
          
        Vector4 c = (e-p).normalize(); // The z_c column vector
        // x_c is the cross product of up and z_c, normalized
        Vector4 a = Vector4(u.asVector3() * c.asVector3(), 0).normalize();
        // y_c is the cross product of z_c and x_c, normalized
        Vector4 b = Vector4(c.asVector3() * a.asVector3(), 0).normalize();   
        // The translation column
        Vector4 d = Vector4(-e[0], -e[1], -e[2], 1); 
        
        Vector3 tmp =  b.asVector3()*c.asVector3();
        std::cout << tmp << std::endl;
        std::cout << a << b << c << d << std::endl;

        // Finally, change the view matrix
        v = Matrix4(a[0], b[0], c[0], d[0],
                    a[1], b[1], c[1], d[1],
                    a[2], b[2], c[2], d[2],
                    a[3], b[3], c[3], d[3]);
        // v = Matrix4(a[0], b[0], c[0], 0,
        //             a[1], b[1], c[1], 0,
        //             a[2], b[2], c[2], 0,
        //             a[3], b[3], c[3], 1) *
        //     Matrix4(1,0,0,d[0],
        //             0,1,0,d[1],
        //             0,0,1,d[2],
        //             0,0,0,1);
        std::cout << v << std::endl;
    }
}
