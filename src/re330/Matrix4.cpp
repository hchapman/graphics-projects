#include "Matrix4.h"
#include "BasicMath.h"

using namespace RE330;

const Matrix4 Matrix4::IDENTITY(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 );

inline Matrix4 rotationX(float a) {
    return Matrix4(
        1, 0, 0, 0, 
        0, cos(a), -sin(a), 0,
        0, sin(a), cos(a), 0,
        0, 0, 0, 1);
}

inline Matrix4 rotationY(float a) {
    return Matrix4(
        cos(a), 0, sin(a), 0,
        0, 1, 0, 0,
        -sin(a), 0, cos(a), 0,
        0, 0, 0, 1);
}

inline Matrix4 rotationZ(float a) {
    return Matrix4(
        cos(a), -sin(a), 0, 0,
        sin(a), cos(a), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}

inline Matrix4 rotationA(Vector3 axis, float a) {
    Vector3 n = axis;
    n.normalize();
    return Matrix4(
        n[0]*n[0] + cos(a) * (1 - n[0]*n[0]), n[0]*n[1] * (1 - cos(a)) - n[2] * sin(a), n[0]*n[2] * (1 - cos(a)) + n[1] * sin(a), 0,
        n[0] * n[1] * (1 - cos(a)) + n[2] * sin(a), n[1] * n[1] * cos(a) * (1 - n[1] * n[1]), n[1] * n[2] * (1 - cos(a)) - n[0] * sin(a), 0,
        n[0] * n[2] * (1 - cos(a)) - n[1] * sin(a), n[1] * n[2] * (1 - cos(a)) + n[0] * sin(a), n[2] * n[2] + cos(a) * (1 - n[2]*n[2]), 0,
        0, 0, 0, 1
    );
}

inline Matrix4 scaling(float x, float y, float z) {
    return Matrix4(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    );
}

inline Matrix4 translation(float x, float y, float z) {
    return Matrix4(
        0, 0, 0, x,
        0, 0, 0, y,
        0, 0, 0, z,
        0, 0, 0, 1
    );
}