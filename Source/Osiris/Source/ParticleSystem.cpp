#include <GL/glew.h>	//	Before GLFW
#include <GL/glfw3.h>	//	After GLEW
#include <time.h>

#include "ParticleSystem.hpp"
#include "App.hpp"

namespace Osiris {

	ParticleSystem::ParticleSystem() {

		
		maxParticles =		1000;
		position =			glm::vec3(0);

		// set up emit timers
		emitTimer = 0;
		emitRate = 1.0f / 100;

		// store all variables passed in
		startColour =		glm::vec4(1,1,0,1);
		endColour =			glm::vec4(1,0,0,1);
		startSize =			1.0f;
		endSize =			2.0f;
		velocityMin =		5;
		velocityMax =		10;
		lifespanMin =		1.0f;
		lifespanMax =		5.0f;

		verticies =			new ParticleVertex[maxParticles];
		particles =			new Particle[maxParticles];
		for ( unsigned int i = 0 ; i < maxParticles ; ++i )
			deadParticles.push_back( &particles[i] );

		// create opengl buffers
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);	
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * sizeof(ParticleVertex), verticies, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0); // position
		glEnableVertexAttribArray(1); // colour
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)16);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	ParticleSystem::~ParticleSystem() {
		delete[] verticies;
		delete[] particles;

		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}

	void ParticleSystem::Update() {

		float deltaTime = (float)App::GetDeltaTime();

		// spawn particles
		emitTimer += deltaTime;
		while (emitTimer > emitRate)
		{
			Emit();
			emitTimer -= emitRate;
		}

		//	Update Particles
		auto iter = aliveParticles.begin();		
		unsigned int i = 0;
		while (iter != aliveParticles.end())
		{
			Particle* particle = (*iter);

			particle->lifetime += deltaTime;
			if (particle->lifetime >= particle->lifespan)
			{
				// return to dead pool
				deadParticles.push_back(particle);
				iter = aliveParticles.erase(iter);
			}
			else
			{
				//	Calculate new particle values
				particle->position += particle->velocity * deltaTime;
				particle->size = glm::mix( startSize, endSize, particle->lifetime / particle->lifespan );
				particle->colour = glm::mix( startColour, endColour, particle->lifetime / particle->lifespan );

				verticies[i].position = glm::vec4(particle->position, 1);
				verticies[i].colour = particle->colour;

				++i;
				++iter;
			}
		}
	}

	void ParticleSystem::Draw(Shader *shader) {
		shader->Bind();

		glm::mat4 m4Model = glm::mat4(1);
		//m4Model = glm::rotate(m4Model, 45.0f, 0.0f, 0.0f, 1.0f);
		shader->SetUniformMatrix("Model", "m4fv", 1, false, true, glm::value_ptr(m4Model));
		shader->SetUniformMatrix("View", "m4fv", 1, false, true, glm::value_ptr(App::GetView()));
		shader->SetUniformMatrix("Projection", "m4fv", 1, false, true, glm::value_ptr(App::GetProjection()));
		shader->SetUniformMatrix("Camera", "m4fv", 1, false, true, glm::value_ptr(*App::GetCamera()));

		//	Re-bind Buffer Data
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, aliveParticles.size() * sizeof(ParticleVertex), verticies);

		// draw particles in the "other" buffer
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, aliveParticles.size());
	}

	void ParticleSystem::Emit() {
		// only emit if there is a dead particle to use
		if (deadParticles.empty())
			return;

		// remove it from the dead-pool
		Particle* particle = deadParticles.back();
		deadParticles.pop_back();

		// assign its starting position
		particle->position = position;

		// randomise its lifespan
		particle->lifetime = 0;
		particle->lifespan = (rand() / (float)RAND_MAX) * (lifespanMax - lifespanMin) + lifespanMin;

		// set starting size and colour
		particle->colour = startColour;
		particle->size = startSize;

		// randomise velocity direction and strength
		glm::vec3 direction = glm::vec3((rand() / (float)RAND_MAX) * 2 - 1, (rand() / (float)RAND_MAX) * 2 - 1, (rand() / (float)RAND_MAX) * 2 - 1);
		float speed = (rand() / (float)RAND_MAX) * (velocityMax - velocityMin) + velocityMin;

		particle->velocity = ( direction ) * speed;

		// add it to the alive-pool
		aliveParticles.push_back(particle);
	}
};

