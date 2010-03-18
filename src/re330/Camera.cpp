#include "Camera.h"

namespace RE330 {
    void Camera::updateViewMatrix() {

        Vector3<float> x_c, y_c, z_c, d;

        // difference btwn camera center(e) and look-at-point(l) normalized
        z_c = (e-l).asVector3().normalize();

        // x_c is the cross product of up and z_c, normalized
        x_c = (u.asVector3() * z_c).normalize();

        // y_c is the cross product of z_c and x_c, normalized
        y_c = (z_c * x_c).normalize();

        // The translation column
        d = e.asVector3() * -1;

        // Finally, change the view matrix
        v = Matrix4<float>(x_c[0], x_c[1], x_c[2], x_c^d,
                    y_c[0], y_c[1], y_c[2], y_c^d,
                    z_c[0], z_c[1], z_c[2], z_c^d,
                    0, 0, 0, 1);
    }
}
