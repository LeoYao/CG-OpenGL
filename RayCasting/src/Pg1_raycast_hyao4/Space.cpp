#include "stdafx.h"
#include "Space.h"


Space::Space()
{
}


Space::~Space()
{
	g_model.destroy();
	triangles.clear();
	lights.clear();
}

const int Space::GetTriangleNum() const
{
	return this->triangleNum;
}

const Triangle& Space::GetTriangle(int i) const
{
	return triangles[i];
}

ModelOBJ& Space::GetModel()
{
	return g_model;
}

bool Space::Load(const char *pszFilename, bool enableScale)
{
	if (!g_model.import(pszFilename))
		return false;

	if (enableScale)
	{
		g_model.normalize(2.0f, true);
	}

	triangleNum = g_model.getNumberOfTriangles();
	triangles.clear();
	triangles.resize(triangleNum);
	int triangleVertexIndex1 = 0;
	int triangleVertexIndex2 = 0;
	int triangleVertexIndex3 = 0;
	for (int i = 0; i < triangleNum; i++)
	{
		triangleVertexIndex1 = g_model.getIndexBuffer()[3 * i];
		triangleVertexIndex2 = g_model.getIndexBuffer()[3 * i + 1];
		triangleVertexIndex3 = g_model.getIndexBuffer()[3 * i + 2];
		ModelOBJ::Vertex vtx1 = g_model.getVertex(triangleVertexIndex1);
		ModelOBJ::Vertex vtx2 = g_model.getVertex(triangleVertexIndex2);
		ModelOBJ::Vertex vtx3 = g_model.getVertex(triangleVertexIndex3);
		triangles[i] = Triangle(vtx1, vtx2, vtx3);
	}

	return true;
}

void Space::AddLight(float x, float y, float z, const float* ia, const float* id, const float* is)
{
	Light l;
	l.position.position[0] = x;
	l.position.position[1] = y;
	l.position.position[2] = z;

	for (int i = 0; i < 3; i++)
	{
		l.intesity[i][0] = ia[i];
		l.intesity[i][1] = id[i];
		l.intesity[i][2] = is[i];
	}

	if (lights.capacity() == lightNum)
	{
		lights.resize((lightNum + 1) * 2);
	}
	lights[lightNum++] = l;
}

const Space::Light & Space::GetLight(int index) const
{
	return lights[index];
}

void Space::ClearLights()
{
	lights.clear();
	lightNum = 0;
}

const int Space::GetLightNum() const
{
	return lightNum;
}