#pragma once

#include "model_obj.h"
#include "Space.h"
#include "model_obj.h"
#include "Ray.h"

class LocalIllumination
{
public:
	LocalIllumination();
	~LocalIllumination();
	COLORREF Illuminate(Space &space, ModelOBJ::Vertex &vertex, Ray & ray, int triangleIndex, bool enableShadow);


};

