/*
 * Copyright (c) 2011 Peter Vasil
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "MeshWizIncludes.h"
#include "BakeTool.h"
#include "KDTree.h"
#include <cmath>

#define DOT2(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1])
#define DOT3(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

using namespace assembly3d;
using namespace assembly3d::wiz;

BakeTool::BakeTool()
{
}

int BakeTool::isInBounds(Mesh *mesh)
{
    int numUnbakable = 0;
    for(int i = 0; i < mesh->getNumberOfVertices(); ++i)
    {
//        Vertex* vert = &mesh->getVertex(i);
        float* pTexCoord = mesh->getTexCoord(i);
        if(pTexCoord[0] > 1.0f ||
           pTexCoord[0] < 0.0f ||
           pTexCoord[1] > 1.0f ||
           pTexCoord[1] < 0.0f)
        {
            numUnbakable++;
        }
    }
    return numUnbakable;
}

int BakeTool::checkUVOverlapping(Mesh *mesh)
{
    int numOverlaps = 0;
//    for(int i = 0; i < mesh->getNumberOfTriangles(); ++i)
//    {
//        const unsigned int* triangle1 = mesh->getTriangle(i);
//        float* pTexCoord00 = mesh->getTexCoord(triangle1[0]);
//        float* pTexCoord01 = mesh->getTexCoord(triangle1[1]);
//        float* pTexCoord02 = mesh->getTexCoord(triangle1[2]);
//        for(int j = i+1; j < mesh->getNumberOfTriangles(); ++j)
//        {
////            if(j == i)
////                continue;
//            const unsigned int* triangle2 = mesh->getTriangle(j);
//            for(unsigned int k = 0; k < 3; ++k)
//            {
////                Vertex* point = &mesh->getVertex(triangle2[k]);
//                float* pTexCoord2 = mesh->getTexCoord(triangle2[k]);

//                if(checkPointInTri(pTexCoord2, pTexCoord00, pTexCoord01, pTexCoord02))
//                {
//                    numOverlaps++;
//                    break;
//                }
//            }
//        }
//    }


    Mesh* meshCopy = new Mesh(*mesh);

    for(int i = 0; i < meshCopy->getNumberOfTriangles(); ++i)
    {
        const unsigned int* triangle = meshCopy->getTriangle(i);

        float* pTexCoord00 = meshCopy->getTexCoord(triangle[0]);
        float* pTexCoord01 = meshCopy->getTexCoord(triangle[1]);
        float* pTexCoord02 = meshCopy->getTexCoord(triangle[2]);

//        Vertex* vert = &mesh->getVertex(i);
        if(pTexCoord00[0] > 1.0f || pTexCoord00[0] < 0.0f ||
           pTexCoord00[1] > 1.0f || pTexCoord00[1] < 0.0f ||
           pTexCoord01[0] > 1.0f || pTexCoord01[0] < 0.0f ||
           pTexCoord01[1] > 1.0f || pTexCoord01[1] < 0.0f ||
           pTexCoord02[0] > 1.0f || pTexCoord02[0] < 0.0f ||
           pTexCoord02[1] > 1.0f || pTexCoord02[1] < 0.0f)
        {
            float tmpTexCoord00[2] = {0.0f, 0.0f};
            float tmpTexCoord01[2] = {0.0f, 0.0f};
            float tmpTexCoord02[2] = {0.0f, 0.0f};
            tmpTexCoord00[0] = fmodf(pTexCoord00[0], 1.0f);
            tmpTexCoord00[1] = fmodf(pTexCoord00[1], 1.0f);
            tmpTexCoord01[0] = fmodf(pTexCoord01[0], 1.0f);
            tmpTexCoord01[1] = fmodf(pTexCoord01[1], 1.0f);
            tmpTexCoord02[0] = fmodf(pTexCoord02[0], 1.0f);
            tmpTexCoord02[1] = fmodf(pTexCoord02[1], 1.0f);
            int numberFoTexCoords = meshCopy->getNumberOfVertices();
            meshCopy->addTexCoord(tmpTexCoord00);
            meshCopy->addTexCoord(tmpTexCoord01);
            meshCopy->addTexCoord(tmpTexCoord02);
            meshCopy->addIndex(numberFoTexCoords);
            meshCopy->addIndex(numberFoTexCoords+1);
            meshCopy->addIndex(numberFoTexCoords+2);
        }
    }

    std::vector<Point> points;
    generatePoints(meshCopy, points);

    KDTree<> kdtree;
    kdtree.generate(points);

    for(int i = 0; i < meshCopy->getNumberOfTriangles(); ++i)
    {
//        const unsigned int* triangle = mesh->getTriangle(i);
        float* pTexCoord00 = meshCopy->getTexCoord(points[i].triangle[0]);
        float* pTexCoord01 = meshCopy->getTexCoord(points[i].triangle[1]);
        float* pTexCoord02 = meshCopy->getTexCoord(points[i].triangle[2]);

        std::vector<int> indices;
        int numFound = kdtree.nearestQuery(points[i], 100, indices);
        for(int j = 0; j < numFound; ++j)
        {

            for(unsigned int k = 0; k < 3; ++k)
            {
                float* pTexCoord2 = meshCopy->getTexCoord(points[indices[j]].triangle[k]);
                if(checkPointInTri(pTexCoord2, pTexCoord00, pTexCoord01, pTexCoord02))
                {
                    numOverlaps++;
                    break;
                }
            }

        }


//    for(int i = 0; i < mesh->getNumberOfTriangles()-1; ++i)
//    {
//        const unsigned int* triangle1 = mesh->getTriangle(i);
//        Vertex* vert00 = &mesh->getVertex(triangle1[0]);
//        Vertex* vert01 = &mesh->getVertex(triangle1[1]);
//        Vertex* vert02 = &mesh->getVertex(triangle1[2]);
//        const unsigned int* triangle2 = mesh->getTriangle(i+1);
//        for(unsigned int k = 0; k < 3; ++k)
//        {
//            Vertex* point = &mesh->getVertex(triangle2[k]);
//            if(checkPointInTri(point->texCoord, vert00->texCoord, vert01->texCoord, vert02->texCoord))
//            {
//                numOverlaps++;
//            }
//        }
//    }
    }
    SAFE_DELETE(meshCopy);

    return numOverlaps;
}

bool BakeTool::checkPointInTri(float* p, float* a, float* b, float* c)
{
    // http://www.blackpawn.com/texts/pointinpoly/default.html

    float v0[2] = {0.0f,0.0f};
    float v1[2] = {0.0f,0.0f};
    float v2[2] = {0.0f,0.0f};

    // Compute vectors
    v0[0] = c[0] - a[0];
    v0[1] = c[1] - a[1];
    v1[0] = b[0] - a[0];
    v1[1] = b[1] - a[1];
    v2[0] = p[0] - a[0];
    v2[1] = p[1] - a[1];

    // Compute dot products
    float dot00 = DOT2(v0, v0);
    float dot01 = DOT2(v0, v1);
    float dot02 = DOT2(v0, v2);
    float dot11 = DOT2(v1, v1);
    float dot12 = DOT2(v1, v2);

    // Compute barycentric coordinates
    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    if((u > 0) && (v > 0) && (u + v < 1))
        return true;
    else
        return false;
}
//void BakeTool::generatePoints(std::vector<unsigned int> triangles,
//                              std::vector<float> texCoords,
//                              std::vector<Point>& points)
//{
//    points.clear();
//    for(int i = 0; i < triangles.size()/3; ++i)
//    {
//        const unsigned int* triangle = &triangles(i*3);
//        float* pTexCoord0 = mesh->getTexCoord(triangle[0]);
//        float* pTexCoord1 = mesh->getTexCoord(triangle[1]);
//        float* pTexCoord2 = mesh->getTexCoord(triangle[2]);

//        float centerX = (pTexCoord0[0] + pTexCoord1[0] + pTexCoord2[0]) / 3;
//        float centerY = (pTexCoord0[1] + pTexCoord1[1] + pTexCoord2[1]) / 3;
//        Point center(centerX, centerY, 0.0f);
//        center.triangle[0] = triangle[0];
//        center.triangle[1] = triangle[1];
//        center.triangle[2] = triangle[2];

//        points.push_back(center);
//    }

//}

void BakeTool::generatePoints(assembly3d::Mesh *mesh, std::vector<Point>& points)
{
    points.clear();
    for(int i = 0; i < mesh->getNumberOfTriangles(); ++i)
    {
        const unsigned int* triangle = mesh->getTriangle(i);
        float* pTexCoord0 = mesh->getTexCoord(triangle[0]);
        float* pTexCoord1 = mesh->getTexCoord(triangle[1]);
        float* pTexCoord2 = mesh->getTexCoord(triangle[2]);

        float centerX = (pTexCoord0[0] + pTexCoord1[0] + pTexCoord2[0]) / 3;
        float centerY = (pTexCoord0[1] + pTexCoord1[1] + pTexCoord2[1]) / 3;
        Point center(centerX, centerY, 0.0f);
        center.triangle[0] = triangle[0];
        center.triangle[1] = triangle[1];
        center.triangle[2] = triangle[2];

        points.push_back(center);
    }

}
