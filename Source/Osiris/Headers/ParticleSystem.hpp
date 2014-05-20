#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <list>

#include "Shader.hpp"
#include "Textures.hpp"


namespace Osiris {

	class ParticleSystem {

		struct Particle
		{
			glm::vec3	position;
			glm::vec3	velocity;
			glm::vec4	colour;
			float		size;
			float		lifetime;
			float		lifespan;
		};

		struct ParticleVertex
		{
			glm::vec4 position;
			glm::vec4 colour;
		};

	protected:

		unsigned int			vao;
		unsigned int			vbo;
		ParticleVertex			*verticies;

		Particle				*particles;
		std::list<Particle*>	aliveParticles;
		std::list<Particle*>	deadParticles;

		unsigned int			maxParticles;				

		glm::vec3				position;
		glm::vec4				startColour;
		glm::vec4				endColour;

		float					emitTimer;
		float					emitRate;
		float					lifespanMin;
		float					lifespanMax;
		float					velocityMin;
		float					velocityMax;
		float					startSize;
		float					endSize;		

	public:
		ParticleSystem();
		virtual ~ParticleSystem();

		void Draw(Shader *shader);
		void Update();

	protected:
		void Emit();

	};
};

