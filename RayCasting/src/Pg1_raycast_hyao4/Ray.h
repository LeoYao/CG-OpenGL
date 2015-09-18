#pragma once

#include <glm/vec3.hpp>
#include "model_obj.h"

class Ray
{
public:
	Ray();
	Ray(const ModelOBJ::Vertex &srcVtx, const ModelOBJ::Vertex &tgtVtx);
	~Ray();
	const ModelOBJ::Vertex &GetSourceVertex() const;
	const ModelOBJ::Vertex &GetTargetVertex() const;
	const glm::vec3 &GetDirVector() const;

private:
	ModelOBJ::Vertex srcVertex;
	ModelOBJ::Vertex tgtVertex;
	glm::vec3 vector;

};

