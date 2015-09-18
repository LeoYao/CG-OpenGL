#pragma once

#include "glm\vec3.hpp"
#include "model_obj.h"

/*
	-1 = triangle is degenerate(a segment or point)
	0 = disjoint(no intersect)
	1 = intersect in unique point I1
	2 = are in the same plane
*/
const int DEGNERATE = -1;
const int DISJOINT = 0;
const int INTERSECT = 1;
const int COINCIDE = 2;
const float FLOAT_ZERO = 0.000;

static COLORREF white = RGB(255, 255, 255);
static COLORREF redColor = RGB(255, 0, 0);
static COLORREF blueColor = RGB(0, 0, 255);
static COLORREF greenColor = RGB(0, 255, 0);
static COLORREF redColor1 = RGB(127, 0, 0);
static COLORREF blueColor1 = RGB(0, 0, 127);
static COLORREF greenColor1 = RGB(0, 127, 0);
static std::vector<COLORREF>	staticColors = { redColor, redColor, greenColor, greenColor, blueColor, blueColor, redColor1, redColor1, blueColor1, blueColor1, white, white };

struct PixelInfo
{
	bool visible = false;
	COLORREF color;
};


class Utils
{
public:
	Utils();
	~Utils();
	static glm::vec3 GetVector(const ModelOBJ::Vertex &srcVtx, const ModelOBJ::Vertex &tgtVtx)
	{
		float x1 = srcVtx.position[0];
		float y1 = srcVtx.position[1];
		float z1 = srcVtx.position[2];

		float x2 = tgtVtx.position[0];
		float y2 = tgtVtx.position[1];
		float z2 = tgtVtx.position[2];

		return glm::vec3(x2 - x1, y2 - y1, z2 - z1);
	}

	static bool isEqual(float x1, float x2)
	{
		return fabs(x1 - x2) <= std::numeric_limits<float>::epsilon();
	}

	static ModelOBJ::Vertex MoveVertex(const ModelOBJ::Vertex &vertex, const glm::vec3 &vector)
	{
		ModelOBJ::Vertex vtx = vertex;
		vtx.position[0] += vector.x;
		vtx.position[1] += vector.y;
		vtx.position[2] += vector.z;

		return vtx;
	}

	static ModelOBJ::Vertex GetVertex(float x, float y, float z)
	{
		ModelOBJ::Vertex vtx;
		vtx.position[0] = x;
		vtx.position[1] = y;
		vtx.position[2] = z;

		return vtx;
	}

};

