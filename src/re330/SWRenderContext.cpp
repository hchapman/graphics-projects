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
	modelview = Matrix4<float>::IDENTITY;
	projection = Matrix4<float>::IDENTITY;
	viewport = Matrix4<float>::IDENTITY;
	total = Matrix4<float>::IDENTITY;
}

void SWRenderContext::setViewport(int w, int h)
{
    width = w, height = h;
	viewport = Matrix4<float>(width/2.f, 0, 0, width/2.f,
                       0, -height/2.f, 0, height/2.f,
                       0, 0, -.5f, .5f,
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

void SWRenderContext::setModelViewMatrix(const Matrix4<float> &m)
{
	modelview = m;
}

void SWRenderContext::setProjectionMatrix(const Matrix4<float> &m)
{
	projection = m;
}

void SWRenderContext::render(Object *object)
{
    delete zBuffer;
    zBuffer = new float[height * width];
    for (int i = height * width; i--; ) {
        zBuffer[i] = -1;
    }

    total = viewport * projection * modelview;
    
	VertexData& vertexData = object->vertexData;
	VertexDeclaration& vertexDeclaration = vertexData.vertexDeclaration;
	VertexBufferBinding& vertexBufferBinding = vertexData.vertexBufferBinding;

	unsigned char *pPtr;	// Pointer to vertex position
	int pStr;				// Stride, i.e., number of bytes between 
                            //   vertex entries
	int pSze;				// Size, i.e., number of vector elements 
                            //   per vertex position (typically 3)

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

		const VertexBuffer& vertexBuffer = vertexBufferBinding.getBuffer(
            element->getBufferIndex());
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
        //for(int i=0; i<3; i++)
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
        rasterize(p, n, c, BASIC_ONLY);
        break;
    case PERSPECTIVE_CORRECT:
        rasterize(p, n, c, PERSPECTIVE_CORRECT);
        break;
    case TWO_LEVEL:
        rasterize(p, n, c, TWO_LEVEL);
        break;
    }
}

void SWRenderContext::rasterizeVertices(float p[3][4], 
                                        float n[3][3], 
                                        float c[3][4]) {
    // Object point, unit view pixel, screen pixel
    Vector4<float> point, upixel, pixel;
    

        
    // Color vector
    Vector4<float> color;

    // Loop through each vertex, and draw them
    for (int vertex = 0; vertex < 3; vertex++) {
        point = Vector4<float>(p[vertex]);

        upixel = projection * (modelview * point);
        upixel /= upixel[3];

        // Ensure that the point is within the view cube
        if ((upixel[0] < 1 && upixel[0] > -1) &&
            (upixel[1] < 1 && upixel[1] > -1) &&
            (upixel[2] < 1 && upixel[2] > -1)) {

            pixel = viewport * upixel;

            color = Vector4<float>(c[vertex]) * 255;

            image->setPixel(pixel[0] / pixel[3],
                            pixel[1] / pixel[3],
                            qRgba(1,1,1,1));
        }
    }   
}

// Two quick utility methods that compute the linear interpolated and
// hyperbolic (perspective correctly) interpolated color of a pixel
inline uint linearColor(const Vector3<float> abc, 
                        const Vector3< Vector4<float> > colors) {
    Vector4<int> c = Vector4<int>(colors ^ abc);
    return qRgba(c[0], c[1], c[2], 1);
}

inline uint hyperbolicColor(const Vector3<float> abc,
                            const Vector3< Vector4<float> > colors,
                            const Vector3<float> homeo) {
    Vector4<int> c = Vector4<int>(
        colors ^ ((abc / homeo) / (abc / homeo).sum()));
    //Vector4<int> c = Vector4<int>( 
    //    ((colors.times(homeo.reciprocal()).times(abc)) / 
    //     (homeo.reciprocal().times(abc))).sum());
    return qRgba(c[0], c[1], c[2], 1);
    //Vector4<int> c = Vector4<int>(((colors ^ homeo.reciprocal()) ^ abc) /
    //                              (homeo.reciprocal() ^ abc));
}

void SWRenderContext::rasterize(float p[3][4], 
                                float n[3][3], 
                                float c[3][4],
                                int type) {
    // Pixels and colors of the triangle vertices
    //Vector4<float> *points = new Vector4<float>[3]; 
    Vector4<float> points[3]; // the pixels in 3-space
    Vector3<float> pixels[3]; // the pixels in 2-space
    Vector4<float> colors[3]; 

    // The homeogeneous coordinates of the vertices
    float homeo[3] = {0,0,0};

    // Loop through each vertex, and find their pixel value
    for (int vertex = 0; vertex < 3; vertex++) {
        points[vertex] = total * Vector4<float>(p[vertex]);

        // Store the homeogeneous coordinates for persp-correct
        homeo[vertex] = points[vertex][3];            

        points[vertex] /= points[vertex][3];
        pixels[vertex] = Vector3<float>(points[vertex][0], points[vertex][1], 1.f);

        colors[vertex] = Vector4<float>(c[vertex]) * 255;
    }
    
    // Bounding box boundaries
    int *bBox = new int[4];   // int[4]: (x1, y1), (x2, y2)
    
    // Find the location of the bounding box based on the points
    for (int i = 0; i < 4; i++)  {
        if ( i / 2 % 2 ) {
            bBox[i] = std::min(std::max(points[0][i%2], 
                              std::max(points[1][i%2], points[2][i%2])),
                               (float)(i%2 ? height : width) - 1);
        }
        else {
            bBox[i] = std::max(std::min(points[0][i%2], 
                              std::min(points[1][i%2], points[2][i%2])),
                          0.f);
        }
    }

    // Barycentric coordinate variables
    float alpha, beta, gamma;

    // The lines and norms of the edges in 2-space
    Vector3<float> lineAB, lineAC, normAB, normAC;
    // The lines of the edges in 3-space
    Vector4<float> lineAB3D, lineAC3D;

    // Determine the edges b-a, c-a of the triangle
    lineAB = pixels[1] - pixels[0];
    lineAC = pixels[2] - pixels[0];

    lineAB3D = points[1] - points[0];
    lineAC3D = points[2] - points[0];

    // Determine the normals to the edges b-a and c-a
    normAB = Vector3<float>(pixels[0][1] - pixels[1][1],
                            pixels[1][0] - pixels[0][0], 0.f);
    normAC = Vector3<float>(pixels[0][1] - pixels[2][1],
                            pixels[2][0] - pixels[0][0], 0.f);

    // The pixel in the box which we are currently processing
    Vector3<float> boxPixel;

    // Check if the triangle is degenerate (if it is, don't draw)
    if (lineAB ^ lineAC) {
        // Cull the triangle. If we would be drawing the inface, skip it instead
        if ((lineAB * lineAC)[2] > 0) {
            // We check this by using that the edges are counter-clockwise
            // along the outside face
            return;
        }

        // Determine which edges we should treat as "inside" the triangle.
        bool edgeInTriangle[3] = {false,false,false};
        Vector3<float> tempEdge;
        for (int i = 0; i < 3; i++) {
            tempEdge = pixels[(i+1)%3] - pixels[i];
            // Check if we are a left edge (slanting down) and inside
            if (tempEdge[1] < 0) {
                edgeInTriangle[i] = true;
            } else if (tempEdge[1] == 0) {
                // Check if we are the top edge, which is inside
                if (pixels[i][1] > pixels[(i+2)%3][1]) {
                    edgeInTriangle[i] = true;
                }
            }
        }

        if (type == 2+TWO_LEVEL) {
            // The triangle is in fact a triangle
            // Loop through all subdivisions of the box
            for (int sub = 0; sub < SUBDIV; sub++) {
                // Check the four corners of the box. If they are all outside 
                // the range of any given barycentric coord, ignore the box.
                //Vector3<float> subPixel

                float abc[4][3]; // Barycentric coordiantes of the corners
                
                // Loop through and calculate the barycentric coordinates
                for (int i = 0; i < 4; i++) {
                    beta = (normAC^(boxPixel - pixels[0])) / 
                        (normAC^lineAB);
                    gamma = (normAB^(boxPixel - pixels[0])) / 
                        (normAB^lineAC);

                    alpha = (1 - beta - gamma);
                }
                

            }
            for (int x = bBox[0]; x <= bBox[2]; x++) {
                for (int y = bBox[1]; y <= bBox[3]; y++) {
                    // Make a vector that is the pixel in the bounding box's center
                    boxPixel = Vector3<float>(x + 0.5, y + 0.5, 1);

                    // Calculate the barycentric coordinates of the pixel
                    beta = (normAC^(boxPixel - pixels[0])) / 
                        (normAC^lineAB);
                    gamma = (normAB^(boxPixel - pixels[0])) / 
                        (normAB^lineAC);

                    alpha = (1 - beta - gamma);
                
                    // Check if the pixel is inside of the triangle
                    if (alpha < 0 || beta < 0 || gamma < 0 ||
                        alpha > 1 || beta > 1 || gamma > 1) {
                        // We're outside the triangle, so there's no need to check
                    } else if (
                        // Check for pixels definitely inside the triangle
                        (alpha < 1 && beta < 1 && gamma < 1 && 
                         alpha > 0 && beta > 0 && gamma > 0) ||
                        // Check for top-left fill convention inner edges
                        (beta == 0 && edgeInTriangle[2]) ||
                        (gamma == 0 && edgeInTriangle[0]) ||
                        (alpha == 0 && edgeInTriangle[1])) {
                        // We're inside the triangle, so let's draw the point.
                        if (type == BASIC_ONLY) {
                            // Only use linear interpolation
                            setPixel(
                                (int)boxPixel[0],
                                (int)boxPixel[1],
                                (points[0][2] + 
                                 lineAB3D[2]*beta + 
                                 lineAC3D[2]*gamma),
                                linearColor(Vector3<float>(alpha,beta,gamma), 
                                            Vector3<Vector4<float> >(colors)));
                        } else {
                            // Use perspective-correct interpolation
                            setPixel(
                                (int)boxPixel[0],
                                (int)boxPixel[1],
                                (points[0][2] + 
                                 lineAB3D[2]*beta + 
                                 lineAC3D[2]*gamma),
                                hyperbolicColor(Vector3<float>(alpha,beta,gamma), 
                                                Vector3<Vector4<float> >(colors),
                                                Vector3<float>(homeo)));
                        }
                    }
                }
            }
        } else {
            // The triangle is in fact a triangle
            // Loop through all pixels in the box
            for (int x = bBox[0]; x <= bBox[2]; x++) {
                for (int y = bBox[1]; y <= bBox[3]; y++) {
                    // Make a vector that is the pixel in the bounding box's center
                    boxPixel = Vector3<float>(x + 0.5, y + 0.5, 1);

                    // Calculate the barycentric coordinates of the pixel
                    beta = (normAC^(boxPixel - pixels[0])) / 
                        (normAC^lineAB);
                    gamma = (normAB^(boxPixel - pixels[0])) / 
                        (normAB^lineAC);

                    alpha = (1 - beta - gamma);
                
                    // Check if the pixel is inside of the triangle
                    if (alpha < 0 || beta < 0 || gamma < 0 ||
                        alpha > 1 || beta > 1 || gamma > 1) {
                        // We're outside the triangle, so there's no need to check
                    } else if (
                        // Check for pixels definitely inside the triangle
                        (alpha < 1 && beta < 1 && gamma < 1 && 
                         alpha > 0 && beta > 0 && gamma > 0) ||
                        // Check for top-left fill convention inner edges
                        (beta == 0 && edgeInTriangle[2]) ||
                        (gamma == 0 && edgeInTriangle[0]) ||
                        (alpha == 0 && edgeInTriangle[1])) {
                        // We're inside the triangle, so let's draw the point.
                        if (type == BASIC_ONLY) {
                            // Only use linear interpolation
                            setPixel(
                                (int)boxPixel[0],
                                (int)boxPixel[1],
                                (points[0][2] + 
                                 lineAB3D[2]*beta + 
                                 lineAC3D[2]*gamma),
                                linearColor(Vector3<float>(alpha,beta,gamma), 
                                            Vector3<Vector4<float> >(colors)));
                        } else {
                            // Use perspective-correct interpolation
                            setPixel(
                                (int)boxPixel[0],
                                (int)boxPixel[1],
                                (points[0][2] + 
                                 lineAB3D[2]*beta + 
                                 lineAC3D[2]*gamma),
                                hyperbolicColor(Vector3<float>(alpha,beta,gamma), 
                                                Vector3<Vector4<float> >(colors),
                                                Vector3<float>(homeo)));
                        }
                    }
                }
            }
        }
    } else {
        // The triangle is not a triangle
    }
}

void SWRenderContext::setPixel(int x, int y, float z, uint colors) {
    // Ensure we are placing a frontmost pixel
    if (zBuffer[y*width + x] <= z && z <= 1) {
        // Update the zbuffer array to show the new z value
        zBuffer[y*width + x] = z;
        image->setPixel(x, y, colors);
    }
}

void SWRenderContext::setWidget(SWWidget *swWidget)
{
	mswWidget = swWidget;
	image = mswWidget->getImage();
}
