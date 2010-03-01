#include "ObjReader.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
using namespace RE330;
#include <exception>
#include <iostream>
#include <limits>
using namespace std;

void ObjReader::normalize(float * vertices, int numVertices)
{
    Vector3 minValues;
    Vector3 maxValues;
    ObjReader::findMinAndMax(vertices, numVertices,
                             &minValues, &maxValues);
    

}

void ObjReader::findMinAndMax(float *vertices, int numVertices,
                              Vector3 * min, Vector3 * max)
{
    float minX = numeric_limits<float>::max();
    float minY = numeric_limits<float>::max();
    float minZ = numeric_limits<float>::max();

    float maxX = numeric_limits<float>::min();
    float maxY = numeric_limits<float>::min();
    float maxZ = numeric_limits<float>::min();

    int remainder;
    float value;
    for (int count; count <= numVertices; count++)
    {
        value = vertices[count];
        remainder = count % 3;
        if (remainder == 0)
        {
            if (value > maxX) { maxX = value; }
            else if (value < minX) { minX = value;}
        } else if (remainder == 1)
        {
            if (value > maxY) { maxY = value; }
            else if (value < minY) { minY = value;}
        } else if (remainder == 2)
        {
            if (value > maxZ) { maxZ = value; }
            else if (value < minZ) { minZ = value;}
        }
    }

    min = new Vector3(minX, minY, minZ);
    max = new Vector3(maxX, maxY, maxZ);
    std::cout << "minX: " << minX <<"  minY:" << minY << "  minZ: " << minZ << std::endl;
    std::cout << "maxX: " << maxX <<"  maxY:" << maxY << "  maxZ: " << maxZ << std::endl;
}

void ObjReader::get_indices(char *word, int *vindex, int *tindex, int *nindex)
{
    char *null = " ";
    char *ptr;
    char *tp;
    char *np;

    /* by default, the texture and normal pointers are set to the null string */
    tp = null;
    np = null;

    /* replace slashes with null characters and cause tp and np to point */
    /* to character immediately following the first or second slash */

    for (ptr = word; *ptr != '\0'; ptr++) {
        if (*ptr == '/') {
            if (tp == null)
                tp = ptr + 1;
            else
                np = ptr + 1;

            *ptr = '\0';
        }
    }

    *vindex = atoi(word);
    *tindex = atoi(tp);
    *nindex = atoi(np);
}


