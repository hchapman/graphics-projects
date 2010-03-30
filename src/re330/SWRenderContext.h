#ifndef __SWRenderContext_h__
#define __SWRenderContext_h__


#include "RE330_global.h"
#include "RenderContext.h"
#include "VertexDeclaration.h"
#include "Matrix4.h"

namespace RE330 {

	/** This class implements the abstract base class RenderContext and
		it is the main interface to the low-level graphics API.
	@remarks
		This class also instantiates the Singleton class to make sure there is
		only one instance at any time.
	*/

	class SWWidget;
		
	class RE330_EXPORT SWRenderContext : public RenderContext
	{
	public:
		void init();
		void setViewport(int width, int height);
		void beginFrame();
		void endFrame();
		void setModelViewMatrix(const Matrix4<float> &m);
		void setProjectionMatrix(const Matrix4<float> &m);
		/** This is the main method for rendering objects. Note that it uses
			OpenSW vertex arrays. See the OpenSW 2.0 book for details.
		*/
		void render(Object *object);

		/** Set the widget that will receive rendering output.
		*/
		void setWidget(SWWidget *swWidget);

		static SWRenderContext& getSingleton(void);
        static SWRenderContext* getSingletonPtr(void);

	private:
		SWWidget *mswWidget;
		QImage *image;
        int height, width;

        int frames_rendered;
        clock_t start_time;

        float *zBuffer;

		Matrix4<float> projection;
		Matrix4<float> modelview;
		Matrix4<float> viewport;
		Matrix4<float> total;

		void rasterizeTriangle(float p[3][4], float n[3][3], float c[3][4]);

        // Specific types of rasterization for each part of the project
        void rasterizeVertices(float p[3][4], float n[3][3], float c[3][4]);
        void rasterize(float p[3][4], float n[3][3], float c[3][4], int type);

        // Set a pixel (with z-buffering)
        void setPixel(int x, int y, float z, uint color);

        // What we should do when we rasterize
        enum rasterize_type_t {
            VERTICES_ONLY,
            BASIC_ONLY,
            PERSPECTIVE_CORRECT,
            TWO_LEVEL
        };

        // The stage of rasterization we should use. Types are above in the enum
        static const rasterize_type_t RASTERIZE_METHOD = TWO_LEVEL;
        // Number of subdivisions to use in TWO_LEVEL
        static const int SUBDIV = 3;

        // Number of frames to count before averaging and printing the FPS
        static const int FRAMECOUNT = 60;
	};

}

#endif
