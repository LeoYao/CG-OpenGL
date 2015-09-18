#include "stdafx.h"
#include "RayCasting.h"


RayCasting::RayCasting()
{
}


RayCasting::~RayCasting()
{
}

PixelInfo RayCasting::Cast(ModelOBJ::Vertex &sourceVertex, Ray &ray, Space &space, LocalIllumination illumination, bool enableIllumination, bool enableShadow)
{
	bool first = true;
	ModelOBJ::Vertex intsectPoint;
	ModelOBJ::Vertex actualIntsectPoint;
	float distance;
	float actualDistance;

	PixelInfo pixelInfo;
	pixelInfo.visible = false;

	int intsectTriangleIndex = -1;

	for (int i = 0; i < space.GetTriangleNum(); i++)
	{
		if (space.GetTriangle(i).Intersect(ray, &intsectPoint, distance) == INTERSECT)
		{
			pixelInfo.visible = true;
			if (first)
			{
				first = false;
				actualDistance = distance;
				actualIntsectPoint = intsectPoint;

				if (!enableIllumination)
					pixelInfo.color = white;
				intsectTriangleIndex = i;
			}
			else
			{
				if (actualDistance > distance)
				{
					actualDistance = distance;
					actualIntsectPoint = intsectPoint;
					if (!enableIllumination)
						pixelInfo.color = white;
					intsectTriangleIndex = i;
				}
			}
		}

		if (enableIllumination && pixelInfo.visible)
		{
			pixelInfo.color = illumination.Illuminate(space, actualIntsectPoint, ray, intsectTriangleIndex, enableShadow);
		}
		
	}

	return pixelInfo;
}