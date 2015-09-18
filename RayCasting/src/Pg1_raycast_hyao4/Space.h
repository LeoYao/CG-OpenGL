#pragma once

#include "model_obj.h"
#include "Triangle.h"

class Space
{
public:
	struct Light
	{
		ModelOBJ::Vertex position;
		float intesity[3][3];
		float color[3];
	};

	Space();
	~Space();
	bool Load(const char *pszFilename, bool enableScale);
	const int GetTriangleNum() const;
	const Triangle& GetTriangle(int i) const;
	ModelOBJ &GetModel();
	void AddLight(float x, float y, float z, const float* ia, const float* id, const float* is);
	void ClearLights();
	const int GetLightNum() const;
	const Light& Space::GetLight(int index) const;

private:
	ModelOBJ g_model;
	std::vector<Triangle> triangles;
	int triangleNum = 0;
	std::vector<Light> lights;
	int lightNum = 0;
};

