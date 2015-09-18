#pragma once

#include "model_obj.h"
#include "Ray.h"
#include "Utils.h"
#include "Space.h"
#include "LocalIllumination.h"

class RayCasting
{
public:
	//RayCasting();
	RayCasting();
	~RayCasting();
	PixelInfo Cast(ModelOBJ::Vertex &sourceVertex, Ray &ray, Space &space, LocalIllumination illumination, bool enableIlluminations, bool enableShadow);
};

