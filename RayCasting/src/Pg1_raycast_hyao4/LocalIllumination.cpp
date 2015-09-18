#include "stdafx.h"
#include "LocalIllumination.h"
#include "Utils.h"
#include <glm/geometric.hpp>
#include "ShadowRay.h"

LocalIllumination::LocalIllumination()
{
}


LocalIllumination::~LocalIllumination()
{
}

COLORREF LocalIllumination::Illuminate(Space &space, ModelOBJ::Vertex &intersectPoint, Ray & ray, int triangleIndex, bool enableShadow)
{
	float color[3]; // RGB
	float ia_result=0.0f, id_result=0.0f, is_result = 0.0f;
	float i_result[3] = { 0, 0, 0 };
	ShadowRay shadowRay;

	const ModelOBJ & model = space.GetModel();
	const int materialIndex = model.getMaterialIndex(triangleIndex);
	const ModelOBJ::Material &material = model.getMaterial(materialIndex);
	const Triangle triangle = space.GetTriangle(triangleIndex);

	for (int light_i = 0; light_i < space.GetLightNum(); light_i++)
	{
		const Space::Light & light = space.GetLight(light_i);
		for (int color_i = 0; color_i < 3; color_i++)
		{
			ia_result = 0.0f;
			id_result = 0.0f;
			is_result = 0.0f;

			ia_result = material.ambient[color_i] * light.intesity[0][color_i];

			
			glm::vec3 lvec = Utils::GetVector(intersectPoint, light.position);
			float distance = glm::length(lvec);
			
			float distance2 = std::fmax(1, 0.2 * pow(distance, 2));
			lvec = lvec / distance;
			glm::vec3 nvec = triangle.GetNormalVector();
			glm::vec3 vvec = ray.GetDirVector() * -1.0f;
			vvec = vvec / glm::length(vvec);

			//Determine if the normal vector is on the same side with view vector
			float checkDirection = glm::dot(vvec, nvec); //
			if (checkDirection < 0.0f)
			{
				nvec = nvec * -1.0f;
			}

			float productOfNL = glm::dot(nvec, lvec);

			const ModelOBJ::Vertex& lightPos = light.position;

			bool shadow = false;
			if (enableShadow)
			{
				shadow = shadowRay.block(space, intersectPoint, lightPos, triangleIndex);
			}
			if (productOfNL < 0.0f || shadow)
			{ 
				
			}
			else
			{
				id_result = material.diffuse[color_i] * light.intesity[1][color_i] * productOfNL / distance2;
				id_result = id_result < 0.0f ? 0 : id_result;

				//Classical model of specular refelection
				/*vvec = vvec / glm::length(vvec);
				glm::vec3 rvec = glm::dot(nvec, lvec) * 2 * nvec - lvec;
				rvec = rvec / glm::length(rvec);

				is_result = material.specular[color_i] * light.intesity[color_i] * pow(glm::dot(rvec, vvec), material.shininess);
				is_result = is_result < 0 ? 0 : id_result;

				*/

				//Blinn-Phong's model of specular refelection
				glm::vec3 hvec = lvec + vvec;
				hvec = hvec / glm::length(hvec);

				is_result = material.specular[color_i] * light.intesity[2][color_i] * pow(glm::dot(nvec, hvec), material.shininess) / distance2;
				is_result = is_result < 0 ? 0 : id_result;
			}

			float tmp_i_result = (ia_result + id_result + is_result) * 255.0f;
			//clamp
			if (tmp_i_result > 255.0f)
			{
				tmp_i_result = 255.0f;
			}
			else if (tmp_i_result < 0.0f)
			{
				tmp_i_result = 0.0f;
			}

			i_result[color_i] += tmp_i_result;

		}
	}

	//clamp
	for (int color_i = 0; color_i < 3; color_i++)
	{
		if (i_result[color_i] > 255.0f)
		{
			i_result[color_i] = 255.0f;
		}
		else if (i_result[color_i] < 0.0f)
		{
			i_result[color_i] = 0.0f;
		}
	}

	return RGB(i_result[0], i_result[1], i_result[2]);
}
