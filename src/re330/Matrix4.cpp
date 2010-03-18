#include "Matrix4.h"
#include "BasicMath.h"

using namespace RE330;

template <class T>
const Matrix4<T> Matrix4<T>::IDENTITY(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 );

template <>
const Matrix4<float> Matrix4<float>::IDENTITY(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 );
   
