#include "SWRenderContext.h"
#include "SWWidget.h"

using namespace RE330;

SWRenderContext* SWRenderContext::getSingletonPtr(void)
{
	return static_cast<SWRenderContext *>(ms_Singleton);
}

SWRenderContext& SWRenderContext::getSingleton(void)
{  
	assert( ms_Singleton );  
	return ( *(static_cast<SWRenderContext *>(ms_Singleton)) );  
}

void SWRenderContext::init()
{
	modelview = Matrix4::IDENTITY;
	projection = Matrix4::IDENTITY;
	viewport = Matrix4::IDENTITY;
	total = Matrix4::IDENTITY;
}

void SWRenderContext::setViewport(int width, int height)
{
	viewport = Matrix4(width/2.f, 0, 0, width/2.f,
                       0, -height/2.f, 0, height/2.f,
                       0, 0, .5f, .5f,
                       0, 0, 0, 1);
}

void SWRenderContext::beginFrame()
{
	image->fill(qRgb(0,0,0));
}

void SWRenderContext::endFrame()
{
	mswWidget->setPixmap(QPixmap::fromImage(*image));
	mswWidget->repaint();
}

void SWRenderContext::setModelViewMatrix(const Matrix4 &m)
{
	modelview = m;
}

void SWRenderContext::setProjectionMatrix(const Matrix4 &m)
{
	projection = m;
}

void SWRenderContext::render(Object *object)
{
	VertexData& vertexData = object->vertexData;
	VertexDeclaration& vertexDeclaration = vertexData.vertexDeclaration;
	VertexBufferBinding& vertexBufferBinding = vertexData.vertexBufferBinding;

	unsigned char *pPtr;	// Pointer to vertex position
	int pStr;				// Stride, i.e., number of bytes between vertex entries
	int pSze;				// Size, i.e., number of vector elements per vertex position (typically 3)

	unsigned char *nPtr;
	int nStr;
	int nSze;

	unsigned char *cPtr;
	int cStr;
	int cSze;

	pPtr = 0;	// Disable all arrays per default
	nPtr = 0;
	cPtr = 0;

	// Set up arrays
	for(int j=0; j<vertexDeclaration.getElementCount(); j++)
	{
		const VertexElement *element = vertexDeclaration.getElement(j);

		const VertexBuffer& vertexBuffer = vertexBufferBinding.getBuffer(element->getBufferIndex());
		unsigned char* buf = vertexBuffer.getBuffer();

		int vertexStride = static_cast<int>(element->getStride());
		int vertexSize = static_cast<int>(element->getSize());
		int offset = element->getOffset();

		switch(element->getSemantic())
		{
			case VES_POSITION :
				pPtr = buf+offset;
				pStr = vertexStride;
				pSze = vertexSize;
				break;
			case VES_NORMAL :
				nPtr = buf+offset;
				nStr = vertexStride;
				nSze = 3;	// always
				break;
			case VES_DIFFUSE :
				cPtr = buf+offset;
				cStr = vertexStride;
				cSze = vertexSize;
				break;
		}
	}
	
	// Draw
	float p[3][4];	// Triangle vertex positions
	float c[3][4];	// Triangle vertex colors
	float n[3][3];	// Triangle vertex normals

	int *iPtr = vertexData.getIndexBuffer();

	for(int i=0; i<vertexData.getIndexCount(); i++)
	{
		// Local index of current triangle vertex
		int k = i%3;

		// Set default values
		p[k][0] = p[k][1] = p[k][2] = 0; 
		p[k][3] = 1.f;
		c[k][0] = c[k][1] = c[k][2] = 1.f;
		c[k][3] = 0;
		n[k][0] = n[k][1] = 0;
		n[k][2] = 1;

		// Vertex position
		if(pPtr)
		{
			float *cur = (float*)(pPtr + pStr*iPtr[i]);
			for(int s=0; s<pSze; s++)
			{
				p[k][s] = *cur;
				cur++;
			}
		}

		// Vertex normal
		if(nPtr)
		{
			float *cur = (float*)(nPtr + nStr*iPtr[i]);
			for(int s=0; s<nSze; s++)
			{
				n[k][s] = *cur;
				cur++;
			}
		}

		// Vertex color
		if(cPtr)
		{
			float *cur = (float*)(cPtr + cStr*iPtr[i]);
			for(int s=0; s<cSze; s++)
			{
				c[k][s] = *cur;
				cur++;
			}
		}

		// Draw triangle
		if(i%3 == 2)
		{
			rasterizeTriangle(p, n, c);
		}
	}
}

