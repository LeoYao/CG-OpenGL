#include "stdafx.h"
#include "ShadowRay.h"
#include "Triangle.h"
#include "Utils.h"
#include "Ray.h"

ShadowRay::ShadowRay()
{
}


ShadowRay::~ShadowRay()
{
}

const bool ShadowRay::block(const Space &space, const ModelOBJ::Vertex &intersectPoint, const ModelOBJ::Vertex &lightPos, const int triangleIndex) const
{
	bool isBlocked = false;

	ModelOBJ::Vertex shadowIntsectPoint;
	float distance;
	Ray lightRay = Ray(intersectPoint, lightPos);
	float lightDistance = glm::length(lightRay.GetDirVector());

	for (int i = 0; i < space.GetTriangleNum(); i++)
	{
		if (i != triangleIndex)
		{
			if (space.GetTriangle(i).Intersect(lightRay, &shadowIntsectPoint, distance) == INTERSECT)
			{
				if (distance < lightDistance)
				{
					isBlocked = true;
					break;
				}
			}
		}
	}
	return isBlocked;
}
