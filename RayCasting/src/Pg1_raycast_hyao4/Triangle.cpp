#include "stdafx.h"
#include "Triangle.h"
#include <glm/geometric.hpp>
#include "Utils.h"
#include <cmath>

Triangle::Triangle()
{
	
}

Triangle::Triangle(const ModelOBJ::Vertex &v1, const ModelOBJ::Vertex &v2, const ModelOBJ::Vertex &v3)
{
	vertexes.resize(3);
	vertexes[0] = v1;
	vertexes[1] = v2;
	vertexes[2] = v3;

	float x1 = v1.position[0];
	float y1 = v1.position[1];
	float z1 = v1.position[2];

	float x2 = v2.position[0];
	float y2 = v2.position[1];
	float z2 = v2.position[2];

	float x3 = v3.position[0];
	float y3 = v3.position[1];
	float z3 = v3.position[2];

	glm::vec3 vec1 = glm::vec3(x2 - x1, y2 - y1, z2 - z1);
	glm::vec3 vec2 = glm::vec3(x3 - x1, y3 - y1, z3 - z1);

	normalVec = glm::cross(vec1, vec2);
	normalVec = normalVec / glm::length(normalVec);

	//glm::dot(vec1, vec2);
}


Triangle::~Triangle()
{
	vertexes.clear();
}

const glm::vec3 &Triangle::GetNormalVector() const
{
	return normalVec;
}

const ModelOBJ::Vertex &Triangle::GetVertex(int index) const
{
	return vertexes[index];
}

/*
    Input:  a ray to intersect to this triangle
    Output: *intsctPnt = intersection point (when it exists)
			*distance = the distance between the intersection point and the source point of the input ray
    Return: -1 = triangle is degenerate (a segment or point)
             0 =  disjoint (no intersect)
             1 =  intersect in unique point I1
             2 =  are in the same plane
*/
int Triangle::Intersect(const Ray &ray, ModelOBJ::Vertex *intsctPnt, float &distance) const
{
	if (Utils::isEqual(normalVec.x, 0) && Utils::isEqual(normalVec.y, 0) && Utils::isEqual(normalVec.z, 0))
	{
		return DEGNERATE;
	}

	float r, a, b; //params to calc ray-plane intersect

	//Two vectors building the triangle/plane
	glm::vec3 u = Utils::GetVector(vertexes[0], vertexes[1]);
	glm::vec3 v = Utils::GetVector(vertexes[0], vertexes[2]);

	
	glm::vec3 w0 = Utils::GetVector(vertexes[0], ray.GetSourceVertex());

	//Project the vector w0 and direction vector of ray to a normal plane of the plane of the triangle
	a = glm::dot(normalVec, w0) * -1;
	b = glm::dot(normalVec, ray.GetDirVector());

	if (Utils::isEqual(b,0)) 
	{
		if (Utils::isEqual(a, 0))
		{
			return COINCIDE;
		}
		else
		{
			return DISJOINT;
		}
	}

	r = a / b;
	if (r < FLOAT_ZERO) //ray goes away from triangle
	{
		return DISJOINT;
	}
	//// if it is for a segment, also test if (r > 1.0) => no intersect

	*intsctPnt = Utils::MoveVertex(ray.GetSourceVertex(), ray.GetDirVector()*r);
	distance = r * glm::length(ray.GetDirVector());

	//Barycentric Coordinate Computation to determine if the intersect point *intsctPnt is in the triangle
	//For vectors w, u, v and sclars s, t, we have w = s*u + t*v.
	//The solution of the equation is:
	// s = ((uv)(wv)-(vv)(wu)) /((uv)^2-(uu)(vv))
	// t = ((uv)(wu)-(uu)(wv)) /((uv)^2-(uu)(vv))
	//
	//If s >= 0 , t >= 0 and s+t <= 1, the intersect is in the triangle
	glm::vec3 w = Utils::GetVector(vertexes[0], *intsctPnt);
	float uu, uv, vv, wu, wv, D;

	uu = glm::dot(u, u);
	uv = glm::dot(u, v);
	vv = glm::dot(v, v);
	wu = glm::dot(w, u);
	wv = glm::dot(w, v);
	D = uv * uv - uu * vv;

	float s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < FLOAT_ZERO || s > 1.0)
	{
		return DISJOINT;
	}

	t = (uv * wu - uu * wv) / D;
	if (t < FLOAT_ZERO || s + t > 1.0)
	{
		return DISJOINT;
	}

	return INTERSECT;
}