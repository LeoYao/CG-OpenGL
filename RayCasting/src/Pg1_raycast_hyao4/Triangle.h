#pragma once

#include "model_obj.h"
#include <glm/vec3.hpp>
#include "Ray.h"

class Triangle
{
public:
	Triangle();
	Triangle(const ModelOBJ::Vertex &v1, const ModelOBJ::Vertex &v2, const ModelOBJ::Vertex &v3);
	~Triangle();
	const glm::vec3 &GetNormalVector() const;
	const ModelOBJ::Vertex &GetVertex(int index) const;
	int Intersect(const Ray &ray, ModelOBJ::Vertex *intsctPnt, float &distance) const;

private:
	std::vector<ModelOBJ::Vertex> vertexes;
	glm::vec3 normalVec;

};