void ObjReader::readObj(const char* fileName, int &nVertices, float **vertices,
                        float **normals, float **texcoords, int &nIndices,
                        int **indices)
{
    FILE * fp;
    fp = fopen(fileName,"r");
    if (fp == NULL){
        perror ("Error opening file");
        return;
    }

    int nv=0, nn=0, nf=0, nt=0;
    char line[81];

    while ( fgets( line, 80, fp ) != NULL ) {
        if (line[0] == 'v') {
            if (line[1] == 'n')
                nn++;
            else if (line[1] == 't')
                nt++;
            else
                nv++;
        } else if (line[0] == 'f') {
            nf++;
        }
    }
    fseek(fp, 0, 0);

    // Arrays for normals, vertices, texture coords
    float *n = new float[3*(nn>nf?nn:nf)];
    float *v = new float[3*nv];
    float *t = new float[2*nt];

    // Index arrays for normals, vertices, texture coords
    int *vInd = new int[3*nf];
    int *nInd = new int[3*nf];
    int *tInd = new int[3*nf];

    int nvertices = 0;
    int nnormals = 0;
    int ntexcoords = 0;
    int nindices  = 0;
    int ntriangles = 0;
    bool noNormals = false;
    bool noTexCoords = false;

    while ( fgets( line, 80, fp ) != NULL ) {
        if (line[0] == 'v') {
            if (line[1] == 'n') {
                float x, y, z;
                sscanf( &line[2], "%f %f %f\n", &x, &y, &z);
                float l = sqrt(x*x+y*y+z*z);
                x = x/l;
                y = y/l;
                z = z/l;
                n[nnormals]=x;
                nnormals++;
                n[nnormals]=y;
                nnormals++;
                n[nnormals]=z;
                nnormals++;
            } else if (line[1] == 't') {
                float u, v;
                sscanf( &line[2], "%f %f\n", &u, &v);
                t[ntexcoords]=u;
                ntexcoords++;
                t[ntexcoords]=v;
                ntexcoords++;
            } else {
                float x, y, z;
                sscanf( &line[1], "%f %f %f\n", &x, &y, &z);
                v[nvertices] = x;
                nvertices++;
                v[nvertices] = y;
                nvertices++;
                v[nvertices] = z;
                nvertices++;
            }
        } else if (line[0] == 'f') {
            char s1[32], s2[32], s3[32];
            int vI, tI, nI;
            sscanf( &line[1], "%s %s %s\n", s1, s2, s3);

            // Indices for first vertex
            get_indices(s1, &vI, &tI, &nI);
            vInd[nindices] = vI-1;
            if (nI)
                nInd[nindices] = nI-1;
            else
                noNormals = true;
            if (tI)
                tInd[nindices] = tI-1;
            else
                noTexCoords = true;
            nindices++;

            // Indices for second vertex
            get_indices(s2, &vI, &tI, &nI);
            vInd[nindices] = vI-1;
            if (nI)
                nInd[nindices] = nI-1;
            else
                noNormals = true;
            if (tI)
                tInd[nindices] = tI-1;
            else
                noTexCoords = true;
            nindices++;

            // Indices for third vertex
            get_indices(s3, &vI, &tI, &nI);
            vInd[nindices] = vI-1;
            if (nI)
                nInd[nindices] = nI-1;
            else
                noNormals = true;
            if (tI)
                tInd[nindices] = tI-1;
            else
                noTexCoords = true;
            nindices++;

            ntriangles++;
        }
    }

    /* We don't support separate indices for normals, vertices, and
       texture coordinates. Need to re-organize arrays. */
    *vertices = new float[ntriangles*9];
    if(!noNormals) *normals = new float[ntriangles*9]; else *normals=0;
    if(!noTexCoords) *texcoords = new float[ntriangles*6]; else *texcoords=0;
    *indices = new int[ntriangles*3];
    nVertices = ntriangles*3;
    nIndices = ntriangles*3;

    for(int i=0; i<ntriangles; i++)
    {
        (*indices)[3*i] = 3*i;
        (*indices)[3*i+1] = 3*i+1;
        (*indices)[3*i+2] = 3*i+2;

        (*vertices)[9*i] = v[3*vInd[3*i]];
        (*vertices)[9*i+1] = v[3*vInd[3*i]+1];
        (*vertices)[9*i+2] = v[3*vInd[3*i]+2];

        (*vertices)[9*i+3] = v[3*vInd[3*i+1]];
        (*vertices)[9*i+4] = v[3*vInd[3*i+1]+1];
        (*vertices)[9*i+5] = v[3*vInd[3*i+1]+2];

        (*vertices)[9*i+6] = v[3*vInd[3*i+2]];
        (*vertices)[9*i+7] = v[3*vInd[3*i+2]+1];
        (*vertices)[9*i+8] = v[3*vInd[3*i+2]+2];

        if(!noNormals)
        {
            (*normals)[9*i] = n[3*nInd[3*i]];
            (*normals)[9*i+1] = n[3*nInd[3*i]+1];
            (*normals)[9*i+2] = n[3*nInd[3*i]+2];

            (*normals)[9*i+3] = n[3*nInd[3*i+1]];
            (*normals)[9*i+4] = n[3*nInd[3*i+1]+1];
            (*normals)[9*i+5] = n[3*nInd[3*i+1]+2];

            (*normals)[9*i+6] = n[3*nInd[3*i+2]];
            (*normals)[9*i+7] = n[3*nInd[3*i+2]+1];
            (*normals)[9*i+8] = n[3*nInd[3*i+2]+2];
        }

        if(!noTexCoords)
        {
            (*texcoords)[6*i] = t[2*tInd[3*i]];
            (*texcoords)[6*i+1] = t[2*tInd[3*i]+1];

            (*texcoords)[6*i+2] = t[2*tInd[3*i+1]];
            (*texcoords)[6*i+3] = t[2*tInd[3*i+1]+1];

            (*texcoords)[6*i+4] = t[2*tInd[3*i+2]];
            (*texcoords)[6*i+5] = t[2*tInd[3*i+2]+1];
        }

    }

    delete[] n;
    delete[] v;
    delete[] t;
    delete[] nInd;
    delete[] vInd;
    delete[] tInd;
}
