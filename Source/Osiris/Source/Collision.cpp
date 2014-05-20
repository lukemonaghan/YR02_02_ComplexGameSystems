#include "Collision.hpp"

namespace Osiris{
	bool Collision::PointAABB(glm::vec2 point,glm::vec2 min,glm::vec2 max){

		if (point.x < max.x &&
			point.y < max.y &&
			point.x > min.x &&
			point.y > min.y)
		{
			return true;
		}
		return false;
	}
	// https://code.google.com/p/opengl-tutorial-org/source/browse/misc05_picking/misc05_picking_custom.cpp
	bool Collision::RayOBB( glm::vec3 v3_RayOrigin, glm::vec3 v3_RayDirection, glm::vec3 v3_AABBMIN, glm::vec3 v3_AABBMAX, glm::mat4 m4_Model, float &f_intersection_distance){
        float tMin = 0.0f;
        float tMax = 100000.0f;
        glm::vec3 OBBposition_worldspace = m4_Model[3].xyz;
        glm::vec3 delta = OBBposition_worldspace - v3_RayOrigin;
        {
			glm::vec3 xaxis = m4_Model[0].xyz;
			float e = glm::dot(xaxis, delta);
			float f = glm::dot(v3_RayDirection, xaxis);
			if ( fabs(f) > 0.001f ){
				float t1 = (e+v3_AABBMIN.x)/f; 
				float t2 = (e+v3_AABBMAX.x)/f; 
				if (t1>t2)			{ float w=t1; t1=t2; t2=w; }
				if ( t2 < tMax )	{ tMax = t2; }
				if ( t1 > tMin )	{ tMin = t1; }
				if (tMax < tMin )	{ return false; }
			}else if(-e+v3_AABBMIN.x > 0.0f || -e+v3_AABBMAX.x < 0.0f) { return false; }
        }
        {
            glm::vec3 yaxis = m4_Model[1].xyz;
            float e = glm::dot(yaxis, delta);
            float f = glm::dot(v3_RayDirection, yaxis);
            if ( fabs(f) > 0.001f ){
                    float t1 = (e+v3_AABBMIN.y)/f;
                    float t2 = (e+v3_AABBMAX.y)/f;
                    if (t1>t2){float w=t1; t1=t2; t2=w;}
					if ( t2 < tMax ) { tMax = t2; }
                    if ( t1 > tMin ) { tMin = t1; }
					if (tMin > tMax) { return false; }
            }else if(-e+v3_AABBMIN.y > 0.0f || -e+v3_AABBMAX.y < 0.0f){ return false; }
        }
        {
            glm::vec3 zaxis = m4_Model[2].xyz;
            float e = glm::dot(zaxis, delta);
            float f = glm::dot(v3_RayDirection, zaxis);
            if ( fabs(f) > 0.001f ){
                    float t1 = (e+v3_AABBMIN.z)/f;
                    float t2 = (e+v3_AABBMAX.z)/f;
                    if (t1>t2){float w=t1;t1=t2;t2=w;}
					if ( t2 < tMax ) {tMax = t2;}
					if ( t1 > tMin ) {tMin = t1;}
					if (tMin > tMax) {return false;}
            }else if(-e+v3_AABBMIN.z > 0.0f || -e+v3_AABBMAX.z < 0.0f){ return false; }
        }
        f_intersection_distance = tMin;
        return true;
	}
}

