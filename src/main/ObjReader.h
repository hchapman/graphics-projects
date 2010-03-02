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

        // given an array of vertices it normalizes and centers it
        static void normalize(float *vertices, int numVertices);

	private:
		static void get_indices(char *word, int *vindex, int *tindex, int *nindex);

        // modifies Vector3 params to hold min/max xyz values
        static void findMinMaxVectors(float *vertices, int numVertices,
                                      Vector3 *minVector, Vector3 *maxVector);

        // finds the largest magnitude of difference between xyz values
        static float findGreatestDiff(Vector3 minVector, Vector3 maxVector);

	};

}

#endif // CSCI330_OBJFILE_H_

