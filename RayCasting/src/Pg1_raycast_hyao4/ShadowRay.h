#pragma once

#include "Space.h"
#include "model_obj.h"
#include <glm/geometric.hpp>

class ShadowRay
{
public:
	ShadowRay();
	~ShadowRay();
	const bool block(const Space &space, const ModelOBJ::Vertex &intersectPoint, const ModelOBJ::Vertex &lightPos, const int triangleIndex) const;
};

