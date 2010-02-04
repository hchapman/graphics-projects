#ifndef __Camera_h__
#define __Camera_h__

#include "RE330_global.h"
#include "Frustum.h"
#include "Matrix4.h"

namespace RE330 
{
	/** Camera specification.
	@remarks
		This class specifies a camera. The default camera is located at
		(0,0,10) in world coordinates and looks towards (0,0,0).
	*/
	class RE330_EXPORT Camera : public Frustum
	{
	public:
		Camera() : v(1,0,0,0, 0,1,0,0, 0,0,1,-10, 0,0,0,1) {};
		const Matrix4 &getViewMatrix() const { return v; }

	private:
		Matrix4 v;
	};
	
}

#endif

