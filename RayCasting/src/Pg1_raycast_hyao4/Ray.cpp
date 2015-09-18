#include "stdafx.h"
#include "Ray.h"


Ray::Ray()
{
}

Ray::Ray(const ModelOBJ::Vertex &srcVtx, const ModelOBJ::Vertex &tgtVtx)
{
	srcVertex = srcVtx;
	tgtVertex = tgtVtx;

	float x1 = srcVertex.position[0];
	float y1 = srcVertex.position[1];
	float z1 = srcVertex.position[2];

	float x2 = tgtVertex.position[0];
	float y2 = tgtVertex.position[1];
	float z2 = tgtVertex.position[2];

	vector = glm::vec3(x2 - x1, y2 - y1, z2 - z1);
}


Ray::~Ray()
{
}

const ModelOBJ::Vertex &Ray::GetSourceVertex() const
{
	return srcVertex;
}

const ModelOBJ::Vertex &Ray::GetTargetVertex() const
{
	return tgtVertex;
}

const glm::vec3 &Ray::GetDirVector() const
{
	return vector;
}