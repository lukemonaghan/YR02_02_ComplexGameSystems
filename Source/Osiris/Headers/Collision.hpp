#pragma once

#include <glm/ext.hpp>

namespace Osiris{
	class Collision {
	public:
		static bool PointAABB(glm::vec2 min,glm::vec2 max, glm::vec2 point);
		static bool RayOBB( glm::vec3 v3_RayOrigin, glm::vec3 v3_RayDirection, glm::vec3 v3_AABBMIN, glm::vec3 v3_AABBMAX, glm::mat4 m4_Model, float &f_intersection_distance);
	};
}