inline void SWRenderContext::rasterizeTriangle(float p[3][4], 
                                               float n[3][3], 
                                               float c[3][4])
{
	// Implement triangle rasterization here.
	// Use viewport*projection*modelview matrix to project vertices to screen.
	// You can draw pixels in the output image using image->setPixel(...);

    switch(RASTERIZE_METHOD) {
    case VERTICES_ONLY:
        rasterizeVertices(p, n, c);
        break;
    case BASIC_ONLY:
        rasterizeBasic(p, n, c);
        break;
    }
}

void SWRenderContext::rasterizeVertices(float p[3][4], 
                                        float n[3][3], 
                                        float c[3][4]) {
    // Object point, unit view pixel, screen pixel
    Vector4 point, upixel, pixel;
        
    // Color vector
    Vector4 color;

    // Loop through each vertex, and draw them
    for (int vertex = 0; vertex < 3; vertex++) {
        point = Vector4(p[vertex]);

        upixel = projection * (modelview * point);
        upixel /= upixel[3];

        // Ensure that the point is within the view cube
        if ((upixel[0] < 1 && upixel[0] > -1) &&
            (upixel[1] < 1 && upixel[1] > -1) &&
            (upixel[2] < 1 && upixel[2] > -1)) {

            pixel = viewport * upixel;

            color = Vector4(c[vertex]) * 255;

            image->setPixel(pixel[0] / pixel[3],
                            pixel[1] / pixel[3],
                            qRgba((int)(color[0]),
                                  (int)(color[1]),
                                  (int)(color[2]),
                                  (int)(color[3])));
        }
    }   
}

void SWRenderContext::rasterizeBasic(float p[3][4], 
                                     float n[3][3], 
                                     float c[3][4]) {
    // Pixels and colors of the triangle vertices
    Vector4 *pixels = new Vector4[3];
    Vector4 *colors = new Vector4[3];

    // Calculate the max width (this is kind of redundant)
    Vector4 limit = viewport * Vector4(1,-1,0,0);

    // Loop through each vertex, and find their pixel value
    for (int vertex = 0; vertex < 3; vertex++) {
        pixels[vertex] = viewport * (projection * 
                                    (modelview * Vector4(p[vertex])));
        pixels[vertex] /= pixels[vertex][3];
    }
    
    // Bounding box boundaries
    int bbX1, bbY1, bbX2, bbY2;
    
    // Find the location of the bounding box based on the pixels
    bbX1 = min(pixels[0][0], min(pixels[1][0], pixels[2][0]));
    bbY1 = min(pixels[0][1], min(pixels[1][1], pixels[2][1]));
    bbX2 = max(pixels[0][0], max(pixels[1][0], pixels[2][0]));
    bbY2 = max(pixels[0][1], max(pixels[1][1], pixels[2][1]));

    // Clip the bounding box to the screen
    bbX1 = max(0, bbX1); bbX2 = min(limit[0], bbX2);
    bbY1 = max(0, bbX2); bbY2 = min(limit[1], bbY2);

    // Loop through all pixels in the box
    for (int x = bbX1; x <= bbX2; x++) {
        for (int y = bbY1; y <= bbY2; y++) {
            // Calculate the barycentric coordinates of the pixel
            float alpha, beta, gamma;
            
        }
    }
}

void SWRenderContext::setWidget(SWWidget *swWidget)
{
	mswWidget = swWidget;
	image = mswWidget->getImage();
}
