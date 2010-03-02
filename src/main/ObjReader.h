#ifndef __ObjReader_h__
#define __ObjReader_h__

#include "RE330_global.h"
#include "Vector3.h"

namespace RE330 {

	class ObjReader
	{
	public:
		static void readObj(const char* fileName, int &nVertices, float **vertices,
                             float **normals, float **texcoords, int &nIndices,
                             int **indices);

        static void normalize(float *vertices, int numVertices);

	private:
		static void get_indices(char *word, int *vindex, int *tindex, int *nindex);

        static void findMinMaxVectors(float *vertices, int numVertices,
                                      Vector3 *minVector, Vector3 *maxVector);

        static float findGreatestDiff(Vector3 minVector, Vector3 maxVector);

	};

}

#endif // CSCI330_OBJFILE_H_

